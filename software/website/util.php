<?php
  require_once('color_converter.php');
  function redirect($location) {
    header('Location: ' . $location);
    die();
  }
?>
