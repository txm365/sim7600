// OTAUpdate.h

#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>



class OTAUpdater {
public:
  static void update_started();
  static void update_finished();
  static void update_progress(int cur, int total);
  static void update_error(int err);

  // Check for firmware updates using HTTPS
  // Parameters:
  //   - projectName: The name of the project
  //   - currentVersion: The current version of the firmware
  static void checkForUpdate(const String &projectName, const String &currentVersion);
};

#endif  // OTA_UPDATE_H
