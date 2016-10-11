<?php
require_once('database.php');
if($_SERVER['REQUEST_METHOD'] === 'POST') {
  $query = 'INSERT INTO entry VALUES()';
  if($connection->query($query)) {
    echo 'Successfully added entry to database! Check the homepage for the result';
  } else {
    die($connection->error);
  }
} else {
  echo 'Only post requests are accepted!';
}
$connection->close();
?>
