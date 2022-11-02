#ifndef BRIGHTCONTROL_H
#define BRIGHTCONTROL_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

#define PERCENT_0 0

#define PERCENT_25 1
#define PERCENT_50 2
#define PERCENT_75 3
#define PERCENT_100 4





typedef struct Bright{

    bool automatic;
    uint8_t bright_percent;
    uint8_t status_machine;
    bool changed;

}Bright;


/***********************************************
*
* @Finality: Create the object.
* @Parameters: -
* @Return: Return object created and initialized
*
************************************************/
Bright * Bright_Create(void);


/***********************************************
*
* @Finality: Motor with the logic of the bright configuration.
* @Parameters: me, of  Bright class
* @Parameters: up_pressed, boolean up button pressed
* @Parameters: low_pressed, boolean low button pressed
* @Parameters: center_pressed, boolean center button pressed
* @Parameters: wind_changed
* @Parameters: tft, from class Adafruit_ST7735
* @Return: -
*
************************************************/
void Motor_Bright_Control(Bright * me, bool up_pressed, bool low_pressed, bool center_pressed, bool wind_changed, Adafruit_ST7735 *tft);




/***********************************************
*
* @Finality: Function that returns the Bright percent 
* @Parameters: Bright object.
* @Return: Return the actual percent bright.
*
************************************************/
uint8_t getPercentBright(const Bright* const me);



/***********************************************
*
* @Finality: free the object info
* @Parameters: Bright object
* @Return: -
*
************************************************/
void Bright_Destroy(Bright* const me);


#endif
