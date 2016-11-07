<?php
  require_once('check.php');
?>
<!doctype html>
<title>Device Configuration - IoT Workshop</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="main.css">
<div class="middle-container">
  <div>
    <form action="api.php">
      <input type="hidden" name="r">
      <input type="hidden" name="d" value="<?php echo $_GET['d']; ?>">
      <div>
        <input class="big-input" name="td" type="text" value="<?php echo $_GET['d']; ?>">
      </div>
      <div class="vertical-gap">
        <input class="big-input" name="c" type="text" value="FF0000">
      </div>
      <div class="text-center">
        <button type="submit" name="t" value="sdc">Configure</button>
      </div>
    </form>
  </div>
</div>
