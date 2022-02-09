#ifndef PWM_H
#define PWM_H



#include "BrightControl.h"

#define PIN_PWM 2

typedef struct {

    //Pin OUT of the esp32
	unsigned char Gpio_pin;
    //Status of the state machine
    unsigned char status_machine;
    //handler Timer 0 pwm
    unsigned char timerPWM;

}PWM;


/***********************************************
*
* @Finality: Motor with the logic of the PWM.
* @Parameters: The object of the PWM Class, the Bright class to know the percentage of pwm,
* @Return: -
*
************************************************/
void Motor_PWM(PWM * me, Bright bright);


/***********************************************
*
* @Finality: Create the object.
* @Parameters: Pin OUTPUT of the PWM
* @Return: Return object created and initialized
*
************************************************/
PWM * PWM_Create(unsigned char pin);

#endif
