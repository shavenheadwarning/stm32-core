#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "wkup.h"
/************************************************
 ALIENTEK NANO STM32������ʵ��15
 ��������ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
   	
int main(void)
{ 
	Stm32_Clock_Init(9);   //ϵͳʱ������
	delay_init(72);	       //��ʱ��ʼ��
	uart_init(72,115200);  //���ڳ�ʼ�� 
	LED_Init();		  	   //��ʼ����LED���ӵ�Ӳ���ӿ�
 	WKUP_Init();		   //��ʼ��WK_UP������ͬʱ����Ƿ�����������
    printf("NANO STM32\r\n");
	printf("WKUP TEST\r\n"); 
	while(1)	
	{
		LED0=!LED0;
		delay_ms(250);
	}											    
}	



