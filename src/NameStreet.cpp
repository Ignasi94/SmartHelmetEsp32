#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "NameStreet.h"

void setNameStreet(char* name_of_street, int x_pos, int y_pos, Adafruit_ST7735 *tft) {
  //tft.setCursor(5, 100);
  static char line1[100], line2[28];
  static int len_of_name = 0;
  static int no_hi_ha_guio = 0;
  static int text_name_alredy_printed = 0;
  static int last_state_size_of_print = 0;
  static char last_name_of_street[100];


  //Si es igual al anterior, borrem
  //last_name_of_street = (char*)malloc(100);
  if (strcmp(last_name_of_street, name_of_street) != 0) {
    for (int16_t x = 0; x < tft->width(); x++) {
      for (int16_t y = (y_pos - 14); y < tft->height(); y++) {
        tft->drawPixel(x, y, ST77XX_WHITE);
      }
    }
  }
  strcpy(last_name_of_street, name_of_street);





  //Borrem anterior printf
  switch (last_state_size_of_print) {
    case 1:
      tft->setCursor(x_pos, y_pos - 10);
      tft->setTextColor(ST77XX_WHITE);
      tft->setTextSize(3);
      tft->println((char *)line1);
      break;

    case 2:
      //borrem linea 1
      tft->setCursor(x_pos, y_pos - 14);
      tft->setTextColor(ST77XX_WHITE);
      tft->setTextSize(2);
      tft->println((char *) line1);
      //Borrem linea 2
      tft->setCursor(x_pos, y_pos + 2);
      tft->setTextColor(ST77XX_WHITE);
      tft->setTextSize(2);
      tft->println((char *)line2);

      break;

    case 3:

      //Borrem linea 1
      tft->setCursor(x_pos, y_pos - 7);
      tft->setTextColor(ST77XX_WHITE);
      tft->setTextSize(1);
      tft->println((char *) line1);

      //Borrem linea 2
      tft->setCursor(x_pos, y_pos + 3);
      tft->setTextColor(ST77XX_BLACK);
      tft->setTextSize(1);
      tft->println((char *)line2);

      break;
  }

  len_of_name = strlen(name_of_street);
  Serial.println("number of len street IS: " + len_of_name);
  Serial.println(len_of_name);

  if (len_of_name <= 8) {

    last_state_size_of_print = 1;
    //line1 = (char*)malloc(26);
    strcpy(line1, name_of_street);
    tft->setCursor(x_pos, y_pos - 10);
    tft->setTextColor(ST77XX_BLACK);
    tft->setTextSize(3);
    tft->println((char *)line1);

  }
  if (len_of_name <= 26 && len_of_name > 8) {
    last_state_size_of_print = 2;
    //line1 = (char*)malloc(26);
    strcpy(line1, name_of_street);
    tft->setCursor(x_pos, y_pos - 14);
    tft->setTextColor(ST77XX_BLACK);
    tft->setTextSize(2);
    //controlem si no hi ha espai entre lineas, possem "-"
    if (name_of_street[13] != ' ') {
      line1[12] = '-';
      no_hi_ha_guio = 0;
    } else {
      no_hi_ha_guio = 1;
    }

    tft->println((char *) line1);

    if (no_hi_ha_guio == 1) {
      for (int i = 0; i <= len_of_name; i++) {
        line2[i + 1] = '\0';
        line2[i] = name_of_street[i + 13 + 1];
      }
    } else {
      for (int i = 0; i <= len_of_name; i++) {
        line2[i + 1] = '\0';
        line2[i] = name_of_street[i + 13 - 1];
      }
    }

    tft->setCursor(x_pos, y_pos + 2);
    tft->setTextColor(ST77XX_BLACK);
    tft->setTextSize(2);
    tft->println((char *)line2);

  }


  if (len_of_name > 26) {
    last_state_size_of_print = 3;
    //line1 = (char*)malloc(26);
    strcpy(line1, name_of_street);
    tft->setCursor(x_pos, y_pos - 7);
    tft->setTextColor(ST77XX_BLACK);
    tft->setTextSize(1);
    //controlem si no hi ha espai entre lineas, possem "-"
    if (name_of_street[27] != ' ') {
      line1[26] = '-';
      no_hi_ha_guio = 0;
    } else {
      no_hi_ha_guio = 1;
    }

    tft->println((char *) line1);

    if (len_of_name > 49) {
      len_of_name = 49;
    }

    if (no_hi_ha_guio == 1) {
      for (int i = 0; i <= len_of_name ; i++) {
        line2[i + 1] = '\0';
        line2[i] = name_of_street[i + 27 + 1];
      }
    } else {
      for (int i = 0; i <= len_of_name; i++) {
        line2[i + 1] = '\0';
        line2[i] = name_of_street[i + 27 - 1];
      }
    }

    tft->setCursor(x_pos, y_pos + 3);
    tft->setTextColor(ST77XX_BLACK);
    tft->setTextSize(1);
    tft->println((char *)line2);

  }

  memset((line1), '\0', (100));
  memset((line2), '\0', (28));

}
