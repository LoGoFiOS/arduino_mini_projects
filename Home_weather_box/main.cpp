/*
 * Weather Station with OLED Display
 * Features:
 * - Current temperature and humidity (BME280)
 * - Weather forecast (8 days)
 * - Multiple screens with statistics
 * - Button navigation
 */

#include <Arduino.h>
// #include <avr/wdt.h>
#include "weather_station.h"
// #include "history_data.h"
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> display;

// Global objects initialization
GyverBME280 bme;
Button btn_down(2);
Button btn_right(8);

// Timers initialization
uint32_t last_sensor_read_time = 0;
uint32_t last_display_blink_time = 0;
#define DISPLAY_TIME_BLINK_INTERVAL 1000
#define SENSOR_READ_INTERVAL 5000

// display
int8_t current_screen = 0;
int8_t display_font_size = 1;
uint8_t cur_t = 0;
uint8_t cur_h = 0;
bool is_display_blinked = false;

//history data
//extern uint16_t co2Buffer;

void setup()
{
  // wdt_disable(); // Disable watchdog on startup
  // Serial.println("System starting");
  // Serial.print("Reset reason: ");
  // Serial.println(MCUSR);
  // MCUSR = 0; // Clear reset flags
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  if (!bme.begin(0x76))
    Serial.println("BME280 Error!");

  // Serial.print(F("Free RAM: "));
  // Serial.println(freeRam());
  delay(500);

  display.init();
  display.clear();
  display_font_size = 1;
  display.setScale(display_font_size);
  display.setCursorXY(CURSOR_X(0), CURSOR_Y(0));
  display.print("initializing...");
  display.update();
  delay(500);
  display.clear();

  setMainScreen();
  // display.clear(); // tmp!!!
  // current_screen = 5;
  // setCO2histScreen();
}

void loop()
{
  uint32_t now = millis();

  // debug
  // static uint32_t last_ram_check = 0;
  // if (millis() - last_ram_check > 1000)
  // {
  //   last_ram_check = millis();
  //   Serial.print("Free RAM: ");
  //   Serial.println(freeRam());
  // }

  // Handle button inputs
  // debug
  btn_down.tick();
  if (btn_down.click())
    changeScreen();

  btn_right.tick();
  if (btn_right.click()) {
    switch (current_screen)
    {
    case 5:
      CO2genRandomValue();
      //drawHistoryGraph(getCO2_HoursAgo, 400, 2000);
      drawCO2Graph(2000, 400);
      setCO2histScreen();
      break;
    case 6:
      TempInGenRandomValue();
      //drawHistoryGraph(getTempIn_HoursAgo, -10, 30);
      drawTempInGraph(30, -10);
      setTempInHistScreen();     
      break;
    }
  }

  // Blink time colon on main screen
  if ((current_screen == 0) && (now - last_display_blink_time >= DISPLAY_TIME_BLINK_INTERVAL))
  {
    last_display_blink_time = now;
    blinkTimeSeparator();
  }

  // Read sensors at regular intervals
  if (now - last_sensor_read_time >= SENSOR_READ_INTERVAL)
  {
    last_sensor_read_time = now;
    readTempAndHumInside();
  }
}

/*
TODO

- Weather screen forecast (2 days by screen. Total 8 days)
- CO2 in history
- Temp in history 24h
- Humidity in history 24h

*/

// #include <LiquidCrystal_I2C.h>
// #include <DallasTemperature.h>
// #include <DHT.h>
// #include <DHT_U.h>

// #define ONE_WIRE_BUS 4  // DS18B20
// #define DHTPIN 2
// #define DHTTYPE DHT11

// DHT dht(DHTPIN, DHTTYPE);
// LiquidCrystal_I2C lcd(0x27, 16, 2); // sda A4, scl a5

// LCD
// lcd.init();
// lcd.backlight();
// lcd.setCursorXY(0, 0);
// lcd.clear();
// lcd.print("Init sensors...");
// delay(1000);
// lcd.clear();
// lcd.print("Temp. & humid.");
// lcd.setCursorXY(0, 1);
// lcd.print("[v.0.1]");
// delay(1000);
// lcd.clear();

// Sun - Sunday.
// Mon - Monday.
// Tue or Tues - Tuesday.
// Wed - Wednesday.
// Thu, Thur, or Thurs - Thursday.
// Fri - Friday.
// Sat - Saturday.