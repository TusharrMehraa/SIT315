const byte LED_PIN = 13;
const byte POT_PIN = A4;

// Default frequency: 0.5 Hz (Blink every 2 seconds)
double currentFrequency = 0.5;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
      
  Serial.begin(9600);
  
  startTimer(currentFrequency); // Initialize with default frequency
}

void loop() {
  double newFreq = readPotentiometer();
  
  // Only update timer if frequency change is significant
  if (abs(newFreq - currentFrequency) > 0.05) {
    currentFrequency = newFreq;
    startTimer(currentFrequency);
  }
  
  delay(100); // Small delay for stability
}

// Read the potentiometer and map the value to a frequency range
double readPotentiometer() {
  int potValue = analogRead(POT_PIN);
  
  // Map potentiometer value (0-1023) to frequency range (0.1 Hz - 5 Hz)
  double frequency = map(potValue, 0, 1023, 1, 50) / 10.0;
  
  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");
  
  return frequency;
}

// Start timer with the given frequency
void startTimer(double timerFrequency) {
  noInterrupts(); // Disable interrupts

  TCCR1A = 0;  // Clear Timer1 Control Register A
  TCCR1B = 0;  // Clear Timer1 Control Register B
  TCNT1 = 0;   // Reset Timer1 counter

  const long clockFreq = 16000000;
  long prescaler;
  int prescalerBits;
  
  // Select appropriate prescaler for accurate timing
  if (timerFrequency >= 1) {
    prescaler = 64;
    prescalerBits = (1 << CS11) | (1 << CS10);
  } else if (timerFrequency >= 0.5) {
    prescaler = 256;
    prescalerBits = (1 << CS12);
  } else {
    prescaler = 1024;
    prescalerBits = (1 << CS12) | (1 << CS10);
  }

  OCR1A = (clockFreq / (prescaler * timerFrequency)) - 1;
  TCCR1B |= (1 << WGM12);  // CTC mode
  TCCR1B |= prescalerBits; // Set prescaler
  TIMSK1 |= (1 << OCIE1A); // Enable compare match interrupt

  interrupts(); // Enable interrupts
}

// Interrupt Service Routine (ISR) to toggle LED
ISR(TIMER1_COMPA_vect) {
  digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1); // Toggle LED
}
