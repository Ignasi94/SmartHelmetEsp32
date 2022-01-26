//
// Modul de gestió de Timers
// Barsalona, Novembre de 1995, Juan Perez & Josep M. Ribes.
// Desembre de 2001. FEC. Ajustat per al Fujitsu 90583
// Marc de 2010. FEC. Ajustat per al PIC24 (com passen els anys...)
// 
#include "TITTIMER.h"
#include "esp32-hal-timer.h"

#define         TI_FALS                         0
#define         TI_CERT                         1

// Tipus imb�cils
typedef unsigned char        BYTE;
typedef unsigned short       WORD;

struct Timer {
	unsigned int h_TicsInicials;
	unsigned char b_busy;
} s_Timers[TI_NUMTIMERS]; 

static unsigned int  h_Tics = 0;
static uint8_t counter;

//Timer 0 variables
//volatile uint32_t interruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
/*
void RSI_Timer0 (void) { 
//	IO_SetValue(GPIO_1, 1); // Per comprovar la temportitzaci�
    	INTCONbits.TMR0IF = 0;    //Resetejo el flag de petici� d'interrupcio
        TMR0H = 64920>>8;
	TMR0L = 64920&0xFF;
	h_Tics++;

	if (h_Tics >= TI_MAXTICS) {
		// Abans que dongui la volta, tots avall.
		for (counter=0;counter<TI_NUMTIMERS;counter++){
            if (s_Timers[counter].b_busy==TI_CERT){
                s_Timers[counter].h_TicsInicials -= h_Tics;
                h_Tics=0;
            }
        }
	}
//	IO_SetValue(GPIO_1, 0); // Per comprovar la temportitzaci�
}
*/

/*
 *As with any interrupt, it is best to place the code executed by the Timer in the internal RAM of the ESP32
 * which is much faster than the Flash memory of the development board.
 *
 * 
 * To do this, simply place the IRAM_ATTR attribute just before the name of the function like this
 */

void IRAM_ATTR  onTimer(){
    static uint8_t bool_test = 0;

    /*if (bool_test == 0){
        bool_test = 1;
        digitalWrite(32, HIGH);
    }else{
        bool_test = 0;
        digitalWrite(32, LOW);
    }*/
    // Increment the counter and set the time of ISR
    portENTER_CRITICAL_ISR(&timerMux);
    h_Tics++;
    portEXIT_CRITICAL_ISR(&timerMux);

	if (h_Tics >= TI_MAXTICS) {
		// Abans que dongui la volta, tots avall.
		for (counter=0;counter<TI_NUMTIMERS;counter++){
            if (s_Timers[counter].b_busy==TI_CERT){
                s_Timers[counter].h_TicsInicials -= h_Tics;
                h_Tics=0;
            }
        }
	}
}

void TiInit () {
    unsigned char counter;
    for (counter=0;counter<TI_NUMTIMERS;counter++) {
    	s_Timers[counter].b_busy=TI_FALS;
    }
    h_Tics=0;

    /*
    * Timer Setup
    * url: https://diyprojects.io/esp32-timers-alarms-interrupts-arduino-code/#.Ye7bT_7MK3A
    */

    //Timer 0
    //Prescaler 80 (80,000,000 / 80 = 1,000,000 tics / sec =>  1usec)
    //True -> trigger by edge
    timer = timerBegin(0, 80, true);

    //timer just configured 
    //function to execute
    //by edge = true
    timerAttachInterrupt(timer, &onTimer, true);

    //Program interrupt with the timer every 1000 * 1usg = 1msg
    timerAlarmWrite(timer, 1000, true);
    timerAlarmEnable(timer);
    // Suposo que anem a 4MHz
    /*T0CONbits.T08BIT = 0; // 16 bits
    T0CONbits.T0CS = 0; // Clock: fosc/4
    T0CONbits.PSA = 0;  // Prescaler
    T0CONbits.T0PS = 0; // Preescaler a 1/4, pols de 4us
    // La resta de valors de T1CON per defecte
    TMR0H = 64920>>8;
    TMR0L = 64920&0xFF;	// (256-6)T * 4us = 1ms 100micros 
    T0CONbits.TMR0ON = 1;		// Activo el timer
    // Activo la interrupci� del timer 1
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;*/
}

char TiGetTimer() {
    unsigned char counter=0;
    while (s_Timers[counter].b_busy==TI_CERT) {
    	counter++;
    	if (counter == TI_NUMTIMERS) return -1;
    }
    s_Timers[counter].b_busy=TI_CERT;
    return (counter);
}

void TiResetTics (unsigned char Handle) {
    //__DI(); h_Tics integer, per tant, indivisible
    s_Timers[Handle].h_TicsInicials=h_Tics;
    //__EI();
}


unsigned int TiGetTics (unsigned char Handle) {
volatile unsigned int actual;
    actual=h_Tics; // indivisible si �s un int, clar.
    return (actual-(s_Timers[Handle].h_TicsInicials));
}

void TiCloseTimer (unsigned char Handle) {
    s_Timers[Handle].b_busy=TI_FALS;
}


void TiEnd () {
}