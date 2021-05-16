#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "malloc.h"  
#include "w25qxx.h"		
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h"  

/************************************************
 ALIENTEK NANO STM32������ʵ��27
 USB������ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 int main(void)
 { 
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA; 
	Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);		//��ʱ��ʼ��
	uart_init(72,115200);//����1��ʼ��   	  
	LED_Init();         //LED��ʼ��	 
	KEY_Init();			//������ʼ��	   
	printf("NANO STM32\r\n");
	printf("USB Card Reader TEST\r\n");
 	W25QXX_Init();
	if(W25QXX_TYPE!=W25Q16) printf("W25Q16 Error!\r\n");//���SPI FLASH����
	else //SPI FLASH ����
	{   															  
 	   	Mass_Memory_Size[0]=2048*1024;//ǰ2M�ֽ�
	    Mass_Block_Size[0] =512;
	    Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
		printf("SPI FLASH Size:2048KB\r\n");
	}
  	delay_ms(1800);
 	USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(300);
   	USB_Port_Set(1);	//USB�ٴ�����   
	printf("USB Connecting...\r\n");//��ʾUSB������
	Data_Buffer=mymalloc(BULK_MAX_PACKET_SIZE*2*4);	//ΪUSB���ݻ����������ڴ�
	Bulk_Data_Buff=mymalloc(BULK_MAX_PACKET_SIZE);	//�����ڴ�
 	//USB����
 	USB_Interrupts_Config();    
 	Set_USBClock();   
 	USB_Init();	    
	delay_ms(1800);	   	    
	while(1)
	{	
		delay_ms(1);				  
		if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
		{	 						   		  	   
			if(USB_STATUS_REG&0x01)//����д		  
			{
                printf("USB Writing...\r\n");//��ʾUSB����д������				
			}
			if(USB_STATUS_REG&0x02)//���ڶ�
			{
                printf("USB Reading...\r\n");//��ʾUSB���ڶ������� 					
			}	 										  
			if(USB_STATUS_REG&0x04)printf("USB Write Err\r\n");//��ʾд�����  
			if(USB_STATUS_REG&0x08)printf("USB Read  Err\r\n");//��ʾ��������    
			USB_STA=USB_STATUS_REG;//��¼����״̬
		}
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==CONFIGURED) printf("USB Connected\r\n");//��ʾUSB�����Ѿ����� 
			else printf("USB DisConnected\r\n");//��ʾUSB���γ��� 
			Divece_STA=bDeviceState;
		}
		tct++;
		if(tct==200)
		{
			tct=0;
			LED0=!LED0;//��ʾϵͳ������
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB������,�����offline������
				bDeviceState=CONFIGURED;
			}else//û�еõ���ѯ 
			{
				offline_cnt++;  
				if(offline_cnt>10)bDeviceState=UNCONNECTED;//2s��û�յ����߱��,����USB���γ���
			}
			USB_STATUS_REG=0;
		}
	} 										    			    
}







