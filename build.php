<?php
/**
 * Drupal Bakery
 *
 * This is a tiny script used to kickstart the codebase.
 * All subsequent calls will be made by drupal-bakery.
 */

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);

if (php_sapi_name() !== 'cli') {
  // For safety :)
  exit('This script must be executed from the command line.');
}

// Define which version of Drush should be installed (latest stable), if needed.
define('BAKE_DRUSH_VERSION', '6.3.0');

// Method to test for Composer presence
define('BAKE_COMPOSER_TEST', 'composer --version 2>/dev/null');

// Method to test drush presence
define('BAKE_DRUSH_TEST', 'drush bake 2>/dev/null');

// Minimum version of drush acceptable for our purposes
define('BAKE_DRUSH_MINIMUM', '6.3.0');

// Typical command used to install dependancies
define('BAKE_COMPOSER_INSTALL', 'composer install --no-interaction --prefer-dist --no-dev --optimize-autoloader');

bake_output('Drupal Bakery - Start', 0, ':');

// Switch root directory to project folder
$error = FALSE;

// Detect if windows is our current OS
$windows = strtoupper(substr(PHP_OS, 0, 3)) === 'WIN';

// Ensure that Composer is installed
$composer_installed = bake_exec(BAKE_COMPOSER_TEST, FALSE);
if (!$composer_installed) {
  bake_output('PHP Composer not found. Installing...');
  $composer_downloaded = bake_exec('php -r "readfile(\'https://getcomposer.org/installer\');" | php');
  if ($composer_downloaded && file_exists('composer.phar')) {

    bake_output('Moving Composer to global location.');
    if ($windows) {
      if (!is_dir('C:\bin')) {
        mkdir('C:\bin');
      }
      $moved = rename('composer.phar', 'C:\bin\composer');
    }
    else {
      $moved = rename('composer.phar', '/usr/local/bin/composer');
      if (!$moved) {
        // Use sudo if we have to
        bake_output('Sudo access required for installing Composer.');
        $moved = bake_exec('sudo mv composer.phar /usr/local/bin/composer');
      }
    }

    // One last check to ensure it installed
    if (!$moved || !bake_exec(BAKE_COMPOSER_TEST, FALSE)) {
      bake_error("Could not install Composer.\n  You will need to get it from: https://getcomposer.org/download");
    }
    else {
      // Finally, success!
      $composer_installed = TRUE;
    }
  }
  else {
    bake_error("Could not download Composer.\n  You will need to get it from: https://getcomposer.org/download");
  }
}

if ($composer_installed) {

  bake_output('Installing/updating drush requirements.');
  // Ensure that we are running the most recent stable version of Drush
  $drush_installed = bake_exec('composer global require drush/drush:6.3.0');
  if ($drush_installed){
    $requirements = bake_exec(BAKE_COMPOSER_INSTALL, FALSE);
    if ($requirements) {

      $home = $_SERVER['HOME'];
      $source = __DIR__ . '/drush';
      $destination = $home . '/.drush/';
      if (!is_dir($destination)) {
        mkdir($destination);
      }
      rcopy($source, $destination);

      bake_exec('./vendor/bin/drush cc drush');
      bake_exec('./vendor/bin/drush bake');
    }
    else {
      bake_error('Could not install local Composer dependencies.');
    }
  }
  else {
    bake_error('Could not install drush.');
  }
}

bake_output('Drupal Bakery - End', 0, ':');

if ($error) {
  exit(1); // A response code other than 0 is a failure
}
else {
  exit(0);
}

/**
 * Simple wrapper for command execution.
 *
 * @param $command
 * @param int $indentation
 * @param bool $output
 */
function bake_exec($command, $print = TRUE, $indentation = 2) {
  exec($command, $output, $stdcode);
  if ($print) {
    bake_output($output, $indentation);
  }
  // If all was good, return output array..
  // unless the output was empty, then return true.
  // If there was an error, always return false.
  return ($stdcode == 0) ? (empty($output) ? TRUE : $output) : FALSE;
}

/**
 * Just a wrapper for printing to stdout.
 *
 * @param $mixed
 * @param int $indentation
 * @param bool $header_char
 */
function bake_output($mixed, $indentation = 1, $header_char = FALSE) {
  $terminator = "\n\n";
  if (!is_array($mixed)) {
    // Make the headers look pretty
    if ($header_char) {
      // Uppercase
      $maxlen = 80 - 2 - 2; // Start with 80, leave room for 2 spaces, and another 2 for collapsed windows
      $length = strlen($mixed);
      $buffera = ($maxlen - $length) / 2;
      $bufferb = is_int($buffera) ? $buffera : $buffera - 1;
      $fgcolor = "0;30";
      $bgcolor = "46";
      $mixed = "\n" . ($buffera > 0 ? str_repeat($header_char, $buffera) : '') . ' '
        . strtoupper($mixed) . ' '
        . ($bufferb > 0 ? str_repeat($header_char, $bufferb) : '') . "\n";
      $mixed = "\033[" . $fgcolor . "m" . " \033[" . $bgcolor . "m" . $mixed . "\033[0m";
      $indentation = 0;
    }
    $mixed = array($mixed);
    $terminator = '';
  }

  foreach ($mixed as $line) {
    echo ($indentation > 0 ? str_repeat('  ', $indentation) : '') . $line . "\n";
  }
  echo $terminator;
}

/**
 * Report an error in the build process.
 *
 * @param $string
 */
function bake_error($string) {
  global $error;
  $error = TRUE;
  fwrite(STDERR, "\n\033[31m  ERROR: " . $string . "\033[37m" . "\n");
}

/**
 * Recursively delete dirrectories
 *
 * @param $dir
 */
function rrmdir($dir) {
  if (is_dir($dir)) {
    $objects = scandir($dir);
    foreach ($objects as $object) {
      if ($object != "." && $object != "..") {
        if (filetype($dir."/".$object) == "dir"){
          rrmdir($dir."/".$object);
        }else{
          unlink($dir."/".$object);
        }
      }
    }
    reset($objects);
    rmdir($dir);
  }
}

/**
 * Recursively copy files from one directory to another
 *
 * @param String $src Source of files being moved
 * @param String $dest Destination of files being moved
 * @return bool
 */
function rcopy($src, $dest) {
  // If source is not a directory stop processing
  if (!is_dir($src)) {
    return FALSE;
  }

  // If the destination directory does not exist create it
  if (!is_dir($dest)) {
    if (!mkdir($dest)) {
      // If the destination directory could not be created stop processing
      return FALSE;
    }
  }

  // Open the source directory to read in files
  $i = new DirectoryIterator($src);
  foreach ($i as $f) {
    if ($f->isFile()) {
      copy($f->getRealPath(), "$dest/" . $f->getFilename());
    }
    else {
      if (!$f->isDot() && $f->isDir()) {
        rcopy($f->getRealPath(), "$dest/$f");
      }
    }
  }

  return TRUE;
}