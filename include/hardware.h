#pragma once

#ifdef ARDUINO_ARCH_RP2040
    // #define OPENKNX_RECOVERY_ON LOW
    #define PROG_LED_PIN 1
    #define PROG_LED_PIN_ACTIVE_ON HIGH
    #define INFO_LED_PIN 3
    #define INFO_LED_PIN_ACTIVE_ON HIGH
    #define PROG_BUTTON_PIN 0
    #define PROG_BUTTON_PIN_INTERRUPT_ON FALLING
    #define SAVE_INTERRUPT_PIN 22
    #define KNX_UART_RX_PIN 17
    #define KNX_UART_TX_PIN 16
    #define PLAYER_BUSY_PIN 3
    #define PLAYER_UART_RX_PIN 9
    #define PLAYER_UART_TX_PIN 8
    #define BINARY_INPUT_PULSE 5
    #define BINARY_INPUT_A_PIN 2
    #define BINARY_INPUT_B_PIN 4
    #define BINARY_INPUT_C_PIN 6
    #define BINARY_INPUT_D_PIN 7
    #define HARDWARE_NAME "MASIFI-SENSOR-BREAKOUT"
    #define SmartMF_HardwareVariant_PIN 26
    #define SmartMF_HardwareRevision_PIN1 10
    #define SmartMF_HardwareRevision_PIN2 11
    #define SmartMF_HardwareRevision_PIN3 12
#endif

#ifdef ARDUINO_ARCH_SAMD
    #define PROG_LED_PIN A3
    #define PROG_LED_PIN_ACTIVE_ON HIGH
    #define PROG_LED_SUPPORT_PWM
    #define PROG_BUTTON_PIN 38
    #define PROG_BUTTON_PIN_INTERRUPT_ON FALLING
    #define INFO_LED_PIN A5
    #define INFO_LED_PIN_ACTIVE_ON HIGH
    // #define KNX_UART_RX_PIN 0
    // #define KNX_UART_TX_PIN 1
    #define HARDWARE_NAME "KONNEKTING-TEST"
#endif

#ifdef ARDUINO_ARCH_ESP32
    #ifndef PROG_LED_PIN
    #define PROG_LED_PIN 2 
    #endif
    #define PROG_LED_PIN_ACTIVE_ON HIGH
    #ifndef PROG_BUTTON_PIN
    #define PROG_BUTTON_PIN 0
    #endif
    #define PROG_BUTTON_PIN_INTERRUPT_ON FALLING
    #define HARDWARE_NAME "ESP32"
#endif

#if (PROG_LED_PIN == 1)
#undef PROG_LED_PIN
#define PROG_LED_PIN -1
#define USE_PROG_LED_ON_SERIAL_TX 1
#endif
