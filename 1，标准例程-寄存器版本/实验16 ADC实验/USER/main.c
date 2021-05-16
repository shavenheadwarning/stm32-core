#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "smg.h"
#include "timer.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��16
 ADCʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
   
// ������������
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00}; 

int main(void)
{ 
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,115200);//���ڳ�ʼ�� 
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    LED_SMG_Init();	     //����ܳ�ʼ��
 	Adc_Init();		  	 //ADC��ʼ��	    
    TIM3_Init(19,7199);  //�����2ms��ʱ��ʾ	
	while(1)
	{}											    
}	
 
//ADC����
static u16 adcx=0;
static u16 adcx1=0;
static float temp=0;

u8 num=0;//�����ֵ
u8 smg_wei=0;//�����λѡ
u8 adc_t=0;//adc����ʱ��ֵ
u8 led_t=0;//led��ʾʱ��ֵ

void TIM3_IRQHandler(void) //TIM3�ж�
{
	if(TIM3->SR&0X0001)//����ж�
	{
        
		 adc_t++;
		 if(adc_t==100)//ADC 200ms����
		 {
			 adc_t=0;
			 adcx=Get_Adc_Average(ADC_CH9,3);//ADCԭʼֵ 
			 temp=(float)adcx*(3.3/4096);//ADC��ѹֵ
			 adcx1=temp;
			 temp-=adcx1;
			 temp*=1000; 
		 }
		 switch(smg_wei)
		 {
			   //ADC����ԭʼֵ
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
			   //ADC�����ĵ�ѹֵ
			   case 5:  num = smg_num[adcx1]|0x01;break;
			   case 6:  num = smg_num[(u16)temp/100]; break; 
			   case 7:  num = smg_num[(u16)temp%100/10];break;    
		 }
		 LED_Write_Data(num,smg_wei);
		 LED_Refresh();
		 smg_wei++;
		 if(smg_wei==8) smg_wei=0;
		 
		 led_t++;
		 if(led_t==250)//500ms��˸
		 {
			 led_t=0;
			 LED0=!LED0;
		 }	
	}
	TIM3->SR&=~(1<<0);//����жϱ�־λ 
}



