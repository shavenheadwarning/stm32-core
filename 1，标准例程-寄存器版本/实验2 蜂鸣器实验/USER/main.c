#include "sys.h"	
#include "delay.h"	
#include "led.h" 
#include "beep.h" 

/************************************************
 ALIENTEK NANO STM32������ʵ��2
 ������ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
	Stm32_Clock_Init(9); 	//ϵͳʱ������
	delay_init(72);	     	//��ʱ��ʼ��  
	LED_Init();		  	    //��ʼ����LED���ӵ�Ӳ���ӿ�
	BEEP_Init();            //��ʼ���������˿�
	while(1)
	{
		LED0=0;
		BEEP=0;		  
		delay_ms(300);//��ʱ300ms
		LED0=1;	  
		BEEP=1;  
		delay_ms(300);//��ʱ300ms
	}
 }

