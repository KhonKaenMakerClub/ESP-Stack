//=================================//
//        ESP-Stack framework      //
// Copyright Khon Kaen Maker Club  //
// By Comdet Phaudphut             // 
//=================================//

//---- wifi connector by CMMC
#include <ESP8266WiFi.h>
#include <WiFiConnector.h>

#define WIFI_SSID        ""
#define WIFI_PASSPHARSE  ""

WiFiConnector wifi;
//---- dht11 library ---
#include "DHT.h"
#define DHTVCC 12
#define DHTPIN 13
#define DHTTYPE DHT11 //DHT22 DHT21
DHT dht(DHTPIN, DHTTYPE);
float humidity,temperature;
//---- IR Reader -----
#include <IRremoteESP8266.h>
#define RECV_PIN 0
#define RECV_VCC 4
#define RECV_GND 5
#define SEND_PIN 14
IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN);
decode_results results;
//---- Server 
#include <ArduinoJson.h>

const char* host = "dweet.io";
const char* thing_name = "kkmc_espstack";
const int sleeping = 5*1000;
StaticJsonBuffer<320> jsonBuffer;
//---- Blynk
#include <SimpleTimer.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h>
char auth[] = "e470c185a1ff42b6a66c908a94144c40";
SimpleTimer timer;
//================================//
void init_dht()
{
  //--- enable pins and apply power
  pinMode(DHTVCC,OUTPUT);
  digitalWrite(DHTVCC,HIGH);
  pinMode(DHTPIN,INPUT);
  dht.begin();
}
void init_wifi()
{  
  wifi = WiFiConnector(WIFI_SSID, WIFI_PASSPHARSE);
  Serial.print("CONNECTING TO ");
  Serial.println(wifi.SSID() + ", " + wifi.psk());
  
  wifi.on_connecting([&](const void* message)
  {
    char buffer[70];
    sprintf(buffer, "[%d] connecting -> ", wifi.counter);
    Serial.print(buffer);
    Serial.println((char*) message);
    delay(500);
  });
  wifi.on_connected([&](const void* message)
  {    
    Serial.print("WIFI CONNECTED => ");
    Serial.println(WiFi.localIP());
    ///------------------//
    Blynk.begin(auth);    
    //-------------------//
  });  
  Serial.println("WIFI Connecting");
  wifi.connect();
}
void init_ir()
{
  pinMode(RECV_VCC,OUTPUT);  
  pinMode(RECV_GND,OUTPUT);  
  pinMode(RECV_PIN,INPUT);
  digitalWrite(RECV_VCC,HIGH);
  digitalWrite(RECV_GND,LOW);
  irrecv.enableIRIn();

  irsend.begin();
}
void init_serial()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("ESP-Stack");
}
void init_timer()
{
  timer.setInterval(5000L, processDHT);  
  timer.setInterval(1000L, processWifi);
}
void init_rgb()
{
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  analogWrite(5,1023);
  analogWrite(0,1023);
  analogWrite(2,1023);
}
//================================//
void processWifi()
{
  wifi.loop();
}
void processDHT()
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }else{

  }
}
void processIR()
{
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
//================================//
void setup() {
  init_serial();
  init_wifi();
  init_dht();
  init_rgb();
  init_timer();
}

void loop() {
  Blynk.run();
  timer.run();
}
BLYNK_READ(V1){  
    Blynk.virtualWrite(V0, millis() / 1000);
    Blynk.virtualWrite(V1, temperature);
}
BLYNK_READ(V2){
    Blynk.virtualWrite(V0, millis() / 1000);
    Blynk.virtualWrite(V2, humidity);  
}
