#ifndef WEATHER_CODES_H
#define WEATHER_CODES_H

#include <Arduino.h>

// WeatherBit API Weather Codes stored in PROGMEM
// Based on https://www.weatherbit.io/api/codes

// Weather code descriptions stored in PROGMEM
const char code_200[] PROGMEM = "Th.storm Light Rain";
const char code_201[] PROGMEM = "Th.storm Rain";
const char code_202[] PROGMEM = "Th.storm Heavy Rain";
const char code_230[] PROGMEM = "Th.storm Light Driz.";
const char code_231[] PROGMEM = "Th.storm Drizzle";
const char code_232[] PROGMEM = "Th.storm Heavy Driz.";
const char code_233[] PROGMEM = "Th.storm Hail";
const char code_300[] PROGMEM = "Light Drizzle";
const char code_301[] PROGMEM = "Drizzle";
const char code_302[] PROGMEM = "Heavy Drizzle";
const char code_500[] PROGMEM = "Light Rain";
const char code_501[] PROGMEM = "Moderate Rain";
const char code_502[] PROGMEM = "Heavy Rain";
const char code_511[] PROGMEM = "Freezing Rain";
const char code_520[] PROGMEM = "Light Shower Rain";
const char code_521[] PROGMEM = "Shower Rain";
const char code_522[] PROGMEM = "Heavy Shower Rain";
const char code_600[] PROGMEM = "Light Snow";
const char code_601[] PROGMEM = "Snow";
const char code_602[] PROGMEM = "Heavy Snow";
const char code_610[] PROGMEM = "Mix Snow/Rain";
const char code_611[] PROGMEM = "Sleet";
const char code_612[] PROGMEM = "Heavy Sleet";
const char code_621[] PROGMEM = "Snow Shower";
const char code_622[] PROGMEM = "Heavy Snow Shower";
const char code_623[] PROGMEM = "Flurries";
const char code_700[] PROGMEM = "Mist";
const char code_711[] PROGMEM = "Smoke";
const char code_721[] PROGMEM = "Haze";
const char code_731[] PROGMEM = "Sand/Dust";
const char code_741[] PROGMEM = "Fog";
const char code_751[] PROGMEM = "Freezing Fog";
const char code_800[] PROGMEM = "Clear Sky";
const char code_801[] PROGMEM = "Few Clouds";
const char code_802[] PROGMEM = "Scattered Clouds";
const char code_803[] PROGMEM = "Broken Clouds";
const char code_804[] PROGMEM = "Overcast Clouds";
const char code_900[] PROGMEM = "Unknown";

// Structure to map weather codes to their descriptions
struct WeatherCodeMap
{
  int16_t code;
  const char *description;
};

// Table mapping codes to descriptions, stored in PROGMEM
const WeatherCodeMap weatherCodeMap[] PROGMEM = {
    {200, code_200},
    {201, code_201},
    {202, code_202},
    {230, code_230},
    {231, code_231},
    {232, code_232},
    {233, code_233},
    {300, code_300},
    {301, code_301},
    {302, code_302},
    {500, code_500},
    {501, code_501},
    {502, code_502},
    {511, code_511},
    {520, code_520},
    {521, code_521},
    {522, code_522},
    {600, code_600},
    {601, code_601},
    {602, code_602},
    {610, code_610},
    {611, code_611},
    {612, code_612},
    {621, code_621},
    {622, code_622},
    {623, code_623},
    {700, code_700},
    {711, code_711},
    {721, code_721},
    {731, code_731},
    {741, code_741},
    {751, code_751},
    {800, code_800},
    {801, code_801},
    {802, code_802},
    {803, code_803},
    {804, code_804},
    {900, code_900}};

const int weatherCodeCount = sizeof(weatherCodeMap) / sizeof(WeatherCodeMap);

// Function to get the description for a given weather code
inline void getWeatherDescription(char *buffer, int16_t weatherCode)
{
  if (!buffer)
    return;
  // Default to "Unknown" if code not found
  strcpy_P(buffer, code_900);

  for (int i = 0; i < weatherCodeCount; i++)
  {
    WeatherCodeMap map;
    memcpy_P(&map, &weatherCodeMap[i], sizeof(WeatherCodeMap));

    if (map.code == weatherCode)
    {
      strcpy_P(buffer, (char *)map.description);
      return;
    }
  }
}

// For shorter descriptions, use this version (saves display space)
// inline void getWeatherDescriptionShort(char* buffer, int16_t weatherCode) {
//   // Get the full description first
//   char fullDesc[30];
//   getWeatherDescription(fullDesc, weatherCode);

//   // Shorten common words
//   if (strstr_P(fullDesc, PSTR("Thunderstorm"))) {
//     strcpy(buffer, "T-Storm");
//   } else if (strstr_P(fullDesc, PSTR("Heavy"))) {
//     strcpy(buffer, "Heavy ");
//     if (strstr_P(fullDesc, PSTR("Rain"))) {
//       strcat(buffer, "Rain");
//     } else if (strstr_P(fullDesc, PSTR("Snow"))) {
//       strcat(buffer, "Snow");
//     } else if (strstr_P(fullDesc, PSTR("Drizzle"))) {
//       strcat(buffer, "Drizzle");
//     } else {
//       strcat(buffer, "Weather");
//     }
//   } else if (strstr_P(fullDesc, PSTR("Light"))) {
//     strcpy(buffer, "Light ");
//     if (strstr_P(fullDesc, PSTR("Rain"))) {
//       strcat(buffer, "Rain");
//     } else if (strstr_P(fullDesc, PSTR("Snow"))) {
//       strcat(buffer, "Snow");
//     } else if (strstr_P(fullDesc, PSTR("Drizzle"))) {
//       strcat(buffer, "Drizzle");
//     } else {
//       strcat(buffer, "Weather");
//     }
//   } else if (strstr_P(fullDesc, PSTR("Scattered Clouds"))) {
//     strcpy(buffer, "Scat Clouds");
//   } else if (strstr_P(fullDesc, PSTR("Broken Clouds"))) {
//     strcpy(buffer, "Broken Clouds");
//   } else if (strstr_P(fullDesc, PSTR("Overcast Clouds"))) {
//     strcpy(buffer, "Overcast");
//   } else if (strstr_P(fullDesc, PSTR("Few Clouds"))) {
//     strcpy(buffer, "Few Clouds");
//   } else if (strstr_P(fullDesc, PSTR("Clear Sky"))) {
//     strcpy(buffer, "Clear");
//   } else {
//     // For other codes, just copy as is but limit length
//     strncpy(buffer, fullDesc, 14);
//     buffer[14] = '\0'; // Ensure null termination
//   }
// }

#endif // WEATHER_CODES_H