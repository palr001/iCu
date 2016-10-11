<?php

$servername = 'localhost';
$username = 'root';
$password = 'InternetOfThingsWorkshop5';
$db = 'internet_of_things_workshop';

$connection = new mysqli($servername, $username, $password, $db);

if($connection->connect_error) {
  die('Connection failed: ' . $connection->connect_error);
}

return $connection;

?>
