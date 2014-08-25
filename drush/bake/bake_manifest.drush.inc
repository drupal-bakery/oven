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
//      'drush bake-manifest' => 'Generates manifests.',
    ),
    'aliases' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE
  );

  $items['bake-manifest-compare'] = array(
    'description' => "Compares previous manifest to what currently exists.",
    'options' => array(),
    'examples' => array(
//      'drush bake-manifest-compare' => 'Generates manifests.',
    ),
    'aliases' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE
  );

  $items['bake-manifest-sync'] = array(
    'description' => "Synchronizes between custom and drupal folders.",
    'options' => array(),
    'examples' => array(
//      'drush bake-manifest-compare' => 'Generates manifests.',
    ),
    'aliases' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE
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
 *
 * Create initial manifest
 */
function drush_bake_manifest($crc_check = TRUE) {

  $manifest = _manifest_build_complete($crc_check);

  $manifest_json = json_encode($manifest);
  file_put_contents('manifest.json', $manifest_json);
}

function drush_bake_manifest_compare($crc_check = TRUE){

  $comparison = _manifest_compare_all($crc_check);
  if (
    empty($comparison['added'])
    && empty($comparison['removed'])
    && empty($comparison['changed'])
  ) {
    drush_print(dt('No changes.'));
  }
  else {
    foreach ($comparison as $type => $manifest_diff){
      if (!empty($manifest_diff)){
        drush_print(dt('Files ' . $type . ':'));
        $paths = array();
        _manifest_build_paths($manifest_diff, '', $paths);
        foreach ($paths as $file_full_path) {
          drush_print('  ' . $file_full_path);
        }
      }
    }
  }
}

/**
 * If changes are detected, synchronize between custom and drupal folders
 *
 * @param bool $crc_check
 */
function drush_bake_manifest_sync($crc_check = TRUE){

  $manifest_a = json_decode(file_get_contents('manifest.json'), TRUE);
  $manifest_b = _manifest_build_complete($crc_check);

  $comparison = _manifest_compare($manifest_a, $manifest_b, $crc_check);

  // Changes detected
  $root = getcwd();
  if (!empty($comparison['added'])){
    $paths = array();
    _manifest_build_paths($comparison['added'], $root, $paths);
  }
  if (!empty($comparison['removed'])){
    $paths = array();
    _manifest_build_paths($comparison['removed'], $root, $paths);
  }
  if (!empty($comparison['changed'])){
    $paths = array();
    _manifest_build_paths($comparison['changed'], $root, $paths);
    die(print_r($paths));
  }


}

/**
 * @param $manifest_diff
 * @param string $root
 * @param array $paths
 */
function _manifest_build_paths($manifest_diff, $root = '', &$paths = array()){
  foreach ($manifest_diff as $name => $subpath){
    if (is_array($subpath)) {
      _manifest_build_paths($subpath, $root . '/' . $name, $paths);
    }
    else {
      $paths[] = $root . '/' . $name;
    }
  }
}

/**
 * Compare previous to current manifest
 *
 * @param bool $crc_check
 * @return array
 */
function _manifest_compare_all($crc_check = TRUE){

  $manifest_a = json_decode(file_get_contents('manifest.json'), TRUE);
  $manifest_b = _manifest_build_complete($crc_check);

  $comparison = _manifest_compare($manifest_a, $manifest_b, $crc_check);

  return $comparison;
}

/**
 * Build a new manifest of both custom and drupal paths.
 *
 * @param bool $crc_check
 * @return array
 */
function _manifest_build_complete($crc_check = TRUE) {

  $excludes = array('.', '..', '.git', 'sites/default/files', 'sites/default/private', '.DS_Store', '.idea');

  $manifest = array(
    'drupal' => _manifest_build(getcwd() . '/drupal', $excludes, $crc_check),
    'custom' => _manifest_build(getcwd() . '/custom', $excludes, $crc_check)
  );

  return $manifest;
}

/**
 * Loop through a folder structure, to construct a manifest array.
 *
 * @param $base_path
 * @param array $excludes
 * @param bool $crc_check
 * @param string $relative_path
 * @return array
 */
function _manifest_build($base_path, $excludes = array('.', '..'), $crc_check = TRUE, $relative_path = '') {
  $result = array();

  $full_path = $base_path . ($relative_path ? '/' . $relative_path : '');
  $filename = basename($full_path);

  // Ensure this is not a file/folder to be excluded
  if (
    !in_array($filename, $excludes) && !in_array($relative_path, $excludes)
  ) {

    if (is_dir($full_path)) {
      // Scan the directory for files/folders
      foreach (scandir($full_path) as $sub_name) {

        $sub_manifest = _manifest_build($base_path, $excludes, $crc_check, $relative_path . '/' . $sub_name);
        if ($sub_manifest) {
          $result[$sub_name] = $sub_manifest;
        }
      }
    }
    else {
      // Just add the one file

      if ($crc_check) {
        $result = filemtime($full_path) . '-' . crc32(file_get_contents($full_path));
      }
      else {
        $result = filemtime($full_path) . '-0';
      }
    }
  }
  return $result;
}

/**
 * Take two manifest arrays, and compare them for any changes, additions or subtractions.
 *
 * @param array $manifest_a
 * @param array $manifest_b
 * @param bool $crc_check
 * @return array
 */
function _manifest_compare($manifest_a = array(), $manifest_b = array(), $crc_check = FALSE) {
  $results = array(
    'added' => array(),
    'removed' => array(),
    'changed' => array()
  );

  if ($manifest_a != $manifest_b){
    $results['added'] = _manifest_diff($manifest_b, $manifest_a, FALSE, $crc_check);
    $results['removed'] = _manifest_diff($manifest_a, $manifest_b, FALSE, $crc_check);
    $results['changed'] = _manifest_diff($manifest_a, $manifest_b, TRUE, $crc_check);
  }

  return $results;
}

/**
 * Recursively diff two manifests
 *
 * @param $manifest_a
 * @param $manifest_b
 * @param bool $find_changes_only
 * @return array
 */
function _manifest_diff($manifest_a, $manifest_b, $find_changes_only = FALSE, $crc_check = TRUE) {
  $result = array();

  foreach ($manifest_a as $key => $value) {

    if (array_key_exists($key, $manifest_b)) {
      // Exists in both manifests

      if (is_array($value)) {
        // This is a folder, go deeper

        $sub_dir = _manifest_diff($value, $manifest_b[$key], $find_changes_only, $crc_check);
        if ($sub_dir) {
          $result[$key] = $sub_dir;
        }
      }
      elseif ($find_changes_only) {
        if ($value != $manifest_b[$key]) {
          // Something has changed, determine if it's something that matters
          list($time_a, $crc_a) = explode('-', $value);
          list($time_b, $crc_b) = explode('-', $manifest_b[$key]);
          if (
            $time_a != $time_b
            && (!$crc_check || ($crc_check && $crc_a != $crc_b))
          ){
            // Either the timestamp has changed,
            // or the CRC and crc checking is enabled
            $result[$key] = $value;
          }
        }
      }
    }
    elseif (!$find_changes_only) {
      // New for $manifest_b

      $result[$key] = $value;
    }
  }
  return $result;
} 