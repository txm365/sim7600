// main.cpp

#include <Arduino.h>
#include "WiFiManagerSetup.h"
#include "OTAUpdate.h"
#include <TelnetStream.h>
#include <PubSubClient.h>
#include <WiFi.h>               // Include the WiFi library for ESP32
#include "time.h"

#define LED_PIN     12


/***************** SET PROJECT DETAILS HERE *************/
const String projectName = "sim7600";
String currentVersion = "0.4";
char sCounter[11] = "";
char sysTime[9] = "";
String deviceIdName = String(ESP.getEfuseMac(),HEX);
bool debugMode = false; //if true, frequent update checks
int updateInterval = 60; //in minutes ifdebug false
/****************************************************/

/****** DECLARE GLOBAL VARIABLES HERE *****/
static unsigned long lastLedToggle = 0;
static unsigned long lastUpdateCheck = 0; 
/*****************************************/

/******************* Time Details *************/
const char* ntpServer = "pool.ntp.org";
int timezone = 2;
const long  gmtOffset_sec = timezone*3600;
const int   daylightOffset_sec = 0;//no daylight saving observed in South Africa

/************** MQTT Details **********************/
WiFiClient espClient;           // Create a WiFiClient instance
PubSubClient  mqtt(espClient);
// MQTT details
const char *broker = "broker.hivemq.com";

String Led = deviceIdName+"/led";
String service_Init = deviceIdName+"/init";
String led_status = deviceIdName+"/ledStatus";
String timeTopic = deviceIdName+"/deviceTime";
String firmawareTopic = deviceIdName+"/firmawareUpdate";

const char *topicLed       = Led.c_str();
const char *topicInit      = service_Init.c_str();
const char *topicLedStatus = led_status.c_str();
const char *topicTime = timeTopic.c_str();
const char *firmawareUpdateTopic = firmawareTopic.c_str();

int ledStatus = LOW;
uint32_t lastReconnectAttempt = 0;
void mqttCallback(char *topic, byte *payload, unsigned int len)
{

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.write(payload, len);
    Serial.println();

    // Only proceed if incoming message's topic matches
    if (String(topic) == topicLed) {
        ledStatus = !ledStatus;
        digitalWrite(LED_PIN, ledStatus);
        Serial.print("ledStatus:");
        Serial.println(ledStatus);
        mqtt.publish(topicLedStatus, ledStatus ? "1" : "0");

    }

    else  if (String(topic) == firmawareUpdateTopic) {
      OTAUpdater::checkForUpdate(projectName, currentVersion);
    }
    
}

boolean mqttConnect()
{
    Serial.print("Connecting to ");
    Serial.print(broker);

    // Connect to MQTT Broker
    boolean status = mqtt.connect("GsmClientTest");

    // Or, if you want to authenticate MQTT:
    // boolean status = mqtt.connect("GsmClientName", "mqtt_user", "mqtt_pass");

    if (status == false) {
        Serial.println(" fail");
        return false;
    }
    
    Serial.println("=== MQTT IS CONNECTED ===");
    TelnetStream.println("=== MQTT IS CONNECTED ===");
    mqtt.publish(topicInit, "Mqtt Service started");
    mqtt.subscribe(topicLed);
    mqtt.subscribe(firmawareUpdateTopic);
    return mqtt.connected();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return;
  }
  
 sprintf(sysTime,"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min, timeinfo.tm_sec);
 mqtt.publish(topicTime, sysTime);
}

void setup() {
  Serial.begin(115200);
  Serial.printf("Initialising");
  for (uint8_t t = 5; t > 0; t--) {
    Serial.printf(".");
    Serial.flush();
    delay(1000);
  }
  Serial.println("Connecting to WIFI...");
  // Use WiFiManagerSetup to configure WiFi
  WiFiManagerSetup::setupWiFi("Configure WIFI", "AutoConnect");
//  Set the LED pin as an OUTPUT
  pinMode(LED_PIN, OUTPUT);
  TelnetStream.begin();
  Serial.println("Connected To WIFI!");
  TelnetStream.println("Connected To WIFI!");
delay(10000);
//****************************************
 Serial.println("MQTT TOPICS");
  TelnetStream.println("MQTT TOPICS");
  Serial.println(topicLed);
  TelnetStream.println(topicLed);
  Serial.println(topicInit);
  TelnetStream.println(topicInit);
  Serial.println(topicLedStatus);
  TelnetStream.println(topicLedStatus);
  // MQTT Broker setup
    mqtt.setServer(broker, 1883);
    mqtt.setCallback(mqttCallback);

    //Initialise NTP server time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
   // Start the Initial firmware update check
  OTAUpdater::checkForUpdate(projectName, currentVersion);
}

void loop() {
  /************* DO NOT DELETE THIS BLOCK ***************/
  //Run the OTA Sketch on EVERY TICK
  if(debugMode){
    updateInterval = 1; //1min
  }
  if (millis() - lastUpdateCheck >= 60000*updateInterval) {
    lastUpdateCheck = millis();

  OTAUpdater::checkForUpdate(projectName, currentVersion);
  
  }
  /******************************************************/

/**************YOUR CODE STARTS HERE *********************/
  static int secCounter = 0;
  static long int lastSecondMark =0;
  if (millis() - lastSecondMark >= 1000){
    
    lastSecondMark = millis();
    if((secCounter % 60) == 0){
    Serial.print(".");
    TelnetStream.print(". ");
    }
    secCounter++;
    sprintf(sCounter, "%d", secCounter);
    //Serial.println(sCounter);
    //TelnetStream.print(sCounter);
     printLocalTime();
    mqtt.publish(topicInit, sCounter);
  }
  // Check if 500 milliseconds have passed for LED toggle
  if (millis() - lastLedToggle >= 500) {
    lastLedToggle = millis();
    // Toggle the onboard LED
   digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  if (!mqtt.connected()) {
        Serial.println("=== MQTT NOT CONNECTED ===");
        TelnetStream.println("=== MQTT NOT CONNECTED ===");
        // Reconnect every 10 seconds
        uint32_t t = millis();
        if (t - lastReconnectAttempt > 10000L) {
            lastReconnectAttempt = t;
            if (mqttConnect()) {
                lastReconnectAttempt = 0;
            }
        }
        delay(100);
        return;
    }

    mqtt.loop();

}
