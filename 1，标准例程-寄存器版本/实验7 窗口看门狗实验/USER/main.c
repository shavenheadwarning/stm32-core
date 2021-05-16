#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "wwdg.h"
/************************************************
 ALIENTEK NANO STM32������ʵ��7
 ���ڿ��Ź� ʵ�� 
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
 	LED_Init();//LED��ʼ��
	LED0=0;//LED0����
	delay_ms(300);	  
	WWDG_Init(0X7F,0X5F,3);//������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8 
 	while(1)
	{
		LED0=1; //��DS0	  	   
	}	 
 }


