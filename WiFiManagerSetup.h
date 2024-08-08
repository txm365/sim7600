// WiFiManagerSetup.h

#ifndef WIFI_MANAGER_SETUP_H
#define WIFI_MANAGER_SETUP_H

#include <Arduino.h>
#include <WiFiManager.h>

class WiFiManagerSetup {
public:
  static void setupWiFi(const String &apName, const String &ssidPrefix);
};

#endif  // WIFI_MANAGER_SETUP_H
