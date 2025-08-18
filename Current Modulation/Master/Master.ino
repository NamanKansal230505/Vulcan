// ====================================================================
// ==        FINAL POLLING MASTER (Direct Detection Logic)           ==
// ====================================================================

const int txPin = 2;
const int rxAnalogPin = A0;
const unsigned long bitDelay = 10; // Tuned speed
const int THRESHOLD = 100; // New threshold for direct 0-0.2V signal

void setup() {
  pinMode(txPin, OUTPUT);
  digitalWrite(txPin, LOW);
  Serial.begin(115200);
  Serial.println("\n[MASTER] Final Polling Master Initialized.");
  Serial.println("---------------------------------");
  delay(2000); // Give slave time to boot
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

// --- Transmission Functions ---
void sendBit(bool bitVal) {
  if (bitVal == 1) { // A '1' is a pulse
    digitalWrite(txPin, HIGH);
    delay(bitDelay);
    digitalWrite(txPin, LOW);
  } else { // A '0' is silence
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
byte receiveByte(unsigned long timeout_ms) {
  unsigned long startWait = millis();
  while (!isPulseDetected()) {
    if (millis() - startWait > timeout_ms) {
      return 255; // Timeout error code
    }
  }
  while (isPulseDetected());

  byte data = 0;
  delay(bitDelay / 2);

  for (int i = 7; i >= 0; i--) {
    if (isPulseDetected()) {
      data |= (1 << i);
    }
    delay(bitDelay);
  }
  return data;
}

void loop() {
  for (int addrToPoll = 1; addrToPoll <= 8; addrToPoll++) {
    Serial.print("Polling slave #");
    Serial.print(addrToPoll);
    Serial.print("... ");

    sendByte(addrToPoll);

    byte responseByte = receiveByte(100);

    if (responseByte == 255) {
      Serial.println("No response.");
    } else {
      int receivedAddr = (responseByte >> 4);
      bool sensorStatus = responseByte & 0x01;

      if (receivedAddr == addrToPoll) {
        Serial.print("Response OK from #");
        Serial.print(receivedAddr);
        Serial.print(" | Sensor is: ");
        Serial.println(sensorStatus ? "OK" : "TRIGGERED");
      } else {
        Serial.print("Response from wrong address! Got data: ");
        Serial.println(responseByte, BIN);
      }
    }
    delay(5);
  }
  Serial.println("--- Polling cycle complete ---\n");
  delay(2000);
}
