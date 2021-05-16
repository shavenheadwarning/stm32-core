#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "smg.h"
#include "timer.h"

/************************************************
 ALIENTEK NANO STM32开发板实验12
 数码管显示实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//共阴数字数组
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,全灭
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};

 int main(void)
 {
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	uart_init(72,115200);//串口初始化 
	LED_SMG_Init();  //数码管初始化
 	LED_Init();      //LED初始化
    TIM3_Init(19,7199);//2ms定时显示		 
 	while(1)
	{		
	}	 
 }

u8 smg_wei=0;//数码管位选
u8 smg_duan=0;//数码管段选
u16 t=0;

void TIM3_IRQHandler(void)//TIM3中断
{
	if(TIM3->SR&0X0001)//溢出中断
	{
		LED_Write_Data(smg_num[smg_duan],smg_wei);//数码管显示
	    LED_Refresh();//刷新显示
	    smg_wei++;
		if(smg_wei==8)  smg_wei=0;	
		t++;
		if(t==500)//数码管每秒更新
		{
			t=0;
			LED0=!LED0;
			smg_duan++;
			if(smg_duan==18) smg_duan=0;
		}		    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 
}




