#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>

// Definición de las variables globales
String mqttCity;     // Variable para almacenar CITY/name recibido por MQTT
String mqttCountry;  // Variable para almacenar CITY/country recibido por MQTT
String mqttCooler;   // Variable para almacenar CITY/cooler recibido por MQTT
String mqttHeater;   // Variable para almacenar CITY/heater recibido por MQTT
String mqttLight;    // Variable para almacenar CITY/light recibido por MQTT
int pwmValue; // Definición de la variable pwmValue como entero

const char* cityTime = "";
float cityWind = 20;
float cityHum = 61;
float cityTemp = 31;

#endif // VARIABLES_H
