#ifndef DISPLAY_HELPER_H
#define DISPLAY_HELPER_H

#include <Arduino.h>
#include <Wire.h>
#include <GyverOLED.h>
#include "weather_codes.h"
#include "forecast_data.h"

// Display related constants and macros
#define CURSOR_X(col) (col * display_font_w * display_font_size)
#define CURSOR_Y(row) (row * display_font_h * display_font_size)

// Display settings
extern int8_t current_screen;
extern int8_t display_font_size;
const int8_t display_font_w = 6;
const int8_t display_font_h = 8;
extern GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> display;

// Current sensor values
extern uint8_t cur_t;
extern uint8_t cur_h;
extern bool is_display_blinked;

// Function declarations for display handling
void changeScreen();
void setMainScreen();
void blinkTimeSeparator();
void displayUpdCurrentT(int16_t cur_t);
void displayUpdCurrentH(int16_t cur_h);
void setWeatherForecastScreen(int8_t forecast_id);
void setCO2histScreen();
void setTempInHistScreen();
void setHumInHistScreen();

// Set main screen with clock, date, and current conditions
inline void setMainScreen()
{
  display_font_size = 3;
  display.setScale(display_font_size);
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(0));
  display.print("16:07");

  display_font_size = 1;
  display.setScale(display_font_size);
  display.setCursorXY(CURSOR_X(16), CURSOR_Y(0));
  display.print("22.03");
  display.setCursorXY(CURSOR_X(16), CURSOR_Y(2) - 2);
  display.print("Sat");
  display.setCursorXY(CURSOR_X(1), CURSOR_Y(4));

  display.print("In     ");
  display.print((char)247);
  display.print("C");
  display.setCursorXY(CURSOR_X(12), CURSOR_Y(4));
  display.print("Hum   %");

  display.setCursorXY(CURSOR_X(1), CURSOR_Y(5) + 4);
  display.print("Out +15");
  display.print((char)247);
  display.print("C");
  display.print(", 2 m/s");

  display.setCursorXY(CURSOR_X(1), CURSOR_Y(6) + 8);
  display.print("Few Clouds");
  display.update();
  displayUpdCurrentT(cur_t);
  displayUpdCurrentH(cur_h);
}

// Blink time separator (colon) function
inline void blinkTimeSeparator()
{
  display_font_size = 3;
  display.setScale(display_font_size);
  display.setCursorXY(CURSOR_X(2), CURSOR_Y(0));
  if (is_display_blinked)
  {
    display.print(F(":"));
  }
  else
  {
    display.print(F(" "));
  }

  display.update();
  is_display_blinked = !is_display_blinked;
  display_font_size = 1;
  display.setScale(display_font_size);
}

// Update current temperature display
inline void displayUpdCurrentT(int16_t cur_t)
{
  /*
  Format for temperature inside is "In  +25°C"
  When the temperature or humidity is updated the display is has not been fully updated. It is not requirement, we can just update the value
  */
  display.setCursorXY(CURSOR_X(5), CURSOR_Y(4));
  display.print(cur_t > 0 ? "+" : cur_t < 0 ? "-"
                                            : " ");
  int16_t abs_t = abs(cur_t);

  if (abs_t > 9)
  {
    display.setCursorXY(CURSOR_X(6), CURSOR_Y(4));
    display.print(abs_t);
  }
  else
  {
    // must be "+  1°C" not "+ 1 °C"
    display.setCursorXY(CURSOR_X(7), CURSOR_Y(4));
    display.print(abs_t);
  }
  display.update();
}

// Update current humidity display
inline void displayUpdCurrentH(int16_t cur_h)
{
  // same logic as with temperature in displayUpdCurrentT()
  display.setCursorXY(CURSOR_X(16), CURSOR_Y(4));
  display.print(cur_h);
  display.update();
}

// Change screen function
inline void changeScreen()
{
  display.clear();
  const uint8_t MAX_SCREENS = 8;
  current_screen = (current_screen + 1) % MAX_SCREENS;
  switch (current_screen)
  {
  case 0:
    setMainScreen();
    break;
  case 1:
    setWeatherForecastScreen(0);
    break;
  case 2:
    setWeatherForecastScreen(1);
    break;
  case 3:
    setWeatherForecastScreen(2);
    break;
  case 4:
    setWeatherForecastScreen(3);
    break;
  case 5:
    setCO2histScreen();
    break;
  case 6:
    setTempInHistScreen();
    break;
  case 7:
    setHumInHistScreen();
    break;
  }
}

// Set weather forecast screen
inline void setWeatherForecastScreen(int8_t forecast_id)
{
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(0));
  display.print(F("Weather forecast"));

  WeatherForecast f;
  char dayStr[4];

  // First forecast
  getForecast(&f, forecast_id);
  getWeekDay(dayStr, f.weekdayId);

  display.setCursorXY(CURSOR_X(0), CURSOR_Y(2));
  display.print(f.date);
  display.print(", ");
  display.print(dayStr);

  display.setCursorXY(CURSOR_X(0), CURSOR_Y(3));
  display.print(f.minTemp);
  display.print("... ");
  display.print(f.maxTemp);
  display.print((char)247); // ° symbol
  display.print("C, ");
  display.print(f.wind);
  display.print(" m/s");

  char desc[21];
  getWeatherDescription(desc, f.weatherCode);
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(4));
  display.print(desc);
  display.update();
}

// Set CO2 history screen
inline void setCO2histScreen()
{
  // display.clearDisplay();
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(0));
  display.print("CO2 in hist");
  display.update();
}

// Set temperature history screen
inline void setTempInHistScreen()
{
  // display.clearDisplay();
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(0));
  display.print("Temp. in hist");
  display.update();
}

// Set humidity history screen
inline void setHumInHistScreen()
{
  // display.clearDisplay();
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(0));
  display.print("Hum. in hist");
  display.update();
}

#endif // DISPLAY_HELPER_H