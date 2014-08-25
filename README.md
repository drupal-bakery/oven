Drupal Bakery Oven
==================

This is a Drupal installation that creates Drupal installations.

The Custom Folder
-----------------

Things should only go in this folder if they are one-off usages to speed development.

The drupal and vendor paths are automatically merged over the respective folders in the project root.

Once a module is stable and ready to be put into a sandbox or standalone repository,
it should be removed from this path and included in your profile.make file.

If you wish to prepend to a file instead of replace it, prefix your file with "prepend___".

Similarly, if you'd like to append a file, append "append___" to your file name.

Commands
--------

#Initialization
Kickstarts this repo and generates all that is needed to run Drupal, and installs drush commands for development.

`php -f start.php`

- Use Composer to install:
-- Drupal Bakery - Drush (this repository). Which requires:
--- Drush
--- Drupal.org Drush (required for validation)
    Other possible modules may go here in future, as needed.
- Run `drush cc drush` (to register new modules)
- Move the downloaded Drush modules to the `~/.drush` folder (or M$ equivalent)
- Run `drush bake`

#To Build Drupal
After running build.php once, this is the command you would typically run to bake (or re-bake) your Drupal.

`drush bake`

- Checks for valid make files.
- Creates a temporary directory.
- Runs `drush make ...` on drupal-org-stub.make
- Runs `drush bake-manifest`

#To start Development
When you about to start coding, run this. Keep it running.

`drush bake-dev`

- Runs `drush bake-manifest-check` to ensure code integrity.
-- If needed, runs `drush bake`
- Runs `drush bake-sync` when a file-change is detected.
- Runs `drush bake-compass` (When file change of an applicable kind is found).
- Runs `drush bake-profile` (When file change of an applicable kind is found).

#Extra Development Tools
These tools are all included in `bake-dev` but they can be executed independently.

`drush bake-compass`

- Runs Compass to compile SCSS/SASS for you (can be part of bake-sync)
    (Need to check if this already exists somewhere)

`drush bake-manifest --dirs=<directory1,directory2> --name=<manifest.json>`

- Loops through all files in a given path.
- Generates a json file (ignoring .git, containing file path and md5)

`drush bake-manifest-check --dirs=<directory1,directory2> --name=<manifest.json>`

- Loops through, and then compares manifest files.

`drush bake-sync`

- Runs `drush bake-manifest-check` (internally)
- If something is new in /drupal:
-- Check to see if it's a project or other:
-- Project - Find the version, and add to the profile (if able).
-- Other - Move it to /custom, and symlink it into /drupal
- Runs `drush bake manifest`

#Onboarding
Here are some things you can use to convert a traditional Drupal installation into a Bakery project.

`drush bake-generate`

- Goes through an existing project, and converts it to a Bakery project.

`drush bake-profile --merge=<false>`

- Create initial profile
- Uses Profiler additions, to chain/merge installation profiles.
- Possibly uses (Drush Remake) to make profile files based on enabled/included modules.

#Distribution
Tools for sending out your Bakery project into the world.

`drush bake-dist`

- Generates a zipped distribution without .git data.

`drush bake-boxfile`

- Generates a Pagodabox Boxfile based on keywords in module descriptions.

___

Pagodabox Workflow
------------------

Before request on cron:
- pagoda list
- Ensure we have 10 quickstarts of drupal-bakery in Pagodabox.
-- If we are short, deploy a quickstart
-- connect it with an entity in a queue.

- Determine the oldest quickstart in the queue.
- add user as collaborator (NO API FOR THIS CURRENTLY)
-- Check for an error here. If so, go no further.
- git add remote pagoda ....
- git push pagoda master --force
- rename the app (per the user)
- make that user the owner
-- If there was a failure at this point, fire warning bells.
- Report the resulting info back to the user.

Used Projects
-------------

Composer
    https://www.drupal.org/project/composer
    A dependency for modules that need composer.

Drush Remake
    https://www.drupal.org/project/drush_remake
    Remakes your installation profiles.

Makefile extra
    https://www.drupal.org/project/makefile_extra
    Remakes your makefiles (By updating module version assignments).

Profiler
    https://www.drupal.org/project/profiler
    Extends typical profile functionality, should be included with all profiles
    Very useful stuff, check out the Readme.txt!

Related Projects
----------------

Drush Rebuild
    https://www.drupal.org/project/rebuild
    Refreshes your (local) environment. So that you don't have to reconfigure,
    Or write things into your .install file. Very neat!

Drush Vagrant
    https://www.drupal.org/project/drush-vagrant
    Sets up local Vagrant based on "blueprints", great for local development.

Turnip
    https://www.drupal.org/project/turnip
    Drupal starting kit similar to Bakery,
    but it goes beyond the codebase to environmental dependencies like Varnish.

DSLM - Drupal Symlink Manager
    https://www.drupal.org/project/dslm
    Similar concept to what we are doing with Symlinks, but in a multi-site way.

Provision profile settings
    https://www.drupal.org/project/provision_profile_settings
    Allows an aegir.settings.php file to work from the profile level.
