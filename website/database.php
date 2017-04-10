<?php

# Edit this to your needs

$servername = 'oege.ie.hva.nl';
$username = 'palr001';
$password = 'tsc3C2zNoK2n7J';
$db = 'zpalr001';
$dsn = "mysql:host={$servername};dbname={$db}";

try {
 $pdo = new PDO($dsn , $username, $password);
} catch(PDOException $e) {
  die($e->getMessage());
}
return $pdo;

?>
