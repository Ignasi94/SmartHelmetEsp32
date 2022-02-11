#include "PWM.h"
#include "TITTIMER.h"
#include "BrightControl.h"
#include "esp32-hal-gpio.h"
#include <stdlib.h>

static uint8_t conv_table_bright[] = {1, 2, 4, 8, 30};


/***********************************************
*
* @Finality: Create the object.
* @Parameters: Pin OUTPUT of the PWM
* @Return: Return object created and initialized
*
************************************************/
PWM * PWM_Create(unsigned char pin)
{
    PWM * pwm = (PWM *) malloc(sizeof(PWM));
    pinMode(pin, OUTPUT);
    pwm->Gpio_pin = pin;
    //but->Handle_Timer = TiGetTimer();
    //pwm->status_machine = 0;

    pwm->timerPWM = TiGetTimer();
    TiResetTics(pwm->timerPWM);

    return pwm;
}

/***********************************************
*
* @Finality: Motor with the logic of the PWM.
* @Parameters: The object of the PWM Class, the Bright class to know the percentage of pwm,
* @Return: -
*
************************************************/
void Motor_PWM(PWM * me, Bright bright)
{
    //uint8_t conv_table_bright[] = {5, 10, 25, 50, 8};
    static uint8_t sw = 0;
    //digitalWrite(me->Gpio_pin, HIGH);
    switch (sw)//(me->status_machine)
    {

        case 0:

            digitalWrite(me->Gpio_pin, LOW);
            TiResetTics(me->timerPWM);
            //me->status_machine = 1;
            sw = 1;
            break;

        case 1:
            
            if(TiGetTics(me->timerPWM) >= (conv_table_bright[bright.bright_percent])){

                sw = 2;
                //me->status_machine = 2;
                //digitalWrite(me->Gpio_pin, LOW);
                //TiResetTics(me->timerPWM);

            }else{
                digitalWrite(me->Gpio_pin, HIGH);
            }
            break;
        
        case 2:
            
            if(TiGetTics(me->timerPWM) >= (30 - conv_table_bright[bright.bright_percent]))//(4 * bright.bright_percent)){
            {
                sw = 1;
                //me->status_machine = 1;
                TiResetTics(me->timerPWM);
                //digitalWrite(me->Gpio_pin, HIGH);

            }else{
                digitalWrite(me->Gpio_pin, LOW);
            }
            break;

        default:
            break;
    }
}