/* 
 * Author: J. Bajic
 */

#ifndef SINE_DC_H
#define	SINE_DC_H
//MAX MICROSTEPs
#define MAX_MS  32
//MIN & MAX PWM VALUE
#define MIN_PWM 250
#define MAX_PWM 500
//SINE angle=[0-pi/2) amp=[0-(MAX_PWM-MIN_PWM)]
static const unsigned int quarter_sine[MAX_MS]={
0	,
12	,
25	,
37	,
49	,
61	,
73	,
84	,
96	,
107	,
118	,
129	,
139	,
149	,
159	,
168	,
177	,
185	,
193	,
201	,
208	,
214	,
220	,
226	,
231	,
235	,
239	,
243	,
245	,
247	,
249	,
250	
};

#endif	/* SINE_DC_H */

