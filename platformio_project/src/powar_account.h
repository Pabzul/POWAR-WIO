#pragma once

#include <SPI.h>
#include <Seeed_FS.h>
#include "Controls.h"
#include "SD/Seeed_SD.h"
#include <rpcWiFi.h>
#include <HTTPClient.h>

struct powar_account {
  String region_id;
  String device_id;
};

/*
Uses Seeed/FS & SD libraries for reading from SD card
accepts powar_account struct as argument
attempts to read account information from file on SD card
if account information is found:
    assign read information to powar_account struct
    return 0
if file is not found (account not setup):
    return 1
if file is found but file is invalid (account setup but invalid?):
    return 2
if file could not be opened:
    return 3
*/
typedef enum {
  POWAR_ACCOUNT_SUCCESS = 0,
  POWAR_ACCOUNT_NOT_FOUND = 1,
  POWAR_ACCOUNT_INVALID = 2,
  POWAR_ACCOUNT_FILE_ERROR = 3
} powar_account_status;

int read_powar_account(powar_account &account) {
  // check if file exists on SD card
  if (!SD.exists("/powar_account.txt")) {
    return POWAR_ACCOUNT_NOT_FOUND;
  }
  // open file
  File account_file = SD.open("/powar_account.txt");
  // check if file is open
  if (!account_file) {
    return POWAR_ACCOUNT_FILE_ERROR;
  }
  // read region_id from file and convert to string
  String region_id = account_file.readStringUntil('\n');
  if (region_id.length() == 0) {
    return POWAR_ACCOUNT_INVALID;
  }
  // read device_id from file
  String device_id = account_file.readStringUntil('\n');
  if (device_id.length() == 0) {
    return POWAR_ACCOUNT_INVALID;
  }
  // assign read information to powar_account struct
  account.region_id = region_id;
  account.device_id = device_id;
  return POWAR_ACCOUNT_SUCCESS;
}

void handleAccount() {
  powar_account account;
  int account_status = read_powar_account(account);
  if (account_status == POWAR_ACCOUNT_NOT_FOUND) {
    // account not setup
    // ask to create a new pairing request
    accountNotSetupScreen();
    // wait for user to press A button
    while (!buttons.isAPressed())
      ;
    accountPairingScreen();
    // http request to server to get pairing code
    HTTPClient http;
    http.begin("http://192.168.1.120:3000/api/pair?type=start");
    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header
    // get MAC address of device
    uint8_t mac[6];
    WiFi.macAddress(mac);
    String mac_address = "";
    for (int i = 0; i < 6; i++) {
      mac_address += String(mac[i], HEX);
    }
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.POST(mac_address);
    // httpCode will be negative on error
    if (httpCode <= 0 || httpCode != HTTP_CODE_OK) {
      Serial.printf("[HTTP] POST... failed, error: %s\n",
                    http.errorToString(httpCode).c_str());
      tft.fillScreen(TFT_BLACK);
      tft.println("Error occured, pairing failed");
      http.end();
      while (!buttons.isAPressed()) {
        // wait for user to press A button
      }
      // TODO control flow to try again
      return;
    }
    // HTTP header has been sent and Server response header has been handled
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    // successful response
    String pairing_code = http.getString();
    Serial.println(pairing_code);
    http.end();
    tft.fillScreen(TFT_BLACK);
    accountPairingCodeScreen(pairing_code);
    // wait for user to press A button
    while (!buttons.isAPressed())
      ;
    // retry pairing request until successful
    while (true) {
      accountPairingScreen();
      // call pair again with pairing code to check if pairing was
      // successful
      http.begin("http://192.168.1.120:3000/api/pair?type=verify");
      Serial.print("[HTTP] POST...\n");
      // body is <mac_address>:<pairing_code>
      String body = mac_address + ":" + pairing_code;
      httpCode = http.POST(body);
      // httpCode will be negative on error
      if (httpCode <= 0 || httpCode != HTTP_CODE_OK) {
        Serial.printf("[HTTP] POST... failed, error: %s\n",
                      http.errorToString(httpCode).c_str());
        tft.fillScreen(TFT_BLACK);
        tft.println("Error occured, pairing failed");
        http.end();
        while (!buttons.isAPressed()) {
          // wait for user to press A button
        }
        // TODO control flow to try again
        return;
      }
      // HTTP header has been sent and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      // successful response
      String res = http.getString().readStringUntil(':');
      // paylaod will be "waiting" if still waiting
      http.end();
      if (res == "waiting") {
        accountPairingWaitingScreen(pairing_code);
        while (!buttons.isAPressed()) {
          // wait for user to press A button
        }
        // TODO control flow to try again
        continue;
      }
      // payload will be "success:<region_id>:<powar_device_id>" if successful
    }
  }
}
