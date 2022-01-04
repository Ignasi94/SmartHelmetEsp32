#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "icons.h"

int last_id_maneuver = 0;
uint8_t buff_ram[SIZE_OF_ICON];
uint32_t aux = 0x00000000;

struct Position
{
	int x;
	int y;
};

void writeInLCD(int col_offset, int fil_offset, const uint8_t (&icon)[SIZE_OF_ICON], Adafruit_ST7735 *tft);
//void writeInLCDBleIcon(int col_offset, int fil_offset, const uint8_t (&icon)[SIZE_OF_ICON], Adafruit_ST7735 *tft);
void writeInLCDBleIcon(int green_not_black, int col_offset, int fil_offset, const uint8_t (&icon)[SIZE_OF_ICON], Adafruit_ST7735 *tft);
void writeInLCDRam(int col_offset, int fil_offset, const uint8_t (&icon)[SIZE_OF_ICON], Adafruit_ST7735 *tft);


void printIcon(int type_of_icon, int id_of_icon, int col_offset, int fil_offset, Adafruit_ST7735 *tft){

  switch(type_of_icon){

    //type ble icons
    case 0:
        //if->1 device connected, else->0 searching
        //if (id_of_icon == 1){
        writeInLCDBleIcon(id_of_icon, col_offset, fil_offset, image_data_352060_bluetooth_searching_icon1_green, tft);   
        //}
      break;

    //Maneuver Icons
    case 1:

      //Only we will actualize icon maneuver id if the id has changed
      //if (last_id_maneuver != id_of_icon){
      //tft->fillScreen(ST77XX_WHITE);
      //tft->fillRect(0, 0, 90, 90, ST7735_WHITE);
      switch(id_of_icon){
        case -1:
          writeInLCD(col_offset, fil_offset, clean_icon, tft);
          break;

        case 1: case 2: case 79:
          writeInLCD(col_offset, fil_offset, image_data_direction_continue_uturn, tft);
          break;

        case 3: case 4: case  5: case 8:
          writeInLCD(col_offset, fil_offset, image_data_direction_new_name_sharp_right, tft);
          break;

        case 6: case 7:
          writeInLCD(col_offset, fil_offset, image_data_direction_rotary_sharp_right, tft);
          break;

        case 9:
          writeInLCD(col_offset, fil_offset, image_data_direction_arrive_right, tft);
          break;

        /*case 10:
          //writeInLCD(col_offset, fil_offset, image_data_direction_depart_right, tft);
            break;*/

        case 11: case 15: case 16: case 18: case 21: case 10:
          writeInLCD(col_offset, fil_offset, image_data_direction_continue_right, tft);
          break;

        case 17:
          writeInLCD(col_offset, fil_offset, image_data_direction_off_ramp_right, tft);
          break;

        case 19: case 20:
          writeInLCD(col_offset, fil_offset, image_data_direction_rotary_right, tft);
          break;

        case 22: case 25: case 26: case 28: case 31:
          writeInLCD(col_offset, fil_offset, image_data_direction_continue_slight_right, tft);
          break;

        case 23: 
          writeInLCD(col_offset, fil_offset, image_data_direction_fork_slight_right, tft);
          break;

        case 24: 
          writeInLCD(col_offset, fil_offset, image_data_direction_merge_slight_right, tft);
          break;

        case 29: case 30:
          writeInLCD(col_offset, fil_offset, image_data_direction_roundabout_slight_right, tft);
          break;

        case 32: case 72:
          writeInLCD(col_offset, fil_offset, image_data_direction_arrive, tft);
          break;

        case 34: case 36: case 37: case 38: case 39: case 42: case 74:
          writeInLCD(col_offset, fil_offset, image_data_direction_continue, tft);
          break;

        case 40: case 41:
          writeInLCD(col_offset, fil_offset, image_data_direction_rotary_straight, tft);
          break;

        case 43: case 46: case 47: case 49: case 52:
          writeInLCD(col_offset, fil_offset, image_data_direction_continue_slight_left, tft);
          break;

        case 44:
          writeInLCD(col_offset, fil_offset, image_data_direction_fork_slight_left, tft);
          break;

        case 45:
          writeInLCD(col_offset, fil_offset, image_data_direction_merge_slight_left, tft);
          break;

        case 48:
          writeInLCD(col_offset, fil_offset, image_data_direction_off_ramp_slight_left, tft);
          break;

        case 50: case 51:
          writeInLCD(col_offset, fil_offset, image_data_direction_roundabout_slight_left, tft);
          break;

        case 53: 
          writeInLCD(col_offset, fil_offset, image_data_direction_arrive_left, tft);
          break;

        /*case 54:
          //writeInLCD(col_offset, fil_offset, image_data_direction_depart_left, tft);
          break;*/

        case 55: case 59: case 60: case 65: case 54:
          writeInLCD(col_offset, fil_offset, image_data_direction_continue_left, tft);
          break;

        case 56:
          writeInLCD(col_offset, fil_offset, image_data_direction_end_of_road_left, tft);
          break;

        case 57:
          writeInLCD(col_offset, fil_offset, image_data_direction_fork_left, tft);
          break;

        case 58:
          writeInLCD(col_offset, fil_offset, image_data_direction_merge_left, tft);
          break;

        case 61:
          writeInLCD(col_offset, fil_offset, image_data_direction_off_ramp_left, tft);
          break;

        case 63: case 64:
          writeInLCD(col_offset, fil_offset, image_data_direction_rotary_left, tft);
          break;

        case 66: case 67: case 68: case 71:
          writeInLCD(col_offset, fil_offset, image_data_direction_new_name_sharp_left, tft);
          break;

        case 69: case 70:
          writeInLCD(col_offset, fil_offset, image_data_direction_rotary_sharp_left, tft);
          break;

        case 75: 
         // writeInLCD(col_offset, fil_offset, image_data_direction_fork, tft);
          break;

        case 76: case 77:
          writeInLCD(col_offset, fil_offset, image_data_direction_rotary, tft);
          break;

        case 78:
          //writeInLCD(col_offset, fil_offset, image_data_direction_updown, tft);
          break;

        case 80:
          //writeInLCD(col_offset, fil_offset, image_data_direction_close, tft);
          break;

        case 81:
          //writeInLCD(col_offset, fil_offset, image_data_direction_flag, tft);
          break;   
        
        break;
        
      }
      
      //last_id_maneuver = id_of_icon; 
      break;

    //clear screen
    case 2:
      tft->fillScreen(ST77XX_WHITE);
      break;

    case 3:
      writeInLCDRam(col_offset, fil_offset, image_data_direction_continue_right, tft);
      break;

    case 4:
      writeInLCDRam(col_offset, fil_offset, image_data_direction_continue_uturn, tft);
      break;

    case 5:
      writeInLCDRam(col_offset, fil_offset, image_data_direction_new_name_sharp_right, tft);
      break;

    case 6:
      writeInLCDRam(col_offset, fil_offset, image_data_direction_arrive_left, tft);
      break;

    default:
      break;
    
  }
}


void writeInLCDRam(int col_offset, int fil_offset, const uint8_t (&icon)[SIZE_OF_ICON], Adafruit_ST7735 *tft){
  //tft.fillScreen(ST77XX_WHITE);
  int fil = 0;
  int col = 0;

 
  //delay(1000);
  //Serial.println("");
  int j = 0;
  
  for (fil = 0 + fil_offset; fil < NUM_FIL_ICON + fil_offset; fil++) {
    for (col = 0 + col_offset; col < NUM_COL_ICON + col_offset; col++) {
        buff_ram[j] = icon[((fil - fil_offset) * NUM_COL_ICON + (col - col_offset))];
        /*if (icon[((fil - fil_offset) * NUM_COL_ICON + (col - col_offset))] == 0x00){
          //aux = 0x00000000;
          buff_ram[j] = 0x00000000;
          //pos[j].x = col;
          //pos[j].y = fil;
          
        }else{
          buff_ram[j] = 0xFFFFFFFF;
        }*/
        j++;
        //image.addPoint(x, y);        
        //tft->drawPixel(col, fil, aux);
        //Serial.print(image_data_direction_arrive_left[(fil * 100 + col)]);
    }
  } 
  /*pos[j].x = -100;
  pos[j].y = -100;   

  for (int i = 0; i < j || i < 100000; i++){
    tft->drawPixel(pos[j].x, pos[j].y, aux);
  }*/
  for (fil = 0 + fil_offset; fil < NUM_FIL_ICON + fil_offset; fil++) {
    for (col = 0 + col_offset; col < NUM_COL_ICON + col_offset; col++) {
        if (buff_ram[((fil - fil_offset) * NUM_COL_ICON + (col - col_offset))] == 0x00){
          aux = 0x00000000;
        }else{
          aux = 0xFFFFFFFF;
        }
        //image.addPoint(x, y);        
        tft->drawPixel(col, fil, aux);
        //Serial.print(image_data_direction_arrive_left[(fil * 100 + col)]);
    }
  }
}


void writeInLCD(int col_offset, int fil_offset, const uint8_t (&icon)[SIZE_OF_ICON], Adafruit_ST7735 *tft){
  //tft.fillScreen(ST77XX_WHITE);
  int fil = 0;
  int col = 0;
  //delay(1000);
  //Serial.println("");
  
  for (fil = 0 + fil_offset; fil < NUM_FIL_ICON + fil_offset; fil++) {
    for (col = 0 + col_offset; col < NUM_COL_ICON + col_offset; col++) {
        if (icon[((fil - fil_offset) * NUM_COL_ICON + (col - col_offset))] == 0x00){
          aux = 0x00000000;
        }else{
          aux = 0xFFFFFFFF;
        }
        //image.addPoint(x, y);        
        tft->drawPixel(col, fil, aux);
        //Serial.print(image_data_direction_arrive_left[(fil * 100 + col)]);
    }
  }    
}

void writeInLCDBleIcon(int green_not_black, int col_offset, int fil_offset, const uint8_t (&icon)[SIZE_OF_ICON], Adafruit_ST7735 *tft){
  //tft.fillScreen(ST77XX_WHITE);
  int fil = 0;
  int col = 0;
  //delay(1000);
  //Serial.println("");
  
  for (fil = 0 + fil_offset; fil < NUM_FIL_ICON + fil_offset; fil++) {
    for (col = 0 + col_offset; col < NUM_COL_ICON + col_offset; col++) {
        
        //image.addPoint(x, y);
       
        if (icon[((fil - fil_offset) * NUM_COL_ICON + (col - col_offset))] == 0xFF){
          aux = 0xFFFFFFFF;
          
        }else{
          if (green_not_black == 1){
            aux = ST77XX_GREEN;
          }else{
            aux = 0x00000000;
          }
        }
                
        tft->drawPixel(col, fil, aux);
        //Serial.print(image_data_direction_arrive_left[(fil * 100 + col)]);
    }
  }    
}
