#include "stm32f10x.h"
#include "PID.h"
#include <math.h>
void PID_Update(PID_t* a)
{
        a->Error1 = a->Error0;
        a->Error0 = a->Target - a->Actual;

		if(fabs(a->Error0) > 0.1)
		{
			a->ErrorInt += a->Error0;
		}
		else
		{
			a->ErrorInt = 0;
		}
				
		a->Out = a->Error0 * a->Kp + a->Ki * a->ErrorInt + a->Kd * (a->Error0 - a->Error1);
			
		if(a->Out > a->OutMax)
		{
			a->Out = a->OutMax;
		}
		if(a->Out < a->OutMin)
		{
			a->Out = a->OutMin;
		}
}
