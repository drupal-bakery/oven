<?php

/**
 * Implementation of hook_drush_command().
 *
 * @return array
 */
function bake_drush_command() {
  $items = array();

  // The 'merge' command
  $items['bake'] = array(
    'description' => "Runs drush make to generate a new drupal folder, including profile and manifest.",
    'options' => array(),
    'examples' => array(
      'drush bake' => 'Generates /drupal folder.',
    ),
    'aliases' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_MAX
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return string
 */
function bake_drush_help($section) {
  switch ($section) {
    case 'drush:bake':
      return dt("Runs drush make to generate a new drupal folder, with profile and manifest.");
  }
}

/**
 * Implements hook_hook().
 */
function drush_bake() {

  $base_path = drush_get_context('DRUSH_DRUPAL_ROOT');
  $start_dir = realpath($base_path . '/../');
  if (is_dir($start_dir) && getcwd() !== $start_dir) {
    chdir($start_dir);
  }
  $destination = getcwd() . '/drupal';
  $tmpdir = drush_tempdir() . '/drupal';

  drush_print('Building Drupal core from drupal-org-core.make');
  drush_invoke('make', array(
    'drupal-org-stub.make',
    $tmpdir,
    '--prepare-install',
    '--working-copy'
  ));

  if (is_dir($tmpdir)) {
    if (is_dir($destination)) {
      drush_delete_dir($destination);
    }
    drush_copy_dir($tmpdir, $destination);
  }

  // Manifest here

  // Recursive copy here

  _bake_recursive_copy('custom', 'drupal');
}


/**
 * Recursively copy a directory, or file
 *
 * @param $source
 * @param $destination
 */
function _bake_recursive_copy($source, $destination) {
  if (is_dir($source)) {
    // Ensure the destination folder exists before we begin
    drush_mkdir($destination, TRUE);

    // Recursively copy folders
    drush_copy_dir($source, $destination, FILE_EXISTS_MERGE);
  }
  elseif (is_file($source)) {
    // Same as above, but for the path to this file
    $segments = explode('/', $destination);
    array_pop($segments);
    drush_mkdir(implode('/', $segments), TRUE);

    // Copy the file
    copy($source, $destination);
  }
}