#include <SPI.h>
#include <Arduino.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <LoRa.h>
#include <HTTPClient.h>
#include <string>
#define ss 5
#define rst 14
#define dio0 2
const char* ssid = "MANG DAY KTX H1-307";
const char* password = "07032022" ;

HTTPClient http;
int httpCode,count =0;
long timer =0;

String postData = "";
String payload = "";
String outgoing;   
String Incoming = "";
String Message = "11";            
           
byte LocalAddress = 0x30;     
byte Destination1 = 0x31;
byte Destination2 = 0x32;       

void sendMessage(String Outgoing, byte Destination) {
           LoRa.beginPacket();             
           LoRa.write(Destination);        
           LoRa.write(LocalAddress);         
           LoRa.print(Outgoing);           
           LoRa.endPacket();               
}

void control_LEDs() {  
           JSONVar myObject = JSON.parse(payload);
           if (JSON.typeof(myObject) == "undefined") {
               Serial.println("Parsing input failed!");
               Serial.println("---------------");
               return;
             }
           if(strcmp(myObject["LED"], "ON") == 0)    {Message = "11";   Serial.println("LED ON");}
           if(strcmp(myObject["LED"], "OFF") == 0)  {Message = "00";   Serial.println("LED OFF"); }
           Serial.println("---------------");
}

void checkCMD()
{
           postData = "id=esp32_01";
           payload = "";
           http.begin("http://192.168.0.103/test/getcommand.php");  
           http.addHeader("Content-Type", "application/x-www-form-urlencoded");        
           httpCode = http.POST(postData);
           payload = http.getString();    
           http.end();  //--> Close connection
           control_LEDs();
}

void onReceive(int packetSize) {
           if (packetSize == 0) return;  
           int recipient = LoRa.read();        
           byte sender = LoRa.read();          
 
           Incoming = "";

           while (LoRa.available()) {
               Incoming += (char)LoRa.read();
             }
  
           if (recipient != LocalAddress) {
           Serial.println("This message is not for me.");
           return; //--> skip rest of function
             }

           Serial.println("Received from: 0x" + String(sender, HEX));
           Serial.println("Message: " + Incoming);
           Serial.println(Incoming.substring(0, 5));
           Serial.println(Incoming.substring(5, 10)); 
           Serial.println(Incoming.substring(10, 13));
           Serial.println(Incoming.substring(13, 16));
           Serial.println(Incoming.substring(16, 17));
           float temp = (Incoming.substring(10, 13)).toFloat()/10;
           float humi = (Incoming.substring(13, 16)).toFloat()/10;
  
           Processing_incoming_data();
           postData = "id=esp32_01";
           payload = "";
           postData = "id=esp32_01";
           postData += "&vib=" + Incoming.substring(0, 5);
           postData += "&gas=" + Incoming.substring(5, 10);
           postData += "&temp=" + String(temp,1);
           postData += "&humi=" + String(humi,1);
           postData += "&air=" + String(count);
           
           if (Incoming.substring(16, 17)== "0")
                    {
                    postData += "&LED=" + String("OFF");
                    postData += "&sound=" + String("OFF"); }
           else {
                      postData += "&LED=" + String("ON");
                      postData += "&sound=" + String("ON"); 
                    }
              
           payload = "";
           Serial.println("---------------updateData.php");
           http.begin("http://192.168.0.103/test/updatedata_and_record.php");
           http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
           httpCode = http.POST(postData);
           payload = http.getString();  
           Serial.print("httpCode : ");
           Serial.println(httpCode);
           Serial.print("payload  : ");
           Serial.println(payload);
           http.end();  
           Serial.println("---------------");

}
    
void Processing_incoming_data() {
           Serial.println();
           Serial.print("Send message to Master : ");
           Serial.println(Message);   
           sendMessage(Message, Destination2);
}

void setup() {
 //________________________________________________________________ khoi tao LoRa
           pinMode(LED_BUILTIN, OUTPUT);
           Serial.begin(115200);
           LoRa.setPins(ss, rst, dio0);
           
           if (!LoRa.begin(433E6)) {            
           Serial.println("LoRa init failed. Check your connections.");
           while (true);                    
             }
           Serial.println("LoRa init succeeded.");

           WiFi.mode(WIFI_STA);
           WiFi.begin(ssid,password);
           Serial.println("conecting");
           while(WiFi.status() != WL_CONNECTED)
             {
               delay(500);
               Serial.print(".");
             }
           Serial.print("ket no thanh cong");   
           Serial.println("LoRa init succeeded.");
           }

void loop() {
           if (millis() - timer > 1000) {
                        timer = millis();
                        checkCMD();   
                }
            onReceive(LoRa.parsePacket());
}



