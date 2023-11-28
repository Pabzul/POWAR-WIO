#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <multi_channel_relay.h>

Multi_Channel_Relay relay;

const int mosfetPin = 2; // Pin PWM para controlar el mosfet
extern int pwmValue; // Declaración de la variable pwmValue como externa

void setupActuators() {
  // Inicializar el relay
  relay.begin(0x11);
  pinMode(mosfetPin, OUTPUT); // Configurar el pin del mosfet como salida
}

void handleActuators() {
  // Verificar si se recibió el mensaje para el cooler
  if (mqttCooler == "HIGH") {
    // Encender el cooler cuando se recibe "HIGH"
    relay.turn_on_channel(2);
  } else if (mqttCooler == "LOW") {
    // Apagar el cooler cuando se recibe "LOW"
    relay.turn_off_channel(2);
  }

  // Verificar si se recibió el mensaje para el heater
  if (mqttHeater == "HIGH") {
    // Encender el heater cuando se recibe "HIGH"
    relay.turn_on_channel(1);
  } else if (mqttHeater == "LOW") {
    // Apagar el heater cuando se recibe "LOW"
    relay.turn_off_channel(1);
  }

  // Controlar el mosfet basado en el mensaje CITY/light
  if (mqttLight.toFloat() >= 0.0 && mqttLight.toFloat() <= 100.0) {
    // Convertir el valor porcentual a una señal PWM (0-255)
    pwmValue = map(mqttLight.toFloat(), 0, 100, 0, 255);
    analogWrite(mosfetPin, pwmValue); // Establecer la señal PWM en el pin del mosfet
//    Serial.println("pwmValue");
//    Serial.println(pwmValue);
  } else {
    // Valor no válido recibido desde MQTT, apagar el mosfet por seguridad
    analogWrite(mosfetPin, 0);
  }
}

#endif // ACTUATORS_H
