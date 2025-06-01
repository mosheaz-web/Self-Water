#include <Arduino.h>
// Variable to store the sensor reading for moisture level
int moisture;

// Threshold for determining whether the soil is dry or wet
int threshold = 300;  

// Pin numbers for the pump relay and soil moisture sensor
const int pumpPin = 3;
const int sensorPin = A0;

// Constants to control relay behavior based on relay type (active LOW)
#define PUMP_ON  LOW
#define PUMP_OFF HIGH

// Time tracking variables
unsigned long lastWatered = 0;  // Timestamp of last watering action
unsigned long cooldownTime = 0.2 * 60 * 1000UL;  // Cooldown time (0.2 minutes = 12 seconds)

// Setup function runs once when the Arduino starts up
void setup() {
  // Set the pump pin as an output (relay control)
  pinMode(pumpPin, OUTPUT);

  // Make sure the pump is off initially
  digitalWrite(pumpPin, PUMP_OFF);  

  // Start serial communication at 9600 baud for debugging
  Serial.begin(9600);
}

// Main loop function runs repeatedly
void loop() {
  // Get the current time in milliseconds since the Arduino started
  unsigned long currentTime = millis();

  // Check if enough time has passed since the last watering
  if (currentTime - lastWatered >= cooldownTime) {

    // Turn off the pump briefly before reading the moisture level
    digitalWrite(pumpPin, PUMP_OFF);  
    delay(100);  // Small delay to allow power to stabilize

    // Variable to accumulate sensor readings
    int total = 0;

    // Take 5 sensor readings to smooth out noise
    for (int i = 0; i < 5; i++) {
      total += analogRead(sensorPin);  // Read moisture sensor value
      delay(20);  // Short delay between readings
    }

    // Calculate the average of the 5 readings
    moisture = total / 5;

    // Output the current moisture level to the Serial Monitor
    Serial.print("Moisture: ");
    Serial.println(moisture);

    // Check if the moisture is below the threshold (soil is dry)
    if (moisture > threshold) {
      // Print to Serial Monitor that the soil is dry and we are turning on the pump
      Serial.println("Soil is dry → Pump ON");
      
      // Turn on the pump for 2 seconds to water the plant
      digitalWrite(pumpPin, PUMP_ON);  
      delay(2000);  // Keep the pump on for 2 seconds
      
      // Turn off the pump after watering
      digitalWrite(pumpPin, PUMP_OFF);
      
      // Print to Serial Monitor that the pump has been turned off
      Serial.println("Pump OFF");

      // Record the time when watering was last performed
      lastWatered = millis();  
    } else {
      // If the soil is wet (moisture level above the threshold), don't water
      Serial.println("Soil is wet → No watering.");
    }
  } else {
    // Print the remaining cooldown time before the next watering check
    Serial.print("Cooldown active. Time remaining: ");
    Serial.println((cooldownTime - (currentTime - lastWatered)) / 1000);
  }

  // Add a small delay to prevent the loop from running too fast
  delay(1000);
}
