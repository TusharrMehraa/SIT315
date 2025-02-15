const int sensorPin = A0;   // LM35 connected to A0
const int interruptPin = 2; // External interrupt pin for push button
const int ledPin = 13;      // Built-in LED pin on most Arduino boards

volatile bool readTemperature = false; // Flag to indicate when to read temperature

void setup() {
  Serial.begin(9600);   // Initialize serial communication
  pinMode(interruptPin, INPUT_PULLUP); // Set interrupt pin as input with internal pull-up
  pinMode(ledPin, OUTPUT); // Set built-in LED as output
  attachInterrupt(digitalPinToInterrupt(interruptPin), readTempISR, FALLING); // Trigger on button press
}

void loop() {
  if (readTemperature) {
    readTemperature = false; // Reset flag

    digitalWrite(ledPin, HIGH); // Turn on LED

    int sensorValue = analogRead(sensorPin);  // Read from LM35
    float voltage = sensorValue * (5.0 / 1023.0); // Convert to voltage
    float temperatureC = voltage * 100.0; // Convert to temperature (Celsius)

    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C - LED ON");

    delay(1000); // Small delay to stabilize reading

    digitalWrite(ledPin, LOW); // Turn off LED
    Serial.println("LED OFF");
  }
}

// Interrupt Service Routine (ISR)
void readTempISR() {
  readTemperature = true; // Set flag to read temperature
}
