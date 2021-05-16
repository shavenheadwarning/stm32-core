#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "ds18b20.h"
#include "timer.h"
#include "smg.h"

/************************************************
 ALIENTEK NANO STM32开发板实验21
 DS18B20数字温度传感器实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

// 共阴数字数组
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,全灭
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};

short temperature;//温度值 
u8 smg_wei=4;//数码管位选
u8 num=0;//数码管数值
u16 led_t=0;//led时间
u16 temp_t=0;//采样时间
u8 flag=0;//温度正负标志位

int main(void)
{ 
	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,115200);	//串口初始化为115200
	delay_init(72);	   	 	//延时初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口
	LED_SMG_Init();         //数码管初始化
    printf("NANO STM32\r\n");
	printf("DS18B20 TEST\r\n");
 	while(DS18B20_Init())	//DS18B20初始化	
	{
		printf("DS18B20 Error\r\n");
		delay_ms(200);
		LED3=!LED3;//LED3闪烁表示DS18B20初始化失败
	}
    LED3=1;	
	printf("DS18B20 OK\r\n");
    TIM3_Init(19,7199);//数码管2ms定时显示 
	while(1)
	{}
}

void TIM3_IRQHandler(void)//TIM3中断
{
	if(TIM3->SR&0X0001)//溢出中断
	{  
        temp_t++;
		if(temp_t==500)//DS18B20 1S采样
		{
			temp_t=0;
			temperature=DS18B20_Get_Temp();//获取温度值
			if(temperature<0)//若温度为负数
			{
				temperature=-temperature;
			    flag=1;
			}else flag=0;	
		}
		switch(smg_wei)
		{
			   case 4:  if(flag)num = 0x02; //显示"-"以表示负温度
			            else num=0x00; 
			            break; 
			   case 5:  num = smg_num[temperature/10/10];break; //温度值
			   case 6:  num = smg_num[temperature/10%10]|0x01; break; 
			   case 7:  num =smg_num[temperature%10];break; 
			     
		}
		
	    LED_Write_Data(num,smg_wei);
	    LED_Refresh();
	    smg_wei++;
		if(smg_wei==8)  smg_wei=4;	
		led_t++;
		if(led_t==250)//LED0 500ms闪烁
		{
			led_t=0;
			LED0=!LED0;
		}
        TIM3->SR&=~(1<<0);//清除中断标志位 
	   	
	}
}






