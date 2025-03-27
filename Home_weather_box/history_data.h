// #pragma once
#include <GyverOLED.h>

const uint8_t HIST_DATA_BUFFER_SIZE = 24;
int16_t co2Buffer[HIST_DATA_BUFFER_SIZE] = {0};  // set 0 by default
uint8_t co2Head = HIST_DATA_BUFFER_SIZE - 1;      // the last by time element

int8_t tempInBuffer[HIST_DATA_BUFFER_SIZE] = {0};  // set 0 by default
uint8_t tempInHead = HIST_DATA_BUFFER_SIZE - 1;      // the last by time element

int8_t humInBuffer[HIST_DATA_BUFFER_SIZE] = {0};  // set 0 by default
uint8_t humInHead = HIST_DATA_BUFFER_SIZE - 1;      // the last by time element

void addCO2(int16_t value) {
    co2Head = (co2Head + 1) % HIST_DATA_BUFFER_SIZE;
    co2Buffer[co2Head] = value;
}

int16_t getCO2_HoursAgo(uint8_t hoursAgo) {
    if (hoursAgo >= HIST_DATA_BUFFER_SIZE) hoursAgo = HIST_DATA_BUFFER_SIZE - 1;
    int16_t index = co2Head - hoursAgo;
    if (index < 0) index += HIST_DATA_BUFFER_SIZE;
    return co2Buffer[index];
}

void addTempIn(int8_t value) {
    tempInHead = (tempInHead + 1) % HIST_DATA_BUFFER_SIZE;
    tempInBuffer[tempInHead] = value;
}

int16_t getTempIn_HoursAgo(uint8_t hoursAgo) {
    if (hoursAgo >= HIST_DATA_BUFFER_SIZE) hoursAgo = HIST_DATA_BUFFER_SIZE - 1;
    int16_t index = tempInHead - hoursAgo;
    if (index < 0) index += HIST_DATA_BUFFER_SIZE;
    return tempInBuffer[index];
}

void addHumIn(int8_t value) {
  humInHead = (humInHead + 1) % HIST_DATA_BUFFER_SIZE;
  humInBuffer[humInHead] = value;
}

int16_t getHumIn_HoursAgo(uint8_t hoursAgo) {
  if (hoursAgo >= HIST_DATA_BUFFER_SIZE) hoursAgo = HIST_DATA_BUFFER_SIZE - 1;
  int16_t index = humInHead - hoursAgo;
  if (index < 0) index += HIST_DATA_BUFFER_SIZE;
  return humInBuffer[index];
}



// for debug
void CO2genRandomValue() {
  int16_t value = 1200;
  int16_t drift = random(-400, 400);
  addCO2(value + drift);
  Serial.print("Generated co2 data: "); Serial.println(value + drift);
}

void printCO2History() {
  for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
    Serial.print("Hour ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(getCO2_HoursAgo(HIST_DATA_BUFFER_SIZE - 1 - i));
  }
}

void printTempInHistory() {
  for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
    Serial.print("Hour ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(getTempIn_HoursAgo(HIST_DATA_BUFFER_SIZE - 1 - i));
  }
}

void TempInGenRandomValue() {
  int8_t value = 15;
  int8_t drift = random(-25, 15);
  addTempIn(value + drift);
  Serial.print("Generated temp. data: "); Serial.println(value + drift);
}