const int sensorPin = A0;   // LM35 connected to A0
const int interruptPin = 2; // External interrupt pin for push button
volatile bool readTemperature = false; // Flag to indicate when to read temperature

void setup() {
  Serial.begin(9600);   // Initialize serial communication
  pinMode(interruptPin, INPUT_PULLUP); // Set interrupt pin as input with internal pull-up
  attachInterrupt(digitalPinToInterrupt(interruptPin), readTempISR, FALLING); // Trigger on button press
}

void loop() {
  if (readTemperature) {
    readTemperature = false; // Reset flag

    int sensorValue = analogRead(sensorPin);  // Read from LM35
    float voltage = sensorValue * (5.0 / 1023.0); // Convert to voltage
    float temperatureC = voltage * 100.0; // Convert to temperature (Celsius)

    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
  }
}

// Interrupt Service Routine (ISR)
void readTempISR() {
  readTemperature = true; // Set flag to read temperature
}
