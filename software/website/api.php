<?php
  require_once('config.php');
  require_once('util.php');
  require_once('database.php');

  $response = -1;

  // Device id and type is needed for every operation
  if(isset($_GET['d']) && isset($_GET['t'])) {
    // Go through all available types of operations
    switch($_GET['t']) {
      case 'sdc': // set device configuration
        // c should be hue
        if(isset($_GET['c']) && isset($_GET['td'])) {
          $hexcolor = hsl2hex([$_GET['c']/360, 1, 0.5]);
          $spring = round((255 / 100) * $_GET['sc']);
          $damp = round((255 / 100) * $_GET['dc']);

          // Check if exists
          $stmt = $pdo->prepare("SELECT * FROM device_configuration WHERE device_id = ? AND target_device_id = ?");
          if ($stmt->execute([$_GET['d'], $_GET['td']])) {
            if($stmt->rowCount() > 0) {
              // Update
              $stmt = $pdo->prepare("UPDATE device_configuration SET color = ?, spring = ?, damp = ?, message = ? WHERE device_id = ? AND target_device_id = ?");
            } else {
              // Create
              $stmt = $pdo->prepare("INSERT INTO device_configuration(color, spring, damp, message, device_id, target_device_id) VALUES (?, ?, ?, ?, ?, ?)");
            }
            if ($stmt->execute([$hexcolor, $spring, $damp, $_GET['m'], $_GET['d'], $_GET['td']])) {
              $response = 1;
            }
          }
        }
      break;
      case 'rdc': // remove device configuration
        if(isset($_GET['td'])) {
          // Check if exists
          $stmt = $pdo->prepare("SELECT * FROM device_configuration WHERE device_id = ? AND target_device_id = ?");
          if ($stmt->execute([$_GET['d'], $_GET['td']])) {
            // Remove all queue items of device and target device
            $stmt = $pdo->prepare("DELETE FROM queue WHERE device_id = ? AND target_device_id = ?");
            if ($stmt->execute([$_GET['d'], $_GET['td']])) {
              // Remove device configuration
              $stmt = $pdo->prepare("DELETE FROM device_configuration WHERE device_id = ? AND target_device_id = ?");
              if ($stmt->execute([$_GET['d'], $_GET['td']])) {
                $response = 1;
              }
            }
          }
        }
      break;
      case 'bdc': // blacklist device configuration
        if(isset($_GET['td']) && isset($_GET['b'])) {
          // Check if exists
          $stmt = $pdo->prepare("UPDATE device_configuration SET blacklist = ? WHERE device_id = ? AND target_device_id = ?");
          if ($stmt->execute([$_GET['b'], $_GET['td'], $_GET['d']])) {
            $response = 1;
          }
        }
      break;
      case 'gqi': // get queue item
          // Get queue item
          $stmt = $pdo->prepare("SELECT * FROM device_configuration WHERE target_device_id = ? AND device_id" .
		" = (SELECT device_id FROM queue WHERE target_device_id = ? ORDER BY timestamp LIMIT 1)");
          if($stmt->execute([$_GET['d'], $_GET['d']])) {
            if ($stmt->rowCount() == 1) {
              $dc = $stmt->fetch();
              // Delete from queue because it's not needed anymore
              $stmt = $pdo->prepare("DELETE FROM queue WHERE target_device_id = ? LIMIT 1");
              if ($stmt->execute([$_GET['d']])) {
                // Return queue item
                // We need this check because workshop 1 hardware isn't compatible with a response of more than the color
                if(isset($_GET['v']) && $_GET['v'] == '2') {
                  $response = $dc['color'] . ',' . $dc['spring'] . ',' . $dc['damp'] . ',' . $dc['message'];
                } else {
                  $response = $dc['color'];
                }
              } else {
                $response = -1;
              }
          }
        }
      break;
      case 'sqi': // set queue item
        // Blacklisted device configuration should be able to insert something in the queue
        $stmt = $pdo->prepare("SELECT * FROM device_configuration WHERE device_id = ? AND blacklist = 0");
        if($stmt->execute([$_GET['d']])) {
          $data = $stmt->fetchAll();
          foreach($data as $row) {
            $stmt = $pdo->prepare("INSERT INTO queue(device_id, target_device_id) VALUES (?, ?)");
            if ($stmt->execute([$_GET['d'], $row['target_device_id']])) {
              $response = 1;
            }
          }
        }
      break;
      case 'id': // insert device
      	$stmt = $pdo->prepare("INSERT INTO device(id) VALUES(?)");
      	if($stmt->execute([$_GET['d']])) {
      	  $response = 1;
      	}
      break;
    }

    // Parameter r can be used for redirecting
    if(isset($_GET['r'])) {
      if($_GET['r'] == '') {
        $location = ROOT . '/dashboard.php?d=' . $_GET['d'];
      } else {
        $location = ROOT . $_GET['r'];
      }
      redirect($location);
    } else {
      echo $response;
    }
  }


?>
