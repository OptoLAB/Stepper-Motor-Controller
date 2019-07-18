/* 
 * Author: J. Bajic
 */

#ifndef STEPPERCONTROLER_H
#define	STEPPERCONTROLER_H

#include "StepperDriver.h"

//DIRECTION
#define CW 1
#define CCW 0
//MODES
#define STEP_1_1    0
#define STEP_1_2    1
#define STEP_1_4    2
#define STEP_1_8    3
#define STEP_1_16   4
#define STEP_1_32   5
//CONTROLLER STATES
#define STOP    0
#define RUN     1
#define HOLD    2
//INF MACRO
#define INF     -1

typedef struct{
    unsigned char   en;             // 0 -disabled, 1-enabled, 
    unsigned char   direction;      // 1 -cw, 0-ccw
    unsigned char   mode;           // 0 - 1/1, 1 - 1/2, 2 - 1/4, 3 - 1/8, 4 - 1/16, 5 - 1/32 step
    unsigned int    period;         // stepper period (between steps)
    int             num_of_steps;   // number of steps to execute
    unsigned int    start_period;   // period from which to start acceleration/deceleration
    unsigned int    end_period;     // period at which to end acceleration/deceleration
    unsigned long   num_counter;    // stepper steps counter
    unsigned long   time_counter;   // stepper timer counter (increment in timer interrupt)
    long            accel;          // 0-disabled, >0 - acceleration, <0 - deceleration
}Stepper_motor;


unsigned char runStepper(Stepper_motor *s);
void updateRampPeriod(Stepper_motor *s);

#endif	/* STEPPERCONTROLER_H */

