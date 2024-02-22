<?php

$file = "data.txt";
$content = file_get_contents($file); // Đọc nội dung từ tập tin

if ($content !== false) {
    $numbers = explode(" ", $content); // Tách chuỗi thành mảng các số
    
    if (count($numbers) >= 29) {
        $var1 = $numbers[0]; // Lưu số đầu tiên vào biến var1
        $var2 = $numbers[1]; // Lưu số thứ hai vào biến var2
        $var3 = $numbers[2]; // Lưu số thứ ba vào biến var3
        $var4 = $numbers[3]; // Lưu số thứ tư vào biến var4
        $var5 = $numbers[4]; // Lưu số thứ năm vào biến var5
        $var6 = $numbers[5]; 
        $var7 = $numbers[6]; 
        $var8 = $numbers[7]; 
        $var9 = $numbers[8]; 
        $var10 = $numbers[9];
        $var11 = $numbers[10]; 
        $var12 = $numbers[11];
		$var13 = $numbers[12]; 
        $var14 = $numbers[13]; 
        $var15 = $numbers[14]; 
        $var16 = $numbers[15]; 
        $var17 = $numbers[16]; 
        $var18 = $numbers[17]; 
        $var19 = $numbers[18]; 
        $var20 = $numbers[19];
		$var21 = $numbers[20]; 
        $var22 = $numbers[21];
		$var23 = $numbers[22]; 
        $var24 = $numbers[23]; 
        $var25 = $numbers[24]; 
        $var26 = $numbers[25]; 
        $var27 = $numbers[26]; 
        $var28 = $numbers[27]; 
        $var29 = $numbers[28]; 
        $var30 = $numbers[29];
    } 
} 
 $data = array($var30,$var29,$var28,$var27,$var26,$var25,$var24,$var23,$var22,$var21,$var20,$var19,$var18,$var17,$var16,$var15,$var14,$var13,$var12,$var11,$var10,$var9,$var8,$var7,$var6,$var5,$var4,$var3,$var2,$var1);
echo json_encode($data);
?>
