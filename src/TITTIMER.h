// M�dul de gesti� de Timers
// Barsalona, Novembre de 1995, Juan Perez & JM Ribes.
// Desembre de 2001. FEC. Ajustat per al Fujitsu 90583
// Mar� de 2010. FEC. Ajustat per al PIC24 (com passen els anys...)
// 

#ifndef _TITTIMER_H_
#define _TITTIMER_H_

// Si anem justos de memoria, aquest define indica el
// nombre de timers d'aquest TAD i es pot modificar sense
// problemes.
#define         TI_NUMTIMERS                    6
#define         TI_MAXTICS                      30000

//void RSI_Timer0(void);
//Pre: Ha d'estar activada l'interrupci� del Timer.
//Post: Incrementa el numero de tics o el reseteja en cas d'haver arribat al final.

//void onTimer();
//Pre: Ha d'estar activada l'interrupci� del Timer.
//Post: Incrementa el numero de tics o el reseteja en cas d'haver arribat al final.

void TiInit (void);
//Pre: Inicialitza el TAD, �s precondici� global haver cridat
//     aquesta funci� quan es cridi qualsevol altra funci�.
//Post: Inicialitza el timer i es fa la configuraci� dels registres T0CON i INTCON.

char TiGetTimer (void);
//Pre: Hi ha algun timer lliure. Maxim TI_NUMTIMERS.
//Post: Retorna un handle per usar les funcions TiGetTics i TiResetTics. 
//      Retorna -1 si no hi ha cap timer disponible.

void TiResetTics (unsigned char Handle);
//Pre: Handle ha estat retornat per Ti_OpenTimer.
//Post: Engega la temporitzaci� associada a 'Handle' i agafa la referencia 
//      temporal del sistema


unsigned int  TiGetTics (unsigned char Handle);
//Pre: Handle ha estat retornat per TiGetTimer.
//     La dist�ncia temmporal entre GetTics i ResetTics ha de ser menor
//     de TI_MAXTICS ms (actualment, 30 segons)
//Post: Retorna els milisegons transcorreguts des de la crida
//      a l'StartTimer del 'Handle'.

void TiCloseTimer (unsigned char Handle);
//Pre: Handle ha estat retornat per Ti_OpenTimer.
//Post: allibera el timer que porta associat aquest 'Handle'

void TiEnd (void);
//Pre:
//Postcondicions: Tanca el TAD.

#endif

