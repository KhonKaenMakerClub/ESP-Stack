#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//--------- temperature --------//
#define ONE_WIRE_BUS 4  // DS18B20 pin <<<<<< PIN SWAP !!! WTF 4 and 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
const float ADJ_TEMP = 0;
//--------- wifi --------//
const char* ssid     = "KKMakerClub";
const char* password = "---- password ----";
const char* host = "dweet.io";
//----------Util---------//
#define DEBUG 0
const unsigned long DEEP_SLEEP_MIN = 60;
void setup() {
  if(DEBUG){
    Serial.begin(115200);
    delay(10);      
    Serial.print("Connecting to ");
    Serial.println(ssid);
  } 
  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) { //Waiting Wifi Connect
    delay(500);
  }
  if(DEBUG){
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  //------ setup digital pins -------//
  pinMode(5,OUTPUT);  
  digitalWrite(5,HIGH);
}
float getTemp(){  
  float temp;
  digitalWrite(5,HIGH);
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
  } while (temp == 85.0 || temp == (-127.0));
  if(DEBUG){
    Serial.print("Temperature: ");
    Serial.println(temp);
  }
  digitalWrite(5,LOW);
  return temp + ADJ_TEMP;
}
void loop() {
  delay(5000);   
  //------ Setup URI -----//  
  //https://dweet.io/dweet/for/my-thing-name?hello=world&foo=bar
  String url = "/dweet/for/kkmakerclub-test?temp=";  
  url += getTemp();
  if(DEBUG){
    Serial.print("Requesting URL: ");
    Serial.println(url);
  }
  //---- connect host ---//
  WiFiClient client;
  if(DEBUG){
    Serial.print("connecting to ");
    Serial.println(host);
  }
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {    
    //Serial.println("connection failed");
    return;
  }  
  //----- Send Data -----//
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  //------ sleep -------//
  if(DEBUG){
    Serial.println("--Deep Sleep--");
    
  }
  ESP.deepSleep(DEEP_SLEEP_MIN*60*1000000L,WAKE_RF_DEFAULT);
}

