#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);
const char* mqttServer = "51.68.165.241";
const int mqttPort = 1883;

extern String mqttCity;     // Variable para almacenar CITY/name recibido por MQTT
extern String mqttCountry;  // Variable para almacenar CITY/country recibido por MQTT

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

  // Verificar el tópico recibido
  if (String(topic) == "CITY/name") {
    mqttCity = message;  // Asignar el valor recibido a la variable global
  } else if (String(topic) == "CITY/country") {
    mqttCountry = message;  // Asignar el valor recibido a la variable global
  }
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
//    tft.fillScreen(TFT_BLACK);
//    tft.println("Connecting to MQTT...");
    if (mqttClient.connect("WioTerminalClient")) {
      Serial.println("Connected to MQTT");
//      tft.println("Connected to MQTT");
      mqttClient.subscribe("CITY/name");
      mqttClient.subscribe("CITY/country");
    } else {
      Serial.print("MQTT connection failed, retrying in 5 seconds...");
//      tft.println("MQTT connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loopMQTT() {
  mqttClient.loop();
}

void publishData() {
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
