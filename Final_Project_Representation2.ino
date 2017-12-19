/*******************************************************************
 *  A project to light up leds based on the current traffic       
 *  conditions from University of Regina to the Airport.                          
 *  Traffic data is being sourced from Google Maps
 *  
 *  Main Hardware:
 *  - ESP8266
 *  - Neopixels
 *                                                                 
 *  Written by Yang Chen, edited from Golden Bridge project by Brian Lough                                         
 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include "FS.h"

// ----------------------------
// Additional libraries - each one of these will need to be installed.
// ----------------------------

#include <WiFiManager.h>
// For configuring the Wifi credentials without re-programing 
// Availalbe on library manager (WiFiManager)
// https://github.com/tzapu/WiFiManager

#include <GoogleMapsApi.h>
// For accessing Google Maps Api
// Availalbe on library manager (GoogleMapsApi)
// https://github.com/witnessmenow/arduino-google-maps-api

#include <ArduinoJson.h>
// For parsing the response from google maps and for the config file
// Available on the library manager (ArduinoJson)
// https://github.com/bblanchon/ArduinoJson

#include <DoubleResetDetector.h>
// For entering Config mode by pressing reset twice
// Not yet available on the library manager
// Go to the github page and there is a download button
// Click Download as zip, and add to Arduino IDE(Sketch->Include Library-> Add .zip library)
// https://github.com/datacute/DoubleResetDetector

#include <Adafruit_NeoPixel.h>
// For controlling the Addressable LEDs
// Available on the library manager (Adafruit Neopixel)
// https://github.com/adafruit/Adafruit_NeoPixel

#include <NTPClient.h>
// For keeping the time, incase we want to do anything based on time
// Available on the library manager (NTPClient)
// https://github.com/arduino-libraries/NTPClient

#include <Servo.h>

char apiKey[45] = "AIzaSyA0WdVqzk4Nw2f08GaEvMU97wvlkufLJiU";

Servo myservo;

String point1 = "50.424536, -104.595330";  // Wascana Parkway & 23th Ave
String point2 = "50.423426, -104.618089";  // 23th Ave & Albert St
String point3 = "50.434832, -104.618060";  // Ablert St & Regina Ave

int ledPins[] = {2,3,4,5,6,7};

int pos = 0;

int input[] = {0,0,0};

int temp;



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

myservo.attach(9);

for(int i = 0; i < 6; i++)
  pinMode(ledPins[i],OUTPUT);

if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  loadConfig();

  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // Adding an additional config on the WIFI manager webpage for the API Key
  WiFiManagerParameter customApiKey("apiKey", "API Key", apiKey, 50);
  wifiManager.addParameter(&customApiKey);


  if (drd.detectDoubleReset()) {
    Serial.println("Double Reset Detected");
    wifiManager.startConfigPortal("BayDisplayConf", "thepassword");
  } else {
    Serial.println("No Double Reset Detected");
    wifiManager.autoConnect("BayDisplayConf", "thepassword");
  }

  strcpy(apiKey, customApiKey.getValue());

  if (shouldSaveConfig) {
    saveConfig();
  }

  mapsApi = new GoogleMapsApi(apiKey, client);

  colour = leds.Color(255, 0, 0);

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  timeClient.begin();
  drd.stop();
/*
myservo.write(pos);
delay(2000);
myservo.write(90);
delay(2000);*/

}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long timeNow = millis();
  if (timeNow > api_due_time)  {
    Serial.println("Checking maps");
    if (checkGoogleMaps()) {
      lightLeds(colour);
    } 
    api_due_time = timeNow + delayBetweenApiCalls;
    led_due_time = timeNow + delayBetweenLedChange;
  }
  timeNow = millis();
  if (timeNow > led_due_time)  {
    Serial.println("Chaning LED");
    twinkleLed();
    led_due_time = timeNow + delayBetweenLedChange;
  }

  
  pos = 0;

for(int i = 0; i < 6; i++)
{
  digitalWrite(ledPins[i], LOW);
}

for(int i = 0; i < 3; i++)
{
  input[i] = random(0 , 100);
  pos += input[i];
  Serial.print(input[i]);
  Serial.print("   ");
  if(input[i] < 50)
  {
    temp = input[i] * 1023 / 50;
    analogWrite(ledPins[i*2],temp);
  }
  else
  {
    temp = (input[i] - 50) * 1023 / 50;
    analogWrite(ledPins[i*2+1], temp);
  }
}
Serial.println(" ");

if(pos >= 150)
myservo.write(160);
else
myservo.write(0);
delay(5000);
}


DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

WiFiClientSecure client;
GoogleMapsApi *mapsApi;

unsigned long api_due_time = 0;

unsigned long led_due_time = 0;

uint32_t colour;

int colourIndex = 0;

// flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  //We might want to indicate on the LEDS somehow that we are in config mode
  
  drd.stop();
}


bool loadConfig() {
  File configFile = SPIFFS.open(BRIDGE_CONFIG_FILE, "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  strcpy(apiKey, json["mapsApiKey"]);
  return true;
}

bool saveConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["mapsApiKey"] = apiKey;

  File configFile = SPIFFS.open(BRIDGE_CONFIG_FILE, "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  return true;
}

bool checkGoogleMaps() {
  Serial.println("Getting traffic for " + point1 + " to " + point3);
    String responseString = mapsApi->distanceMatrix(point1, point3, "now");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& response = jsonBuffer.parseObject(responseString);
    if (response.success()) {
      if (response.containsKey("rows")) {
        JsonObject& element = response["rows"][0]["elements"][0];
        String status = element["status"];
        if(status == "OK") {

          int durationInSeconds = element["duration"]["value"];
          int durationInTrafficInSeconds = element["duration_in_traffic"]["value"];
          colour = getColour(durationInTrafficInSeconds, durationInSeconds);
          Serial.println("Duration In Traffic:  " + durationInTrafficInSeconds);
          return true;

        }
        else {
          Serial.println("Got an error status: " + status);
          return false;
        }
      } else {
        Serial.println("Reponse did not contain rows");
        return false;
      }
    } else {
      if(responseString == ""){
        Serial.println("No response, probably timed out");
      } else {
        Serial.println("Failed to parse Json. Response:");
        Serial.println(responseString);
      }

      return false;
    }

    return false;
}

