<?php

if (getenv('PLATFORM') == 'PAGODABOX'){
  $databases = array(
    'default' => array(
      'default' => array(
        'driver' => 'mysql',
        'database' => getenv('DATABASE1_NAME'),
        'username' => getenv('DATABASE1_USER'),
        'password' => getenv('DATABASE1_PASS'),
        'host' => getenv('DATABASE1_HOST'),
        'port' => getenv('DATABASE1_PORT'),
        'prefix' => ''
      )
    )
  );

  exec(
    "drush site-install --db-url="
            . $databases['default']['default']['driver']
    . "://" . $databases['default']['default']['username']
    . ":"   . $databases['default']['default']['password']
    . "@"   . $databases['default']['default']['host']
    . ":"   . $databases['default']['default']['port']
    . "/"   . $databases['default']['default']['database']
  );
}

