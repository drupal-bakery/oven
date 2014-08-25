; Drupal Bakery
;
; This makefile defines which core version of Drupal to use
; and how that version may need to be patched.
;
; Typically we just need the latest stable release.


api = 2
core = 7.x


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

; Elysia Cron, for recieving and firing off workers from the queue
; and thus building drupal gzips with drush.
projects[elysia_cron][version] = 2.1
projects[elysia_cron][type] = module

; THINGS NOT IN USE AT THIS TIME

; Features will likely be handy for saving/reverting configuration later
projects[features][version] = 2.2
projects[features][type] = module

; Feeds Import to import Projects from Drupal.org
projects[feed_import][version] = 3.3
projects[feed_import][type] = module

projects[ctools][version] = 1.4
projects[ctools][type] = module


; Themes
; --------

; Bootstrap is a good fit for a tiny app like this
projects[bootstrap][version] = 3.0
projects[bootstrap][type] = theme


; Libraries
; ---------

; Isotope library - For Views Isotope
libraries[isotope][download][type] = git
libraries[isotope][download][url] = https://github.com/metafizzy/isotope.git
libraries[isotope][download][branch] = master
