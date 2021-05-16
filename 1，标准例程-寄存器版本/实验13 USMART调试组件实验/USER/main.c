#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
/************************************************
 ALIENTEK NANO STM32开发板实验13
 USMART调试组件实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 
//LED状态设置函数
void led_set(u8 sta)
{
	LED2=sta;
} 
//函数参数调用测试函数
void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}  	
int main(void)
{ 
    Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	uart_init(72,115200);//串口初始化 
	LED_Init();		  	//初始化与LED连接的硬件接口
	usmart_dev.init(72);//初始化USMART				 	
    while(1)
	{		 	  
		LED0=!LED0;					 
		delay_ms(500);	
	}											    
}	



