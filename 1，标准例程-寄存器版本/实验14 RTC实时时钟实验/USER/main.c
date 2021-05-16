#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "rtc.h"
#include "smg.h"
#include "timer.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��14
 RTCʵʱʱ��ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 

// ������������
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};

int main(void)
{ 
    Stm32_Clock_Init(9);    //ϵͳʱ������
	delay_init(72);	        //��ʱ��ʼ��
	uart_init(72,115200);   //���ڳ�ʼ�� 
	LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
	LED_SMG_Init();	        //����ܳ�ʼ��
	usmart_dev.init(72);	//��ʼ��USMART
    TIM3_Init(19,7199);     //�����2ms��ʱ��ʾ		 
	while(RTC_Init())		//RTC��ʼ����һ��Ҫ��ʼ���ɹ�
	{ 
		printf("RTC ERROR!\r\n");
		delay_ms(800);
		printf("RTC Trying...\r\n");	
	}		    								    
	while(1)
	{								  
	} 											    
}	

u8 smg_wei=0;//�����λѡ
u8 num=0;//�������ֵ
u8 time=0;//ʱ��ֵ

void TIM3_IRQHandler(void)//TIM3�ж�
{
	if(TIM3->SR&0X0001)//����ж�
	{
		switch(smg_wei)
		{
		   case 0:  num = smg_num[calendar.hour/10]; break;//ʱ
		   case 1:  num = smg_num[calendar.hour%10]; break; 
		   case 2:   
		   case 5:  num = 0x02; break;
		   case 3:  num = smg_num[calendar.min/10]; break; //��
		   case 4:  num = smg_num[calendar.min%10]; break; 
		   case 6:  num = smg_num[calendar.sec/10]; break; //��
		   case 7:  num = smg_num[calendar.sec%10]; break;   
	   }
	   if(time!=calendar.sec)//LED0ÿ����˸
	   {
			time=calendar.sec;
			LED0=!LED0;
	   }
		
	   LED_Write_Data(num,smg_wei);//д����
	   LED_Refresh();//������ʾ
	   smg_wei++;//�����λѡ�仯
	   if(smg_wei==8) smg_wei=0;
			
	}
	TIM3->SR&=~(1<<0);//����жϱ�־λ 
}


