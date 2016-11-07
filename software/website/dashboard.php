<!doctype html>
<title>IoT Workshop Dashboard</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="main.css">
<div class="middle-container">
  <div>
    <p class="text-center">Hi T123</p>
    <form action="device_configuration.php">
      <input type="hidden" name="d" value="<?php echo $_GET['d']; ?>">
      <button type="submit" class="circle-button">+</button>
    </form>
    <table class="full-width">
      <tr>
        <td>Y123</td>
        <td>#FF0000</td>
        <td>
          <div class="float-right">
            <form action="api.php">
              <input type="hidden" name="d" value="<?php echo $_GET['d']; ?>">
              <input type="hidden" name="td" value="<?php echo 'Y123'; ?>">
              <button type="submit" name="t" value="rd">-</button>
            </form>
          </div>
        </td>
      </tr>
      <tr>
        <td>D321</td>
        <td>#FF00FF</td>
        <td>
          <div class="float-right">
            <form action="api.php">
              <input type="hidden" name="d" value="<?php echo $_GET['d']; ?>">
              <input type="hidden" name="td" value="<?php echo 'D321'; ?>">
              <button type="submit" name="t" value="rd">-</button>
            </form>
          </div>
        </td>
      </tr>
  </table>
</div>
