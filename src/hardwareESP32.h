#pragma once

// MUst be updated to the new Project HW !!!
#define PROG_LED_PIN 1
#define PROG_LED_PIN_ACTIVE_ON LOW
#define PROG_BUTTON_PIN 0
#define PROG_BUTTON_PIN_INTERRUPT_ON FALLING
#define SAVE_INTERRUPT_PIN 9 
#define ESP32_DEV_BOARD

#ifndef ARDUINO_ARCH_ESP32
#ifndef ARDUINO_ARCH_RP2040
/* Define Arduino Board */
/* ******************** */
//#define ArduinoZero
//#define ItsyBitsyM0
#define Xiao
#endif
#endif

//Debug Outputs
#define KDEBUG_min
#define KDEBUG
#define KDEBUG_ID
#define KDEBUG_Rocker           // comment this line to disable DEBUG Rocker info
#define KDEBUG_handled          // comment this line to disable DEBUG info Data handled
//#define KDEBUG_Received         // comment this line to disable DEBUG mode for Recvived Data infomration
//#define KDEBUG_Repeater         // comment this line to disable DEBUG mode for Repeater
//#define KDEBUG_BaseID           // comment this line to disable DEBUG mode for Base-ID
//#define KDEBUG_Sender_ID        // comment this line to disable DEBUG mode for Sender-ID

// Defines for Testing
//#define EnOceanTEST

// disable define to disable KNX Funktion (for Testing ENocean ONLY)
#define KNXenable 

#ifdef KDEBUG_min
// Get correct serial port for debugging
#ifdef ArduinoZero
#define SERIAL_PORT SerialUSB   // Serial port for Arduino Zero
#endif 
#ifdef ItsyBitsyM0
#define SERIAL_PORT Serial      // Serial port for ItsyBitsy
#endif
#ifdef Xiao
#define SERIAL_PORT SerialUSB      // Serial port for ItsyBitsy
#endif
#ifdef ARDUINO_ARCH_RP2040
#define SERIAL_PORT Serial
#endif
#ifdef ESP32
#define SERIAL_PORT Serial
#endif
#endif
