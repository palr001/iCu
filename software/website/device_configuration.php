<?php
  require_once('check.php');
?>
<!doctype html>
<title>Device Configuration - IoT Workshop</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="main.css">
<link href="https://fonts.googleapis.com/css?family=Maven+Pro" rel="stylesheet">
<div class="middle-container">
  <div>
    <form action="api.php">
      <input type="hidden" name="r">
      <input type="hidden" name="d" value="<?php echo $_GET['d']; ?>">
      <div class="text-center">
        <input name="td" type="text" value="enter ID, e.g. T111">
      </div>
      <div class="vertical-gap-20 text-center">
        <input name="c" type="color" value="#ff0000">
      </div>
      <div class="text-center">
        <button type="submit" name="t" value="sdc" class="std-button">Configure</button>
      </div>
    </form>
  </div>
</div>
