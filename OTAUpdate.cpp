 // OTAUpdate.cpp

#include "OTAUpdate.h"
#include <TelnetStream.h>
#include <WiFi.h>
const char* rootCACertificate = \
  // Your root CA certificate content
  //open gitbash
  //type: openssl s_client -showcerts -connect example.url.com:443
  //Take the second certificate
  // format it at:  https://unreeeal.github.io/ssl_esp.html

 "-----BEGIN CERTIFICATE-----\n" \
"MIIFBTCCAu2gAwIBAgIQS6hSk/eaL6JzBkuoBI110DANBgkqhkiG9w0BAQsFADBP\n" \
"MQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJuZXQgU2VjdXJpdHkgUmVzZWFy\n" \
"Y2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBYMTAeFw0yNDAzMTMwMDAwMDBa\n" \
"Fw0yNzAzMTIyMzU5NTlaMDMxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBF\n" \
"bmNyeXB0MQwwCgYDVQQDEwNSMTAwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
"AoIBAQDPV+XmxFQS7bRH/sknWHZGUCiMHT6I3wWd1bUYKb3dtVq/+vbOo76vACFL\n" \
"YlpaPAEvxVgD9on/jhFD68G14BQHlo9vH9fnuoE5CXVlt8KvGFs3Jijno/QHK20a\n" \
"/6tYvJWuQP/py1fEtVt/eA0YYbwX51TGu0mRzW4Y0YCF7qZlNrx06rxQTOr8IfM4\n" \
"FpOUurDTazgGzRYSespSdcitdrLCnF2YRVxvYXvGLe48E1KGAdlX5jgc3421H5KR\n" \
"mudKHMxFqHJV8LDmowfs/acbZp4/SItxhHFYyTr6717yW0QrPHTnj7JHwQdqzZq3\n" \
"DZb3EoEmUVQK7GH29/Xi8orIlQ2NAgMBAAGjgfgwgfUwDgYDVR0PAQH/BAQDAgGG\n" \
"MB0GA1UdJQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATASBgNVHRMBAf8ECDAGAQH/\n" \
"AgEAMB0GA1UdDgQWBBS7vMNHpeS8qcbDpHIMEI2iNeHI6DAfBgNVHSMEGDAWgBR5\n" \
"tFnme7bl5AFzgAiIyBpY9umbbjAyBggrBgEFBQcBAQQmMCQwIgYIKwYBBQUHMAKG\n" \
"Fmh0dHA6Ly94MS5pLmxlbmNyLm9yZy8wEwYDVR0gBAwwCjAIBgZngQwBAgEwJwYD\n" \
"VR0fBCAwHjAcoBqgGIYWaHR0cDovL3gxLmMubGVuY3Iub3JnLzANBgkqhkiG9w0B\n" \
"AQsFAAOCAgEAkrHnQTfreZ2B5s3iJeE6IOmQRJWjgVzPw139vaBw1bGWKCIL0vIo\n" \
"zwzn1OZDjCQiHcFCktEJr59L9MhwTyAWsVrdAfYf+B9haxQnsHKNY67u4s5Lzzfd\n" \
"u6PUzeetUK29v+PsPmI2cJkxp+iN3epi4hKu9ZzUPSwMqtCceb7qPVxEbpYxY1p9\n" \
"1n5PJKBLBX9eb9LU6l8zSxPWV7bK3lG4XaMJgnT9x3ies7msFtpKK5bDtotij/l0\n" \
"GaKeA97pb5uwD9KgWvaFXMIEt8jVTjLEvwRdvCn294GPDF08U8lAkIv7tghluaQh\n" \
"1QnlE4SEN4LOECj8dsIGJXpGUk3aU3KkJz9icKy+aUgA+2cP21uh6NcDIS3XyfaZ\n" \
"QjmDQ993ChII8SXWupQZVBiIpcWO4RqZk3lr7Bz5MUCwzDIA359e57SSq5CCkY0N\n" \
"4B6Vulk7LktfwrdGNVI5BsC9qqxSwSKgRJeZ9wygIaehbHFHFhcBaMDKpiZlBHyz\n" \
"rsnnlFXCb5s8HKn5LsUgGvB24L7sGNZP2CX7dhHov+YhD+jozLW2p9W4959Bz2Ei\n" \
"RmqDtmiXLnzqTpXbI+suyCsohKRg6Un0RC47+cpiVwHiXZAW+cn8eiNIjqbVgXLx\n" \
"KPpdzvvtTnOPlC7SQZSYmdunr3Bf9b77AiC/ZidstK36dRILKz7OA54=\n" \
"-----END CERTIFICATE-----\n";

void OTAUpdater::update_started() {
  Serial.println("HTTPS update process started");
  TelnetStream.println("HTTPS update process started");
}

void OTAUpdater::update_finished() {
  Serial.println("HTTPS update process finished");
  TelnetStream.println("HTTPS update process finished");
}

void OTAUpdater::update_progress(int cur, int total) {
  // Update progress
  static int latestProgress = -1;
  int progress = (cur*100)/total;
  if(progress>latestProgress){
    latestProgress =progress;
  Serial.printf("Downloading firmware: %d%%\n", progress);
  TelnetStream.print("Downloading firmware: ");
  TelnetStream.print(progress);
  TelnetStream.println("%");
  //Serial.printf("Downloading %d of %d bytes...\n", cur, total);
  }
}

void OTAUpdater::update_error(int err) {
  Serial.printf("CALLBACK: HTTPS update fatal error code %d\n", err);
  TelnetStream.print("CALLBACK: HTTPS update fatal error code: ");
  TelnetStream.println(err);
}

void OTAUpdater::checkForUpdate(const String &projectName, const String &currentVersion) {
  // Define the update URL in this file
    String deviceName =String(ESP.getEfuseMac(),HEX);
    String macAddr = WiFi.macAddress();
    String fileName = projectName+".ino.esp32.bin"; //firmware.bin on vscode
    

    WiFiClientSecure client;  // Use WiFiClientSecure for HTTPS

    // Define the update URL in this file
    
     String updateUrl = "https://otaup.com/update_server.php?project_name=" + projectName + "&file_name=" + currentVersion + "_" + fileName+"&device_name="+deviceName+"&device_macaddress="+macAddr;
     Serial.println("\nDevice: "+deviceName+" | Project Name: " + projectName + " | Version: v" + currentVersion);
     //TelnetStream.println("\nDevice: "+deviceName+" | Project Name: " + projectName + " | Version: v" + currentVersion);
    Serial.println("Checking for firmware updates...");
    TelnetStream.println("\nChecking for firmware updates...");
    /*if(debugMode){
      Serial.println(updateUrl);
    }*/
    // Set the root CA certificate for secure communication
    client.setCACert(rootCACertificate);

    HTTPClient http;

    if (http.begin(client, updateUrl)) {
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.print(payload);
        TelnetStream.print(payload);
        if (payload.equals("No update available.")) {
          Serial.println("\nDevice: "+deviceName+" | Project Name: " + projectName + " | Version: v" + currentVersion);
          TelnetStream.println("\nDevice: "+deviceName+" | Project Name: " + projectName + " | Version: v" + currentVersion);
        } else {
          Serial.println("Update available. \nDownloading and installing...");
          TelnetStream.println("Update available. \nDownloading and installing...");
          httpUpdate.onStart(update_started);
          httpUpdate.onEnd(update_finished);
          httpUpdate.onProgress(update_progress);
          httpUpdate.onError(update_error);

          t_httpUpdate_return ret = httpUpdate.update(client, updateUrl.c_str());

          switch (ret) {
            case HTTP_UPDATE_FAILED:
              Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
              break;

            case HTTP_UPDATE_NO_UPDATES:
              Serial.println("HTTP_UPDATE_NO_UPDATES");
              break;

            case HTTP_UPDATE_OK:
              Serial.println("HTTP_UPDATE_OK");
              // Perform any additional tasks after a successful update if needed
              break;
          }
        }
      } else {
        Serial.printf("HTTP request failed with error code: %d\n", httpCode);
        TelnetStream.print("HTTP request failed with error code: ");
        TelnetStream.println(httpCode);
      }
      http.end();
    } else {
      Serial.println("Unable to connect to the server.");
      TelnetStream.print("Unable to connect to the server.");
      
    }
  
}
