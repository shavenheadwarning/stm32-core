#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "beep.h"
#include "usart.h"
#include "exti.h"
/************************************************
 ALIENTEK NANO STM32开发板实验5
 外部中断实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
int main(void)
{	
 
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化  
	uart_init(72,115200);//串口初始化为115200
	LED_Init();		  	 //初始化与LED连接的硬件接口 
	BEEP_Init();         //蜂鸣器初始化 
 	EXTIX_Init();		 //外部中断初始化
    LED0=0;				 //点亮LED
	while(1)
	{	    
		printf("OK\n");	
		delay_ms(1000);	  
	} 
}


