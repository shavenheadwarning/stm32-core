#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//SPI��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/3/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////    
 
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��
void SPI2_Init(void)
{

    RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ�� 	 
	RCC->APB1ENR|=1<<14;      //SPI2ʱ��ʹ�� 
	
	//����ֻ���SPI�ڳ�ʼ��
	GPIOB->CRH&=0X000FFFFF; 
	GPIOB->CRH|=0XBBB00000;//PB13.14.15���� 	    
	GPIOB->ODR|=0X7<<13;    //PB13.14.15����
   
    SPI2->CR1|=0<<10;//ȫ˫��ģʽ	
	SPI2->CR1|=1<<9; //���nss����
	SPI2->CR1|=1<<8;  

	SPI2->CR1|=1<<2; //SPI����
	SPI2->CR1|=0<<11;//8bit���ݸ�ʽ	
	SPI2->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1
	SPI2->CR1|=1<<0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	SPI2->CR1|=7<<3; //Fsck=Fcpu/256
	SPI2->CR1|=0<<7; //MSBfirst   
	SPI2->CR1|=1<<6; //SPI�豸ʹ��
	
	SPI2_ReadWriteByte(0xff);//��������(��Ҫ���ã�ά��MOSIΪ��)	


}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
  
void SPI2_SetSpeed(u8 SpeedSet)
{	
	SpeedSet&=0X07;			//���Ʒ�Χ
	SPI2->CR1&=0XFFC7; 
	SPI2->CR1|=SpeedSet<<3;	//����SPI2�ٶ�  
	SPI2->CR1|=1<<6; 		//SPI�豸ʹ�� 	
} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		
    u16 retry=0;				 
	while((SPI2->SR&1<<1)==0)//�ȴ���������	
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}			  
	SPI2->DR=TxData;	 	  //����һ��byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) //�ȴ�������һ��byte  
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}	  						    
	return SPI2->DR;          //�����յ�������	
   
	
}

