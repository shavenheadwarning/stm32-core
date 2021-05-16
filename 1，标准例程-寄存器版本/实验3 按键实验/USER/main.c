#include "sys.h"	
#include "delay.h"	
#include "led.h" 
#include "key.h" 
#include "beep.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��3
 ����ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
	vu8 key=0;	
	Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);	    //��ʱ��ʼ��  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();         //��ʼ���밴�����ӵ�Ӳ���ӿ�
	BEEP_Init();		//��ʼ��BEEP
	LED0=0;			    //����LED
	while(1)
	{
		key=KEY_Scan(0);//�õ���ֵ
		switch(key)
		{				 
			case KEY0_PRES://KEY0
				 LED0=!LED0;
				 break;
			case KEY1_PRES://KEY1
				 LED1=!LED1;
				 break;
			case KEY2_PRES://����BEEP
				 BEEP=!BEEP;
				 break;
			case WKUP_PRES://KEY_UP				
				 LED0=!LED0;
				 LED1=!LED1;
				 break;
			default:
				 delay_ms(10);	
		} 
	}
 }

