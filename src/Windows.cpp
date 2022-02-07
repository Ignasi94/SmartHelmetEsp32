#include "Windows.h"
#include "SwButton.h"



/***********************************************
*
* @Finality: Function that returns the Accelerometer data
* @Parameters: Imc object
* @Return: Return the id of the window
*
************************************************/
uint8_t getIDWindow(const Window* const me){

    return me->id_win;

}



/***********************************************
*
* @Finality: free the object info
* @Parameters: Imc object
* @Return: -
*
************************************************/
void WINDOW_Destroy(Window * const me){    

    free(me);

}



/***********************************************
*
* @Finality: Initialize the object
* @Parameters: Window object
* @Return: -
*
************************************************/
void WINDOW_Init(Window* const me) {

    me->id_win = WINDOW_NAVIGATION;
    me->changed = false;
    me->status_machine = 0;

}




/***********************************************
*
* @Finality: Create the object
* @Parameters: -
* @Return: Return object created and initialized
*
************************************************/
Window * WINDOW_Create(void){
    Window * me = (Window *) malloc(sizeof(Window));
 


    if(me!=NULL)
    {
    
        WINDOW_Init(me);
    
    }
    
    return me;

}


/***********************************************
*
* @Finality: Motor with the logic of the button.
* @Parameters: The object of the button
* @Return: Return a bool, = 0 if windows not changed, = 1 if changed
*
************************************************/
bool Motor_WINDOW_SELECT(Window * me, bool left_pressed, bool right_pressed){
    static uint8_t last_status_machine;
    bool aux_changed;
    switch (me->status_machine)
    {
        case WINDOW_NAVIGATION:

            if (left_pressed){
                me->status_machine = WINDOW_BRIGHT;
            }
            
            break;

        case WINDOW_BRIGHT:

            if (left_pressed){
                me->status_machine = WINDOW_NAVIGATION;
            }
            if (right_pressed){
                me->status_machine = WINDOW_DISPLAY;
            }

            break;

        case WINDOW_DISPLAY:

            if (left_pressed){
                me->status_machine = WINDOW_BRIGHT;
            }
            if (right_pressed){
                me->status_machine = WINDOW_VOLUMEN;
            }

            break;
        
        case WINDOW_VOLUMEN:

            if (left_pressed){
                me->status_machine = WINDOW_DISPLAY;
            }

            break;

        default:
            break;
    }

    if (last_status_machine == me->status_machine){
        aux_changed = false;
    }else{
        aux_changed = true;
    }

    last_status_machine = me->status_machine;
    
    if (aux_changed){
        return  true;
    }else{
        return  false;
    }
}