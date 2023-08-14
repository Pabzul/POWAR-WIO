#include "rpcWiFi.h"

const char* ssid = "DOCTORS";
const char* password =  "D0W3R$123456";

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
    tft.println("Connecting WiFi...");
    WiFi.begin(ssid, password);
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address: ");
  tft.println("Connected to the WiFi network");
  tft.println("IP Address: ");
  Serial.println(WiFi.localIP()); // prints out the device's IP address
  tft.println(WiFi.localIP()); // prints out the device's IP address
}