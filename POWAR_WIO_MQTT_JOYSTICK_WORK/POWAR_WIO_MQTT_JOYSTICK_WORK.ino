#include <WiFi.h>
#include <PubSubClient.h>
#include "TFT_eSPI.h"
#include "Seeed_FS.h"
#include "RawImage.h"
#include "Inicio.h"
#include "SHT40_read.h"
#include "CO2_read.h"
#include "Moisture_read.h"
#include "tone.h"
#include "City.h"
#include "pantallas.h"
#include "WIFI_config.h"
#include "WIO_MQTT.h"
#include "Joystick.h" // Incluimos el archivo Joystick.h

TFT_eSPI tft;

int currentScreen = 1;
bool joystickPressed = false;

unsigned long previousMillis = 0;
const long interval = 1000;
const long joystickInterval = 100; // Intervalo de 100 milisegundos para revisar el joystick

// Definiciones del joystick
#define JOYSTICK_UP_PIN WIO_5S_UP
#define JOYSTICK_DOWN_PIN WIO_5S_DOWN

// Creamos un objeto Joystick
Joystick joystick(JOYSTICK_UP_PIN, JOYSTICK_DOWN_PIN);

unsigned long previousMillisSensors = 0; // Variable para controlar el intervalo de los sensores
unsigned long previousMillisJoystick = 0; // Variable para controlar el intervalo del joystick

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(3);
  pinMode(BUZZER_PIN, OUTPUT);

  TextosInicio();
  looptone();
  tft.fillScreen(TFT_BLACK);

  tft.println("Connecting WiFi");
  setupWiFi();

  tft.println("Connecting MQTT");
  setupMQTT();

  tft.println("Setting SHT40");
  setupSHT40();

  tft.println("Setting CO2");
  setCO2();

  // Inicializamos el joystick
  joystick.begin();
  
  // Mostrar la pantalla inicial
  Pantalla0();
}

void loop() {
  // Print the loop() execution to check if it runs regularly
//  Serial.println("loop() is running...");

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  readSensorsAndPublishData();
  checkJoystick();
}

void checkJoystick() {
  // Obtener el tiempo actual
  unsigned long currentMillis = millis();

  // Verificar si ha pasado el tiempo suficiente para revisar el joystick nuevamente
  if (currentMillis - previousMillis >= joystickInterval) {
    previousMillis = currentMillis;

    // Verificar si el joystick ha sido pulsado
    if (joystick.isUpPressed() || joystick.isDownPressed()) {
      if (!joystickPressed) {
        joystickPressed = true;

        // Cambiar de pantalla en función del movimiento del joystick
        if (joystick.isUpPressed()) {
          // Movimiento hacia arriba
          currentScreen = (currentScreen % 3) + 1; // Cambiar a la siguiente pantalla (1, 2, 3)
        } else if (joystick.isDownPressed()) {
          // Movimiento hacia abajo
          currentScreen = (currentScreen + 2) % 3; // Cambiar a la pantalla anterior en sentido contrario (3, 2, 1)
        }

        // Mostrar la pantalla correspondiente
        switch (currentScreen) {
          case 1:
            Pantalla1();
            break;
          case 2:
            Pantalla2();
            break;
          case 3:
            Pantalla3();
            break;
          default:
            break;
        }
      }
    } else {
      joystickPressed = false;
    }
  }
}


void readSensorsAndPublishData() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisSensors >= interval) {
    previousMillisSensors = currentMillis;

    // Add debug output for sensor readings
    Serial.println("Reading sensors...");
    readSHT40();
    ReadCO2();
    Moisture_read();
    publishData();
    Serial.println("Sensor readings updated and published.");
  }
}
