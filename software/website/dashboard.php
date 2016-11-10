<?php
  require_once('check.php');
?>
<!doctype html>
<title>Dashboard - IoT Workshop</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="main.css">
<link rel="stylesheet" href="normalize.css">
<link href="https://fonts.googleapis.com/css?family=Maven+Pro" rel="stylesheet">
<div id="color-container"></div>
<div class="middle-container">
  <div>
    <h1 class="text-center">Hi <?php echo $_GET['d']; ?>,</h1>
    <h2>how are you?</h2>
    <form action="device_configuration.php" class="vertical-gap-30 text-center">
      <input type="hidden" name="d" value="<?php echo $_GET['d']; ?>">
      <button type="submit" class="circle-button">+</button>
    </form>
    <table class="full-width">
      <?php
        $sql = "SELECT * FROM device_configuration WHERE device_id = '" . $_GET['d'] . "'";
        if($result = $connection->query($sql)) {
          while($row = $result->fetch_assoc()) {
            echo '<tr>
                    <td class="bold-text">' . $row['target_device_id'] . '</td>
                    <td class="small-cell"><div class="color-circle" style="background-color:' . $row['color'] . ';"></div></td>
                    <td>
                      <div class="vertical-gap-5 small-cell">
                        <form action="api.php">
                          <input type="hidden" name="r">
                          <input type="hidden" name="d" value="' . $row['device_id'] . '">
                          <input type="hidden" name="td" value="' . $row['target_device_id'] . '">
                          <button type="submit" name="t" value="rdc" class="del-button">-</button>
                        </form>
                      </div>
                    </td>
                  </tr>';
          }
        }
      ?>
  </table>
</div>
