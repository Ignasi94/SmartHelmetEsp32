#ifndef WINDOWS_H
#define WINDOWS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>



#define WINDOW_NAVIGATION 0x00
#define WINDOW_BRIGHT 0x01
#define WINDOW_DISPLAY 0x02
#define WINDOW_VOLUMEN  0x03


typedef struct Window{

    uint8_t id_win;
    bool changed;
    //Status of the state machine
    uint8_t status_machine;

}Window;


/***********************************************
*
* @Finality: Create the object.
* @Parameters: -
* @Return: Return object created and initialized
*
************************************************/
Window * WINDOW_Create(void);


/***********************************************
*
* @Finality: Motor with the logic of the window selection.
* @Parameters: The object of the window class
* @Return: Return a bool, = 0 if windows not changed, = 1 if changed
*
************************************************/
bool Motor_WINDOW_SELECT(Window * me, bool left_pressed, bool right_pressed);




/***********************************************
*
* @Finality: Function that returns the window 
* @Parameters: Window object
* @Return: Return the window ID
*
************************************************/
uint8_t getIDWindow(const Window* const me);



/***********************************************
*
* @Finality: free the object info
* @Parameters: Imc object
* @Return: -
*
************************************************/
void WINDOW_Destroy(Window* const me);


#endif
