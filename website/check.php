<?php
require_once('config.php');
require_once('util.php');
require_once('database.php');
$value = $_GET['d'];

# Checks if device is valid
if(!isset($_GET['d'])) {
  redirect(ROOT);
} else {
  $stmt = $pdo->prepare("SELECT * FROM icu_device WHERE id = ?");
  if ($stmt->execute([$_GET['d']])) {
    if($stmt->rowCount() == 0) {
		if ($value != "" || $value != null){
		$pdo->query("INSERT INTO icu_device (id) VALUE ('".$value."')");
		}
		redirect(ROOT);
		die();
		
}
}
}
?>
