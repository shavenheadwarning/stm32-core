#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "iwdg.h"
/************************************************
 ALIENTEK NANO STM32开发板实验6
 独立看门狗 实验 
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
    KEY_Init();          //按键初始化	 
	delay_ms(300);   	 //让人看得到灭
	IWDG_Init(4,625);    //与分频数为64,重载值为625,溢出时间为1s	   
	LED0=0;				 //点亮LED0
	while(1)
	{
		if(KEY_Scan(0)==WKUP_PRES)IWDG_Feed();//如果WK_UP按下,则喂狗
		delay_ms(10);
	}
}


