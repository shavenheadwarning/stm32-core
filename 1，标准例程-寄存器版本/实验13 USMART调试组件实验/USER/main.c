#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
/************************************************
 ALIENTEK NANO STM32������ʵ��13
 USMART�������ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 
//LED״̬���ú���
void led_set(u8 sta)
{
	LED2=sta;
} 
//�����������ò��Ժ���
void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}  	
int main(void)
{ 
    Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,115200);//���ڳ�ʼ�� 
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	usmart_dev.init(72);//��ʼ��USMART				 	
    while(1)
	{		 	  
		LED0=!LED0;					 
		delay_ms(500);	
	}											    
}	



