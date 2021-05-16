#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "dht11.h" 	 
#include "timer.h"
#include "smg.h" 
 
/************************************************
 ALIENTEK NANO STM32������ʵ��22
 DHT11������ʪ�ȴ�����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 
// ������������
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00}; 
 
u8 smg_wei=2;//�����λѡ
u8 num=0;//��ֵ
u16 led_t=0;//ledͳ��ʱ��
u16 dht11_t=0;//dht11����ʱ��
 u8 temperature;//�¶�ֵ  	    
u8 humidity;//ʪ��ֵ

int main(void)
{	    
	Stm32_Clock_Init(9);   //ϵͳʱ������
	delay_init(72);	       //��ʱ��ʼ��
	uart_init(72,115200);  //���ڳ�ʼ�� 
	LED_Init();		  	   //��ʼ����LED���ӵ�Ӳ���ӿ�
    LED_SMG_Init();        //����ܳ�ʼ��
    printf("NANO STM32\r\n");
	printf("DHT11 TEST\r\n");
 	while(DHT11_Init())	//DHT11��ʼ��	
	{
		printf("DHT11 Error\r\n");
		delay_ms(200);
		LED3=!LED3;//LED3��˸��ʾDHT11��ʼ��ʧ��
	}
    LED3=1;	
	printf("DHT11 OK\r\n");
	TIM3_Init(19,7199);//�����2ms��ʱ��ʾ  
	while(1)
	{}
}
 
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if(TIM3->SR&0X0001)//����ж�
	{ 
        dht11_t++;
		if(dht11_t==500)//DHT11 1S����
		{
			dht11_t=0;
			DHT11_Read_Data(&temperature,&humidity);//��ȡ��ʪ��ֵ		
		}
		switch(smg_wei)
		{
			   case 2:  num = smg_num[temperature/10]; break;//�¶�ֵ
			   case 3:  num =smg_num[temperature%10];break; 
			   case 6:  num = smg_num[humidity/10]; break;//ʪ��ֵ
			   case 7:  num =smg_num[humidity%10];break;    
		}
	    LED_Write_Data(num,smg_wei);
	    LED_Refresh();
	    smg_wei++;
		if(smg_wei==4)  smg_wei=6;
		if(smg_wei==8)  smg_wei=2;	
		led_t++;
		if(led_t==250)//LED0ÿ500ms��˸
		{
			led_t=0;
			LED0=!LED0;
		}		
	}
	TIM3->SR&=~(1<<0);//����жϱ�־λ 
}

