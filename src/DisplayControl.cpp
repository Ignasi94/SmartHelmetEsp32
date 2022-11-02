#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "DisplayControl.h"




/***********************************************
*
* @Finality: Function that returns the Volume percent, and play/stop status
* @Parameters: Display object.
* @Return: Return the actual percent volume.
*
************************************************/
uint8_t getPercentVolume(const Display* const me)
{

    return me->volume_percent;

}



/***********************************************
*
* @Finality: free the object info
* @Parameters: Display object
* @Return: -
*
************************************************/
void Display_Destroy(Display* const me)
{ 

    free(me);

}



/***********************************************
*
* @Finality: Initialize the object
* @Parameters: Window object
* @Return: -
*
************************************************/
void Display_init(Display* const me)
{

    me->playNotStop = false;
    me->volume_percent = PERCENT_50;
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
Display * Display_Create(void)
{

    Display * me = (Display *) malloc(sizeof(Display));

    if(me!=NULL)
    {
    
        Display_init(me);
    
    }
    
    return me;

}


/***********************************************
*
* @Finality: Function that actualize the screen from the display configuration
* @Parameters: -
* @Return: -
*
************************************************/
void Actualize_Window_Display(Display* const me, Adafruit_ST7735 *tft, bool wind_changed)
{
    static uint8_t last_volume = PERCENT_0;

    /*
    tft->fillCircle(x_centre, y_centre, rad, ST77XX_RED);
    tft->fillCircle(x_centre, y_centre, rad - thick, ST77XX_WHITE);
    */
   
    if (wind_changed)
    {
        //Clear screen
        tft->fillScreen(ST77XX_WHITE);

        tft->setCursor(10, 10);
        tft->setTextSize(1);
        tft->println("Display Config");
        
    }
    //Set text cursor
    tft->setCursor(13, 52);
    //Set text color
    tft->setTextColor(ST77XX_BLACK);
    //Set text size
    tft->setTextSize(2);

    if (me->playNotStop)
    {
        //Print "Play" 
        tft->setCursor(13, 52);
        tft->setTextColor(ST77XX_WHITE);
        tft->println("Pause");
        tft->setTextColor(ST77XX_BLACK);
        tft->setCursor(13, 52);
        tft->println("Play");
    }
    else
    {
        //Print "M" that means manual
        tft->setTextColor(ST77XX_WHITE);
        tft->setCursor(13, 52);
        tft->println("Play");
        tft->setTextColor(ST77XX_BLACK);
        tft->setCursor(13, 52);
        tft->println("Pause");
    }

    



    if (me->volume_percent != last_volume || wind_changed)
    {
        tft->fillRoundRect(110, 10, 30, 110, 13, ST77XX_WHITE);
        switch (me->volume_percent)
        {
            case PERCENT_0:
                break;
            

            case PERCENT_25:
                tft->fillRoundRect(110, 89, 30, 25, 10, ST77XX_BLACK);
                break;


            case PERCENT_50:
                tft->fillRoundRect(110, 64, 30, 50, 10, ST77XX_BLACK);
                break;


            case PERCENT_75:
                tft->fillRoundRect(110, 39, 30, 75, 10, ST77XX_BLACK);
                break;

            case PERCENT_100:
                tft->fillRoundRect(110, 14, 30, 100, 10, ST77XX_BLACK);
                break;
            
        }
    }
    last_volume = me->volume_percent;
    
}


/***********************************************
*
* @Finality: Motor with the logic of the display configuration.
* @Parameters: The object of the display class
* @Return: -
*
************************************************/
void Motor_Display_Control(Display *me, bool up_pressed, bool low_pressed, bool center_pressed, bool wind_changed, Adafruit_ST7735 *tft)
{
    uint8_t last_percent;

    //Automatic volume
    if (center_pressed == true)
    {
        me->playNotStop = false;
    }

    //Manual volume
    if (up_pressed)
    {
        
        if (me->volume_percent >= 4){
            me-> volume_percent = 4;
        }else{
            me->volume_percent++;
        }
    }

    if (low_pressed)
    {
        
        if(me->volume_percent <= 0)
        {
            me->volume_percent = 0;
        }
        else
        {
            me->volume_percent--;
        }
    }

    if (center_pressed == true)
    {
        me->playNotStop = true;
    }

    if (wind_changed || center_pressed || low_pressed || up_pressed) Actualize_Window_Display(me, tft, wind_changed);

    if (last_percent != me->volume_percent)
    {
        me->changed = true;
    }else{
        me->changed = false;
    }

    last_percent = me->volume_percent;
}