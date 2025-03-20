#include <Arduino.h>
#include <GyverBME280.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


#define EB_NO_FOR
#define EB_NO_CALLBACK
#define EB_NO_COUNTER
#define EB_NO_BUFFER
#define EB_DEB_TIME 50      // таймаут гашения дребезга кнопки (кнопка)
#define EB_CLICK_TIME 500   // таймаут ожидания кликов (кнопка)
#define EB_HOLD_TIME 600    // таймаут удержания (кнопка)
#define EB_STEP_TIME 200    // таймаут импульсного удержания (кнопка)
#include <EncButton.h>

#define SCL A5
#define SDA A4

GyverBME280 bme;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// display
uint8_t  current_screen = 0;
uint8_t  display_font_size = 1; // scale
const uint8_t  display_font_w = 6; // in pixels, 21 chars per string with scale 1
const uint8_t  display_font_h = 8; // in pixels, 8 rows with scale 1
bool  is_display_blinked = false;

#define CURSOR_X(col) (col * display_font_w * display_font_size)
#define CURSOR_Y(row) (row * display_font_h * display_font_size)

// timers
uint32_t last_sensor_read_time = 0;
uint32_t last_display_blink_time = 0;
uint32_t last_display_update_time = 0;
uint32_t last_button_listened_time = 0;
constexpr uint32_t DISPLAY_TIME_BLINK_INTERVAL = 1000;
constexpr uint32_t BUTTON_TIME_LISTENER_INTERVAL = 100;
constexpr uint32_t SENSOR_READ_INTERVAL = 5000;

// sensors data
int16_t  prev_t = 0;
int16_t  cur_t = 0;
int16_t  prev_h = 0;
int16_t  cur_h = 0;

// buttons
Button btn_down(2);
Button btn_right(8);

// Labels
// const char* temp_label = "Temperature: ";
// const char* humid_label = "Humidity: ";

struct WeatherForecast {
  const char* date;
  const char* day;
  int8_t minTemp;
  int8_t maxTemp;
  int8_t wind;
  const char* condition;
};

WeatherForecast forecast[8] = {
  {"18.03", "Tue",  0,  4, 2, "Scattered clouds"},
  {"19.03", "Wed", -1,  8, 3, "Clear Sky"},
  {"20.03", "Thu",  0, 12, 1, "Clear Sky"},
  {"21.03", "Fri",  2, 16, 4, "Clear Sky"},
  {"22.03", "Sat",  6, 12, 2, "Moderate rain"},
  {"23.03", "Sun", 10, 17, 3, "Overcast clouds"},
  {"24.03", "Mon",  9, 16, 6, "Broken clouds"},
  {"25.03", "Tue",  8, 15, 2, "Clear Sky"}
};

void readTempAndHumInside();
void buttonChangeScreen();
void setMainScreen();
void blinkTimeSeparator();
void displayUpdCurrentT(int16_t cur_t);
void displayUpdCurrentH(int16_t cur_h);
void setWeatherForecastScreen();
void displayForecast(int8_t index_begin, int8_t index_end);
void setCO2histScreen();
void setTempInHistScreen();
void setHumInHistScreen();

void setup()
{
  Serial.begin(9600);
  if (!bme.begin(0x76)) Serial.println("Error!");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);

  // display_font_size = 1;
  // display.setTextSize(display_font_size);
  // display.setCursor(0 * display_font_w * display_font_size, 0 * display_font_h * display_font_size);
  // display.print("initializing...");
  // display.display(); 
  // delay(1000);

  for (int i = 0; i < 8; i++) {
    Serial.print("forecast[");
    Serial.print(i);
    Serial.print("].date: ");
    Serial.println(forecast[i].date);
  }

  setMainScreen();  
}

void loop()
{
  uint32_t now = millis();

  btn_down.tick();
  if (btn_down.click()) buttonChangeScreen();
  btn_right.tick();
  if (btn_right.click()) Serial.println("btn_right click");


  // blink symbol : on Main screen
  if ((current_screen == 0) && (now - last_display_blink_time >= DISPLAY_TIME_BLINK_INTERVAL))
  {
    last_display_blink_time = now;
    blinkTimeSeparator();
  }

  // read temperature and humidity inside
  if (now - last_sensor_read_time >= SENSOR_READ_INTERVAL)
  {
    last_sensor_read_time = now;
    readTempAndHumInside();
  }
}

void readTempAndHumInside(){
  // Read BME280 - Temperature
  prev_t = cur_t;
  cur_t = (int16_t)round(bme.readTemperature());

  // Read BME280 - Humidity
  prev_h = cur_h;
  cur_h = (int16_t)round(bme.readHumidity());

  // Serial.print(F("T.: "));
  // Serial.print(cur_t);
  // Serial.println(F("°C"));
  // Serial.print(F("H: "));
  // Serial.print(cur_h);
  // Serial.println(F("%"));
  // if (now - last_display_update_time >= LCD_UPDATE_INTERVAL) {
  //   last_display_update_time = now;
  // }

  if (!isnan(cur_t) && (prev_t != cur_t) && (current_screen == 0))
  {
    displayUpdCurrentT(cur_t);
  }
  if (!isnan(cur_h) && (prev_h != cur_h) && (current_screen == 0))
  {
    displayUpdCurrentH(cur_h);
  }
}

void buttonChangeScreen(){
  current_screen = (current_screen + 1) % 8; // not gt 7
  Serial.println(current_screen);
  switch(current_screen) {
    case 0: setMainScreen(); break;
    case 1: setWeatherForecastScreen(); break;
    case 2: setWeatherForecastScreen(); break;
    case 3: setWeatherForecastScreen(); break;
    case 4: setWeatherForecastScreen(); break;
    case 5: setCO2histScreen(); break;
    case 6: setTempInHistScreen(); break;
    case 7: setHumInHistScreen(); break;
  }
}

void setMainScreen(){
  readTempAndHumInside();
  display.clearDisplay();

  display_font_size = 3;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("10:45");

  display_font_size = 1;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(16), CURSOR_Y(0));
  display.print("18.03");
  display.setCursor(CURSOR_X(16), CURSOR_Y(2)-2);
  display.print("Tue");
  display.setCursor(CURSOR_X(1), CURSOR_Y(4));

  display.print("In     ");
  display.print((char)247);
  display.print("C");
  display.setCursor(CURSOR_X(12), CURSOR_Y(4));
  display.print("Hum   %");

  display.setCursor(CURSOR_X(1), CURSOR_Y(5)+4);
  display.print("Out +15");
  display.print((char)247);
  display.print("C");
  display.print(", 2 m/s");

  display.setCursor(CURSOR_X(1), CURSOR_Y(6)+8);
  display.print("Cloudy");
  display.display(); 
  displayUpdCurrentT(cur_t);
  displayUpdCurrentH(cur_h);
}

void blinkTimeSeparator(){
  display_font_size = 3;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(2), CURSOR_Y(0));
  if (is_display_blinked) {
    display.print(":");
  } else {
    // replace ":" to black rectangle to "clear" it
    display.fillRect(2 * display_font_w * display_font_size, 0 * display_font_h * display_font_size, display_font_w * display_font_size, display_font_h * display_font_size, BLACK);
  }

  display.display();
  is_display_blinked = !is_display_blinked;
  display_font_size = 1;
  display.setTextSize(display_font_size);
}

void displayUpdCurrentT(int16_t cur_t)
{
  /*
  Format for temperature inside is "In  +25°C"
  When the temperature or humidity is updated the display is has not been fully updated. It is not requirement, we can just update the value
  But the display can not just "update" a char. Instead of this I draw a black rectangle to "clear" the previous value. 
  */
  // fill chars with current temperature (e.g. "+25" in "In  +25°C") with a rect to set a new value instead of
  display.fillRect(5 * display_font_w * display_font_size, 4 * display_font_h * display_font_size, 3 * display_font_w * display_font_size, display_font_h * display_font_size, BLACK);

  display.setCursor(CURSOR_X(5), CURSOR_Y(4));
  display.print(cur_t > 0 ? "+" : cur_t < 0 ? "-" : " ");
  int16_t abs_t = abs(cur_t);

  if (abs_t > 9) {
    display.setCursor(CURSOR_X(6), CURSOR_Y(4));
    display.print(abs_t);
  } else {
    // must be "+  1°C" not "+ 1 °C"
    display.setCursor(CURSOR_X(7), CURSOR_Y(4));
    display.print(abs_t);
  }
  display.display();  
}

void displayUpdCurrentH(int16_t cur_h)
{
  // same logic as with temperature in displayUpdCurrentT()
  display.fillRect(16 * display_font_w * display_font_size, 4 * display_font_h * display_font_size, 2 * display_font_w * display_font_size, display_font_h * display_font_size, BLACK);
  display.setCursor(CURSOR_X(16), CURSOR_Y(4));
  display.print(cur_h);
  display.display();  
}

void setWeatherForecastScreen(){
  //display.clearDisplay();
  // display_font_size = 1;
  // display.setTextSize(display_font_size);
  // display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  // display.print("Weather forecast");
  // display.display();
switch(current_screen) {
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

void displayForecast(int8_t index_begin, int8_t index_end) {
  display.clearDisplay();
  display_font_size = 1;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("Weather forecast ");
  // display.print(index_begin);
  // display.print(" ");
  // display.print(index_end);

  // display.setCursor(CURSOR_X(0), CURSOR_Y(1));
  // Serial.println(forecast[0].date);
  // Serial.println(forecast[index_begin].date);
  // Serial.print("index_begin: ");
  // Serial.println(index_begin);
  //Serial.println(String(forecast[index_begin].date));
  // Serial.print("forecast[index_begin].date: ");
  // Serial.println(forecast[index_begin].date);
  // display.print(forecast[index_begin].date);
  // display.print(", ");
  // display.print(forecast[index_begin].day);

  // display.setCursor(CURSOR_X(0), CURSOR_Y(2));
  // display.print(forecast[index_begin].minTemp);
  // display.print(" ... ");
  // display.print(forecast[index_begin].maxTemp);
  // display.print((char)247);
  // display.print("C");
  // display.setCursor(CURSOR_X(0), CURSOR_Y(3));
  // display.print(forecast[index_begin].condition);


  // display.setCursor(CURSOR_X(0), CURSOR_Y(4));
  // display.print(forecast[index_end].date);
  // display.print(", ");
  // display.print(forecast[index_end].day);

  // display.setCursor(CURSOR_X(0), CURSOR_Y(5));
  // display.print(forecast[index_end].minTemp);
  // display.print(" ... ");
  // display.print(forecast[index_end].maxTemp);
  // display.print((char)247);
  // display.print("C");
  // display.setCursor(CURSOR_X(0), CURSOR_Y(6));
  // display.print(forecast[index_end].condition);

  display.display();
}

void setCO2histScreen(){
  display.clearDisplay();
  display_font_size = 1;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("CO2 in hist");
  display.display();
}

void setTempInHistScreen(){
  display.clearDisplay();
  display_font_size = 1;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("Temp. in hist");
  display.display();
}

void setHumInHistScreen(){
  display.clearDisplay();
  display_font_size = 1;
  display.setTextSize(display_font_size);
  display.setCursor(CURSOR_X(0), CURSOR_Y(0));
  display.print("Hum. in hist");
  display.display();
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
  // lcd.setCursor(0, 0);
  // lcd.clear();
  // lcd.print("Init sensors...");
  // delay(1000);
  // lcd.clear();
  // lcd.print("Temp. & humid.");
  // lcd.setCursor(0, 1);
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