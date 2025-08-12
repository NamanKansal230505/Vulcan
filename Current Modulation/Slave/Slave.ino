// =========================================================
// ==           Simple Pulse Detector                   ==
// =========================================================

// The pin connected to your shunt resistor circuit
const int rxAnalogPin = A0;

// The sensitivity of the detector. Any analog reading HIGHER than
// this value will be considered a pulse. 50 is a reliable value.
const int THRESHOLD = 30;

void setup() {
  // Initialize Serial communication to print messages to your computer.
  // Set your Serial Monitor to this speed.
  Serial.begin(115200);
  Serial.println("Pulse Detector Initialized. Waiting for pulses...");
}

void loop() {
  // 1. Read the raw value from the analog pin (0-1023).
  int sensorValue = analogRead(rxAnalogPin);
//  Serial.println(sensorValue);

  // 2. Check if the value is greater than our threshold.
  if (sensorValue > THRESHOLD) {
    // 3. If it is, we have detected a pulse. Print a confirmation message.
    Serial.print("Pulse Detected! Raw Value: ");
    Serial.println(sensorValue);
    
    // Wait a moment to prevent printing hundreds of messages for a single pulse.
    }
  
}
