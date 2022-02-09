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
void Actualize_Window_Bright(Bright* const me, Adafruit_ST7735 *tft, bool wind_changed)
{

    /*
    tft->fillCircle(x_centre, y_centre, rad, ST77XX_RED);
    tft->fillCircle(x_centre, y_centre, rad - thick, ST77XX_WHITE);
    */
   
    if (wind_changed)
    {
        //Clear screen
        tft->fillScreen(ST77XX_WHITE);
        tft->fillCircle(50, 65, 30, ST77XX_BLACK);
    }
    tft->fillCircle(50, 65, 30 - 4, ST77XX_WHITE);
    //Set text cursor
    tft->setCursor(40, 52);
    //Set text color
    tft->setTextColor(ST77XX_BLACK);
    //Set text size
    tft->setTextSize(4);

    if (me->automatic)
    {
        //Print "A" that means automátic
        tft->println("A");
    }
    else
    {
        //Print "M" that means manual
        tft->println("M");
    }

    tft->fillRoundRect(110, 10, 30, 110, 13, ST77XX_WHITE);



    switch (me->bright_percent)
    {
        case PERCENT_0:

            break;
        
        case PERCENT_12_5:
            tft->fillRoundRect(110, 97, 30, 17, 10, ST77XX_BLACK);
            break;

        case PERCENT_25:
            tft->fillRoundRect(110, 89, 30, 25, 10, ST77XX_BLACK);
            break;

        case PERCENT_37_5:
            tft->fillRoundRect(110, 77, 30, 37, 10, ST77XX_BLACK);
            break;

        case PERCENT_50:
            tft->fillRoundRect(110, 64, 30, 50, 10, ST77XX_BLACK);
            break;

        case PERCENT_62_5:
            tft->fillRoundRect(110, 52, 30, 62, 10, ST77XX_BLACK);
            break;

        case PERCENT_75:
            tft->fillRoundRect(110, 39, 30, 75, 10, ST77XX_BLACK);
            break;

        case PERCENT_87_5:

            tft->fillRoundRect(110, 27, 30, 87, 10, ST77XX_BLACK);
            break;

        case PERCENT_100:

            tft->fillRoundRect(110, 14, 30, 100, 10, ST77XX_BLACK);
            break;
        
    }
    
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

    if (wind_changed || center_pressed || low_pressed || up_pressed) Actualize_Window_Bright(me, tft, wind_changed);

    if (last_percent != me->bright_percent){
        me->changed = true;
    }else{
        me->changed = false;
    }

    last_percent = me->bright_percent;
}