// declare to which Pin the sensor is connected
int moisturePin = A0;

// declare the initial value of the sensor
int moisture = 0;
unsigned long previousMillisMoisture = 0;
const long moistureInterval = 1000; // Interval of 1000 milliseconds (1 second) to read the moisture sensor

void Moisture_read() {
    // Get the current time in milliseconds
    unsigned long currentMillis = millis();

    // Check if enough time has passed to read the moisture sensor
    if (currentMillis - previousMillisMoisture >= moistureInterval) {
        // Store the current time for the next reading
        previousMillisMoisture = currentMillis;

        // Read the value from the sensor:
        moisture = analogRead(moisturePin);
        moisture = map(moisture, 1023, 0, 100, 0);

        // Print the value in the serial monitor
        Serial.print("Moisture: ");
        Serial.println(moisture);
    }
}
