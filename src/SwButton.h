#ifndef ICM20948_H
#define ICM20948_H

#define UP                32

#define LOW               0x0
#define HIGH              0x1

#define BUTTON_UP 34
#define BUTTON_LOW 35
#define BUTTON_LEFT 32
#define BUTTON_RIGHT 33
#define BUTTON_CENTER 26


typedef struct {

    //Pin IN of the esp32
	unsigned char Gpio_pin;
    //0 if not pressed, 1 if pressed
    bool status_pressed_not_pressed;
    //Status of the state machine
    unsigned char status_machine;
    //handler Timer 0 button
    unsigned char timerButton;

}Button;

/***********************************************
*
* @Finality: Create the object.
* @Parameters: Pin input of the button
* @Return: Return object created and initialized
*
************************************************/
Button * Button_Create(unsigned char pin);

/***********************************************
*
* @Finality: Motor with the logic of the button.
* @Parameters: The object of the button
* @Return: Return a bool, = 0 if not pulsed, = 1 if pulsed
*
************************************************/
bool Motor_Button(Button * me);



/***********************************************
*
* @Finality: Function the status
* @Parameters: Button object
* @Return: Return a char, = 0 if not pulsed, = 1 if pulsed
*
************************************************/
bool getStatus(Button * me);




/***********************************************
*
* @Finality: free the object info
* @Parameters: Button object
* @Return: -
*
************************************************/
void Button_Destroy(Button* const me);


#endif
