#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "rtc.h"
#include "smg.h"
#include "timer.h"

/************************************************
 ALIENTEK NANO STM32开发板实验14
 RTC实时时钟实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 

// 共阴数字数组
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,全灭
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};

int main(void)
{ 
    Stm32_Clock_Init(9);    //系统时钟设置
	delay_init(72);	        //延时初始化
	uart_init(72,115200);   //串口初始化 
	LED_Init();				//初始化与LED连接的硬件接口
	LED_SMG_Init();	        //数码管初始化
	usmart_dev.init(72);	//初始化USMART
    TIM3_Init(19,7199);     //数码管2ms定时显示		 
	while(RTC_Init())		//RTC初始化，一定要初始化成功
	{ 
		printf("RTC ERROR!\r\n");
		delay_ms(800);
		printf("RTC Trying...\r\n");	
	}		    								    
	while(1)
	{								  
	} 											    
}	

u8 smg_wei=0;//数码管位选
u8 num=0;//数码管数值
u8 time=0;//时间值

void TIM3_IRQHandler(void)//TIM3中断
{
	if(TIM3->SR&0X0001)//溢出中断
	{
		switch(smg_wei)
		{
		   case 0:  num = smg_num[calendar.hour/10]; break;//时
		   case 1:  num = smg_num[calendar.hour%10]; break; 
		   case 2:   
		   case 5:  num = 0x02; break;
		   case 3:  num = smg_num[calendar.min/10]; break; //分
		   case 4:  num = smg_num[calendar.min%10]; break; 
		   case 6:  num = smg_num[calendar.sec/10]; break; //秒
		   case 7:  num = smg_num[calendar.sec%10]; break;   
	   }
	   if(time!=calendar.sec)//LED0每秒闪烁
	   {
			time=calendar.sec;
			LED0=!LED0;
	   }
		
	   LED_Write_Data(num,smg_wei);//写数据
	   LED_Refresh();//更新显示
	   smg_wei++;//数码管位选变化
	   if(smg_wei==8) smg_wei=0;
			
	}
	TIM3->SR&=~(1<<0);//清除中断标志位 
}


