#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "tpad.h"

/************************************************
 ALIENTEK NANO STM32开发板实验11
 电容触摸按键实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 
 int main(void)
 {	
 	u8 t=0;  	
	Stm32_Clock_Init(9);    //系统时钟设置
	delay_init(72);	        //延时初始化
	uart_init(72,115200);   //串口初始化 
 	LED_Init();			    //LED端口初始化
    TPAD_Init(6);			//初始化触摸按键
   	while(1)
	{					  						  		 
 		if(TPAD_Scan(0))	//成功捕获到了一次上升沿(此函数执行时间至少15ms)
		{
			LED5=!LED5;		//LED5取反
		}
		t++;
		if(t==15)		 
		{
			t=0;
			LED0=!LED0;		//LED0取反,提示程序正在运行
		}
		delay_ms(10);
	}
 }

