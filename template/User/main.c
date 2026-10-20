#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "key.h"
#include "RP.h"
#include "PWM.h"
#include "Motor.h"
#include "encoder.h"
#include "serial.h"
#include "PID.h"
#include <math.h>
#include "AD.h"
int16_t Speed,Position;
uint8_t KeyNumber;
uint8_t Runstate = 0;
uint16_t Angle;
#define CENTER_ANGLE 2060
#define CENTER_RANGE 500
#define START_PWM 35
#define START_TIME 100
PID_t Angle_PID = {
	.Target = CENTER_ANGLE,
	.Kp = 0.3,
	.Ki = 0.01,
	.Kd = 0.4,
	.OutMax = 100,
	.OutMin = -100
};
PID_t Position_PID = {
	.Target = 0,
	.Kp = 0.4,
	.Ki = 0,
	.Kd = 4,
	.OutMax = 100,
	.OutMin = -100
};
int main()
{
	OLED_Init();
	LED_Init();
	Key_Init();
	RP_Init();
	Motor_Init();
	Encoder_Init();
	Serial_Init();
	AD_Init();
	Timer_Init();
	while(1)
	{
		KeyNumber = Key_GetNum();
		if (KeyNumber == 1)
		{
			if(Runstate == 0)
			{
				Runstate = 21;
			}
			else
			{
				Runstate = 0;
			}
		}
		if (KeyNumber == 2)
		{
			Position_PID.Target += 408;
			if(Position_PID.Target >= 4080)
			Position_PID.Target = 4080;
		}
		if (KeyNumber == 3)
		{
			Position_PID.Target -= 408;
			if(Position_PID.Target <= -4080)
			Position_PID.Target = -4080;
		}
		if(Runstate)
		{
			LED_ON();
		}
		else
		{
			LED_OFF();
		}
		// Angle_PID.Kp = RP_GetValue(1) / 4095.0 * 2;
		// Angle_PID.Ki = RP_GetValue(2) / 4095.0 * 2;
		// Angle_PID.Kd = RP_GetValue(3) / 4095.0 * 2;
		// Position_PID.Kp = RP_GetValue(1) / 4095.0 * 2;
		// Position_PID.Ki = RP_GetValue(2) / 4095.0 * 2;
		// Position_PID.Kd = RP_GetValue(3) / 4095.0 * 9;
		// Position_PID.Target = RP_GetValue(4) / 4095.0 * 816 - 408;
		OLED_Printf(42,0,OLED_6X8,"%02d",Runstate);
		OLED_Printf(0,0,OLED_6X8,"Angle");
		OLED_Printf(0,12,OLED_6X8,"Kp:%05.3f",Angle_PID.Kp);
		OLED_Printf(0,20,OLED_6X8,"Ki:%05.3f",Angle_PID.Ki);
		OLED_Printf(0,28,OLED_6X8,"Kd:%05.3f",Angle_PID.Kd);
		OLED_Printf(0,40,OLED_6X8,"Tar:%04.0f",Angle_PID.Target);
		OLED_Printf(0,48,OLED_6X8,"Ac:%04d",Angle);
		OLED_Printf(0,56,OLED_6X8,"Out:%+04.0f",Angle_PID.Out);
		OLED_Printf(64,0,OLED_6X8,"Position");
		OLED_Printf(64,12,OLED_6X8,"Kp:%05.3f",Position_PID.Kp);
		OLED_Printf(64,20,OLED_6X8,"Ki:%05.3f",Position_PID.Ki);
		OLED_Printf(64,28,OLED_6X8,"Kd:%05.3f",Position_PID.Kd);
		OLED_Printf(64,40,OLED_6X8,"Tar:%+05.0f",Position_PID.Target);
		OLED_Printf(64,48,OLED_6X8,"Ac:%+05.0f",Position_PID.Actual);
		OLED_Printf(64,56,OLED_6X8,"Out:%+04.0f",Position_PID.Out);
		OLED_Update();
	}

}
void TIM1_UP_IRQHandler(void)
{
	static uint16_t count0 = 0;
	static uint16_t count1 = 0;
	static uint16_t count2 = 0;
	static uint16_t countTime = 0;
	static uint16_t Angle0;
	static uint16_t Angle1;
	static uint16_t Angle2;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET)
	{
		Key_Tick();
		Angle = AD_GetValue();
		Speed = Encoder_Get();
		Position += Speed;
		switch (Runstate)
		{
		case 0:
			Motor_SetPWM(0);
			break;
		case 1:
			count0 ++;
			if(count0 >= 40)
			{
				count0 = 0;
				Angle2 = Angle1;
				Angle1 = Angle0;
				Angle0 = Angle;

				if(Angle0 > CENTER_ANGLE + CENTER_RANGE && 
				   Angle1 > CENTER_ANGLE + CENTER_RANGE &&
				   Angle2 > CENTER_ANGLE + CENTER_RANGE &&
				   Angle1 < Angle0 && Angle1 < Angle2)
				{
					Runstate = 21;
				}

				if(Angle0 < CENTER_ANGLE - CENTER_RANGE && 
				   Angle1 < CENTER_ANGLE - CENTER_RANGE &&
				   Angle2 < CENTER_ANGLE - CENTER_RANGE &&
				   Angle1 > Angle0 && Angle1 > Angle2)
				{
					Runstate = 31;
				}
				if(Angle0 > CENTER_ANGLE - CENTER_RANGE && Angle0 < CENTER_ANGLE + CENTER_RANGE
				&&Angle1 > CENTER_ANGLE - CENTER_RANGE && Angle1 < CENTER_ANGLE + CENTER_RANGE)
				{
					Position = 0;
					Angle_PID.ErrorInt = 0;
					Position_PID.ErrorInt = 0;
					Runstate = 4;
				}
			}
			break;
		case 21:
			Motor_SetPWM(START_PWM);
			Runstate = 22;
			countTime = START_TIME;
			break;
		case 22:
		    countTime --;
			if(countTime == 0)
			{
				Runstate = 23;
			}
			break;
		case 23:
			Motor_SetPWM(-START_PWM);
			Runstate = 24;
			countTime = START_TIME;
			break;
		case 24:
			countTime --;
			if(countTime == 0)
			{
				Motor_SetPWM(0);
				Runstate = 1;
			}
			break;
		case 31:
			Motor_SetPWM(-START_PWM);
			Runstate = 32;
			countTime = START_TIME;
			break;
		case 32:
			countTime --;
			if(countTime == 0)
			{
				Runstate = 33;
			}
			break;
		case 33:
			Motor_SetPWM(START_PWM);
			Runstate = 34;
			countTime = START_TIME;
			break;
		case 34:
			countTime --;
			if(countTime == 0)
			{
				Motor_SetPWM(0);
				Runstate = 1;
			}
			break;
		case 4:
			if(Angle > CENTER_ANGLE + CENTER_RANGE || Angle < CENTER_ANGLE - CENTER_RANGE)
			{
				Runstate = 0;
			}
			count1 ++;
			count2 ++;
			if(count1 >= 5)
			{
				count1 = 0;
			    Angle_PID.Actual = Angle;
				PID_Update(&Angle_PID);
				Motor_SetPWM(Angle_PID.Out);
			}
			if(count2 >= 50)
			{
				count2 = 0;
				Position_PID.Actual = Position;
				PID_Update(&Position_PID);
				Angle_PID.Target = CENTER_ANGLE - Position_PID.Out;
			}
			break;
		default:
			break;
		}
		
		}

		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}
// uint8_t KeyNumber;
// int16_t Speed;
// PID_t Inner = {
// 	.Kp = 0.3,
// 	.Ki = 0.3,
// 	.Kd = 0,
// 	.OutMax = 100,
// 	.OutMin = -100
// };

// PID_t Outer = {
// 	.Kp = 0.3,
// 	.Ki = 0,
// 	.Kd = 0.4,
// 	.OutMax = 20,
// 	.OutMin = -20
// };

// int16_t speed,location;
// // float OuterTarget,OuterActual,OuterOut;
// // float OuterKp = 0.3,OuterKi = 0,OuterKd = 0.4;
// // float OuterError0,OuterError1,OuterErrorInt,OuterError2;
// // float C,DifOut,Actual1,a;
// uint8_t MotorFlag = 1;
// #define EPSILON 0.01
// int main()
// {
// 	Key_Init();
// 	OLED_Init();
// 	Motor_Init();
// 	Timer_Init();
// 	Encoder_Init();
// 	RP_Init();
// 	Serial_Init();
// 	LED_Init();
// 	OLED_Printf(0,0,OLED_8X16,"2 * PID Control");
// 	OLED_Update();

// 	while (1)
// 	{
// 		KeyNumber = Key_GetNum();
// 		if(KeyNumber == 1)
// 		{
// 			MotorFlag = !MotorFlag;
// 		}
// 		if(MotorFlag)
// 		LED_ON();
// 		else
// 		LED_OFF();
// 		// Outer.Kp = RP_GetValue(1) / 4095.0 * 2;
// 		// Outer.Ki = RP_GetValue(2) / 4095.0 * 2;
// 		// Outer.Kd = RP_GetValue(3) / 4095.0 * 2;
// 		// Inner.Target = RP_GetValue(4) / 4095.0 * 300 - 150; //定速控制用
// 		Outer.Target = RP_GetValue(4) / 4095.0 * 816 - 408;
// 		OLED_Printf(0,16,OLED_8X16,"Kp:%4.2f",Outer.Kp);
// 		OLED_Printf(0,32,OLED_8X16,"Ki:%4.2f",Outer.Ki);
// 		OLED_Printf(0,48,OLED_8X16,"Kd:%4.2f",Outer.Kd);
		
// 		OLED_Printf(64,16,OLED_8X16,"Tar:%+04.0f",Outer.Target);
// 		OLED_Printf(64,32,OLED_8X16,"Act:%+04.0f",Outer.Actual);
// 		OLED_Printf(64,48,OLED_8X16,"Out:%+04.0f",Outer.Out);

// 		Serial_Printf("%f,%f,%f\r\n",Outer.Target,Outer.Actual,Outer.Out);
// 		OLED_Update();
// 	}
	
// } 
// void TIM1_UP_IRQHandler(void)
// {
// 	static uint16_t count1,count2;
// 	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
// 	{
// 		// Encoder_Tick();
// 		Key_Tick();
// 		count1 ++;
// 		count2 ++;
// 		//外环定位置控制
// 		if(count2 >= 40)
// 		{
// 			speed = Encoder_Get();
// 			location += speed;
// 			count2 = 0;
// 			Outer.Actual = location;
// 			PID_Update(&Outer);
// 			Inner.Target = Outer.Out;
// 		}
// 		//内环定速度
// 		if(count1 >= 40)
// 		{
// 			count1 = 0;
// 			Inner.Actual = speed;//Actual改成+=即可改为定位置
// 			PID_Update(&Inner);
// 			Motor_SetPWM(Inner.Out);
// 		}	

// 		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
// 	}
		
		
// }
