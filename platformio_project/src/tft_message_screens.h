#pragma once

void SD_card_not_found_screen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.println("Is the SD card inserted?");
  tft.setTextSize(3);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("SD Card", tft.width() / 2, tft.height() * 3 / 9);
  tft.drawString("Not Found", tft.width() / 2, tft.height() * 4 / 9);
  playNote('c', 500);
}

// screen to show when WiFiManager could not connect to WiFi and is entering
// config mode display a friendly message for the user
void wifiConfigModeCallbackScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.println("WiFi Connection Not Found");
  tft.setTextSize(3);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("POWAR STEAM", tft.width() / 2, tft.height() * 3 / 9);
  tft.drawString("Configure My WiFi", tft.width() / 2, tft.height() * 4 / 9);
  // playNote('f', 200);
}
// define AButton
#define AButton tft.width() / 2 + 12
// screen to show when no user account was found on SD card and the POWAR device
// is asking the user to initiate a pairing request with arrow pointing at top
// left button to continue
void accountNotSetupScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Account Pairing", tft.width() / 2, tft.height() * 3 / 9);
  tft.drawString("Required", tft.width() / 2, tft.height() * 4 / 9);
  tft.drawString("Press Button", tft.width() / 2, tft.height() * 5 / 9);
  tft.drawString("to Continue", tft.width() / 2, tft.height() * 6 / 9);
  tft.fillTriangle(AButton, 0, AButton - 10, 10, AButton + 10, 10, TFT_BLUE);
  // playNote('e', 200);
}

// screen to show when powar device is contacting server for a pairing code
void accountPairingScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Contacting Server", tft.width() / 2, tft.height() * 3 / 9);
  tft.drawString("Please Wait", tft.width() / 2, tft.height() * 5 / 9);
  // playNote('f', 200);
}

// screen to show when powar device is displaying pairing code "Your Pairing
// Code XXX..."
void accountPairingCodeScreen(String pairing_code) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Your Pairing Code", tft.width() / 2, tft.height() * 3 / 9);
  tft.drawString(pairing_code, tft.width() / 2, tft.height() * 4 / 9);
  tft.drawString("Press Button", tft.width() / 2, tft.height() * 5 / 9);
  tft.drawString("When Ready", tft.width() / 2, tft.height() * 6 / 9);
  tft.fillTriangle(AButton, 0, AButton - 10, 10, AButton + 10, 10, TFT_BLUE);
  // playNote('g', 200);
}

// screen to show when powar device pairing request is waiting on server
void accountPairingWaitingScreen(String pairing_code) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Unconfirmed", tft.width() / 2, tft.height() * 3 / 9);
  tft.drawString(pairing_code, tft.width() / 2, tft.height() * 4 / 9);
  tft.drawString("Try Again", tft.width() / 2, tft.height() * 5 / 9);
  tft.drawString("When Ready", tft.width() / 2, tft.height() * 6 / 9);
  tft.fillTriangle(AButton, 0, AButton - 10, 10, AButton + 10, 10, TFT_BLUE);
  // playNote('a', 200);
}
