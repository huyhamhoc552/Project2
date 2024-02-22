<?php
  require 'database.php';
  
  //---------------------------------------- 
  if (!empty($_POST)) {
    //........................................ 
    $id = $_POST['id'];
	$vib = $_POST['vib'];
	$gas = $_POST['gas'];
    $temp = $_POST['temp'];
    $humi = $_POST['humi'];
    $air = $_POST['air'];
    $LED = $_POST['LED'];
    $sound = $_POST['sound'];
    //........................................
    
    //........................................ 
    date_default_timezone_set("Asia/Ho_Chi_minh"); 
    $tm = date("H:i:s");
    $dt = date("Y-m-d");
    //........................................
    
    //........................................ 
    $pdo = Database::connect();
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
 
    $sql = "UPDATE datasensor_update SET vib = ?,gas = ?,temp = ?, humi = ?,air = ?,LED = ?,sound = ?, time = ?, date = ? WHERE id = ?";
    $q = $pdo->prepare($sql);
    $q->execute(array($vib,$gas,$temp,$humi,$air,$LED,$sound,$tm,$dt,$id));
	$file = "data.txt";
       $str= " ";

$content = $vib;
$space = " ";
$currentContent = file_get_contents($file);
file_put_contents($file, $space . $currentContent);
// Đọc nội dung hiện tại của tập tin
$currentContent = file_get_contents($file);
file_put_contents($file, $content . $currentContent);
	
	
	
	
	
    Database::disconnect();
    //........................................ 
    
    //........................................ Entering data into a table.
    $id_key;
    $board = $_POST['id'];
    $found_empty = false;
    
    $pdo = Database::connect();
    
    //:::::::: Process to check if "id" is already in use.
    while ($found_empty == false) {
      $id_key = generate_string_id(10);
      // replace_with_your_table_name, on this project I use the table name 'esp32_table_dht11_leds_record'.
      // This table is used to store and record DHT11 sensor data updated by ESP32. 
      // This table is also used to store and record the state of the LEDs, the state of the LEDs is controlled from the "home.php" page. 
      // This table is operated with the "INSERT" command, so this table will contain many rows.
      // Before saving and recording data in this table, the "id" will be checked first, to ensure that the "id" that has been created has not been used in the table.
      $sql = 'SELECT * FROM datasensor_record WHERE id="' . $id_key . '"';
      $q = $pdo->prepare($sql);
      $q->execute();
      
      if (!$data = $q->fetch()) {
        $found_empty = true;
      }
    }
    //::::::::
    
    //:::::::: The process of entering data into a table.
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    // replace_with_your_table_name, on this project I use the table name 'esp32_table_dht11_leds_record'.
    // This table is used to store and record DHT11 sensor data updated by ESP32. 
    // This table is also used to store and record the state of the LEDs, the state of the LEDs is controlled from the "home.php" page. 
    // This table is operated with the "INSERT" command, so this table will contain many rows.
		$sql = "INSERT INTO datasensor_record (id,board,vib,gas,temp,humi,air,LED,sound,time,date) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
		$q = $pdo->prepare($sql);
		$q->execute(array($id_key,$board,$vib,$gas,$temp,$humi,$air,$LED,$sound,$tm,$dt));
    //::::::::
    
    Database::disconnect();
    //........................................ 
  }
  //---------------------------------------- 
  
  //---------------------------------------- Function to create "id" based on numbers and characters.
  function generate_string_id($strength = 16) {
    $permitted_chars = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $input_length = strlen($permitted_chars);
    $random_string = '';
    for($i = 0; $i < $strength; $i++) {
      $random_character = $permitted_chars[mt_rand(0, $input_length - 1)];
      $random_string .= $random_character;
    }
    return $random_string;
  }
  //---------------------------------------- 
?>

