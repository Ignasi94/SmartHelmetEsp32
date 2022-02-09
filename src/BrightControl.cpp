#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "BrightControl.h"




/***********************************************
*
* @Finality: Function that returns the Bright percent 
* @Parameters: Bright object.
* @Return: Return the actual percent bright.
*
************************************************/
uint8_t getPercentBright(const Bright* const me){

    return me->bright_percent;

}



/***********************************************
*
* @Finality: free the object info
* @Parameters: Bright object
* @Return: -
*
************************************************/
void Bright_Destroy(Bright* const me) { 

    free(me);

}



/***********************************************
*
* @Finality: Initialize the object
* @Parameters: Window object
* @Return: -
*
************************************************/
void Bright_init(Bright* const me) {

    me->automatic = false;
    me->bright_percent = PERCENT_50;
    me->status_machine = 0;
    me->changed = false;

}




/***********************************************
*
* @Finality: Create the object.
* @Parameters: -
* @Return: Return object created and initialized
*
************************************************/
Bright * Bright_Create(void){

    Bright * me = (Bright *) malloc(sizeof(Bright));

    if(me!=NULL)
    {
    
        Bright_init(me);
    
    }
    
    return me;

}


/***********************************************
*
* @Finality: Function that actualize the screen from the bright configuration
* @Parameters: -
* @Return: -
*
************************************************/
void Actualize_Window_Bright(Bright* const me, Adafruit_ST7735 *tft)
{
    //Clear screen
    tft->fillScreen(ST77XX_WHITE);
    /*
    tft->fillCircle(x_centre, y_centre, rad, ST77XX_RED);
    tft->fillCircle(x_centre, y_centre, rad - thick, ST77XX_WHITE);
    */
    tft->fillCircle(50, 50, 10, ST77XX_RED);
    tft->fillCircle(50, 50, 10 - 2, ST77XX_WHITE);
    //Set text cursor
    tft->setCursor(50, 10);
    //Set text color
    tft->setTextColor(ST77XX_BLACK);
    //Set text size
    tft->setTextSize(3);
    if (me->automatic){
        tft->println("A");
    }else{
        tft->println("M");
    }
    //Print "A" that means automátic
}


/***********************************************
*
* @Finality: Motor with the logic of the bright configuration.
* @Parameters: The object of the Bright class
* @Return: -
*
************************************************/
void Motor_Bright_Control(Bright *me, bool up_pressed, bool low_pressed, bool center_pressed, bool wind_changed, Adafruit_ST7735 *tft){
    uint8_t aux_bright_changed = false;
    uint8_t last_percent;
    if (me->automatic)
    {
        //Automatic bright
        if (center_pressed == true)
        {
            me->automatic = false;
        }

    }else{
        //Manual bright
        if (up_pressed)
        {
            
            if (me->bright_percent >= 8){
                me-> bright_percent = 8;
            }else{
                me->bright_percent++;
            }
        }

        if (low_pressed)
        {
            
            if(me->bright_percent <= 0)
            {
                me->bright_percent = 0;
            }
            else
            {
                me->bright_percent--;
            }
        }

        if (center_pressed == true)
        {
            me->automatic = true;
        }

    }

    if (wind_changed || center_pressed || low_pressed || up_pressed) Actualize_Window_Bright(me, tft);

    if (last_percent != me->bright_percent){
        me->changed = true;
    }else{
        me->changed = false;
    }

    last_percent = me->bright_percent;
}