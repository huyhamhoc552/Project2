#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <Wire.h>
#include <SimpleKalmanFilter.h>
#include <AHT20.h>
AHT20 aht20;
int Slv =1;
#define ss PB4
#define rst PB5
#define dio0 PB3
#define ADC1 PA0
#define LED1 PA1
#define led PC13
float humidity = 0;
float temperature = 0;
String Incoming = "";
String Incoming1 = "";
String Message = "";
String Message1 = "";
String Message0 = ""; 
String dataS = "";            
int key = 1,state =0;
byte LocalAddress = 0x32;             
byte Destination0 = 0x30;  
byte Destination1 = 0x31;  
String outgoing;              
    
long lastSendTime = 0,Timetemp=0,timerw;        // last send time
int interval = 2000;  


void sendMessage(String Outgoing, byte Destination) {
  LoRa.beginPacket();             //--> start packet
  LoRa.write(Destination);        //--> add destination address
  LoRa.write(LocalAddress);       //--> add sender address
 
  LoRa.print(Outgoing);           //--> add payload
  LoRa.endPacket();               //--> finish packet and send it
}
//________________________________________________________________________________ 

//________________________________________________________________________________ Subroutines for receiving data (LoRa Ra-02).
void onReceive(int packetSize) {
  if (packetSize == 0) return;  
  //---------------------------------------- read packet header bytes:
  int recipient = LoRa.read();        //--> recipient address
  byte sender = LoRa.read();          //--> sender address
 
  Incoming = "";
  while (LoRa.available()) {
    Incoming += (char)LoRa.read();
  }
  
  if (recipient != LocalAddress) {

    return; //--> skip rest of function
  }
   else {
         // if (Incoming == "00" || Incoming =="10" ) ;
           // digitalWrite(LED1,HIGH);
        //  if (Incoming == "01" || Incoming =="11" ) 
          //  digitalWrite(LED1,LOW);           ;           
        }
  if (int(sender)==Destination1 ) Message0 = Incoming;
  if (int(sender)==Destination0 ) Message1 = Incoming;
 
}
void setup() {
  Serial.begin(9600);
  
  Wire.begin();
  aht20.begin();
  //pinMode(led, OUTPUT);
  pinMode(LED1, OUTPUT);
  //digitalWrite(led,HIGH);
  digitalWrite(LED1,HIGH);

  // ------- Khoi tao LoRa
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    
    while (1);
    }

  
}

void loop() {
     
  if (millis() - Timetemp > 5000) {
    if (aht20.available() == true)
  {
     temperature = aht20.getTemperature();
     humidity = aht20.getHumidity();
    

    dataS = String(int(temperature*10)) + String(int(humidity*10));}
    Timetemp = millis();
  }
String str1 = Message0.substring(0, 5);
String str2 = Message0.substring(5, 10);
float vib = str1.toFloat();
float gas = str2.toFloat();
if (millis() - timerw > 600) {
if (vib > 0.0276 || gas > 200 || temperature > 40) 
       {str1="1";
        if (state == 0) {
                          digitalWrite(LED1,LOW);
                          state =1;
                          } else
                          {
                          digitalWrite(LED1,HIGH);
                          state =0;
                          }
       }
if (vib < 0.0276 && gas < 200 && temperature < 40) 
      {str1="0";
       digitalWrite(LED1,HIGH);
      }
 timerw = millis();
}
  if (millis() - lastSendTime > 600) {
    lastSendTime = millis();
     Slv++;
    if (Slv > 2) Slv = 1;                
                  
    if (Slv == 1)  
    
      sendMessage(str1, Destination1);
    if (Slv == 2) 
      { if (Message0 =="" ) Message0="          ";
      sendMessage(Message0+dataS+str1, Destination0); }
    }
   
  onReceive(LoRa.parsePacket());
}



