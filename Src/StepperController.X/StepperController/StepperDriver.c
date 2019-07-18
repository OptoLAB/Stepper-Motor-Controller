/* 
 * Author: J. Bajic
 */

#ifndef STEPPERDRIVER_C
#define	STEPPERDRIVER_C

#include <p30fxxxx.h>
#include <outcompare.h>
#include <timer.h>
#include "StepperDriver.h"

char driver_state;
char driver_step;

void configDriverPins(void)
{
	ADPCFGbits.PCFG9=1;   
	ADPCFGbits.PCFG10=1;
	ADPCFGbits.PCFG11=1;
	ADPCFGbits.PCFG12=1;
	TRISDbits.TRISD0=0;     // ENA - PWMA
	TRISDbits.TRISD1=0;     // ENB - PWMB
	TRISBbits.TRISB9=0;     // IN1
	TRISBbits.TRISB10=0;    // IN2
	TRISBbits.TRISB11=0;    // IN3
	TRISBbits.TRISB12=0;    // IN4
    
    LATBbits.LATB9=0;       // IN1
	LATBbits.LATB10=0;      // IN2
	LATBbits.LATB11=0;      // IN3
	LATBbits.LATB12=0;      // IN4
}

void configPWMA(void)
{
	PR2=MAX_PWM-1;
	OC1R=0;
	OC1RS=20;
	OC1CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE;
	T2CONbits.TON=1;
	OC1RS=0;
}

void configPWMB(void)
{
	PR3=MAX_PWM-1;
	OC2RS=20;
	OC2R=0;
	OC2CON =OC_IDLE_CON & OC_TIMER3_SRC & OC_PWM_FAULT_PIN_DISABLE;
	T3CONbits.TON=1;
	OC2RS=0;
}

void setPolarityPhaseA(unsigned char polarity)
{
    if(polarity){ IN1_PHA=1; IN2_PHA=0; }
    else        { IN1_PHA=0; IN2_PHA=1; }
}
void setPolarityPhaseB(unsigned char polarity)
{
    if(polarity){ IN1_PHB=1; IN2_PHB=0; }
    else        { IN1_PHB=0; IN2_PHB=1; }
}
void setPWMPhaseA(unsigned int pwm)
{
    OC1RS=pwm;
}
void setPWMPhaseB(unsigned int pwm)
{
    OC2RS=pwm;
}

void setDriverState(char state, unsigned char x)
{
    if(state>3)state=0;
    if(state<0)state=3;
    switch(state)
    {
        case 0:	//cosine 0-pi/2
                setPWMPhaseA(SINE_DC(PI_2-x));
                setPolarityPhaseA(PLUS);
                //sine 0-pi/2
                setPWMPhaseB(SINE_DC(x));
                setPolarityPhaseB(PLUS);
                break;
        case 1:	//cosine pi/2-pi
                setPWMPhaseA(SINE_DC(x));
                setPolarityPhaseA(MINUS);
                //sine pi/2-pi
                setPWMPhaseB(SINE_DC(PI_2-x));
                setPolarityPhaseB(PLUS);	
                break;
        case 2:	//cosine pi-3pi/2
                setPWMPhaseA(SINE_DC(PI_2-x));
                setPolarityPhaseA(MINUS);
                //sine pi-3pi/2
                setPWMPhaseB(SINE_DC(x));
                setPolarityPhaseB(MINUS);
                break;
        case 3:	//cosine 3pi/2-2pi
                setPWMPhaseA(SINE_DC(x));
                setPolarityPhaseA(PLUS);
                //sine 3pi/2-2pi
                setPWMPhaseB(SINE_DC(PI_2-x));
                setPolarityPhaseB(MINUS);
                break;
    }
    driver_state=state;
    driver_step=x;
}

char getDriverState(char *x)
{
    *x=driver_step;
    return driver_state;
}

#endif	/* STEPPERDRIVER_C */

