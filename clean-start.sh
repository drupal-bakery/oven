#!/bin/sh

rm -rf vendor
rm -rf drush/drupalorg_drush
rm -rf drush/drush_remake
rm -rf drush/makefile_extra
rm -rf drupal
rm composer.lock
composer clear-cache
php -f start.php
