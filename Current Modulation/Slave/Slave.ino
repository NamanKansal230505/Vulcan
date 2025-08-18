// ====================================================================
// ==         FINAL POLLING SLAVE (Direct Detection Logic)           ==
// ====================================================================

// --- PLC Receiver Configuration ---
const int rxAnalogPin = A0;
const int THRESHOLD = 100; // New threshold for direct 0-0.2V signal

// --- Pin and Slave Configuration ---
const int txPin = 2;
const int sensorPin = 6;
const int ledPin = 7;
const int myAddress = 4; // << CHANGE THIS FOR EACH SLAVE

// --- Protocol Timing ---
const unsigned long bitDelay = 10; // Must match the master

void setup() {
  pinMode(txPin, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(txPin, LOW);
  Serial.begin(115200);
  Serial.print("\n[SLAVE] Final Polling Slave Initializing. My Address: ");
  Serial.println(myAddress);
  Serial.println("---------------------------------");
  Serial.println("[SLAVE] Ready and listening for my address...");
}

// --- Helper Functions ---
void delayMicrosExact(unsigned long duration) {
  unsigned long start = micros();
  while (micros() - start < duration);
}

// MODIFIED: Simplified pulse detection
bool isPulseDetected() {
  return analogRead(rxAnalogPin) > THRESHOLD;
}

// --- Transmission Function ---
void sendBit(bool bitVal) {
  if (bitVal == 1) {
    digitalWrite(txPin, HIGH);
    delay(bitDelay);
    digitalWrite(txPin, LOW);
  } else {
    delay(bitDelay);
  }
}

void sendByte(byte data) {
  sendBit(true); // Start Pulse
  for (int i = 7; i >= 0; i--) {
    sendBit((data >> i) & 1);
  }
}

// --- Reception Function ---
byte receiveByte() {
  while (!isPulseDetected());
  while (isPulseDetected());

  byte data = 0;
  delay(bitDelay / 2);
  Serial.print(analogRead(rxAnalogPin));


  for (int i = 7; i >= 0; i--) {
    if (isPulseDetected()) {
      data |= (1 << i);
    }
    delay(bitDelay);
  }
  return data;
}

void loop() {
  byte polledAddress = receiveByte();
  
  if (polledAddress == myAddress) {
    Serial.print("I was polled! (Address #");
    Serial.print(polledAddress);
    Serial.println("). Sending status response...");
    
    bool sensorStatus = digitalRead(sensorPin);
    digitalWrite(ledPin, !sensorStatus); 
    byte statusByte = (myAddress << 4) | sensorStatus;

    sendByte(statusByte);
  }
}
