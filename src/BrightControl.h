#ifndef BRIGHTCONTROL_H
#define BRIGHTCONTROL_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define PERCENT_0 0
#define PERCENT_12_5 1
#define PERCENT_25 2
#define PERCENT_37_5 3
#define PERCENT_50 4
#define PERCENT_62_5 5
#define PERCENT_75 6
#define PERCENT_87_5 7
#define PERCENT_100  8





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
* @Parameters: The object of the Bright class
* @Return: -
*
************************************************/
void Motor_Bright_Control(Bright * me, bool up_pressed, bool low_pressed, bool center_pressed);




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
