#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "dht11.h" 	 
#include "timer.h"
#include "smg.h" 
 
/************************************************
 ALIENTEK NANO STM32开发板实验22
 DHT11数字温湿度传感器实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 
// 共阴数字数组
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,全灭
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00}; 
 
u8 smg_wei=2;//数码管位选
u8 num=0;//数值
u16 led_t=0;//led统计时间
u16 dht11_t=0;//dht11采样时间
 u8 temperature;//温度值  	    
u8 humidity;//湿度值

int main(void)
{	    
	Stm32_Clock_Init(9);   //系统时钟设置
	delay_init(72);	       //延时初始化
	uart_init(72,115200);  //串口初始化 
	LED_Init();		  	   //初始化与LED连接的硬件接口
    LED_SMG_Init();        //数码管初始化
    printf("NANO STM32\r\n");
	printf("DHT11 TEST\r\n");
 	while(DHT11_Init())	//DHT11初始化	
	{
		printf("DHT11 Error\r\n");
		delay_ms(200);
		LED3=!LED3;//LED3闪烁表示DHT11初始化失败
	}
    LED3=1;	
	printf("DHT11 OK\r\n");
	TIM3_Init(19,7199);//数码管2ms定时显示  
	while(1)
	{}
}
 
void TIM3_IRQHandler(void)   //TIM3中断
{
	if(TIM3->SR&0X0001)//溢出中断
	{ 
        dht11_t++;
		if(dht11_t==500)//DHT11 1S采样
		{
			dht11_t=0;
			DHT11_Read_Data(&temperature,&humidity);//读取温湿度值		
		}
		switch(smg_wei)
		{
			   case 2:  num = smg_num[temperature/10]; break;//温度值
			   case 3:  num =smg_num[temperature%10];break; 
			   case 6:  num = smg_num[humidity/10]; break;//湿度值
			   case 7:  num =smg_num[humidity%10];break;    
		}
	    LED_Write_Data(num,smg_wei);
	    LED_Refresh();
	    smg_wei++;
		if(smg_wei==4)  smg_wei=6;
		if(smg_wei==8)  smg_wei=2;	
		led_t++;
		if(led_t==250)//LED0每500ms闪烁
		{
			led_t=0;
			LED0=!LED0;
		}		
	}
	TIM3->SR&=~(1<<0);//清除中断标志位 
}

