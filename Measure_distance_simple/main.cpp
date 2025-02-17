#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define trig_pin 12
#define echo_pin 11

int dist = 0;
int dist_filt = 0;
int t = 24; // speed of sound depends on temperature
float current_DS18B20_t; // time to LCD update
float project_delay = 50;

LiquidCrystal_I2C lcd(0x27, 16, 2); // USE DEFAULT PINS sda: A4, scl: A5!

int get_mm(int t);
void lcd_update(int dist_filt);

void setup()
{
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Init project...");
  delay(1000);
  lcd.clear();
  lcd.print("Distance meas.");
  lcd.setCursor(0, 1);
  lcd.print("v.0.1");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ???");
  delay(1000);
}

void loop()
{
  dist = get_mm(t);
  int prev_dist_filt = dist_filt;
  dist_filt += (dist - dist_filt) * 0.2; // apply filter to avoid anomalies in data
//   if (dist_filt > 400){ // to avoid anomalies
//     dist_filt = 400;
//   }
  if(prev_dist_filt != dist_filt){
    lcd_update(dist_filt);
  }
  Serial.println(String(dist_filt) + " мм");
  delay(project_delay);
}

// get measure in mm
int get_mm(int t)
{
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10); // send a signal for 10 microsec
  digitalWrite(trig_pin, LOW);
  uint32_t us = pulseIn(echo_pin, HIGH); // measure the signal
  // 't * 6 / 10' - additional coef. to account temperature
  // 2000 - to calc the distance in mm. divided by 2 (the signal does 2 ways: forward and back)
  return (us * (t * 6 / 10 + 330) / 2000ul);
}

void lcd_update(int dist_filt) {
  static uint32_t tmr;
  if (millis() - tmr >= 30) {
    //lcd.clear();
    tmr = millis();
    // lcd.setCursor(0, 0);
    // lcd.print("Distance: ");
    lcd.setCursor(9, 0); // LCD doesn't not need to be fully refreshed
    lcd.print(dist_filt);
    lcd.print("mm ");

    // String s = "";
    // for (int i = 0; i < round(dist_filt/16); i++) {
    //   s += "*";
    // }
    // lcd.setCursor(0, 1);
    // lcd.print(s);
  }
}