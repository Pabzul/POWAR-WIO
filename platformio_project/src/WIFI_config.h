#pragma once

#include <rpcWiFi.h>
// needed for WiFi Manager library
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

WiFiManager wifiManager;

void configModeCallback(WiFiManager* myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  wifiConfigModeCallbackScreen();
}

void setupWiFi() {
  // only uncomment this for testing
  // wifiManager.resetSettings();
  Serial.println(wifiManager.getSSID());
  wifiManager.setAPCallback(configModeCallback);
  if (!wifiManager.autoConnect("POWAR STEAM")) {
    NVIC_SystemReset();
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // prints out the device's IP address
}
