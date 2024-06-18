#pragma once

#include <Arduino.h>
#include <SensirionI2cSht4x.h>
#include <Wire.h>

SensirionI2cSht4x sht4x;

float temperature = 0.0;
float humidity = 0.0;

void setupSHT40() {
    int8_t i2cAddress = 0x44;  
    sht4x.begin(Wire, i2cAddress);

  uint32_t serialNumber;
  uint16_t error = sht4x.serialNumber(serialNumber);
  if (error) {
    Serial.print("Error trying to execute serialNumber(): ");
    Serial.println(error);
  } else {
    Serial.print("Serial Number: ");
    Serial.println(serialNumber);
  }
}

void readSHT40() {
  uint16_t error = sht4x.measureHighPrecision(temperature, humidity);
  if (error) {
    Serial.print("Error trying to execute measureHighPrecision(): ");
    Serial.println(error);
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C");
    Serial.print("\t");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
}

void PrintDHT() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, tft.height() / 8);
  tft.setTextSize(2);
  tft.print("Temp: ");
  tft.setCursor(0, tft.height() * 2 / 8);
  tft.setTextSize(3);
  tft.print(temperature);
  tft.print(" C");
  tft.setCursor(0, tft.height() * 5 / 8);
  tft.setTextSize(2);
  tft.print("Hum: ");
  tft.setCursor(0, tft.height() * 6 / 8);
  tft.setTextSize(3);
  tft.print(humidity);
  tft.print(" %");
}
