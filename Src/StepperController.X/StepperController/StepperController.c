/* 
 * Author: J. Bajic
 */
#include "StepperController.h" 

long rmdr=0;
unsigned char status=STOP;

void updateRampPeriod(Stepper_motor *s)
{
    long Cn,Cn1;
    long n;
   
    // start of acceleration/deceleration 
    if((s->period==s->start_period)&&(s->num_counter==0))rmdr=0;
    
    n=s->accel;
    Cn1=s->period;
                           
    Cn=Cn1-((2*Cn1+rmdr)/(4*n+1));	
    rmdr=((2*Cn1+rmdr)%(4*n+1));
    
    if(n>0) n++;   //acceleration  
    if(n<0) n--;   //deceleration   

    if(((n>0)&&(Cn<s->end_period))||((n<0)&&(Cn>s->end_period))) 
    {   // end of acceleration/deceleration  
        s->period=s->end_period;
        s->accel=0;
    }
    else                                                                
    {   // acceleration/deceleration in progress -> update period
        s->period=Cn;
        s->accel=n;
    }
}


unsigned char runStepper(Stepper_motor *s)
{
    char step;
    char driver_state;

	if(s->en)
	{
        if(s->time_counter>=s->period)
        {
            if((s->num_counter<s->num_of_steps)||(s->num_of_steps==INF))
            {
            
                status=RUN;

                driver_state=getDriverState(&step);
                
                if(s->direction==CW)  step+=(MAX_MS>>s->mode);
                if(s->direction==CCW) step-=(MAX_MS>>s->mode);


                if(step>(MAX_MS-1))
                {
                    driver_state++;
                    step=0;	
                }
                if(step<0)
                {
                    driver_state--;
                    step=MAX_MS-(MAX_MS>>s->mode);	
                }

                setDriverState(driver_state,step);
                
                if((s->accel!=0)) updateRampPeriod(s);  //if ramp on (acceleration/deceleration)
                
                s->num_counter++;               
            }
            else status=HOLD;

            s->time_counter=0;
		}
		
	}
	else 
    {
        setPWMPhaseA(0);
        setPWMPhaseB(0);
        status=STOP;
    }
    
    return status;
        
}



