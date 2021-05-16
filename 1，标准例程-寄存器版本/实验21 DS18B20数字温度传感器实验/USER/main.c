#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "ds18b20.h"
#include "timer.h"
#include "smg.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��21
 DS18B20�����¶ȴ�����ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

// ������������
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};

short temperature;//�¶�ֵ 
u8 smg_wei=4;//�����λѡ
u8 num=0;//�������ֵ
u16 led_t=0;//ledʱ��
u16 temp_t=0;//����ʱ��
u8 flag=0;//�¶�������־λ

int main(void)
{ 
	Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,115200);	//���ڳ�ʼ��Ϊ115200
	delay_init(72);	   	 	//��ʱ��ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LED_SMG_Init();         //����ܳ�ʼ��
    printf("NANO STM32\r\n");
	printf("DS18B20 TEST\r\n");
 	while(DS18B20_Init())	//DS18B20��ʼ��	
	{
		printf("DS18B20 Error\r\n");
		delay_ms(200);
		LED3=!LED3;//LED3��˸��ʾDS18B20��ʼ��ʧ��
	}
    LED3=1;	
	printf("DS18B20 OK\r\n");
    TIM3_Init(19,7199);//�����2ms��ʱ��ʾ 
	while(1)
	{}
}

void TIM3_IRQHandler(void)//TIM3�ж�
{
	if(TIM3->SR&0X0001)//����ж�
	{  
        temp_t++;
		if(temp_t==500)//DS18B20 1S����
		{
			temp_t=0;
			temperature=DS18B20_Get_Temp();//��ȡ�¶�ֵ
			if(temperature<0)//���¶�Ϊ����
			{
				temperature=-temperature;
			    flag=1;
			}else flag=0;	
		}
		switch(smg_wei)
		{
			   case 4:  if(flag)num = 0x02; //��ʾ"-"�Ա�ʾ���¶�
			            else num=0x00; 
			            break; 
			   case 5:  num = smg_num[temperature/10/10];break; //�¶�ֵ
			   case 6:  num = smg_num[temperature/10%10]|0x01; break; 
			   case 7:  num =smg_num[temperature%10];break; 
			     
		}
		
	    LED_Write_Data(num,smg_wei);
	    LED_Refresh();
	    smg_wei++;
		if(smg_wei==8)  smg_wei=4;	
		led_t++;
		if(led_t==250)//LED0 500ms��˸
		{
			led_t=0;
			LED0=!LED0;
		}
        TIM3->SR&=~(1<<0);//����жϱ�־λ 
	   	
	}
}






