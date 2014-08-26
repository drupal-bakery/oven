; Drupal Bakery
;
; This makefile defines the custom installation profile.


api = 2
core = 7.x


; -----------
; Drupal Core


projects[drupal][type] = core
projects[drupal][version] = 7.31


; --------------
; Drupal Patches


; projects[drupal][patch][972536] = http://drupal.org/files/issues/object_conversion_menu_router_build-972536-1.patch
; projects[drupal][patch][992540] = http://drupal.org/files/issues/992540-3-reset_flood_limit_on_password_reset-drush.patch
; projects[drupal][patch][1355984] = http://drupal.org/files/1355984-timeout_on_install_with_drush_si-make.patch
; projects[drupal][patch][1369024] = http://drupal.org/files/1369024-theme-inc-add-messages-id-make-D7.patch
; projects[drupal][patch][1369584] = http://drupal.org/files/1369584-form-error-link-from-message-to-element-D7.patch
; projects[drupal][patch][1697570] = http://drupal.org/files/drupal7.menu-system.1697570-29.patch


