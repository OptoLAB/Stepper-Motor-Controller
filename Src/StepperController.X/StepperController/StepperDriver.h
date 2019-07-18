/* 
 * Author: J. Bajic
 */

#ifndef STEPPERDRIVER_H
#define	STEPPERDRIVER_H

#include "sine_dc.h"

//DRIVER PINS
#define IN1_PHA     LATBbits.LATB9
#define IN2_PHA     LATBbits.LATB10
#define IN1_PHB     LATBbits.LATB11
#define IN2_PHB     LATBbits.LATB12
#define EN_PHA      LATDbits.LATD0
#define EN_PHB      LATDbits.LATD1

//POLARITY  
#define PLUS        (1)
#define MINUS       (0)

//SINE PWM DUTY CYCLES
#define SINE_DC(x)  (MIN_PWM+quarter_sine[x])
#define PI_2        (MAX_MS-1)

//FUNCTIONS
void configDriverPins(void);
void configPWMA(void);
void configPWMB(void);
void setPolarityPhaseA(unsigned char polarity);
void setPolarityPhaseB(unsigned char polarity);
void setPWMPhaseA(unsigned int pwm);
void setPWMPhaseB(unsigned int pwm);
void setDriverState(char state, unsigned char x);
char getDriverState(char *x);



#endif	/* STEPPERDRIVER_H */

