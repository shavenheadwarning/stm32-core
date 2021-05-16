#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
/************************************************
 ALIENTEK NANO STM32开发板实验9
 PWM输出实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

 int main(void)
 {	
	u16 led6pwmval=0;    
	u8 dir=1;	
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	LED_Init();		  	 //初始化与LED连接的硬件接口
	TIM3_PWM_Init(899,0);//不分频。PWM频率=72000/(899+1)=80Khz 
   	while(1)
	{
 		delay_ms(10);	 
		if(dir)led6pwmval++;
		else led6pwmval--;	 
 		if(led6pwmval>300)dir=0;
		if(led6pwmval==0)dir=1;	   					 
		LED6_PWM_VAL=led6pwmval;			
	} 
}

