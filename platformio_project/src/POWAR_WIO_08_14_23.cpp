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
#include "variables.h"
#include "pantallas.h"
#include "WIO_MQTT.h"
#include "Controls.h"   // Incluimos el archivo Joystick.h
#include "actuators.h"  // Incluimos el archivo Joystick.h
#include "tft_message_screens.h"
#include "WIFI_config.h"
#include "powar_account.h"
TFT_eSPI tft;

int currentScreen = 1;
bool joystickPressed = false;

unsigned long previousMillis = 0;
const long interval = 1000;  // Intervalo de 1 segundo para publicar datos

unsigned long previousSensorUpdateMillis = 0;
const long sensorUpdateInterval =
    5000;  // Intervalo de 5 segundos para actualizar los sensores

unsigned long previousJoystickMillis = 0;
const long joystickInterval =
    200;  // Intervalo de 200 milisegundos para revisar el joystick

// Definiciones del joystick
#define JOYSTICK_UP_PIN WIO_5S_UP
#define JOYSTICK_DOWN_PIN WIO_5S_DOWN

// Creamos un objeto Joystick
Joystick joystick(JOYSTICK_UP_PIN, JOYSTICK_DOWN_PIN);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  tft.begin();
  tft.setRotation(3);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialise SD card
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
    SD_card_not_found_screen();
    while (1)
      ;
  }

  joystick.begin();
  buttons.begin();

  TextosInicio();
  looptone();
  tft.fillScreen(TFT_BLACK);

  tft.println("Connecting WiFi");
  setupWiFi();

  // handle powar account info (assign to powarAccount global variable)
  tft.println("Connecting Account");
  handleAccount();
  tft.println("Connecting MQTT");
  setupMQTT();

  tft.println("Setting SHT40");
  setupSHT40();

  // tft.println("Setting CO2");
  // setCO2();

  // tft.println("Setting Actuators");
  // setupActuators();

  // Inicializamos el joystick

  // Mostrar la pantalla inicial
  Pantalla1();
}

void checkJoystick();
void updateSensors();

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  checkJoystick();
  updateSensors();

  // Agregar la llamada a handleActuators()
  // handleActuators();
}

void checkJoystick() {
  // Obtener el tiempo actual
  unsigned long currentMillis = millis();

  // Verificar si ha pasado el tiempo suficiente para revisar el joystick
  // nuevamente
  if (currentMillis - previousJoystickMillis >= joystickInterval) {
    previousJoystickMillis = currentMillis;

    // Verificar si el joystick ha sido pulsado hacia arriba o hacia abajo
    bool isUpPressed = joystick.isUpPressed();
    bool isDownPressed = joystick.isDownPressed();

    // Solo realizar cambios de pantalla si se ha presionado el joystick
    if (isUpPressed || isDownPressed) {
      if (!joystickPressed) {
        joystickPressed = true;

        // Cambiar de pantalla
        if (isUpPressed) {
          currentScreen = (currentScreen + 1) %
                          5;  // Asegura que currentScreen esté entre 1 y 4
        } else if (isDownPressed) {
          currentScreen = (currentScreen - 1 + 4) % 4 +
                          1;  // Asegura que currentScreen esté entre 1 y 4
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
          case 4:
            Pantalla4();
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

void updateSensors() {
  // Obtener el tiempo actual
  unsigned long currentMillis = millis();

  // Verificar si ha pasado el tiempo suficiente para actualizar los sensores
  // nuevamente
  if (currentMillis - previousSensorUpdateMillis >= sensorUpdateInterval) {
    previousSensorUpdateMillis = currentMillis;

    // Actualizar los valores de los sensores
    readSHT40();
    // ReadCO2();
    Moisture_read();
    publishData();
    // Actualizar las pantallas de las lecturas de los sensores
    // Nota: Aquí asumimos que las funciones Pantalla1(), Pantalla2(),
    // Pantalla3() actualizan las pantallas con los valores de los sensores.
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
      case 4:
        Pantalla4();
        break;
      default:
        break;
    }
  }
}
