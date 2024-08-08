// WiFiManagerSetup.cpp

#include "WiFiManagerSetup.h"

void WiFiManagerSetup::setupWiFi(const String &apName, const String &ssidPrefix) {
  WiFiManager wifiManager;

  // Set callback for config mode
  wifiManager.setAPCallback([](WiFiManager *myWiFiManager) {
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    Serial.println("Config Portal Running");
  });

  // Uncomment the following line to reset saved WiFi credentials
  //wifiManager.resetSettings();

  // Try to connect to saved WiFi credentials
  if (!wifiManager.autoConnect(apName.c_str())) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    // Reset and try again or put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  // If connected, print WiFi credentials
  Serial.println("Connected to WiFi!");
  Serial.println("WiFi SSID: " + WiFi.SSID());
}
