#ifndef FORECAST_DATA_H
#define FORECAST_DATA_H

#include <Arduino.h>

// Day names stored in PROGMEM
const char weekDays[7][4] PROGMEM = {
  "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

// Forecast data structure
struct WeatherForecast
{
  char date[6];   // "dd.mm" + null
  uint8_t weekdayId;  // 0=Mon, ..., 6=Sun
  int8_t minTemp;
  int8_t maxTemp;
  int8_t wind;
  uint16_t weatherCode;
};

// Sample forecast data stored in PROGMEM
const WeatherForecast forecast[4] PROGMEM = {
    {"18.03", 0, 0, 4, 2, 802},
    {"19.03", 1, -1, 8, 3, 800},
    {"20.03", 2, 0, 12, 1, 800},
    {"21.03", 3, 2, 16, 4, 800},
    // {"22.03", 4, 6, 12, 2, 801},
    // {"23.03", 5, 10, 17, 3, 500},
    // {"24.03", 6, 9, 16, 6, 800},
    // {"25.03", 0, 8, 15, 2, 800}
  };


// Helper: get forecast object from PROGMEM
inline void getForecast(WeatherForecast* out, uint8_t index) {
  memcpy_P(out, &forecast[index], sizeof(WeatherForecast));
}

inline void getWeekDay(char* buffer, uint8_t index) {
  if (index > 6) {
    strcpy_P(buffer, PSTR("???"));
    return;
  }
  strcpy_P(buffer, weekDays[index]);
}

// Helper functions to get data from PROGMEM forecast
// inline void getDateString(char *buffer, uint8_t index)
// {
//   if (!buffer)
//     return;
//   WeatherForecast f;
//   memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
//   strcpy_P(buffer, f.date); // Use strcpy_P for PROGMEM strings
// }

// inline void getDayString(char *buffer, uint8_t index)
// {
//   if (!buffer)
//     return;
//   WeatherForecast f;
//   memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
//   strcpy_P(buffer, f.day);
// }

// inline int8_t getMinTemp(uint8_t index)
// {
//   WeatherForecast f;
//   memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
//   return f.minTemp;
// }

// inline int8_t getMaxTemp(uint8_t index)
// {
//   WeatherForecast f;
//   memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
//   return f.maxTemp;
// }

// inline int8_t getWind(uint8_t index)
// {
//   WeatherForecast f;
//   memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
//   return f.wind;
// }

// inline uint16_t getConditionCode(uint8_t index)
// {
//   WeatherForecast f;
//   memcpy_P(&f, &forecast[index], sizeof(WeatherForecast));
//   return f.weatherCode;
// }

#endif // FORECAST_DATA_H