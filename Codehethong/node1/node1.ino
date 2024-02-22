#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <Wire.h>
#include <SimpleKalmanFilter.h>
#include <AHT20.h>
AHT20 aht20;


#define ss PB4
#define rst PB5
#define dio0 PB3
#define ADC1 PA0
#define LED1 PA1
#define BUZZER PA2
#define led PC13
//==================================
const float lsbPerG = 16384.0;
float AccX,AccY,AccZ,AngleRoll,AnglePitch;
int value = 0,state =0;
float gmax=0,g =0;
long timervib = 0;
long timergas = 0;
long timerw = 0;
int count =1;
SimpleKalmanFilter kalmanG(1, 1, 0.001);
//==================================
String gas = "";
String vib = "";
String Incoming = "";
String Message = "";            
byte LocalAddress = 0x31;            
byte Destination2 = 0x32;  
String outgoing;              
   
//==================================


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
  if (packetSize == 0) return;  //--> if there's no packet, return


  //---------------------------------------- read packet header bytes:
  int recipient = LoRa.read();        //--> recipient address
  byte sender = LoRa.read();          //--> sender address
 
  //----------------------------------------




  Incoming = "";


  //---------------------------------------- Get all incoming data.
  while (LoRa.available()) {
    Incoming += (char)LoRa.read();
  }
 
  if (recipient != LocalAddress) {
   
    return;
  }
    else
    {
          if (Incoming == "00" || Incoming =="01" )
            {
            digitalWrite(LED1,HIGH);
            digitalWrite(BUZZER,HIGH);
            }
          if (Incoming == "10" || Incoming =="11" )
          {// digitalWrite(BUZZER,LOW);
          //  digitalWrite(LED1,LOW);
          }
    }
  //----------------------------------------


 


 
  Processing_incoming_data();
}
//________________________________________________________________________________


//________________________________________________________________________________ Subroutines to process data from incoming messages, then send messages to the Master.
void Processing_incoming_data() {
 
 Message = vib;
   
    if ( gas.length()==2) Message +=gas+ "   ";
     if ( gas.length()==3) Message +=gas+ "  ";
       if ( gas.length()==4) Message +=gas+ " ";
         if ( gas.length()==1) Message +=gas+ "    ";
          if ( gas.length()==5) Message +=gas;
   // Message +="ok";
    sendMessage(Message,Destination2);




}






void setup() {
  Serial.begin(9600);
  Wire.setClock(100000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  pinMode(LED1, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(BUZZER, HIGH);
  // ------- Khoi tao LoRa
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
    }


  Serial.println("LoRa init succeeded.");
}


void loop() {
   
    onReceive(LoRa.parsePacket());
   
   if(millis() - timervib > 300) {
  gyro_signals();
   float X=1.023*sin(AnglePitch/180*3.142);
   float Y=1.023*sin(AngleRoll/180*3.142);
   float Z= sqrt(1.023*1.023-X*X-Y*Y);
   g = abs(sqrt((AccX-X)*(AccX-X)+(AccY-Y)*(AccY-Y)+(AccZ-Z)*(AccZ-Z)));
   float Acc = kalmanG.updateEstimate(g*16384);
   g = Acc/16384;
   if ( abs(g) < 0.001) g =0.001;
   if ( g > gmax) gmax = g;
   vib= String(gmax,3);
   
  count++;
  if (count==3) {
                  count =1;
                  gmax  =0.0001;
                 
                 }
 timervib = millis();
   }

 if (millis() - timerw > 500)
    { if (g > 0.0276 || value > 200 || Incoming == "1")
        {
          digitalWrite(BUZZER,LOW);
          if (state == 0) {
                          digitalWrite(LED1,LOW);
                          state =1;
                          } else
                          {
                          digitalWrite(LED1,HIGH);
                          state =0;
                          }
        }
        if (g < 0.0276 && value < 200 && Incoming == "0")
                {
                 digitalWrite(LED1,HIGH);
                 digitalWrite(BUZZER,HIGH); 
                }
      timerw = millis();
      
     }

  if (millis() - timergas > 5000)
    {
      timergas = millis();
     value = analogRead(ADC1);
     }
    gas = String(value);
   }




void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();




  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission();




  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t AccXLSB = Wire.read()<<8|Wire.read();
  int16_t AccYLSB = Wire.read()<<8|Wire.read();
  int16_t AccZLSB = Wire.read()<<8|Wire.read();
   
 
  AccX= (AccXLSB/16384.0);
  AccY= (AccYLSB/16384.0);
  AccZ= (AccZLSB/16384.0);
 
 
  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))/(3.142/180);
  AnglePitch=atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))/(3.142/180);
}






