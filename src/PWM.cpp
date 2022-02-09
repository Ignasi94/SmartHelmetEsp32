#include "PWM.h"
#include "TITTIMER.h"
#include "BrightControl.h"
#include "esp32-hal-gpio.h"
#include <stdlib.h>

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
    pwm->status_machine = 0;

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
    switch (me->status_machine)
    {

        case 0:

            digitalWrite(me->Gpio_pin, LOW);
            TiResetTics(me->timerPWM);
            me->status_machine = 1;
            break;

        case 1:
            digitalWrite(me->Gpio_pin, HIGH);
            if(TiGetTics(me->timerPWM) > (2 * bright.bright_percent)){

                me->status_machine = 2;
                //TiResetTics(me->timerPWM);

            }
            break;
        
        case 2:
            digitalWrite(me->Gpio_pin, LOW);
            if(TiGetTics(me->timerPWM) >= (4 * bright.bright_percent)){

                me->status_machine = 1;
                TiResetTics(me->timerPWM);

            }
            break;

        default:
            break;
    }


}