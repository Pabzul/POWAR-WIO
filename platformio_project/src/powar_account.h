#pragma once

#include <SPI.h>
#include <Seeed_FS.h>
#include "Controls.h"
#include "SD/Seeed_SD.h"
#include <rpcWiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define POWAR_ACCOUNT_FILE "/.powar_account_v3.txt"

// String PLATFORM_URL =
// "http://powar-platform-git-develop-powar-steam-dev.vercel.app:80";
String PLATFORM_URL = "http://170.64.183.247:5000";
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

// read powar_acount file from SD and assign to powar_account struct
powar_account_status read_powar_account(powar_account& account) {
  // check if file exists on SD card
  if (!SD.exists(POWAR_ACCOUNT_FILE)) {
    return POWAR_ACCOUNT_NOT_FOUND;
  }
  // open file
  File account_file = SD.open(POWAR_ACCOUNT_FILE);
  // check if file is open
  if (!account_file) {
    return POWAR_ACCOUNT_FILE_ERROR;
  }
  // read powar_account from file in new line seperated format
  String schema_version = account_file.readStringUntil('\n');
  Serial.println("schema version: " + schema_version);
  account.region_id = account_file.readStringUntil('\n');
  account.region_id.trim();
  account.device_id = account_file.readStringUntil('\n');
  account.device_id.trim();
  account.name = account_file.readStringUntil('\n');
  account.name.trim();
  int module_count = account_file.parseInt();
  account_file.readStringUntil('\n');
  for (int i = 0; i < module_count; i++) {
    device_module module;
    module.module_id = account_file.readStringUntil('\n');
    module.module_id.trim();
    module.module_type = account_file.readStringUntil('\n');
    module.module_type.trim();
    module.module_topic = account_file.readStringUntil('\n');
    module.module_topic.trim();
    account.modules.push_back(module);
  }

  return POWAR_ACCOUNT_SUCCESS;
}

// create powar_account file on SD card, delete and create again if file is
// already present
void write_powar_account(powar_account& account) {
  // delete file if it exists
  if (SD.exists(POWAR_ACCOUNT_FILE)) {
    SD.remove(POWAR_ACCOUNT_FILE);
  }
  // create file
  File account_file = SD.open(POWAR_ACCOUNT_FILE, FILE_WRITE);
  // write powar_account struct to file in new line seperated format
  account_file.println("SCHEMA_VERSION:1");
  account_file.println(account.region_id);
  account_file.println(account.device_id);
  account_file.println(account.name);
  account_file.println(account.modules.size());
  for (int i = 0; i < account.modules.size(); i++) {
    account_file.println(account.modules[i].module_id);
    account_file.println(account.modules[i].module_type);
    account_file.println(account.modules[i].module_topic);
  }
  account_file.close();
}

void setupAccount() {
  // account not setup
  // ask to create a new pairing request
  accountNotSetupScreen();
  // wait for user to press A button
  while (!buttons.isAPressed())
    ;
  accountPairingScreen();
  // http request to server to get pairing code
  // start connection and send HTTP header
  // get MAC address of device
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String mac_address = "";
  for (int i = 0; i < 6; i++) {
    mac_address += String(mac[i], HEX);
  }
  String pairing_code;
  {
    HTTPClient http;
    Serial.print("[HTTP] POST...\n");
    http.begin(PLATFORM_URL + "/api/pair?type=start");
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.POST(mac_address);
    // httpCode will be negative on error
    if (httpCode <= 0 || httpCode != HTTP_CODE_OK) {
      Serial.printf("[HTTP] POST... failed, error: %s\n",
        http.errorToString(httpCode).c_str());
      tft.fillScreen(TFT_BLACK);
      tft.println("Error occurred, pairing failed");
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
    pairing_code = http.getString();
    Serial.println(pairing_code);
    http.end();
  }
  tft.fillScreen(TFT_BLACK);
  accountPairingCodeScreen(pairing_code);
  // wait for user to press A button
  while (!buttons.isAPressed())
    ;
  // retry pairing request until successful
  while (true) {
    accountPairingScreen();
    StreamString res;
    {
      HTTPClient http;
      // call pair again with pairing code to check if pairing was
      // successful
      http.begin(PLATFORM_URL + "/api/pair?type=verify");
      http.addHeader("Content-Type", "text/plain");
      Serial.print("[HTTP] POST...\n");
      // body is <mac_address>:<pairing_code>
      String body = mac_address + ":" + pairing_code;
      int httpCode = http.POST(body);
      // httpCode will be negative on error
      if (httpCode <= 0 || httpCode != HTTP_CODE_OK) {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        tft.fillScreen(TFT_BLACK);
        tft.println("Error occured, pairing failed");
        http.end();
        while (!buttons.isAPressed()) {
          // wait for user to press A button
        }
        // Ensure http.end() is called only once
        // http.end();
        // TODO control flow to try again
        continue;
      }
      else {
        // Successful POST request
        Serial.printf("[HTTP] POST... success, code: %d\n", httpCode);
        String payload = http.getString();
        res.print(payload); // Copy payload to StreamString
        Serial.println("Response payload: " + payload);
      }

      http.end();
    }
    String type = res.readStringUntil(':');
    // paylaod will be "waiting" if still waiting
    if (type == "waiting") {
      accountPairingWaitingScreen(pairing_code);
      while (!buttons.isAPressed()) {
        // wait for user to press A button
      }
      continue;
    }
    // payload will be "success:<region_id>:<powar_device_id>" if successful
    if (type == "success") {
      /**
  On success we tell the POWAR device the information it needs for subscribing
  and publishing to topics for its modules, this means the module_id and the
  module type. We send back (including newlines)
  ```success:<region_id>:<powar_device_id>
  name:<device_name>
  module:<subscribe|publish>:<OutputModuleType|InputModuleType>:<module_id>
  module:<publish|publish>:<OutputModuleType|InputModuleType>:<module_id>
  stop:
  ```
  subscribe|publish will determine how the module is treated,
  OutputModuleType|InputModuleType will tell what service the module is for. The
  module_id will be used in constructing the topic
  */
  // read region_id and powar_device_id
      powar_account account;
      String region_id = res.readStringUntil(':');
      account.region_id = region_id;
      String powar_device_id = res.readStringUntil('\n');
      Serial.println("region_id: " + region_id);
      account.device_id = powar_device_id;
      Serial.println("powar_device_id: " + powar_device_id);
      String prop;
      while (true) {
        prop = res.readStringUntil(':');
        Serial.println("prop: " + prop);
        if (prop == "stop") {
          break;
        }
        else if (prop == "name") {
          account.name = res.readStringUntil('\n');
          Serial.println("name: " + account.name);
        }
        else if (prop == "module") {
          String module_type = res.readStringUntil(':');
          Serial.println("module_type: " + module_type);
          String module_topic = res.readStringUntil(':');
          Serial.println("module_topic: " + module_topic);
          String module_id = res.readStringUntil('\n');
          Serial.println("module_id: " + module_id);
          device_module module;
          module.module_id = module_id;
          module.module_type = module_type;
          module.module_topic = module_topic;
          account.modules.push_back(module);
        }
        else {
          Serial.println("unknown property: " + prop);
        }
      }
      // write account information to SD card
      write_powar_account(account);
      break;
    }
  }
}


void handleAccount() {
  powar_account account;
  powar_account_status account_status = read_powar_account(account);
  if (account_status == POWAR_ACCOUNT_NOT_FOUND) {
    setupAccount();
  }
  account_status = read_powar_account(account);
  if (account_status != POWAR_ACCOUNT_SUCCESS) {
    Serial.println("Error reading account from SD card");
    Serial.println(account_status);
    return;
  }
  Serial.println("region_id: " + account.region_id);
  Serial.println("device_id: " + account.device_id);
  Serial.println("name: " + account.name);
  for (int i = 0; i < account.modules.size(); i++) {
    Serial.println("module_id: " + account.modules[i].module_id);
    Serial.println("module_type: " + account.modules[i].module_type);
    Serial.println("module_topic: " + account.modules[i].module_topic);
  }
  powarAccount = account;
}
