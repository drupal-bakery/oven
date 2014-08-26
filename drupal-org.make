; Drupal Bakery
;
; This makefile defines which core version of Drupal to use
; and how that version may need to be patched.
;
; Typically we just need the latest stable release.


api = 2
core = 7.x

; --------
; Profiler
;
; We are installing profiler above the profile level because we are assuming that one or more profiles will likely need it.

projects[profiler][version] = 2.0-beta2
projects[profiler][type] = library

; --------
; Modules

; jQuery Update is needed for Bootstrap 3 (the current theme I am using)
projects[jquery_update][version] = 2.4
projects[jquery_update][type] = module

; Views to display list of recipes created
projects[views][version] = 3.8
projects[views][type] = module

; Views Isotope for cool filtering / arranging
projects[views_isotope][version] = 2.0-alpha1
projects[views_isotope][type] = module

; Queue UI for viewing the list of batch processing jobs waiting for completion.
projects[queue_ui][version] = 2.0-rc1
projects[queue_ui][type] = module

; Elysia Cron, for receiving and firing off workers from the queue
; and thus building drupal gzips with drush.
projects[elysia_cron][version] = 2.1
projects[elysia_cron][type] = module

; Features will likely be handy for saving/reverting configuration later
projects[features][version] = 2.2
projects[features][type] = module

; Feeds Import to import Projects from Drupal.org
projects[feed_import][version] = 3.3
projects[feed_import][type] = module

; Ctools required for views
projects[ctools][version] = 1.4
projects[ctools][type] = module

; Entity Form for collecting user data (vs webform)
projects[entityform][version] = 2.x-dev
projects[entityform][type] = module

; Strongarm for setting configuration by Features
projects[strongarm][version] = 2.0
projects[strongarm][type] = module

; Entity API required by entityform
projects[entity][version] = 1.5
projects[entity][type] = module

; Themes
; --------

; Bootstrap is a good fit for a tiny app like this
projects[bootstrap][version] = 3.0
projects[bootstrap][type] = theme

projects[adminimal_theme][version] = 1.17
projects[adminimal_theme][type] = theme

projects[adminimal_admin_menu][version] = 1.5
projects[adminimal_admin_menu][type] = module

projects[admin_menu][version] = 3.0-rc4
projects[admin_menu][type] = module

; Libraries
; ---------

; Isotope library - For Views Isotope
libraries[isotope][download][type] = git
libraries[isotope][download][url] = https://github.com/metafizzy/isotope.git
libraries[isotope][download][branch] = master
