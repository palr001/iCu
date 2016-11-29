<?php
  require_once('check.php');
  if(isset($_GET['d']) && isset($_GET['td'])) {
  $stmt = $pdo->prepare("SELECT * FROM device_configuration WHERE device_id = ? AND target_device_id = ?");
  if ($stmt->execute([$_GET['d'], $_GET['td']])) {
    if($stmt->rowCount() > 0) {
      $data = $stmt->fetch();
      $target_device_id = $data['target_device_id'];
      $color = round(360 * hex2hsl($data['color'])[0]);
      $spring = round((100 / 255) * $data['spring']);
      $damp = round((100 / 255) * $data['damp']);
      $message = $data['message'];
    }
  }
}
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
        <input name="td" type="text" placeholder="enter ID, e.g. T111" value="<?php echo (isset($target_device_id) ? $target_device_id : '') ?>">
      </div>
      <div class="vertical-gap-30">
        <div id="colorPickerValue" class="color-value" style="background-color: hsl(<?php echo (isset($color) ? $color : '180') ?>, 100%, 50%)"></div>
        <input name="c" type="range" min="0" max="359" value="<?php echo (isset($color) ? $color : '180') ?>" class="color-picker" id="colorPicker">
      </div>
      <div class="vertical-gap-30">
        <div class="text-center">
          <label>Spring constant: <span id="springConstantRangeValue"><?php echo (isset($spring) ? $spring : '50') ?></span>%</label>
        </div>
        <input name="sc" type="range" min="0" max="100" value="<?php echo (isset($spring) ? $spring : '') ?>" step="1" id="springConstantRange">
      </div>
      <div class="vertical-gap-30">
        <div class="text-center">
          <label>Damp constant: <span id="dampConstantRangeValue"><?php echo (isset($damp) ? $damp : '50') ?></span>%</label>
        </div>
        <input name="dc" type="range" min="0" max="100" value="<?php echo (isset($damp) ? $damp : '') ?>" step="1" id="dampConstantRange">
      </div>
      <div class="vertical-gap-30">
        <div class="text-center">
          <label for="message">Sinterklaas poem:</label>
        </div>
        <textarea name="m" id="message" rows="5"><?php echo (isset($message) ? $message : '') ?></textarea>
      </div>
      <div class="text-center">
        <button type="submit" name="t" value="sdc" class="std-button">Configure</button>
      </div>
    </form>
  </div>
</div>
<script>
  window.onload = function() {
    // Color picker
    var colorPicker = document.getElementById("colorPicker");
    colorPicker.addEventListener("input", function(event) {
      changeColorPickerValue(event.target.value);
    });
    // IE 10 doesn't support input
    colorPicker.addEventListener("change", function(event) {
      changeColorPickerValue(event.target.value);
    });

    // Sprint Constant
    var springConstantRange = document.getElementById("springConstantRange");
    springConstantRange.addEventListener("input", function(event) {
      changeText(document.getElementById("springConstantRangeValue"), event.target.value);
    });
    // IE 10 doesn't support input
    springConstantRange.addEventListener("change", function(event) {
      changeText(document.getElementById("springConstantRangeValue"), event.target.value);
    });

    // Damp Constant
    var dampConstantRange = document.getElementById("dampConstantRange");
    dampConstantRange.addEventListener("input", function(event) {
      changeText(document.getElementById("dampConstantRangeValue"), event.target.value);
    });
    // IE 10 doesn't support input
    dampConstantRange.addEventListener("change", function(event) {
      changeText(document.getElementById("dampConstantRangeValue"), event.target.value);
    });
  }

  function changeColorPickerValue(value) {
    var colorPickerValue = document.getElementById("colorPickerValue");
    colorPickerValue.style.backgroundColor = "hsl(" + value + ", 100%, 50%)"
  }

  function changeText(element, value) {
    element.innerHTML = value;
  }
</script>
