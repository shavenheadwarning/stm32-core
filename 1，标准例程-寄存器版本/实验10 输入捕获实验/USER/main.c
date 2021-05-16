#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
/************************************************
 ALIENTEK NANO STM32������ʵ��10
 ���벶��ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

extern u8   TIM2CH1_CAPTURE_STA;	//���벶��״̬		    				
extern u16  TIM2CH1_CAPTURE_VAL;	//���벶��ֵ
int main(void)
{	 
    u16 t=0;
	u32 temp=0; 
	Stm32_Clock_Init(9);			//ϵͳʱ������
	LED_Init();                     //LED��ʼ��
	uart_init(72,115200);	 		//���ڳ�ʼ��Ϊ115200
	delay_init(72);	   	 			//��ʱ��ʼ�� 
 	TIM2_Cap_Init(0XFFFF,72-1);	    //��1Mhz��Ƶ�ʼ��� 
   	while(1)
	{
 		delay_ms(10); 		 
		if(TIM2CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//���ʱ���ܺ�
			temp+=TIM2CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%d us\r\n",temp);	//��ӡ�ܵĸߵ�ƽʱ��
 			TIM2CH1_CAPTURE_STA=0;			//������һ�β���
 		}
		t++;
		if(t==20)
		{
			t=0;
			LED0=!LED0;
		}
	}
}
