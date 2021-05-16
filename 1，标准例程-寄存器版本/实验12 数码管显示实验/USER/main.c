#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "smg.h"
#include "timer.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��12
 �������ʾʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//������������
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};

 int main(void)
 {
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,115200);//���ڳ�ʼ�� 
	LED_SMG_Init();  //����ܳ�ʼ��
 	LED_Init();      //LED��ʼ��
    TIM3_Init(19,7199);//2ms��ʱ��ʾ		 
 	while(1)
	{		
	}	 
 }

u8 smg_wei=0;//�����λѡ
u8 smg_duan=0;//����ܶ�ѡ
u16 t=0;

void TIM3_IRQHandler(void)//TIM3�ж�
{
	if(TIM3->SR&0X0001)//����ж�
	{
		LED_Write_Data(smg_num[smg_duan],smg_wei);//�������ʾ
	    LED_Refresh();//ˢ����ʾ
	    smg_wei++;
		if(smg_wei==8)  smg_wei=0;	
		t++;
		if(t==500)//�����ÿ�����
		{
			t=0;
			LED0=!LED0;
			smg_duan++;
			if(smg_duan==18) smg_duan=0;
		}		    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 
}




