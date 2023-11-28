#ifndef PANTALLAS_H
#define PANTALLAS_H

#include <TFT_eSPI.h> // Agrega la librería TFT_eSPI si no está incluida

extern float temperature; // Variable de temperatura global definida en otro lugar
extern float humidity;    // Variable de humedad global definida en otro lugar
extern int moisture;      // Variable de humedad del suelo global definida en otro lugar
extern String mqttCity;   // Declaración externa de la variable mqttCity
extern String mqttCountry; // Declaración externa de la variable mqttCountry
extern String mqttLight;  // Declaración externa de la variable mqttLight
extern int pwmValue; // Definición de la variable pwmValue como entero

void PantallaCambio() {
  tft.fillScreen(TFT_WHITE);
}

void Pantalla0() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  tft.println("Setup Ready!");
  tft.println("");
  tft.println("MOVE JOYSTICK");
}

void Pantalla1() {
  tft.fillScreen(TFT_WHITE);
  tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("BOX SENSORS", 160, 15);

  tft.drawFastVLine(160, 50, 190, TFT_DARKGREEN);
  tft.drawFastHLine(0, 140, 320, TFT_DARKGREEN);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Temperature", 80, 65);
  tft.setTextSize(3);
  tft.drawNumber(temperature, 50, 95);
  tft.drawString("C", 110, 95);

  tft.setTextSize(2);
  tft.drawString("Humidity", 85, 160);
  tft.setTextSize(3);
  tft.drawNumber(humidity, 50, 190);
  tft.drawString("%RH", 110, 190);

  tft.setTextSize(2);
  tft.drawString("CO2", 240, 65);
  tft.setTextSize(3);
  tft.drawNumber(co2_eq_ppm, 200, 95);
  tft.drawString("ppm", 280, 95);

  tft.setTextSize(2);
  tft.drawString("tVoc", 240, 160);
  tft.setTextSize(3);
  tft.drawNumber(tvoc_ppb, 200, 190);
  tft.drawString("ppb", 280, 190);
}

void Pantalla2() {
  tft.fillScreen(TFT_WHITE);
  tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("PLANT MOISTURE", 160, 15);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(5);
  tft.drawNumber(moisture, 160, tft.height() / 2);
  tft.setTextSize(3);
  tft.drawString("%", 240, 95);

  tft.drawFastHLine(0, 180, 320, TFT_DARKGREEN);

  if (moisture <= 30) {
    tft.setTextSize(3);
    tft.drawString("DRY SOIL", 160, 200);
  }

  if (moisture >= 70) {
    tft.setTextSize(3);
    tft.drawString("TOO WET", 160, 200);
  }

  if (moisture > 30 && moisture < 70) {
    tft.setTextSize(3);
    tft.drawString("GOOD MOISTURE", 160, 200);
  }
}

void Pantalla3() {
  tft.fillScreen(TFT_WHITE);
  tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);

  tft.drawFastHLine(0, 140, 320, TFT_DARKGREEN);

  tft.drawString("CITY SIMULATION", 160, 15);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("City", 35, 65);
  tft.setTextSize(3);
  tft.drawString(mqttCity, 160, 95); // Mostrar el valor de CITY/name recibido por MQTT
  tft.setTextSize(2);
  tft.drawString("Country", 55, 155);
  tft.setTextSize(3);
  tft.drawString(mqttCountry, 160, 185); // Mostrar el valor de CITY/country recibido por MQTT
}

void Pantalla4() {
  tft.fillScreen(TFT_WHITE);
  tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);

  tft.drawFastHLine(0, 140, 320, TFT_DARKGREEN);

  tft.drawString("LIGHT FACTORS", 160, 15);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("City Light:", 35, 65);
  tft.setTextSize(3);
  tft.drawString(mqttLight, 160, 95); // Mostrar el valor de CITY/light recibido por MQTT
  tft.setTextSize(2);
  tft.drawString("Box Light:", 55, 155);
  tft.setTextSize(3);
  tft.drawString(String(pwmValue), 160, 185); // Corregir "pmwValue" a "pwmValue"

}

#endif // PANTALLAS_H