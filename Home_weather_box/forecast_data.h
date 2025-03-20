#ifndef FORECAST_DATA_H
#define FORECAST_DATA_H

#include <Arduino.h>

// Day names stored in PROGMEM
const char day_mon[] PROGMEM = "Mon";
const char day_tue[] PROGMEM = "Tue";
const char day_wed[] PROGMEM = "Wed";
const char day_thu[] PROGMEM = "Thu";
const char day_fri[] PROGMEM = "Fri";
const char day_sat[] PROGMEM = "Sat";
const char day_sun[] PROGMEM = "Sun";

// Forecast data structure
struct WeatherForecast {
  const char* date;
  const char* day;
  int8_t minTemp;
  int8_t maxTemp;
  int8_t wind;
  uint16_t weatherCode;
};

// Sample forecast data stored in PROGMEM
const WeatherForecast forecast[8] PROGMEM = {
  {"18.03", day_mon,  0,  4, 2, 700},
  {"19.03", day_tue, -1,  8, 3, 800},
  {"20.03", day_wed,  0, 12, 1, 800},
  {"21.03", day_thu,  2, 16, 4, 800},
  {"22.03", day_sat,  6, 12, 2, 801},
  {"23.03", day_sun, 10, 17, 3, 500},
  {"24.03", day_mon,  9, 16, 6, 800},
  {"25.03", day_tue,  8, 15, 2, 800}
};

// Helper functions to get data from PROGMEM forecast

inline void getDateString(char* buffer, uint8_t index) {
  if (!buffer) return;
  WeatherForecast f;
  memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
  strcpy_P(buffer, f.date);  // Use strcpy_P for PROGMEM strings
}

inline void getDayString(char* buffer, uint8_t index) {
  if (!buffer) return;
  WeatherForecast f;
  memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
  strcpy_P(buffer, f.day);
}

inline int8_t getMinTemp(uint8_t index) {
  WeatherForecast f;
  memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
  return f.minTemp;
}

inline int8_t getMaxTemp(uint8_t index) {
  WeatherForecast f;
  memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
  return f.maxTemp;
}

inline int8_t getWind(uint8_t index) {
  WeatherForecast f;
  memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
  return f.wind;
}

inline uint16_t getConditionCode(uint8_t index) {
  WeatherForecast f;
  memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
  return f.weatherCode;
}

#endif // FORECAST_DATA_H