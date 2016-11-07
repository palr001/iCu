<?php
  require_once('database.php');
  if(isset($_GET['d']) && isset($_GET['td'])) {
    switch($_GET['t']) {
      case 'dc': // device configuration
        if(isset($_GET['c'])) {
          // Check if exists
          $sql = "SELECT * FROM device_configuration WHERE device_id = " . $_GET['d'] . " AND target_device_id = " . $_GET['td'];
          if ($result = $mysqli->query($sql)) {
            // Update
            $sql = "UPDATE device_configuration SET color = " . $_GET['c'] . " WHERE device_id = " . $_GET['d'] . " AND target_device_id = " . $_GET['td'];
            if ($result = $mysqli->query($sql)) {
              return true;
            }
          } else {
            // Create
            $sql = "INSERT INTO device_configuration(color, device_id, target_device_id) VALUES ('" . $_GET['c'] . "', '" . $_GET['d'] . "', '" . $_GET['td'] . ")";
            if ($result = $mysqli->query($sql)) {
              return true;
            }
          }
        }
      break;
      case 'rdc': // remove device configuration
        // Check if exists
        $sql = "SELECT * FROM device_configuration WHERE device_id = " . $_GET['d'] . " AND target_device_id = " . $_GET['td'] . "'";
        if ($result = $mysqli->query($sql)) {
          // Remove all queue items of device and target device
          $sql = "DELETE FROM queue WHERE device_id = '" . $_GET['d'] . "' AND target_device_id = '" . $_GET['td'] . "'";
          if ($result = $mysqli->query($sql)) {
            // Remove device configuration
            $sql = "DELETE FROM device_configuration WHERE device_id = '" . $_GET['d'] . "' AND target_device_id = '" . $_GET['td'] . "'";
            if ($result = $mysqli->query($sql)) {
              return true;
            }
          }
        }
      break;
      case 'gqi': // get queue item
        // Get queue item
        $sql = "SELECT * FROM queue WHERE device_id = " . $_GET['d'] . " AND target_device_id = " . $_GET['td'] . "' LIMIT 1";
        if ($queue_item = $mysqli->query($sql)) {
          // Delete from queue because it's not needed anymore
          $sql = "DELETE FROM queue WHERE device_id = '" . $_GET['d'] . "' AND target_device_id = '" . $_GET['td'] . "' LIMIT 1";
          if ($result = $mysqli->query($sql)) {
            return $queue_item;
          }
        }
      break;
    }
  }
  return false;
?>
