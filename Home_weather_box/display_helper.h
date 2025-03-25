#ifndef DISPLAY_HELPER_H
#define DISPLAY_HELPER_H

#include <Arduino.h>
#include <Wire.h>
#include <GyverOLED.h>
#include "weather_codes.h"
#include "forecast_data.h"
#include "history_data.h"
// #include "graph_helper.h"

// Display related constants and macros
#define CURSOR_X(col) (col * display_font_w * display_font_size) // get first left-top X coor of col, pixels 0 - 127!!!
#define CURSOR_Y(row) (row * display_font_h * display_font_size) // get first left-top X coor of row

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

extern uint8_t uint8_tHIST_DATA_BUFFER_SIZE;

// Function declarations for display handling
void changeScreen();
void setMainScreen();
void blinkTimeSeparator();
void displayUpdCurrentT(int16_t cur_t);
void displayUpdCurrentH(int16_t cur_h);
void setWeatherForecastScreen(int8_t forecast_id);
void drawCO2Graph(uint16_t max_value, uint16_t min_value);
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
  //display.print((char)247);
  display.print((char)127);
  display.print("C");
  display.setCursorXY(CURSOR_X(12), CURSOR_Y(4));
  display.print("Hum   %");

  display.setCursorXY(CURSOR_X(1), CURSOR_Y(5) + 4);
  display.print("Out +15");
  //display.print((char)247);
  display.print((char)127);
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
  display.print(F("Weather forecast, 7d"));

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
  //display.print((char)247); // ° symbol
  display.print((char)127);
  display.print(F("C, "));
  display.print(f.wind);
  display.print(" m/s");

  char desc[21];
  getWeatherDescription(desc, f.weatherCode);
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(4));
  display.print(desc);
  display.update();
}

// inline void drawHistoryGraph(
//   uint16_t (*getValueAt)(uint8_t),
//   int16_t minVal,
//   int16_t maxVal,
//   uint8_t barWidth = 3
// ) {
//   for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
//       int16_t val = getValueAt(i);
//       int8_t y = (val >= minVal && val <= maxVal)
//                  ? map(val, minVal, maxVal, 59, 11)
//                  : 65;
//       //uint8_t x1 = CURSOR_X(5) + barWidth * i;
//       //display.rect(x1, y, x1 + barWidth - 1, 59, OLED_FILL);
//       display.rect(CURSOR_X(5) + barWidth*i, y, CURSOR_X(5) + barWidth*(i+1)-1, 59, OLED_FILL);
//   }
// }

inline void drawCO2Graph(uint16_t max_value, uint16_t min_value){
  /*
    The greatest point on the graph: 11px. Max value: 2000
    The lowest: 59px. Min value: 400

    Each hour on the graph has 4px length by x. 
    The y pos must by calculated. 
  */

  display.rect(CURSOR_X(5), CURSOR_Y(1), CURSOR_X(17), CURSOR_Y(8), OLED_CLEAR);

  for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
    uint16_t co2 = getCO2_HoursAgo(i);
    uint8_t y_coor;
    if ((co2 >= min_value) && (co2 <= max_value)) {
      y_coor = map(co2, min_value, max_value, 59, 11);
    } else {
      y_coor = 65;
    }

    display.rect(CURSOR_X(5) + 3*i, y_coor, CURSOR_X(5) + 3*(i+1)-1, 59, OLED_FILL);
    //display.fastLineH(y_coor, CURSOR_X(5) + 3*i, CURSOR_X(5) + 3*(i+1)-1);    
  }
}

// Set CO2 history screen
inline void setCO2histScreen()
{
  display.setCursorXY(CURSOR_X(6), CURSOR_Y(0));
  display.print("CO2 in. 24h");

  uint16_t max_value = 2000;
  uint16_t min_value = 400;
  int16_t step = 400;
  int8_t graph_shift = -4;
  for (int i = 0; i <= 4; i += 1) {
    display.setCursorXY(CURSOR_X(0), CURSOR_Y(1) + (display_font_h + 4) * i + graph_shift); // 4px - distance between rows
    display.print(max_value - step*i);
    display.fastLineH(CURSOR_Y(1) + (display_font_h + 4) * i + display_font_h - 1 + graph_shift, CURSOR_X(4) + 1, CURSOR_X(5) - 1);
    // 11 23 35 47 59
    //Serial.println(CURSOR_Y(1) + (display_font_h + 4) * i + display_font_h - 1 + graph_shift);
  }
  display.fastLineV(CURSOR_X(4) + display_font_w/2, CURSOR_Y(1), CURSOR_Y(8));
  
  drawCO2Graph(max_value, min_value);
  //drawHistoryGraph(getCO2_HoursAgo, min_value, max_value);
  // debug. Check pixel perfect titels
  //display.rect(CURSOR_X(0), CURSOR_Y(1), CURSOR_X(4) - 1, CURSOR_Y(2) - 1);


  // display.setCursorXY(CURSOR_X(17), CURSOR_Y(1));
  // display.print("cur:");
  // display.setCursorXY(CURSOR_X(17), CURSOR_Y(2));
  // display.print("500");
  // display.setCursorXY(CURSOR_X(17), CURSOR_Y(3)+4);
  // display.print("max:");
  // //display.update();
  // display.setCursorXY(CURSOR_X(17), CURSOR_Y(4)+8);
  // display.print("900");
  // //display.update();
  // //Serial.print(CURSOR_Y(3) + 4); Serial.print(" "); Serial.print(CURSOR_Y(3) + 4 + display_font_h);
  // display.setCursorXY(CURSOR_X(17), CURSOR_Y(5)+4);
  // display.print("min:");
  // display.setCursorXY(CURSOR_X(17), CURSOR_Y(6)+8);
  // display.print("850");
  display.update();
}

inline void drawTempInGraph(int8_t max_value, int8_t min_value){
  /*
    The greatest point on the graph: 11px. Max value: 30
    The lowest: 59px. Min value: 0

    Each hour on the graph has 4px length by x. 
    The y pos must by calculated. 
  */

  display.rect(CURSOR_X(5), CURSOR_Y(1), CURSOR_X(17), CURSOR_Y(8), OLED_CLEAR);

  for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
    int16_t temp = getTempIn_HoursAgo(i);
    int8_t y_coor;
    if ((temp >= min_value) && (temp <= max_value)) {
      y_coor = map(temp, min_value, max_value, 59, 11);
    } else {
      y_coor = 65;
    }

    display.rect(CURSOR_X(5) + 3*i, y_coor, CURSOR_X(5) + 3*(i+1)-1, 59, OLED_FILL);
    //display.fastLineH(y_coor, CURSOR_X(5) + 3*i, CURSOR_X(5) + 3*(i+1)-1);    
  }
}

// Set temperature history screen
inline void setTempInHistScreen()
{
  display.setCursorXY(CURSOR_X(6), CURSOR_Y(0));
  display.print("Temp in. 24h");

  int16_t max_value = 30;
  int16_t min_value = -10;
  int16_t step = 10;
  int8_t graph_shift = -4;
  for (int i = 0; i <= 4; i += 1) {
    display.setCursorXY(CURSOR_X(0), CURSOR_Y(1) + (display_font_h + 4) * i + graph_shift); // 4px - distance between rows
    display.print(max_value - step*i);
    display.fastLineH(CURSOR_Y(1) + (display_font_h + 4) * i + display_font_h - 1 + graph_shift, CURSOR_X(4) + 1, CURSOR_X(5) - 1);
    // 11 23 35 47 59
    //Serial.println(CURSOR_Y(1) + (display_font_h + 4) * i + display_font_h - 1 + graph_shift);
  }
  display.fastLineV(CURSOR_X(4) + display_font_w/2, CURSOR_Y(1), CURSOR_Y(8));
  
  //drawHistoryGraph(getTempIn_HoursAgo, min_value, max_value);
  drawTempInGraph(max_value, min_value);  
}

// Set humidity history screen
inline void setHumInHistScreen()
{
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(0));
  display.print("Hum. in. 24h");
  display.update();
}

#endif // DISPLAY_HELPER_H