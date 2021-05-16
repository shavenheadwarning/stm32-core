#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "smg.h"
#include "timer.h"

/************************************************
 ALIENTEK NANO STM32开发板实验16
 ADC实验 
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
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	uart_init(72,115200);//串口初始化 
	LED_Init();		  	 //初始化与LED连接的硬件接口
    LED_SMG_Init();	     //数码管初始化
 	Adc_Init();		  	 //ADC初始化	    
    TIM3_Init(19,7199);  //数码管2ms定时显示	
	while(1)
	{}											    
}	
 
//ADC变量
static u16 adcx=0;
static u16 adcx1=0;
static float temp=0;

u8 num=0;//数码管值
u8 smg_wei=0;//数码管位选
u8 adc_t=0;//adc采样时间值
u8 led_t=0;//led显示时间值

void TIM3_IRQHandler(void) //TIM3中断
{
	if(TIM3->SR&0X0001)//溢出中断
	{
        
		 adc_t++;
		 if(adc_t==100)//ADC 200ms采样
		 {
			 adc_t=0;
			 adcx=Get_Adc_Average(ADC_CH9,3);//ADC原始值 
			 temp=(float)adcx*(3.3/4096);//ADC电压值
			 adcx1=temp;
			 temp-=adcx1;
			 temp*=1000; 
		 }
		 switch(smg_wei)
		 {
			   //ADC采样原始值
			   case 0:  if(adcx>1000)num = smg_num[adcx/1000];
						else         num = 0x00;				 
					    break;
			   case 1: 	if(adcx>100) num = smg_num[adcx%1000/100];
						else         num = 0x00;
						break; 
			   case 2:  if(adcx>10)  num = smg_num[adcx%100/10];
						else         num = 0x00;					
				        break;
			   case 3:  num = smg_num[adcx%10]; break;
			   case 4:  num = 0x00; break; 
			   //ADC换算后的电压值
			   case 5:  num = smg_num[adcx1]|0x01;break;
			   case 6:  num = smg_num[(u16)temp/100]; break; 
			   case 7:  num = smg_num[(u16)temp%100/10];break;    
		 }
		 LED_Write_Data(num,smg_wei);
		 LED_Refresh();
		 smg_wei++;
		 if(smg_wei==8) smg_wei=0;
		 
		 led_t++;
		 if(led_t==250)//500ms闪烁
		 {
			 led_t=0;
			 LED0=!LED0;
		 }	
	}
	TIM3->SR&=~(1<<0);//清除中断标志位 
}



