<!doctype html>
<title>Internet of Things Workshop</title>
<h1>Button Press Timestamps</h1>
<?php
require_once('database.php');

if($result = $connection->query('SELECT * FROM entry ORDER BY timestamp DESC')) {
  $entries = $result->fetch_all();
} else {
  die($connection->error);
}

$connection->close();
?>
<div>
<?php
  if(count($entries) > 1) {
    for($i = 0; $i < count($entries); $i++) {
      echo '<div>' . $entries[$i][1] . '</div>';
    }
  } else {
    echo 'No entries found!';
  }
?>
</div>
