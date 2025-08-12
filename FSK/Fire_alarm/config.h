// config.h - RELIABLE-FAST VERSION

#ifndef CONFIG_H
#define CONFIG_H

// --- Communication Parameters ---
#define SAMPLING_FREQUENCY    10000
#define N                     128     // High accuracy sample size
#define TARGET_FREQ_1         1100
#define TARGET_FREQ_2         2200
#define PREAMBLE_MS           50

// --- Timing (Balanced for Speed and Reliability) ---
#define BIT_DURATION_US       30000   // CHANGED: Increased for a large safety margin
#define GUARD_TIME_US         3000    // CHANGED: Proportional guard time
#define TOTAL_BIT_TIME_US     (BIT_DURATION_US + GUARD_TIME_US)
#define SAMPLING_DURATION_US  (N * 1000000L / SAMPLING_FREQUENCY) // 12800 us

// --- Protocol Definition ---
#define MESSAGE_LENGTH_BITS   11

// --- Threshold & Pins ---
#define MAGNITUDE_THRESHOLD   1800
#define ANALOG_PIN            34
#define TRANSMIT_PIN          4
#define LED_PIN               2

#endif
