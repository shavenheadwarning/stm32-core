#include "smg.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//�������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/3/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  

//���������IO��ʼ��
void LED_SMG_Init(void)
{

//   GPIO_InitTypeDef  GPIO_InitStructure;

//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);//ʹ��PB,PC�˿�ʱ��
//   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ���� PB4����ͨIO��ʹ��,�����ܵ�IOʹ��
//   
//   //74HC595  DS-PB3 LCLK-PB4 SCK-PB5
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3��PB4��PB5
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//   GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB3/4/5

//   //74HC138  A0-PC10 A1-PC11 A2-PC12
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
//   GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC10/11/12

//   GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_5);
//   GPIO_SetBits(GPIOB,GPIO_Pin_4);
//   GPIO_ResetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	
   RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��	 
   RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��
   RCC->APB2ENR|=1<<0;    //ʹ��AFIOʱ��
   
   JTAG_Set(SWD_ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ���� PB4����ͨIO��ʹ��,�����ܵ�IOʹ��
   
   GPIOB->CRL&=0XFF000FFF; 
   GPIOB->CRL|=0X00333000;//PB3~5�������
    
   GPIOC->CRH&=0XFFF000FF; 
   GPIOC->CRH|=0X00033300;//PC10~12�������  
    	
   GPIOB->BRR = 1<<3|1<<5;
   GPIOB->BSRR = 1<<4;
   GPIOC->BSRR = 1<<10|1<<11|1<<12;
   
   
	

}
//74HC138����
//�����λѡ
//num:Ҫ��ʾ������ܱ�� 0-7(��8�������)
void LED_Wei(u8 num)
{
    LED_A0=num&0x01;
	LED_A1=(num&0x02)>>1;
	LED_A2=(num&0x04)>>2;
}
//74HC595����
//�������ʾ
//duan:��ʾ�Ķ���
//wei:Ҫ��ʾ������ܱ�� 0-7(��8�������)
void LED_Write_Data(u8 duan,u8 wei)
{
	u8 i;
	for( i=0;i<8;i++)//���Ͷ�
	{
		LED_DS=(duan>>i)&0x01;
		LED_SCK=0;
		delay_us(5);
		LED_SCK=1;
	}
    LED_Wei(wei);//��ѡ��λ
}
//74HC595����
//�����ˢ����ʾ
void LED_Refresh(void)
{
	LED_LCLK=1;
	delay_us(5);
	LED_LCLK=0;
}

