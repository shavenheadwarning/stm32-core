#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "beep.h"
#include "usart.h"
#include "exti.h"
/************************************************
 ALIENTEK NANO STM32������ʵ��5
 �ⲿ�ж�ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
int main(void)
{	
 
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��  
	uart_init(72,115200);//���ڳ�ʼ��Ϊ115200
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
	BEEP_Init();         //��������ʼ�� 
 	EXTIX_Init();		 //�ⲿ�жϳ�ʼ��
    LED0=0;				 //����LED
	while(1)
	{	    
		printf("OK\n");	
		delay_ms(1000);	  
	} 
}


