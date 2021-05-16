#include "sys.h"	
#include "delay.h"	
#include "led.h" 
#include "key.h" 
#include "beep.h"

/************************************************
 ALIENTEK NANO STM32开发板实验3
 按键实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
	vu8 key=0;	
	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);	    //延时初始化  
	LED_Init();		  	//初始化与LED连接的硬件接口
	KEY_Init();         //初始化与按键连接的硬件接口
	BEEP_Init();		//初始化BEEP
	LED0=0;			    //点亮LED
	while(1)
	{
		key=KEY_Scan(0);//得到键值
		switch(key)
		{				 
			case KEY0_PRES://KEY0
				 LED0=!LED0;
				 break;
			case KEY1_PRES://KEY1
				 LED1=!LED1;
				 break;
			case KEY2_PRES://控制BEEP
				 BEEP=!BEEP;
				 break;
			case WKUP_PRES://KEY_UP				
				 LED0=!LED0;
				 LED1=!LED1;
				 break;
			default:
				 delay_ms(10);	
		} 
	}
 }

