#include <Arduino.h>
// Variable to store the sensor reading for moisture level
int moisture;
// Threshold for determining whether the soil is dry or wet
int threshold = 300;
const int pumpPin = 3;
const int sensorPin = A0;

#define PUMP_ON  LOW
#define PUMP_OFF HIGH

unsigned long lastWatered = 0;
unsigned long cooldownTime = 0.2 * 60 * 1000UL;  // Cooldown time (0.2 minutes = 12 seconds)


void setup() {
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, PUMP_OFF);  // Make sure pump is off
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();

 // Check if enough time has passed since the last watering
  if (currentTime - lastWatered >= cooldownTime) {
    digitalWrite(pumpPin, PUMP_OFF);  // Ensure pump is off before reading
    delay(100);  // Allow power to settle

    // Take 5 sensor readings to smooth out noise
    int total = 0;
    for (int i = 0; i < 5; i++) {
      total += analogRead(sensorPin);
      delay(20);
    }
    moisture = total / 5;

    Serial.print("Moisture: ");
    Serial.println(moisture);

    if (moisture > threshold) {
      Serial.println("Soil is dry → Pump ON");
      digitalWrite(pumpPin, PUMP_ON);
      delay(2000);  // Pump ON for 2 seconds
      digitalWrite(pumpPin, PUMP_OFF);
      Serial.println("Pump OFF");
      lastWatered = millis();
    } else {
      Serial.println("Soil is wet → No watering.");
    }
  } else {
        // Take 5 sensor readings to smooth out noise
    Serial.print("Cooldown active. Time remaining: ");
    Serial.println((cooldownTime - (currentTime - lastWatered)) / 1000);
  }

  delay(1000);
}