#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "tpad.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��11
 ���ݴ�������ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 
 int main(void)
 {	
 	u8 t=0;  	
	Stm32_Clock_Init(9);    //ϵͳʱ������
	delay_init(72);	        //��ʱ��ʼ��
	uart_init(72,115200);   //���ڳ�ʼ�� 
 	LED_Init();			    //LED�˿ڳ�ʼ��
    TPAD_Init(6);			//��ʼ����������
   	while(1)
	{					  						  		 
 		if(TPAD_Scan(0))	//�ɹ�������һ��������(�˺���ִ��ʱ������15ms)
		{
			LED5=!LED5;		//LED5ȡ��
		}
		t++;
		if(t==15)		 
		{
			t=0;
			LED0=!LED0;		//LED0ȡ��,��ʾ������������
		}
		delay_ms(10);
	}
 }

