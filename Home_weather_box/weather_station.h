#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H

#include <Arduino.h>
#include <Wire.h>
#include <GyverBME280.h>
#include <GyverOLED.h>
#include <MHZ19_uart.h>

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

// Project files
#include "weather_codes.h"
#include "forecast_data.h"
#include "display_helper.h"

// Global objects
extern GyverBME280 bme;
extern uint8_t current_screen;
extern MHZ19_uart mhz19;

int8_t cur_t = 0;
int8_t cur_h = 0;
int16_t cur_co2 = 0;
bool is_display_blinked = false;

// Function to read temperature and humidity from BME280
inline void readTempAndHumInside()
{
  static int8_t prev_t = cur_t;
  cur_t = (int8_t)round(bme.readTemperature());
  addTempIn(cur_t);

  static int8_t prev_h = cur_h;
  cur_h = (int8_t)round(bme.readHumidity());
  addHumIn(cur_h);

  // Update display if values changed
  if (!isnan(cur_t) && (prev_t != cur_t) && (current_screen == 0))
    displayUpdCurrentT(cur_t);
  if (!isnan(cur_h) && (prev_h != cur_h) && (current_screen == 0))
    displayUpdCurrentH(cur_h);
}

inline void readCO2(){
  static int16_t prev_co2 = cur_co2;
  cur_co2 = mhz19.getCO2PPM(); // values 400...5000
  addCO2(cur_co2);
  if (!isnan(cur_co2) && (cur_co2 != prev_co2) && (current_screen == 0)){
    displayUpdCurrentCO2(cur_co2);
  }
    //displayUpdCurrentH(cur_h);
}

// Function to check available RAM
// inline int freeRam()
// {
//   extern int __heap_start, *__brkval;
//   int v;
//   return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
// }

#endif // WEATHER_STATION_H