<?php

# Edit this to your needs
$servername = 'localhost';
$username = DB_USERNAME;
$password = DB_PASSWORD;
$db = DB_NAME;

try {
  $pdo = new PDO(
      'mysql:host=' . $servername . ';dbname=' . $db,
      $username,
      $password
    );
} catch(PDOException $e) {
  die($e->getMessage());
}

return $pdo;

?>
