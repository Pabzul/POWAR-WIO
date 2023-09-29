#ifndef WIO_MQTT_H
#define WIO_MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>

extern String mqttCity;
extern String mqttCountry;
extern String mqttCooler;
extern String mqttHeater;
extern String mqttLight;  // Add the variable for CITY/light

WiFiClient espClient;
PubSubClient mqttClient(espClient);
const char* mqttServer = "51.68.165.241";
const int mqttPort = 1883;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el tópico: ");
  Serial.println(topic);
  Serial.print("Contenido: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Convertir el contenido del payload a una cadena
  String message = String((char*)payload).substring(0, length);

  // Verificar el tópico recibido y asignar el valor a la variable
  // correspondiente

  // TODO subscribe to modules listed in powarAccount.modules
  if (String(topic) == "CITY/name") {
    mqttCity = message;
  } else if (String(topic) == "CITY/country") {
    mqttCountry = message;
  } else if (String(topic) == "CITY/cooler") {
    mqttCooler = message;
  } else if (String(topic) == "CITY/heater") {
    mqttHeater = message;
  } else if (String(topic) == "CITY/light") {
    mqttLight = message;  // Store the value received for CITY/light
  }
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnectMQTT() {
  // TODO subscribe to modules listed in powarAccount.modules
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect("WioTerminalClient")) {
      Serial.println("Connected to MQTT");
      mqttClient.subscribe("CITY/name");
      mqttClient.subscribe("CITY/country");
      mqttClient.subscribe("CITY/cooler");
      mqttClient.subscribe("CITY/heater");
      mqttClient.subscribe("CITY/light");  // Subscribe to the CITY/light topic
    } else {
      Serial.print("MQTT connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loopMQTT() { mqttClient.loop(); }

void publishData() {
  // TODO publish for modules listed in powarAccount.modules
  char tempMessage[10];
  snprintf(tempMessage, sizeof(tempMessage), "%.2f", temperature);
  mqttClient.publish("POWAR/temp", tempMessage);

  char humMessage[10];
  snprintf(humMessage, sizeof(humMessage), "%.2f", humidity);
  mqttClient.publish("POWAR/hum", humMessage);

  char moistMessage[10];
  snprintf(moistMessage, sizeof(moistMessage), "%d", moisture);
  mqttClient.publish("POWAR/moist", moistMessage);

  Serial.println("Published data");
}

#endif  // WIO_MQTT_H
