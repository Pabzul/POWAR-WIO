#pragma once

#include <Arduino.h>
#include "sensirion_common.h"
#include "sgp30.h"

s16 err = 0;
u16 tvoc_ppb, co2_eq_ppm;

void setCO2() {
  s16 err;
  u32 ah = 0;
  u16 scaled_ethanol_signal, scaled_h2_signal;

/*For wio link!*/
#if defined(ESP8266)
  pinMode(15, OUTPUT);
  digitalWrite(15, 1);
  Serial.println("Set wio link power!");
  delay(500);
#endif
  /*  Init module,Reset all baseline,The initialization takes up to around 15
     seconds, during which all APIs measuring IAQ(Indoor air quality ) output
     will not change.Default value is 400(ppm) for co2,0(ppb) for tvoc*/
  while (sgp_probe() != STATUS_OK) {
    Serial.println("SGP failed");
    while (1)
      ;
  }
  /*Read H2 and Ethanol signal in the way of blocking*/
  err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal,
                                          &scaled_h2_signal);
  if (err == STATUS_OK) {
    Serial.println("get ram signal!");
  } else {
    Serial.println("error reading signals");
  }

  // ah=get_relative_humidity();
  /*
        The function sgp_set_absolute_humidity() need your own implementation
    */
  // sgp_set_absolute_humidity(ah);

  // Set absolute humidity to 13.000 g/m^3
  // It's just a test value
  sgp_set_absolute_humidity(13000);
  err = sgp_iaq_init();
  //
}

void ReadCO2() {
  err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
  if (err == STATUS_OK) {
    Serial.print("tVOC  Concentration:");
    Serial.print(tvoc_ppb);
    Serial.println("ppb");

    Serial.print("CO2eq Concentration:");
    Serial.print(co2_eq_ppm);
    Serial.println("ppm");

  } else {
    Serial.println("error reading IAQ values\n");
  };
}

/*
void PrintCO2() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, tft.height() / 8);
  tft.setTextSize(2);
  tft.print("tVOC  Concentration:");
  tft.setCursor(0, tft.height() * 2 / 8);
  tft.setTextSize(3);
  tft.print(tvoc_ppb);
  tft.print("ppm");
  tft.setCursor(0, tft.height() * 5 / 8);
  tft.setTextSize(2);
  tft.print("CO2eq Concentration:");
  tft.setCursor(0, tft.height() * 6 / 8);
  tft.setTextSize(3);
  tft.print(co2_eq_ppm);
  tft.print("ppm");
}
*/
