/* 
 * Author: J. Bajic
 */

#include <p30fxxxx.h>
#include "timer.h"
#include "uart.h"
#include "StepperDriver.h"
#include "StepperController.h"

_FOSC(CSW_FSCM_OFF & XT_PLL8);
_FWDT(WDT_OFF);
_FGS(CODE_PROT_OFF);

#define FCY             (20000000L)
#define PERIOD_MS       (0.01)
#define BAUD_RATE       (28800)
#define T1_PR           (FCY/(1000/PERIOD_MS))	
#define U1_BRG          ((FCY/BAUD_RATE)/16-1)

unsigned int i,j;
unsigned char rx_byte=0,rx_buff[13];
unsigned char stat=0;


Stepper_motor s={0,CW,STEP_1_1,0,0,0,0,0,0,0};

void init(void)
{
	configDriverPins();
	configPWMA();
	configPWMB();

    ConfigIntTimer1(T1_INT_PRIOR_1 & T1_INT_ON);
	OpenTimer1(T1_ON & T1_GATE_OFF & T1_IDLE_CON & T1_PS_1_1 & T1_SYNC_EXT_OFF & T1_SOURCE_INT, T1_PR);

    ConfigIntUART1(UART_RX_INT_EN & UART_RX_INT_PR6 & UART_TX_INT_DIS & UART_TX_INT_PR2); 
    OpenUART1(UART_EN & UART_DIS_LOOPBACK & UART_NO_PAR_8BIT & UART_1STOPBIT,UART_TX_PIN_NORMAL & UART_TX_ENABLE ,U1_BRG);   
}

void sendByteOnCommand(char command, unsigned char byte)
{
    if(rx_byte==command)
    {
        WriteUART1(byte);
        while(!BusyUART1());
        WriteUART1(0x0A);
        while(!BusyUART1());
        rx_byte=0;
    }
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)	//0.01ms
{
   	TMR1 =0;
    IFS0bits.T1IF = 0;   
    
    s.time_counter++;
}

// U A R T 1
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) 
{

    IFS0bits.U1RXIF = 0;
	rx_byte=U1RXREG;

    for(i=0;i<12;i++)rx_buff[12-i]=rx_buff[11-i];
	rx_buff[0]=rx_byte;

	if(rx_buff[0]=='S'&&rx_buff[12]=='S')
	{   
        s.en=0;
        
		s.mode=rx_buff[10];
        s.direction=rx_buff[9];
        s.end_period=(rx_buff[8]<<8)+rx_buff[7];
        s.start_period=(rx_buff[6]<<8)+rx_buff[5];
		s.num_of_steps=(rx_buff[4]<<8)+rx_buff[3];
        s.accel=(rx_buff[2]<<8)+rx_buff[1];
        
        if(s.accel!=0)s.period=s.start_period;
        else s.period=s.end_period;
        
        if(rx_buff[11])
        {
            s.num_counter=0;
            s.time_counter=0;
            s.en=1;
        }
        
		rx_buff[0]=rx_buff[10]=0;
		rx_byte=0;
	}
} 

int main(void)
{
	
	init();
	for(i=0;i<700;i++)
	for(j=0;j<3000;j++);
    
	while(1)
	{
        stat=runStepper(&s);
        sendByteOnCommand('M',stat);
	}

    return 0;
}


