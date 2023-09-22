#pragma once

void LogoPowar() { drawImage<uint16_t>("POWAR2.bmp", 0, 0); }

void TextosInicio() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextSize(3);
  tft.setTextColor(TFT_GREEN);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Welcome", tft.width() / 2, tft.height() * 3 / 9);
  tft.drawString("to", tft.width() / 2, tft.height() * 4 / 9);
  tft.drawString("POWAR", tft.width() / 2, tft.height() * 5 / 9);
  delay(1000);
  tft.setTextSize(2);
  tft.setTextColor(TFT_DARKGREEN);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("www.powarsteam.com", tft.width() / 2, tft.height() * 8 / 10);
}

void LeyendoSensor() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Leyendo Sensor", tft.width() / 2, tft.height() / 2);
}
