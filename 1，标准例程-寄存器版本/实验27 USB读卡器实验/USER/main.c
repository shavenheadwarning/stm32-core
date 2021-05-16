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
 ALIENTEK NANO STM32开发板实验27
 USB读卡器实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

 int main(void)
 { 
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA; 
	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,115200);//串口1初始化   	  
	LED_Init();         //LED初始化	 
	KEY_Init();			//按键初始化	   
	printf("NANO STM32\r\n");
	printf("USB Card Reader TEST\r\n");
 	W25QXX_Init();
	if(W25QXX_TYPE!=W25Q16) printf("W25Q16 Error!\r\n");//检测SPI FLASH错误
	else //SPI FLASH 正常
	{   															  
 	   	Mass_Memory_Size[0]=2048*1024;//前2M字节
	    Mass_Block_Size[0] =512;
	    Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
		printf("SPI FLASH Size:2048KB\r\n");
	}
  	delay_ms(1800);
 	USB_Port_Set(0); 	//USB先断开
	delay_ms(300);
   	USB_Port_Set(1);	//USB再次连接   
	printf("USB Connecting...\r\n");//提示USB连接中
	Data_Buffer=mymalloc(BULK_MAX_PACKET_SIZE*2*4);	//为USB数据缓存区申请内存
	Bulk_Data_Buff=mymalloc(BULK_MAX_PACKET_SIZE);	//申请内存
 	//USB配置
 	USB_Interrupts_Config();    
 	Set_USBClock();   
 	USB_Init();	    
	delay_ms(1800);	   	    
	while(1)
	{	
		delay_ms(1);				  
		if(USB_STA!=USB_STATUS_REG)//状态改变了 
		{	 						   		  	   
			if(USB_STATUS_REG&0x01)//正在写		  
			{
                printf("USB Writing...\r\n");//提示USB正在写入数据				
			}
			if(USB_STATUS_REG&0x02)//正在读
			{
                printf("USB Reading...\r\n");//提示USB正在读出数据 					
			}	 										  
			if(USB_STATUS_REG&0x04)printf("USB Write Err\r\n");//提示写入错误  
			if(USB_STATUS_REG&0x08)printf("USB Read  Err\r\n");//提示读出错误    
			USB_STA=USB_STATUS_REG;//记录最后的状态
		}
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==CONFIGURED) printf("USB Connected\r\n");//提示USB连接已经建立 
			else printf("USB DisConnected\r\n");//提示USB被拔出了 
			Divece_STA=bDeviceState;
		}
		tct++;
		if(tct==200)
		{
			tct=0;
			LED0=!LED0;//提示系统在运行
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB连接了,则清除offline计数器
				bDeviceState=CONFIGURED;
			}else//没有得到轮询 
			{
				offline_cnt++;  
				if(offline_cnt>10)bDeviceState=UNCONNECTED;//2s内没收到在线标记,代表USB被拔出了
			}
			USB_STATUS_REG=0;
		}
	} 										    			    
}







