// master_final.ino
//use 1uf cap with 3.3v to Gnd voltage divider
#include "config.h"

int dc_bias = 2048;

void manual_tone(int pin, int frequency) {
  long half_period_delay = 1000000L / (frequency * 2);
  unsigned long start = micros();
  while (micros() - start < BIT_DURATION_US) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(half_period_delay);
    digitalWrite(pin, LOW);
    delayMicroseconds(half_period_delay);
  }
}

void transmit_bit(int bit) {
  int freq = bit ? TARGET_FREQ_2 : TARGET_FREQ_1;
  manual_tone(TRANSMIT_PIN, freq);
  delayMicroseconds(GUARD_TIME_US);
}

void transmit_preamble() {
  int freq = TARGET_FREQ_1;
  long half_period_delay = 1000000L / (freq * 2);
  unsigned long start = micros();
  while (micros() - start < (PREAMBLE_MS * 1000)) {
    digitalWrite(TRANSMIT_PIN, HIGH);
    delayMicroseconds(half_period_delay);
    digitalWrite(TRANSMIT_PIN, LOW);
    delayMicroseconds(half_period_delay);
  }
  delay(5);
}

void setup() {
  Serial.begin(115200);
  pinMode(TRANSMIT_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Master Initializing...");
  delay(1000);
  long total = 0;
  for (int i = 0; i < 256; i++) { total += analogRead(ANALOG_PIN); delay(2); }
  dc_bias = total / 256;
  Serial.print("Calibration complete. DC Bias measured: ");
  Serial.println(dc_bias);
}

void loop() {
  for (uint8_t addr = 0; addr < 3; addr++) {
    Serial.print("\n--- Polling slave: "); Serial.println(addr);
    
    transmit_preamble();
    transmit_message(addr);

    uint8_t received_addr, received_data;
    if (receive_response(received_addr, received_data)) {
      if (received_addr == addr) {
        Serial.print(">>> SUCCESS: Response from Slave "); 
        Serial.print(addr);

        // ADDED THIS LINE to print the sensor value in binary
        Serial.print(" -> Sensor Data: ");
        Serial.println(received_data, BIN);
        
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
      } else {
        Serial.print(">>> ERROR: Address mismatch.");
      }
    } else {
      Serial.println(">>> FAIL: No response.");
    }
  }
  delay(2000);
}

void transmit_message(uint8_t address) {
  transmit_bit(1); transmit_bit(1);
  for (int i = 3; i >= 0; i--) transmit_bit((address >> i) & 1);
  for (int i = 0; i < 4; i++) transmit_bit(0);
  transmit_bit(0);
}

bool receive_response(uint8_t &address, uint8_t &data) {
  unsigned long message_start_time;
  int bit;

  unsigned long timeout_start = millis();
  while (true) {
    if (millis() - timeout_start > 500) return false;
    unsigned long pre_listen_time = micros();
    bit = receive_bit();
    if (bit == 1) {
      message_start_time = pre_listen_time;
      break;
    }
    delay(10);
  }

  uint8_t received_bits[MESSAGE_LENGTH_BITS];
  received_bits[0] = 1;
  for (int i = 1; i < MESSAGE_LENGTH_BITS; i++) {
    unsigned long next_bit_center_time = message_start_time + (i * TOTAL_BIT_TIME_US) + (BIT_DURATION_US / 2);
    unsigned long listen_start_time = next_bit_center_time - (SAMPLING_DURATION_US / 2);
    while (micros() - message_start_time < (listen_start_time - message_start_time));
    received_bits[i] = receive_bit();
    if (received_bits[i] == -1) return false;
  }

  if (received_bits[0] != 1 || received_bits[1] != 1 || received_bits[MESSAGE_LENGTH_BITS - 1] != 0) return false;
  
  address = 0; data = 0;
  int bit_index = 2;
  for (int i = 0; i < 4; i++) address = (address << 1) | received_bits[bit_index++];
  for (int i = 0; i < 4; i++) data = (data << 1) | received_bits[bit_index++];
  return true;
}

int receive_bit() {
  float samples[N];
  unsigned long sampling_period_us = 1000000L / SAMPLING_FREQUENCY;
  unsigned long start_time = micros();
  for (int i = 0; i < N; i++) {
    while (micros() - start_time < (i * sampling_period_us));
    samples[i] = analogRead(ANALOG_PIN) - dc_bias;
  }
  float mag1 = goertzel(samples, N, TARGET_FREQ_1, SAMPLING_FREQUENCY);
  float mag2 = goertzel(samples, N, TARGET_FREQ_2, SAMPLING_FREQUENCY);
  if (mag2 > MAGNITUDE_THRESHOLD && mag2 > mag1) return 1;
  if (mag1 > MAGNITUDE_THRESHOLD && mag1 > mag2) return 0;
  return -1;
}

float goertzel(float *samples, int n, float target_freq, float sampling_freq) {
  float omega = 2.0 * PI * target_freq / sampling_freq;
  float sine = sin(omega); float cosine = cos(omega);
  float coeff = 2.0 * cosine;
  float q0 = 0, q1 = 0, q2 = 0;
  for (int i = 0; i < n; i++) {
    float window = 0.54 - 0.46 * cos(2.0 * PI * i / (n - 1));
    q0 = coeff * q1 - q2 + samples[i] * window;
    q2 = q1; q1 = q0;
  }
  float real = q1 - q2 * cosine;
  float imag = q2 * sine;
  return sqrt(real * real + imag * imag);
}
