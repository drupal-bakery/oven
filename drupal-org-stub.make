; Drupal Bakery
;
; This makefile pulls in the core and builds the profile.


api = 2
core = 7.x


includes[] = drupal-org-core.make

projects[l10n_install][type] = profile
projects[l10n_install][version] = 1.0-rc4

projects[bakery][type] = profile
projects[bakery][download][type] = file
projects[bakery][download][url] = ./drupal-org.make
