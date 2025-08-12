// =========================================================
// ==           Simple Pulse Transmitter                  ==
// =========================================================

// The pin connected to the Gate of your MOSFET
const int txPin = 2;

void setup() {
  // Set the transmitter pin as an output
  pinMode(txPin, OUTPUT);
  
  // Start serial communication to show that it's working
  Serial.begin(115200);
  Serial.println("Pulse Transmitter Initialized.");
}

void loop() {
  // Announce that we're sending a pulse
  Serial.println("Sending pulse...");

  // Create a strong, 100-millisecond pulse
  digitalWrite(txPin, HIGH);
  delay(100);
  digitalWrite(txPin, LOW);

  // Wait for the rest of the 2-second interval
  delay(1900);
}
