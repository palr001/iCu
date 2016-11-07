<?php
require_once('database.php');
$location = "http://" . $_SERVER['SERVER_NAME'] . "/iot-workshop";
if(!isset($_GET['d'])) {
  header("Location: " . $location);
  die();
} else {
  $device_id = $connection->real_escape_string($_GET['d']);
  $sql = "SELECT * FROM device WHERE id = '" . $device_id . "'";
  if ($result = $connection->query($sql)) {
    if($result->num_rows == 0) {
      header("Location: " . $location);
      die();
    }
  }
}
?>
