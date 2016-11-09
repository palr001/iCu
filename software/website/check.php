<?php
require_once('config.php');
require_once('util.php');
require_once('database.php');

# Checks if device is valid
if(!isset($_GET['d'])) {
  redirect(ROOT);
} else {
  $device_id = $connection->real_escape_string($_GET['d']);
  $sql = "SELECT * FROM device WHERE id = '" . $device_id . "'";
  if ($result = $connection->query($sql)) {
    if($result->num_rows == 0) {
      header('Location: ' . ROOT);
      die();
    }
  }
}
?>
