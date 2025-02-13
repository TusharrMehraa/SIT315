const int tempSensorPin = A0;  // LM35 Temperature Sensor
const int sensorPin = 7;       // Single signal pin for Ultrasonic Sensor
const int ledPin = 3;          // LED for object detection

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(sensorPin, OUTPUT); // Configure ultrasonic sensor signal pin
  pinMode(ledPin, OUTPUT);
}

float getDistance() {
  pinMode(sensorPin, OUTPUT);
  digitalWrite(sensorPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensorPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensorPin, LOW);
  
  pinMode(sensorPin, INPUT);
  long duration = pulseIn(sensorPin, HIGH);
  float distance = duration * 0.034 / 2;  // Convert time to distance (cm)
  return distance;
}

void loop() {
  // Read LM35 Temperature
  int tempValue = analogRead(tempSensorPin);
  float voltage = tempValue * (5.0 / 1023.0);
  float temperatureC = voltage * 100.0;

  // Read Ultrasonic Sensor Distance
  float distance = getDistance();

  // Print sensor values
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If object detected within 10 cm, turn on LED
  if (distance < 10) {  
    digitalWrite(ledPin, HIGH);
    Serial.println("Object detected! LED ON!");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("No object nearby. LED OFF.");
  }

  delay(1000); // Small delay to avoid excessive readings
}
