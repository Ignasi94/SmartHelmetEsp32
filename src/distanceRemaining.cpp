#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "distanceRemaining.h"

int distRemMet = 0;
int distRemKm = 0;


int arroundMet(int remainInMeters) {
  int toRet = 0;
  int mod = 0;
  if (remainInMeters < 10) {
    toRet = 0;
  } else {
    if (remainInMeters < 99) {
      toRet = (int )(remainInMeters / 10);
      toRet = toRet * 10;
    } else {
      mod = remainInMeters % 100;
      toRet = (int )(remainInMeters / 100);
      toRet = toRet * 100;
      if (mod <= 10) {
        toRet = toRet;
      } else {
        if (mod <= 35) {
          toRet = toRet + 25;

        } else {
          if ( mod <= 64) {
            toRet = toRet + 50;
          } else {
            if ( mod <= 85) {
              toRet = toRet + 75;
            } else {
              toRet = toRet + 100;
            }
          }
        }
      }
    }
  }
  return toRet;
}

int arroundKm(int remainInMeters) {
  return (int) (remainInMeters / 1000);
}



void printRemainingDistance(int totalMeters, Adafruit_ST7735 *tft, int * clear_remain_meters){
  
  static int unit = 0;//0 if meters, 1 if km
  static char buff_met[10], buff_km[10];
  static char value[10];//value of m/km in string
  static char last_value[10];
  static int distRemKm = 0, distRemMet = 0;
  static int first_time = 1;

  if (first_time == 1 || *clear_remain_meters == 1){
    first_time = 0;
    *clear_remain_meters = 0;
    for (int16_t x = 2; x < 90; x++) {
      for (int16_t y = 77; y < 94; y++) {
        tft->drawPixel(x, y, ST77XX_WHITE);
      }
    }
  }
  

  
  if (totalMeters < 1000){
     
    distRemMet = arroundMet(totalMeters);
    memset((value), '\0', (10));
    itoa(distRemMet, value, 10);
    strcat(value, " m");
  }else{
    if (totalMeters < 3000){
      
      distRemMet = (arroundMet((totalMeters) % 1000)) / 10;
      distRemKm = arroundKm(totalMeters);
    
      memset((buff_met), '\0', (5));
      memset((buff_km),  '\0', (5));

      itoa(distRemMet, buff_met, 10);
      itoa(distRemKm, buff_km, 10);

      strcat(buff_km, ",");
      strcat(buff_km, buff_met);

      memset((value), '\0', (10));
      strcpy(value, buff_km);
      strcat(value, " Km");
    }else{
      distRemKm = arroundKm(totalMeters);
      memset((value), '\0', (10));
      itoa(distRemKm, value, 10);
      strcat(value, " Km");
    }       
  }
  strcat(value, " to:");

  
  if (strcmp(last_value, value) != 0){
    tft->setCursor(2, 77);
    tft->setTextColor(ST77XX_WHITE);
    tft->setTextSize(2);
    tft->println((char *)last_value);
  }

  strcpy(last_value, value);
    
  tft->setCursor(2, 77);
  tft->setTextColor(ST77XX_BLACK);
  tft->setTextSize(2);
  tft->println((char *)value);
}
