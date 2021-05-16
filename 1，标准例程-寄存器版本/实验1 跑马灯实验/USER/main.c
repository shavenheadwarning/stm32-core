#include "led.h"
#include "delay.h"
#include "sys.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��1
 �����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

/*ʵ��LED�ƽ�����˸*/
/*
int main(void)
{	

	Stm32_Clock_Init(9); 	//ϵͳʱ������
	delay_init(72);	     	//��ʱ��ʼ��  
	LED_Init();	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	 
	while(1)
	{	
		LED0=0;//LED0��
		LED1=1;//LED1��
		delay_ms(500);
		LED0=1;//LED0��
		LED1=0;//LED1��
		delay_ms(500);	
	}
}
*/
 
/*ʵ����ˮ��*/

 int main(void)
 {	
	u8 LED=0x00;
	u8 i=0;
	
	Stm32_Clock_Init(9); 	//ϵͳʱ������
	delay_init(72);	     	//��ʱ��ʼ��  
	LED_Init();	            //��ʼ����LED���ӵ�Ӳ���ӿ�
	 
	while(1)
	{
		for(i=0;i<8;i++)
		{
			LED++;	
			GPIOC->BRR = LED;
			delay_ms(200);
			LED<<=1;//LED���ε���	
		}
		LED++;	
		GPIOC->BRR = LED;
		delay_ms(200);
		LED=0xff;//LEDȫ��
		GPIOC->BSRR = LED;
		delay_ms(200);
	}
 }

