#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"

/************************************************
 ALIENTEK NANO STM32开发板实验8
 定时器中断实验 
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
	uart_init(72,115200);//串口初始化 
	LED_Init();		  	//初始化与LED连接的硬件接口
	TIM3_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
   	while(1)
	{
		LED0=!LED0;
		delay_ms(200);		   
	}
}
