#ifndef __PID_H
#define __PID_H

typedef struct
{
    float Target,Actual,Out;
    float Kp,Ki,Kd;
    float Error0,Error1,ErrorInt;
    float OutMax;
    float OutMin;
}PID_t;

void PID_Update(PID_t* a);




#endif
