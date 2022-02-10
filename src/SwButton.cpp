#include "SwButton.h"
#include "TITTIMER.h"
#include "esp32-hal-gpio.h"
#include <stdlib.h>

/***********************************************
*
* @Finality: Create the object.
* @Parameters: Pin input of the button
* @Return: Return object created and initialized
*
************************************************/
Button * Button_Create(unsigned char pin){
    
    Button * but = (Button *) malloc(sizeof(Button));
    but->Gpio_pin = pin;
    //but->Handle_Timer = TiGetTimer();
    but->status_machine = 0;
    but->status_pressed_not_pressed = 0;

    but->timerButton = TiGetTimer();
    TiResetTics(but->timerButton);

    return but;
}



/***********************************************
*
* @Finality: Motor with the logic of the button.
* @Parameters: The object of the button
* @Return: Return a char, = 0 if not pulsed, = 1 if pulsed
*
************************************************/
bool Motor_Button(Button * me){
    switch (me->status_machine)
    {
        case 0:
            me->status_pressed_not_pressed = false;
            if (digitalRead(me->Gpio_pin) != LOW){
               
                TiResetTics(me->timerButton);
                me->status_machine = 1;
            }

            break;

        case 1:
            if(TiGetTics(me->timerButton) > 40){
                if (digitalRead(me->Gpio_pin) != LOW){
                    me->status_machine = 2;
                }else{
                    TiResetTics(me->timerButton);
                    me->status_machine = 0;
                }
            }
            break;

        case 2:
            if (digitalRead(me->Gpio_pin) == LOW){
                TiResetTics(me->timerButton);
                me->status_machine = 3;
            }
            break;
        
        case 3: 
        
            if(TiGetTics(me->timerButton) > 40){
                if (digitalRead(me->Gpio_pin) == LOW){
                    me->status_machine = 4;
                }
            }
            break;

        case 4:
            me->status_pressed_not_pressed = true;
            me->status_machine = 0;

        default:
            break;
    }
    return me->status_pressed_not_pressed;

}

bool getStatus(Button * me){
    return me->status_pressed_not_pressed;
}

/***********************************************
*
* @Finality: free the object info
* @Parameters: Imc object
* @Return: -
*
************************************************/
void Button_Destroy(Button* const me){    

    free(me);

}



/***********************************************
*
* @Finality: Create the object
* @Parameters: -
* @Return: Return object created and initialized
*
************************************************/
/*
Icm * ICM_Create(void){
    Icm * this = (Icm *) malloc(sizeof(Icm));
 
    I2CInit();
	ExtIntInit();

    if(this!=NULL)
    {
    
        ICM_Init(this);
    
    }
    
    return this;

}*/