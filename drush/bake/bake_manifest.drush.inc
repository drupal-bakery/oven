<?php

/**
 * Implementation of hook_drush_command().
 *
 * @return array
 */
function bake_manifest_drush_command() {
  $items = array();

  $items['bake-manifest'] = array(
    'description' => "Generates a manifest of drupal and custom folders.",
    'options' => array(),
    'examples' => array(
      'drush bake-manifest' => 'Generates manifests.',
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
function bake_manifest_drush_help($section) {
  switch ($section) {
    case 'drush:bake-manifest':
      return dt("Generates manifests.");
  }
}

/**
 * Implements hook_hook().
 */
function drush_bake_manifest() {
  $destination = getcwd() . '/drupal';

  $manifest = _manifest_build($destination, $destination);
  $manifest_json = json_encode($manifest);

  file_put_contents('manifest.json', $manifest_json);

}


function _manifest_build($source, $original) {
  $manifest = array();

  $excluded_files = array('.', '..', '.git');
  $excluded_folders = array('sites/default/files', 'sites/default/private');
  $filename = basename($source);

  if (
    !in_array($filename, $excluded_files)
    && !in_array(str_replace($original . '/', '', $source), $excluded_folders)
  ) {

    if (is_dir($source)) {

      foreach (scandir($source) as $filename) {

        $manifest[$filename] = _manifest_build($source . '/' . $filename, $original);
      }
    }
    else {

      $manifest[$filename] = filemtime($source) . '_' . crc32(file_get_contents($source));
    }
  }
  return $manifest;
}