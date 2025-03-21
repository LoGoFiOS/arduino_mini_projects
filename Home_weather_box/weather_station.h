#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H

#include <Arduino.h>
#include <Wire.h>
#include <GyverBME280.h>
#include <Adafruit_SSD1306.h>

// Button definitions using EncButton library
#define EB_NO_FOR
#define EB_NO_CALLBACK
#define EB_NO_COUNTER
#define EB_NO_BUFFER
#define EB_DEB_TIME 50    // Button debounce timeout
#define EB_CLICK_TIME 500 // Click wait timeout
#define EB_HOLD_TIME 600  // Hold timeout
#define EB_STEP_TIME 200  // Pulse hold timeout
#include <EncButton.h>

#include "weather_codes.h"
#include "forecast_data.h"
#include "display_helper.h"

// Pin definitions
#define SCL A5
#define SDA A4

// Global objects
extern GyverBME280 bme;
extern uint8_t current_screen;

// Function to read temperature and humidity from BME280
inline void readTempAndHumInside()
{
  static uint8_t prev_t = 0;
  static uint8_t prev_h = 0;
  // Read BME280 - Temperature
  prev_t = cur_t;
  cur_t = (int16_t)round(bme.readTemperature());

  // Read BME280 - Humidity
  prev_h = cur_h;
  cur_h = (int16_t)round(bme.readHumidity());

  // Update display if values changed and on main screen
  if (!isnan(cur_t) && (prev_t != cur_t) && (current_screen == 0))
    displayUpdCurrentT(cur_t);
  if (!isnan(cur_h) && (prev_h != cur_h) && (current_screen == 0))
    displayUpdCurrentH(cur_h);
}

// Function to check available RAM
inline int freeRam()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

#endif // WEATHER_STATION_H