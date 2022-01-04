#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "SpeedLimit.h"

int lastSpeedValue = 0;



void setSpeedLimitCircle(int x_centre, int y_centre, int thick, int rad, int speed_lim, Adafruit_ST7735 *tft, int * come_from_disconnect_speed_limiter) {
  static int circle_alredy_printed = 0;
  static int last_speed_lim = 30;


  if (circle_alredy_printed == 0 || *come_from_disconnect_speed_limiter == 1) {
    tft->fillCircle(x_centre, y_centre, rad, ST77XX_RED);
    tft->fillCircle(x_centre, y_centre, rad - thick, ST77XX_WHITE);
    //circle_alredy_printed++;
  }

  if (last_speed_lim != speed_lim || circle_alredy_printed == 0 || *come_from_disconnect_speed_limiter == 1) {
    tft->fillCircle(x_centre, y_centre, rad - thick, ST77XX_WHITE);
    if (speed_lim <= 99) {
      //When have 2 digits
      tft->setCursor(x_centre - 15, rad - 7);
    } else {
      //When have 3 digits
      tft->setCursor(x_centre - 26, rad - 7);
    }
    tft->setTextColor(ST77XX_BLACK);
    tft->setTextSize(3);
    tft->println((int)(speed_lim));
  }

  if (circle_alredy_printed == 0) {
    circle_alredy_printed++;
  }

  if (*come_from_disconnect_speed_limiter == 1){
    *come_from_disconnect_speed_limiter = 0;
  }

  last_speed_lim = speed_lim;
}
