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
// const char* mqttServer = "192.168.1.120";
// const char* mqttServer = "51.68.165.241";
const char* mqttServer = "170.64.183.247";
const int mqttPort = 1883;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Mensaje recibido en el tópico: ");
  Serial.println(topic);
  Serial.println("Contenido: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Convertir el contenido del payload a una cadena
  // eng: convert payload to string
  String message = String((char*)payload).substring(0, length);

  // Verificar el tópico recibido y asignar el valor a la variable
  // correspondiente
  // eng: check the topic and assign the value to the corresponding variable

  // if (String(topic) == "CITY/name") {
  //   mqttCity = message;
  // } else if (String(topic) == "CITY/country") {
  //   mqttCountry = message;
  // } else if (String(topic) == "CITY/cooler") {
  //   mqttCooler = message;
  // } else if (String(topic) == "CITY/heater") {
  //   mqttHeater = message;
  // } else if (String(topic) == "CITY/light") {
  //   mqttLight = message;  // Store the value received for CITY/light
  // }
  // updated message topics for new format
  // topics are now CITY/{region_id}/{device_id}/{module.module_topic} where
  // module_topic == 'city_text' | 'country_text' | 'cooler_bool' |
  // 'heater_bool' | 'light_percent' | 'rain_percent'
  String topic_str = String(topic);
  if (topic_str == "CITY/" + powarAccount.region_id + "/" +
                       powarAccount.device_id + "/city_text") {
    mqttCity = message;
  } else if (topic_str == "CITY/" + powarAccount.region_id + "/" +
                              powarAccount.device_id + "/country_text") {
    mqttCountry = message;
  } else if (topic_str == "CITY/" + powarAccount.region_id + "/" +
                              powarAccount.device_id + "/cooler_bool") {
    mqttCooler = message;
  } else if (topic_str == "CITY/" + powarAccount.region_id + "/" +
                              powarAccount.device_id + "/heater_bool") {
    mqttHeater = message;
  } else if (topic_str == "CITY/" + powarAccount.region_id + "/" +
                              powarAccount.device_id + "/light_percent") {
    mqttLight = message;
  }
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnectMQTT() {
  // subscribe to modules listed in powarAccount.modules
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect("WioTerminalClient")) {
      // iterate through input modules and subscribe to each module's topic
      // topic string is constructed from
      // `POWAR/{region_id}/{device_id}/{module_id}
      String topic_root =
          "CITY/" + powarAccount.region_id + "/" + powarAccount.device_id + "/";
      Serial.println("## Subscribing Modules ##");
      for (int i = 0; i < powarAccount.modules.size(); i++) {
        device_module module = powarAccount.modules[i];
        Serial.println("Topic: " + module.module_topic);
        Serial.println("Type: " + module.module_type);
        if (module.module_type == "subscribe") {
          Serial.println("Subscribing to ");
          Serial.println(topic_root + module.module_topic);
          mqttClient.subscribe((topic_root + module.module_topic).c_str());
        } else {
          Serial.println("ignored module " + module.module_id);
        }
      }
      // Serial.println("Connected to MQTT");
      // mqttClient.subscribe("CITY/name");
      // mqttClient.subscribe("CITY/country");
      // mqttClient.subscribe("CITY/cooler");
      // mqttClient.subscribe("CITY/heater");
      // mqttClient.subscribe("CITY/light");  // Subscribe to the CITY/light
      // topic
    } else {
      Serial.print("MQTT connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loopMQTT() { mqttClient.loop(); }

void publishData() {
  // char tempMessage[10];
  // snprintf(tempMessage, sizeof(tempMessage), "%.2f", temperature);
  // mqttClient.publish("POWAR/temp", tempMessage);

  // char humMessage[10];
  // snprintf(humMessage, sizeof(humMessage), "%.2f", humidity);
  // mqttClient.publish("POWAR/hum", humMessage);

  // char moistMessage[10];
  // snprintf(moistMessage, sizeof(moistMessage), "%d", moisture);
  // mqttClient.publish("POWAR/moist", moistMessage);
  // Serial.println("Published data");

  // publish  modules listed in powarAccount.modules
  String topic_root =
      "POWAR/" + powarAccount.region_id + "/" + powarAccount.device_id + "/";
  // iterate through output modules and publish to each module's topic
  Serial.println("## Publishing Modules ##");
  for (int i = 0; i < powarAccount.modules.size(); i++) {
    device_module module = powarAccount.modules[i];
    Serial.println("Topic: " + module.module_topic);
    Serial.println("Type: " + module.module_type);
    Serial.println("MQTT Topic:");
    Serial.println(topic_root + module.module_id);
    if (module.module_type == "publish") {
      String topic = topic_root + module.module_id;
      // module.topic is one of "humidity_percent", "moisture_percent",
      // "temperature_celsius"
      Serial.println("Publishing to");
      Serial.println(topic_root + module.module_id);
      if (module.module_topic == "humidity_percent") {
        char message[10];
        snprintf(message, sizeof(message), "%.2f", humidity);
        mqttClient.publish(topic.c_str(), message);
      } else if (module.module_topic == "moisture_percent") {
        char message[10];
        snprintf(message, sizeof(message), "%d", moisture);
        mqttClient.publish(topic.c_str(), message);
      } else if (module.module_topic == "temperature_celsius") {
        char message[10];
        snprintf(message, sizeof(message), "%.2f", temperature);
        mqttClient.publish(topic.c_str(), message);
      } else {
        Serial.println("unknown module topic " + module.module_topic);
      }
    } else {
      Serial.println("ignored module " + module.module_id);
    }
  }
}

#endif  // WIO_MQTT_H
