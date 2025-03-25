// #pragma once
#include <GyverOLED.h>

const uint8_t HIST_DATA_BUFFER_SIZE = 24;
uint16_t co2Buffer[HIST_DATA_BUFFER_SIZE] = {0};  // set 0 by default
uint8_t co2Head = HIST_DATA_BUFFER_SIZE - 1;      // the last by time element

uint16_t tempInBuffer[HIST_DATA_BUFFER_SIZE] = {0};  // set 0 by default
int8_t tempInHead = HIST_DATA_BUFFER_SIZE - 1;      // the last by time element

uint16_t humInBuffer[HIST_DATA_BUFFER_SIZE] = {0};  // set 0 by default
uint8_t humInHead = HIST_DATA_BUFFER_SIZE - 1;      // the last by time element


void addCO2(uint16_t value) {
    co2Head = (co2Head + 1) % HIST_DATA_BUFFER_SIZE;
    co2Buffer[co2Head] = value;
}

uint16_t getCO2_HoursAgo(uint8_t hoursAgo) {
    if (hoursAgo >= HIST_DATA_BUFFER_SIZE) hoursAgo = HIST_DATA_BUFFER_SIZE - 1;
    int16_t index = co2Head - hoursAgo;
    if (index < 0) index += HIST_DATA_BUFFER_SIZE;
    return co2Buffer[index];
}

void printCO2History() {
    for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
      Serial.print("Hour ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(getCO2_HoursAgo(HIST_DATA_BUFFER_SIZE - 1 - i));
    }
  }

// for debug
void CO2genRandomValue() {
    int16_t value = 1200;
    uint16_t drift = random(-400, 400);
    addCO2(value + drift);
}

void addTempIn(uint16_t value) {
    tempInHead = (tempInHead + 1) % HIST_DATA_BUFFER_SIZE;
    tempInBuffer[tempInHead] = value;
}

uint16_t getTempIn_HoursAgo(uint8_t hoursAgo) {
    if (hoursAgo >= HIST_DATA_BUFFER_SIZE) hoursAgo = HIST_DATA_BUFFER_SIZE - 1;
    int16_t index = tempInHead - hoursAgo;
    if (index < 0) index += HIST_DATA_BUFFER_SIZE;
    return tempInBuffer[index];
}

void printTempInHistory() {
    for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
      Serial.print("Hour ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(getTempIn_HoursAgo(HIST_DATA_BUFFER_SIZE - 1 - i));
    }
  }

// for debug
void TempInGenRandomValue() {
    int16_t value = 15;
    int16_t drift = random(-25, 15);
    addTempIn(value + drift);
    //Serial.println(value + drift);
}

// for debug
// void CO2genRandomData() {
//     for (int i = 0; i < HIST_DATA_BUFFER_SIZE; i++) {
//         addCO2(CO2genRandomValue());
//     }
//     printCO2History();
// }

