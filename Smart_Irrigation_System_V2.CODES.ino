/*
 Smart Irrigation System V2
 Made by Paavan Sharma

 Hardware:
 ESP32
 FC-28 Soil Moisture Sensor
 DHT11
 SSD1306 OLED
 Active Buzzer
 Blynk IoT

 Pump is manual. The system only recommends START/STOP.
*/

// YOU MUST CONNECT IT TO WIFI/MOBILE HOTSPOT ELSE IT WORK START UP!!!!
#define BLYNK_TEMPLATE_ID "YOURTEMPLATEID "
#define BLYNK_TEMPLATE_NAME "Soil Moisture Sensor"
#define BLYNK_AUTH_TOKEN "BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

char ssid[] = "WIFI_NAME";
char pass[] = "WIFI_PASS";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define soilPin 32
#define buzzerPin 19
#define DHTPIN 23
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

int soilRaw=0;
int soilValue=0;
float temp=0;
float hum=0;

bool pumpState=false;
bool buzzerEnable=true;

String soilStatus="";

BlynkTimer timer;

void readSensors();
void oled();
void buzzer();
void sendData();
void checkWiFi();

void setup()
{
  Serial.begin(115200);

  pinMode(buzzerPin,OUTPUT);
  digitalWrite(buzzerPin,LOW);

  Wire.begin(21,22);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();

  dht.begin();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,20);
  display.print("HI");
  display.setCursor(5,45);
  display.print("PAAVAN");
  display.display();

  digitalWrite(buzzerPin,HIGH);
  delay(200);
  digitalWrite(buzzerPin,LOW);
  delay(1800);

  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED) delay(500);

  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass);
  timer.setInterval(1000L,sendData);
}

void loop()
{
  checkWiFi();

  if(Blynk.connected()) Blynk.run();

  timer.run();

  readSensors();
  buzzer();
  oled();
}

void readSensors()
{
  long sum=0;
  for(int i=0;i<10;i++)
  {
    sum+=analogRead(soilPin);
    delay(5);
  }

  soilRaw=sum/10;
//you can find the readings in the serial monitor (CTRL+SHIFT+M)
  // Air ~4095 (reading of sensor in air)
  // Wet soil ~3200
  if(soilRaw>=4080)
    soilValue=0;
  else
  {
    soilValue=map(soilRaw,4040,3200,0,100);
    soilValue=constrain(soilValue,0,100);
  }

  temp=dht.readTemperature();
  hum=dht.readHumidity();

  if(isnan(temp)) temp=0;
  if(isnan(hum)) hum=0;

  if(soilValue<30) soilStatus="START PUMP";
  else if(soilValue<80) soilStatus="NORMAL";
  else soilStatus="STOP PUMP";

  Serial.print("Raw:");
  Serial.print(soilRaw);
  Serial.print(" Soil:");
  Serial.print(soilValue);
  Serial.print("% Temp:");
  Serial.print(temp);
  Serial.print(" Hum:");
  Serial.println(hum);
}

void buzzer()
{
  if(!buzzerEnable){ digitalWrite(buzzerPin,LOW); return; }

  if(soilValue<30){
    digitalWrite(buzzerPin,HIGH); delay(150);
    digitalWrite(buzzerPin,LOW); delay(1850);
  }
  else if(soilValue<80){
    digitalWrite(buzzerPin,LOW);
  }
  else if(soilValue<100){
    digitalWrite(buzzerPin,HIGH); delay(80);
    digitalWrite(buzzerPin,LOW); delay(320);
  }
  else{
    digitalWrite(buzzerPin,HIGH);
  }
}

void oled()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(5,0);
  display.print("SMART IRRIGATION V2");
  display.drawLine(0,10,127,10,WHITE);

  display.setCursor(0,18);
  display.print("Soil : "); display.print(soilValue); display.print("%");

  display.setCursor(0,30);
  display.print("Temp : "); display.print(temp); display.print(" C");

  display.setCursor(0,42);
  display.print("Hum  : "); display.print(hum); display.print("%");

  display.setCursor(0,54);
  display.print("WiFi:");

  if(WiFi.status()==WL_CONNECTED) display.print("OK");
  else display.print("OFF");

  display.setCursor(70,54);
  display.print(soilStatus);

  display.display();
}

BLYNK_WRITE(V6){ pumpState=param.asInt(); }
BLYNK_WRITE(V8){ buzzerEnable=param.asInt(); }

void sendData()
{
  Blynk.virtualWrite(V0,soilValue);
  Blynk.virtualWrite(V1,WiFi.status()==WL_CONNECTED);
  Blynk.virtualWrite(V3,temp);
  Blynk.virtualWrite(V4,hum);
  Blynk.virtualWrite(V5,soilStatus);
  Blynk.virtualWrite(V7,pumpState?"PUMP ON":"PUMP OFF");
}

void checkWiFi()
{
  if(WiFi.status()==WL_CONNECTED) return;

  WiFi.disconnect();
  WiFi.begin(ssid,pass);

  unsigned long t=millis();
  while(WiFi.status()!=WL_CONNECTED && millis()-t<10000)
    delay(500);
}
