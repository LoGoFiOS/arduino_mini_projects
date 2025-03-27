#ifndef DISPLAY_HELPER_H
#define DISPLAY_HELPER_H

#include <Arduino.h>
#include <Wire.h>
#include <GyverOLED.h>
#include "weather_codes.h"
#include "forecast_data.h"
#include "history_data.h"

// Display related constants and macros
#define CURSOR_X(col) (col * display_font_w * display_font_size) // get first left-top X coor of col, pixels 0 - 127!!!
#define CURSOR_Y(row) (row * display_font_h * display_font_size) // get first left-top X coor of row

// Display settings
extern GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> display;
extern uint8_t current_screen;
extern uint8_t display_font_size;
const uint8_t display_font_w = 6;
const uint8_t display_font_h = 8;

// Current sensor values
extern int8_t cur_t;
extern int8_t cur_h;
extern int16_t cur_co2;
extern bool is_display_blinked;

extern uint8_t uint8_tHIST_DATA_BUFFER_SIZE;

// Function declarations for display handling
void changeScreen();

void setMainScreen();
void blinkTimeSeparator();
void displayUpdCurrentT(int8_t cur_t);
void displayUpdCurrentH(int8_t cur_h);
void displayUpdCurrentCO2(int16_t cur_co2);

void setWeatherForecastScreen(int8_t forecast_id);

// void setGraph(int16_t max_value, int16_t min_value, int16_t step, int8_t graph_shift = -4);
// void drawHistoryGraph(int16_t (*getValueAt)(uint8_t), int16_t max_value, int16_t min_value);
void setCO2histScreen();
void setTempInHistScreen();
void setHumInHistScreen();
// void testSetCO2();

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

  // display.setCursorXY(CURSOR_X(1), CURSOR_Y(6) + 8);
  // display.print("Few Clouds");
  display.update();
  displayUpdCurrentT(cur_t);
  displayUpdCurrentH(cur_h);
  displayUpdCurrentCO2(cur_co2);
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

inline void displayUpdCurrentT(int8_t cur_t)
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

inline void displayUpdCurrentH(int8_t cur_h)
{
  // same logic as with temperature in displayUpdCurrentT()
  display.setCursorXY(CURSOR_X(16), CURSOR_Y(4));
  display.print(cur_h);
  display.update();
}

inline void displayUpdCurrentCO2(int16_t cur_co2)
{
  display.setCursorXY(CURSOR_X(1), CURSOR_Y(6)+8);
  display.print(cur_co2);
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

inline void setGraph(int16_t max_value, int16_t min_value, int16_t step, int8_t graph_shift = -4){
  for (int i = 0; i <= 4; i += 1) {
    display.setCursorXY(CURSOR_X(0), CURSOR_Y(1) + (display_font_h + 4) * i + graph_shift); // 4px - distance between rows
    display.print(max_value - step*i);
    display.fastLineH(CURSOR_Y(1) + (display_font_h + 4) * i + display_font_h - 1 + graph_shift, CURSOR_X(4) + 1, CURSOR_X(5) - 1);
    // 11 23 35 47 59
    //Serial.println(CURSOR_Y(1) + (display_font_h + 4) * i + display_font_h - 1 + graph_shift);
  }
  display.fastLineV(CURSOR_X(4) + display_font_w/2, CURSOR_Y(1), CURSOR_Y(8));
  display.update();
}

inline void drawHistoryGraph(
    int16_t (*getValueAt)(uint8_t),
    int16_t max_value,
    int16_t min_value
)
{
  /*
    The greatest point on the graph: 11px. Max value: 
    The lowest: 59px. Min value: 

    Each hour on the graph has 4px length by x. 
    The y pos must by calculated. 
  */
  //Serial.print("max_value: "); Serial.print(max_value); Serial.print("min_value: "); Serial.println(min_value);  
  // clear display
  display.rect(CURSOR_X(5), CURSOR_Y(1), CURSOR_X(17), CURSOR_Y(8), OLED_CLEAR);

  uint8_t barWidth = 3;

  for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
    int16_t val = getValueAt(i);
    //Serial.print("Data from buffer: "); Serial.println(val);
    uint8_t y_coor;
    if ((val >= min_value) && (val <= max_value)) {
      y_coor = map(val, min_value, max_value, 59, 11);
      // a few option to show data
      display.rect(CURSOR_X(5) + barWidth*i, y_coor, CURSOR_X(5) + barWidth*(i+1)-1, 59, OLED_FILL);
      //display.fastLineH(y_coor, CURSOR_X(5) + 3*i, CURSOR_X(5) + 3*(i+1)-1);
      //Serial.print(" pos y: "); Serial.println(y_coor);
    } else {
      //y_coor = 65;
      // TODO: show somehow data missing 
    }

    
    
     
  }
  display.update();
}

inline void setCO2histScreen()
{
  display.setCursorXY(CURSOR_X(6), CURSOR_Y(0));
  display.print("CO2 in. 24h");
  setGraph(2000, 400, 400);
  drawHistoryGraph(getCO2_HoursAgo, 2000, 400);


  //drawHistoryGraph(getCO2_HoursAgo, min_valueue, max_valueue);
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
}

// Set temperature history screen
inline void setTempInHistScreen()
{
  display.setCursorXY(CURSOR_X(6), CURSOR_Y(0));
  display.print("Temp in. 24h");
  setGraph(30, -10, 10);
  drawHistoryGraph(getTempIn_HoursAgo, 30, -10); 
}

// Set humidity history screen
inline void setHumInHistScreen()
{
  display.setCursorXY(CURSOR_X(6), CURSOR_Y(0));
  display.print("Hum. in. 24h");
  setGraph(100, 0, 25);
  drawHistoryGraph(getHumIn_HoursAgo, 100, 0);
  display.update();
}


#endif // DISPLAY_HELPER_H




// inline void drawTempInGraph(int8_t max_valueue, int8_t min_valueue){
//   /*
//     The greatest point on the graph: 11px. Max value: 30
//     The lowest: 59px. Min value: 0

//     Each hour on the graph has 4px length by x. 
//     The y pos must by calculated. 
//   */

//   // display.rect(CURSOR_X(5), CURSOR_Y(1), CURSOR_X(17), CURSOR_Y(8), OLED_CLEAR);

//   for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
//     int16_t temp = getTempIn_HoursAgo(i);
//     int8_t y_coor;
//     if ((temp >= min_valueue) && (temp <= max_valueue)) {
//       y_coor = map(temp, min_valueue, max_valueue, 59, 11);
//     } else {
//       y_coor = 65;
//     }

//     display.rect(CURSOR_X(5) + 3*i, y_coor, CURSOR_X(5) + 3*(i+1)-1, 59, OLED_FILL);
//     //display.fastLineH(y_coor, CURSOR_X(5) + 3*i, CURSOR_X(5) + 3*(i+1)-1);    
//   }
// }


// void testSetCO2()
// {
//   display.setCursorXY(CURSOR_X(0), CURSOR_Y(7));
//   display.print("CO2: ");
//   display.print(cur_co2ppm);
//   display.update(); 
//   Serial.print("co2: ");
//   Serial.println(cur_co2ppm);
// }


  //   for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
  //     int16_t val = getValueAt(i);
  //     int8_t y = (val >= min_value && val <= max_value)
  //                ? map(val, min_value, max_value, 59, 11)
  //                : 65;
  //     //uint8_t x1 = CURSOR_X(5) + barWidth * i;
  //     //display.rect(x1, y, x1 + barWidth - 1, 59, OLED_FILL);
  //     display.rect(CURSOR_X(5) + barWidth*i, y, CURSOR_X(5) + barWidth*(i+1)-1, 59, OLED_FILL);
  // }