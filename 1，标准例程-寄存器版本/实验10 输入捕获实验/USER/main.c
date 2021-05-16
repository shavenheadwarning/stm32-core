#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
/************************************************
 ALIENTEK NANO STM32开发板实验10
 输入捕获实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

extern u8   TIM2CH1_CAPTURE_STA;	//输入捕获状态		    				
extern u16  TIM2CH1_CAPTURE_VAL;	//输入捕获值
int main(void)
{	 
    u16 t=0;
	u32 temp=0; 
	Stm32_Clock_Init(9);			//系统时钟设置
	LED_Init();                     //LED初始化
	uart_init(72,115200);	 		//串口初始化为115200
	delay_init(72);	   	 			//延时初始化 
 	TIM2_Cap_Init(0XFFFF,72-1);	    //以1Mhz的频率计数 
   	while(1)
	{
 		delay_ms(10); 		 
		if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//溢出时间总和
			temp+=TIM2CH1_CAPTURE_VAL;		//得到总的高电平时间
			printf("HIGH:%d us\r\n",temp);	//打印总的高点平时间
 			TIM2CH1_CAPTURE_STA=0;			//开启下一次捕获
 		}
		t++;
		if(t==20)
		{
			t=0;
			LED0=!LED0;
		}
	}
}
