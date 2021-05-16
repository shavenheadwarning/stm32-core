#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��8
 ��ʱ���ж�ʵ�� 
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
	uart_init(72,115200);//���ڳ�ʼ�� 
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	TIM3_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
   	while(1)
	{
		LED0=!LED0;
		delay_ms(200);		   
	}
}
