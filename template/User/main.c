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
/*OLED屏测试*/
// int main()
// {
// 	OLED_Init();
// 	OLED_ShowString(0,0,"你好世界！",OLED_8X16);

// 	OLED_Update();
// 	while(1)
// 	{

// 	}
// }
/*LED测试*/
// int main()
// {
// 	LED_Init();
// 	while (1)
// 	{
// 		LED_ON();
// 		Delay_ms(500);
// 		LED_OFF();
// 		Delay_ms(500);
// 		LED_Turn();
// 		Delay_ms(500);
// 		LED_Turn();
// 		Delay_ms(500);
// 	}
	
// }
/*定时器中断测试*/
// volatile uint16_t i = 0;
// int main()
// {
// 	OLED_Init();
// 	Timer_Init();
// 	while(1)
// 	{
// 		OLED_Printf(0,0,OLED_8X16,"i = %05d",i);

// 		OLED_Update();
// 	}
// }
/*电位器旋钮测试*/
// int main(void)
// {
// 	OLED_Init();
// 	RP_Init();

// 	while (1)
// 	{
// 		OLED_Printf(0, 0, OLED_8X16,"RP1:%04d",RP_GetValue(1));
// 		OLED_Printf(0,16,OLED_8X16,"RP2:%04d",RP_GetValue(2));
// 		OLED_Printf(0,32,OLED_8X16,"RP3:%04d",RP_GetValue(3));
// 		OLED_Printf(0,48,OLED_8X16,"RP4:%04d",RP_GetValue(4));

// 		OLED_Update();
// 	}
// }
/*非阻塞按键测试*/
// uint16_t i = 0;
// uint16_t j = 0;
// uint8_t KeyNumber;
// int main()
// {
// 	Key_Init();
// 	OLED_Init();

// 	Timer_Init();
// 	while (1)
// 	{
// 		KeyNumber = Key_GetNum();
// 		if(KeyNumber == 1)
// 		{
// 			j ++;
// 		}
// 		if(KeyNumber == 2)
// 		{
// 			j --;
// 		}
// 		if(KeyNumber == 3)
// 		{
// 			j += 10;
// 		}
// 		if(KeyNumber == 4)
// 		{
// 			j -= 10;
// 		}
// 		OLED_Printf(0,0,OLED_8X16,"i = %05d",i);
// 		OLED_Printf(0,16,OLED_8X16,"J = %05d",j);
// 		OLED_Update();
// 	}
	
// }
// void TIM1_UP_IRQHandler(void)
// {
// 	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
// 	{
// 		i++;
// 		Key_Tick();
// 		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
// 	}
// }
/*电机驱动测试*/
// uint8_t KeyNumber;
// int16_t PWM;
// int main()
// {
// 	Key_Init();
// 	OLED_Init();
// 	Motor_Init();
// 	Timer_Init();
// 	while (1)
// 	{
// 		KeyNumber = Key_GetNum();
// 		if(KeyNumber == 1)
// 		{
// 			PWM += 10;
// 			if(PWM > 100) 
// 			PWM = 100;
// 		}
// 		if(KeyNumber == 2)
// 		{
// 			PWM -= 10;
// 			if(PWM < -100) 
// 			PWM = -100;
// 		}
// 		if(KeyNumber == 3)
// 		{
// 			PWM = 0;
// 		}
		
// 		Motor_SetPWM(PWM);

// 		OLED_Printf(0,0,OLED_8X16,"PWM = %05d",PWM);
// 		OLED_Update();
// 	}
	
// }
// void TIM1_UP_IRQHandler(void)
// {
// 	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
// 	{
		
// 		Key_Tick();
// 		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
// 	}
// }
/*编码器测试*/
// int16_t speed;
// int16_t location;
// int main()
// {
// 	OLED_Init();
// 	Timer_Init();
// 	Encoder_Init();
// 	while (1)
// 	{
		
// 		OLED_Printf(0,0,OLED_8X16,"speed = %05d",speed);
// 		OLED_Printf(0,16,OLED_8X16,"location = %05d",location);
// 		OLED_Update();
// 	}
	
// }
// void TIM1_UP_IRQHandler(void)
// {
// 	static uint16_t counter;
// 	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
// 	{
// 		counter ++;
// 		if(counter >= 40)
// 		{
// 			counter = 0;

// 			speed = Encoder_Get();
// 			location += speed;
// 		}
// 		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
// 	}
// }
/*串口测试*/
uint16_t RP1,RP2,RP3,RP4;
int main(void)
{
	OLED_Init();
	RP_Init();
	Serial_Init();

	while (1)
	{
		RP1 = RP_GetValue(1);
		RP2 = RP_GetValue(2);
		RP3 = RP_GetValue(3);
		RP4 = RP_GetValue(4);
		OLED_Printf(0,0, OLED_8X16,"RP1:%04d",RP1);
		OLED_Printf(0,16,OLED_8X16,"RP2:%04d",RP2);
		OLED_Printf(0,32,OLED_8X16,"RP3:%04d",RP3);
		OLED_Printf(0,48,OLED_8X16,"RP4:%04d",RP4);

		OLED_Update();

		Serial_Printf("%d,%d,%d,%d\r\n",RP1,RP2,RP3,RP4);
		Delay_ms(20);
	}
}