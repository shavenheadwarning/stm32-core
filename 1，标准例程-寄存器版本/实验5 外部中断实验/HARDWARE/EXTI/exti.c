#include "exti.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/3/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  	  
 
 
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
 
    KEY_Init();//��ʼ��������Ӧioģʽ
    Ex_NVIC_Config(GPIO_C,8,FTIR); 		//�½��ش���
	Ex_NVIC_Config(GPIO_C,9,FTIR);		//�½��ش���
	Ex_NVIC_Config(GPIO_D,2,FTIR);		//�½��ش���
	Ex_NVIC_Config(GPIO_A,0,RTIR);		//�����ش���

	MY_NVIC_Init(2,2,EXTI0_IRQn,2);    	//��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,1,EXTI9_5_IRQn,2);  	//��ռ2�������ȼ�1����2
	MY_NVIC_Init(2,0,EXTI2_IRQn,2);  	//��ռ2�������ȼ�0����2	  
 
} 
//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
    delay_ms(10);    //����
	if(WK_UP==1)     //KEY_UP����
	{	  
		LED0=!LED0;
		LED1=!LED1;	
	}
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ
}
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
    delay_ms(10);    //����			 
    if(KEY2==0)	     //KEY2����
    {
       BEEP=!BEEP;
    }
    EXTI->PR=1<<2;     //���LINE2�ϵ��жϱ�־λ  
}
//�ⲿ�ж�5_9�����жϳ���
 void EXTI9_5_IRQHandler(void)
{		
	delay_ms(10);   //����			 
	if(KEY0==0)	    //KEY0����
	{
		LED0=!LED0;
	}
 	if(KEY1==0)	    //KEY1����
	{
		LED1=!LED1;
	}
	EXTI->PR=1<<8;  //���LINE8�ϵ��жϱ�־λ  
	EXTI->PR=1<<9;  //���LINE9�ϵ��жϱ�־λ  	
}
