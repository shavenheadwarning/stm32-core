#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "wwdg.h"
/************************************************
 ALIENTEK NANO STM32开发板实验7
 窗口看门狗 实验 
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
 	LED_Init();//LED初始化
	LED0=0;//LED0点亮
	delay_ms(300);	  
	WWDG_Init(0X7F,0X5F,3);//计数器值为7f,窗口寄存器为5f,分频数为8 
 	while(1)
	{
		LED0=1; //灭DS0	  	   
	}	 
 }


