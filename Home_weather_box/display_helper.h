#ifndef DISPLAY_HELPER_H
#define DISPLAY_HELPER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "weather_codes.h"
#include "forecast_data.h"

// Display related constants and macros
#define CURSOR_X(col) (col * display_font_w * display_font_size)
#define CURSOR_Y(row) (row * display_font_h * display_font_size)

// Display settings
extern uint8_t current_screen;
extern uint8_t display_font_size;
const uint8_t display_font_w = 6;
const uint8_t display_font_h = 8;
extern Adafruit_SSD1306 display;

// Current sensor values
extern uint8_t cur_t;
extern uint8_t cur_h;
extern bool is_display_blinked;

// Function declarations for display handling
// void setDisplaySettings(int8_t display_scale, bool updateDisplay = true);
void changeScreen();
void setMainScreen();
void blinkTimeSeparator();
void displayUpdCurrentT(int16_t cur_t);
void displayUpdCurrentH(int16_t cur_h);
void setWeatherForecastScreen();
void displayForecast(int8_t index_begin, int8_t index_end);
void setCO2histScreen();
void setTempInHistScreen();
void setHumInHistScreen();

// Display settings function implementation
// inline void setDisplaySettings(int8_t display_scale, bool updateDisplay = true) {
//   if (updateDisplay) display.clearDisplay();
//   //display_font_size = display_scale;
//   display.setTextSize(display_scale);
// }

// Set main screen with clock, date, and current conditions
inline void setMainScreen()
{
  display.display();
  display_font_size = 3;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("10:45");

  display_font_size = 1;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(16), CURSOR_Y(0));
  display.print("18.03");
  display.setCursor(CURSOR_X(16), CURSOR_Y(2) - 2);
  display.print("Tue");
  display.setCursor(CURSOR_X(1), CURSOR_Y(4));

  display.print("In     ");
  display.print((char)247);
  display.print("C");
  display.setCursor(CURSOR_X(12), CURSOR_Y(4));
  display.print("Hum   %");

  display.setCursor(CURSOR_X(1), CURSOR_Y(5) + 4);
  display.print("Out +15");
  display.print((char)247);
  display.print("C");
  display.print(", 2 m/s");

  display.setCursor(CURSOR_X(1), CURSOR_Y(6) + 8);
  display.print("Cloudy");
  display.display();
  displayUpdCurrentT(cur_t);
  displayUpdCurrentH(cur_h);
}

// Blink time separator (colon) function
inline void blinkTimeSeparator()
{
  display_font_size = 3;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(2), CURSOR_Y(0));
  if (is_display_blinked)
  {
    display.print(":");
  }
  else
  {
    // Replace ":" with black rectangle to "clear" it
    display.fillRect(2 * display_font_w * display_font_size, 0 * display_font_h * display_font_size,
                     display_font_w * display_font_size, display_font_h * display_font_size, BLACK);
  }

  display.display();
  is_display_blinked = !is_display_blinked;
  display_font_size = 1;
  display.setTextSize(display_font_size);
}

// Update current temperature display
inline void displayUpdCurrentT(int16_t cur_t)
{
  /*
Format for temperature inside is "In  +25°C"
When the temperature or humidity is updated the display is has not been fully updated. It is not requirement, we can just update the value
But the display can not just "update" a char. Instead of this I draw a black rectangle to "clear" the previous value.
*/
  // fill chars with current temperature (e.g. "+25" in "In  +25°C") with a rect to set a new value instead of
  display.fillRect(5 * display_font_w * display_font_size, 4 * display_font_h * display_font_size,
                   3 * display_font_w * display_font_size, display_font_h * display_font_size, BLACK);

  display.setCursor(CURSOR_X(5), CURSOR_Y(4));
  display.print(cur_t > 0 ? "+" : cur_t < 0 ? "-"
                                            : " ");
  int16_t abs_t = abs(cur_t);

  if (abs_t > 9)
  {
    display.setCursor(CURSOR_X(6), CURSOR_Y(4));
    display.print(abs_t);
  }
  else
  {
    // must be "+  1°C" not "+ 1 °C"
    display.setCursor(CURSOR_X(7), CURSOR_Y(4));
    display.print(abs_t);
  }
  display.display();
}

// Update current humidity display
inline void displayUpdCurrentH(int16_t cur_h)
{
  // same logic as with temperature in displayUpdCurrentT()
  display.fillRect(16 * display_font_w * display_font_size, 4 * display_font_h * display_font_size,
                   2 * display_font_w * display_font_size, display_font_h * display_font_size, BLACK);
  display.setCursor(CURSOR_X(16), CURSOR_Y(4));
  display.print(cur_h);
  display.display();
}

// Change screen function
inline void changeScreen()
{
  display.clearDisplay();
  const uint8_t MAX_SCREENS = 8;
  current_screen = (current_screen + 1) % MAX_SCREENS;
  Serial.println(current_screen);
  switch (current_screen)
  {
  case 0:
    setMainScreen();
    break;
  case 1 ... 4:
    setWeatherForecastScreen();
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
inline void setWeatherForecastScreen()
{
  // display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  // display.print("Weather forecast");
  // display.display();

  switch (current_screen)
  {
  case 1:
    displayForecast(0, 1);
    break;
  case 2:
    displayForecast(2, 3);
    break;
  case 3:
    displayForecast(4, 5);
    break;
  case 4:
    displayForecast(6, 7);
    break;
  }
}

// Display forecast for specific day indices
inline void displayForecast(int8_t index_begin, int8_t index_end)
{
  // setDisplaySettings(1);
  // display.clearDisplay();
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("Weather forecast ");

  // Temporary buffers for strings
  // char dateStr[5];
  // char dayStr[4];

  // getDateString(dateStr, index_begin);
  // getDayString(dayStr, index_begin);

  //   display.setCursor(CURSOR_X(0), CURSOR_Y(1));
  //   display.print(dateStr);
  //   display.print(", ");
  //   display.print(dayStr);

  //   display.setCursor(CURSOR_X(0), CURSOR_Y(2));
  //   display.print(getMinTemp(index_begin));
  //   display.print(" ... ");
  //   display.print(getMaxTemp(index_begin));
  //   display.print((char)247);
  //   display.print("C, ");
  //   display.print(getWind(index_begin));
  //   display.print(" m/s");

  //   char weatherDesc[21];
  //   getWeatherDescription(weatherDesc, getConditionCode(index_begin));
  //   display.setCursor(CURSOR_X(0), CURSOR_Y(3));
  //   display.print(weatherDesc);

  //   // Second forecast if available
  //   if (index_end < 8) {
  //     getDateString(dateStr, index_end);
  //     getDayString(dayStr, index_end);

  //     display.setCursor(CURSOR_X(0), CURSOR_Y(4));
  //     display.print(dateStr);
  //     display.print(", ");
  //     display.print(dayStr);

  //     display.setCursor(CURSOR_X(0), CURSOR_Y(5));
  //     display.print(getMinTemp(index_end));
  //     display.print(" ... ");
  //     display.print(getMaxTemp(index_end));
  //     display.print((char)247);
  //     display.print("C, ");
  //     display.print(getWind(index_end));
  //     display.print(" m/s");

  //     getWeatherDescription(weatherDesc, getConditionCode(index_end));
  //     display.setCursor(CURSOR_X(0), CURSOR_Y(6));
  //     display.print(weatherDesc);
  //   }

  display.display();
}

// Set CO2 history screen
inline void setCO2histScreen()
{
  // display.clearDisplay();
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("CO2 in hist");
  display.display();
}

// Set temperature history screen
inline void setTempInHistScreen()
{
  // display.clearDisplay();
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("Temp. in hist");
  display.display();
}

// Set humidity history screen
inline void setHumInHistScreen()
{
  // display.clearDisplay();
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("Hum. in hist");
  display.display();
}

#endif // DISPLAY_HELPER_H