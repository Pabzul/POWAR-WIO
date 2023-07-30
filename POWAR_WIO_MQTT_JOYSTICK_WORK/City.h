#ifndef CITY_H
#define CITY_H

#include <Arduino.h>

// Definición de las variables globales
String mqttCity;    // Variable para almacenar CITY/name recibido por MQTT
String mqttCountry; // Variable para almacenar CITY/country recibido por MQTT
const char* cityTime = "";
float cityWind = 20;
float cityHum = 61;
float cityTemp = 31;

#endif // CITY_H