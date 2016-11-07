<?php
  require_once('database.php');
  $response = ['status' => 0];

  $arr = [];
  foreach($_GET as $key => $value) {
    $arr[$key] = $connection->real_escape_string($value);
  }

  // Device id is needed for every operation
  if(isset($arr['d']) && isset($arr['t'])) {
    // Go through all available types of operations
    switch($arr['t']) {
      case 'sdc': // device configuration
        if(isset($arr['c']) && isset($arr['td'])) {
          // Check if exists
          $sql = "SELECT * FROM device_configuration WHERE device_id = '" . $arr['d'] . "' AND target_device_id = '" . $arr['td'] . "'";
          if ($result = $connection->query($sql)) {
            if($result->num_rows > 0) {
              // Update
              $sql = "UPDATE device_configuration SET color = '" . $arr['c'] . "' WHERE device_id = '" . $arr['d'] . "' AND target_device_id = '" . $arr['td'] . "'";
              if ($connection->query($sql)) {
                $response = ['status' => 1];
              }
            } else {
              // Create
              $sql = "INSERT INTO device_configuration(color, device_id, target_device_id) VALUES ('" . $arr['c'] . "', '" . $arr['d'] . "', '" . $arr['td'] . "')";
              if ($connection->query($sql)) {
                $response = ['status' => 1];
              }
            }
          }
        }
      break;
      case 'rdc': // remove device configuration
        if(isset($arr['td'])) {
          // Check if exists
          $sql = "SELECT * FROM device_configuration WHERE device_id = '" . $arr['d'] . "' AND target_device_id = '" . $arr['td'] . "'";
          if ($connection->query($sql)) {
            // Remove all queue items of device and target device
            $sql = "DELETE FROM queue WHERE device_id = '" . $arr['d'] . "' AND target_device_id = '" . $arr['td'] . "'";
            if ($connection->query($sql)) {
              // Remove device configuration
              $sql = "DELETE FROM device_configuration WHERE device_id = '" . $arr['d'] . "' AND target_device_id = '" . $arr['td'] . "'";
              if ($connection->query($sql)) {
                $response = ['status' => 1];
              }
            }
          }
        }
      break;
      case 'gqi': // get queue item
          // Get queue item
          $sql = "SELECT * FROM queue q LEFT JOIN device_configuration dc ON q.target_device_id = dc.target_device_id " .
          "WHERE q.target_device_id = '" . $arr['d'] . "' LIMIT 1";

          if ($queue_item = $connection->query($sql)) {
            // Delete from queue because it's not needed anymore
            $sql = "DELETE FROM queue WHERE target_device_id = '" . $arr['d'] . "' LIMIT 1";
            if ($result = $connection->query($sql)) {
              // Return queue item
              $response = ['status' => 1, 'queue_item' => $queue_item->fetch_assoc()];
            }
        }
      break;
      case 'sqi': // set queue item
        $sql = "SELECT * FROM device_configuration WHERE device_id = '" . $arr['d'] . "'";
        if($result = $connection->query($sql)) {
          while($row = $result->fetch_assoc()) {
            $sql = "INSERT INTO queue(device_id, target_device_id) VALUES ('" . $arr['d'] . "', '" . $row['target_device_id'] . "')";
            if ($connection->query($sql)) {
              // Return result
              $response = ['status' => 1];
            } else {
              $response = ['status' => 0, 'message' => $connection->error];
            }
          }
          $result->free();
        }
      break;
    }

    $connection->close();

    // Parameter r can be used for redirecting
    if(isset($_GET['r'])) {
      $location = "http://" . $_SERVER['SERVER_NAME'] . "/iot-workshop/dashboard.php?d=" . $arr['d'];
      header("Location: " . $location);
      die();
    } else {
      echo json_encode($response);
    }
  }


?>
