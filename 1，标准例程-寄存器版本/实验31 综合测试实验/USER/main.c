#include "led.h"
#include "delay.h"
#include "sys.h"
#include "includes.h" 
#include "key.h"
#include "usart.h"
#include "malloc.h"	
#include "smg.h"
#include "rtc.h"
#include "remote.h"
#include "beep.h"
#include "adc.h"
#include "24cxx.h"
#include "w25qxx.h"
#include "usmart.h"
#include "timer.h"
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��31
 �ۺϲ���ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/ 

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	

//ADC����
//�����������ȼ�
#define ADC_TASK_PRIO       			7 
//���������ջ��С
#define ADC_STK_SIZE  		    		64
//�����ջ
OS_STK ADC_TASK_STK[ADC_STK_SIZE];
//������
void adc_task(void *pdata);


//ң�ؿ�������
//�����������ȼ�
#define REMOTE_TASK_PRIO    			6 
//���������ջ��С
#define REMOTE_STK_SIZE  		 		64
//�����ջ	
OS_STK REMOTE_TASK_STK[REMOTE_STK_SIZE];
//������
void remote_task(void *pdata);

//��ˮ������
//�����������ȼ�
#define LEDS_TASK_PRIO       		 	5
//���������ջ��С
#define LEDS_STK_SIZE  				    64
//�����ջ	
OS_STK LEDS_TASK_STK[LEDS_STK_SIZE];
//������
void leds_task(void *pdata);

//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			4 
//���������ջ��С
#define MAIN_STK_SIZE  					128
//�����ջ	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

//USB����������
//�����������ȼ�
#define USB_TASK_PRIO       			3
//���������ջ��С
#define USB_STK_SIZE  		    		128
//�����ջ
OS_STK USB_TASK_STK[USB_STK_SIZE];
//������
void usb_task(void *pdata);

//����ɨ������
//�����������ȼ�
#define KEY_TASK_PRIO       			2 
//���������ջ��С
#define KEY_STK_SIZE  					64
//�����ջ	
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//������
void key_task(void *pdata);
//////////////////////////////////////////////////////////////////////////////
    
OS_EVENT * msg_key;	//���������¼���	 

u8 key_event=0;//�����¼� 1:��ˮ�� 2:RTCʱ�� 3:����ң�� 4:ADC�ɼ� 5:USB�¼�

//������������
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
u8 smg_duan=0;//����ܶ�ѡ
u8 smg_wei=0;//�����λѡ

//ң�ذ���
u8 remote_key=0;
u8 num1=0;
u8 num=0;

//ADC��ѹ����
u16 adcx=0;
u16 adcx1=0;
float temp=0;

//ÿ2msִ��һ��,����ɨ����ʾ��
void TIM2_IRQHandler(void)//TIM3�ж�
{
	static u16 led_t=0;
	
	if(TIM2->SR&0X0001)//����ж�
	{   
        if(++led_t==250)//500ms��˸ 
	    { 
		  led_t=0;
		  LED0=!LED0;
	    }
		if(key_event==2)//RTCʱ��
		{
			switch(smg_wei)
			{	
			   case 0:  smg_duan = smg_num[calendar.hour/10]; break; //ʱ
			   case 1:  smg_duan = smg_num[calendar.hour%10]; break; 
			   case 2:   
			   case 5:  smg_duan = 0x02;                     break; 
			   case 3:  smg_duan = smg_num[calendar.min/10]; break; //��
			   case 4:  smg_duan = smg_num[calendar.min%10]; break; 
			   case 6:  smg_duan = smg_num[calendar.sec/10]; break; //��
			   case 7:  smg_duan = smg_num[calendar.sec%10]; break;    
			}
		}
		else if(key_event==3)//����ң��
		{

			 switch(remote_key)
			 {
					case 104:num1=0x00; num = smg_num[1]; BEEP=0;break; //����'1'
					case 152:num1=0x00;num=smg_num[2];BEEP=0;break;     //����'2'	   
					case 176:num1=0x00;num=smg_num[3];BEEP=0;break;     //����'3'	    
					case 48:num1=0x00;num=smg_num[4];BEEP=0;break;      //����'4'		    
					case 24:num1=0x00;num=smg_num[5];BEEP=0;break;      //����'5'		    
					case 122:num1=0x00;num=smg_num[6];BEEP=0;break;     //����'6'		  
					case 16:num1=0x00;num=smg_num[7];BEEP=0;break;      //����'7'			   					
					case 56:num1=0x00;num=smg_num[8];BEEP=0;break;      //����'8'	 
					case 90:num1=0x00;num=smg_num[9];BEEP=0;break;      //����'9'
					case 66:num1=0x00;num=smg_num[0];BEEP=0;break;      //����'0'
					case 82:num1=0x00;num=smg_num[17];BEEP=0;break;     //����'DELETE'
					case 162:num1= smg_num[1];num = smg_num[0];BEEP =0; break;//����'POWER'    
					case 98:num1= smg_num[1];num = smg_num[1];BEEP =0;  break;//����'UP'
					case 226:num1= smg_num[1];num = smg_num[2];BEEP =0; break;//����'ALIENTEK'
					case 34:num1= smg_num[1];num = smg_num[3];BEEP =0;  break;//����'LEFT'
					case 2:num1= smg_num[1];num = smg_num[4];BEEP =0;   break;//����'PLAY'
					case 194:num1= smg_num[1];num = smg_num[5];BEEP =0; break;//����'RIGHT'
					case 224:num1= smg_num[1];num = smg_num[6];BEEP =0; break;//����'VOL-'
					case 168:num1= smg_num[1];num = smg_num[7];BEEP =0; break;//����'DOWN'
					case 144:num1= smg_num[1];num = smg_num[8];BEEP =0; break;//����'VOL+'		
			 }
			 if(!remote_key)
			 {
				BEEP=1;
			 }
			 switch(smg_wei)
			 {
				 case 0: 
				 case 1:
				 case 2: 
				 case 3: smg_duan = 0xff; break;
				 case 6: if(num1) smg_duan = num1;
						 else     smg_duan = 0x00;
						 break;
				 case 7: smg_duan = num; break;
			 } 
		}
		else if(key_event==4)//ADC��ѹ�ɼ�
		{
			 switch(smg_wei)
			 {
				   //ADC����ֵ
				   case 0:  
							if(adcx>1000)smg_duan = smg_num[adcx/1000];
							else         smg_duan = 0x00;				 
							break;
				   case 1: 	
							if(adcx>100) smg_duan = smg_num[adcx%1000/100];
							else         smg_duan = 0x00;
							break; 
				   case 2:   
							if(adcx>10)  smg_duan = smg_num[adcx%100/10];
							else         smg_duan = 0x00;					
							break;
				   case 3:  
							smg_duan = smg_num[adcx%10];
							break;	   
				   case 4: smg_duan = 0x00; break; 
				   //ADC�����ĵ�ѹֵ
				   case 5:  smg_duan = smg_num[adcx1]|0x01;break;
				   case 6:  smg_duan = smg_num[(u16)temp/100]; break; 
				   case 7:  smg_duan = smg_num[(u16)temp%100/10];break; 
					 
			 }
		}
		if((key_event>1)&&(key_event<5))
		{
			LED_Write_Data(smg_duan,smg_wei);//�������ʾ
			LED_Refresh();//ˢ����ʾ
		
			smg_wei++;
			if(key_event==3) 
			{
				if(smg_wei==4) smg_wei=6;
			}
			if(smg_wei>7) smg_wei=0;		
		}				   	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 
}

//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;   	    
	pdata = pdata; 	
	msg_key=OSMboxCreate((void*)0);	//������Ϣ����(����)
	  	  
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)	
    OSTaskCreate(usb_task,(void *)0,(OS_STK*)&USB_TASK_STK[USB_STK_SIZE-1],USB_TASK_PRIO);//USB������		    
 	OSTaskCreate(adc_task,(void *)0,(OS_STK*)&ADC_TASK_STK[ADC_STK_SIZE-1],ADC_TASK_PRIO);//ADC��ѹ�ɼ�						   
 	OSTaskCreate(leds_task,(void *)0,(OS_STK*)&LEDS_TASK_STK[LEDS_STK_SIZE-1],LEDS_TASK_PRIO);//��ˮLED��ʾ	 				    				   
	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);//��������
    OSTaskCreate(remote_task,(void *)0,(OS_STK*)&REMOTE_TASK_STK[REMOTE_STK_SIZE-1],REMOTE_TASK_PRIO);//ң�ؿ���
	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);//��Ϣ����������			   
 	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

//ADC�������
void adc_task(void *pdata)
{
	while(1)
	{
        if(key_event==4)
		{
			adcx=Get_Adc_Average(ADC_CH9,10);//ADCԭʼֵ 
			temp=(float)adcx*(3.3/4096);//ADC��ѹֵ
			adcx1=temp;
			temp-=adcx1;
			temp*=1000; 
		}
		delay_ms(100);
	}									 
}

#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8	//�ߵ��ֽڽ����궨��
#define led_time 70  //�ӳ�ʱ�� (��λ:ms)
//��ˮ����ʾ����
void leds_task(void *pdata)
{	 
   	u16 LED=0xffff;
	u16 led=0xffff;
	u8 i=0;	
	u8 flag=1;//LED��ʾ��־ 1:������ 0:������
	while(1)
	{
	   if(key_event==1)
	   {
		  LED=0xffff;//LED���¸�ֵ 
		  for(i=0;i<8;i++)
		  {
			  if(flag)
			  {
				LED<<=1;//LED��������						
				GPIOC->BRR = ~LED;
			  }else
			  {
				  LED>>=1;//LED��������
				  led = swap16(LED);
				  GPIOC->BRR = ~led;
			  }
			 delay_ms(led_time);	
		  }
		  LED=0x00ff;//LEDȫ��
		  GPIOC->BSRR = LED;
		  flag=!flag;
	    }
	   delay_ms(15);
	}
}     
//ң��������
void remote_task(void *pdata)
{
	while(1)
	{   
		if(key_event==3)
		{
			remote_key = Remote_Scan();
		}
		delay_ms(10);	 
	}									 
}
//USB����������
void usb_task(void *pdata)
{
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA; 
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
			if(bDeviceState==CONFIGURED) 
			{				
				printf("USB Connected\r\n");//��ʾUSB�����Ѿ����� 
			}
			else
			{
				printf("USB DisConnected\r\n");//��ʾUSB���γ���					
			}				
			Divece_STA=bDeviceState;
		}
		tct++;
		if(tct==200)
		{
			tct=0;
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
//������
void main_task(void *pdata)
{							 
	u32 key=0;	
	u8 err;	  
	u8 i=0;
	u8 oldkey=1;
				 
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2
	key=KEY1_PRES;	
	OSMboxPost(msg_key,(void*)key);//������Ϣ
 	while(1)
	{
		key=(u32)OSMboxPend(msg_key,10,&err);
		if(key) BEEP=1;//�رշ�����
		switch(key)
		{ 
				case KEY0_PRES://LED����ʾ
					 if(oldkey!=key)
					 {
					   oldkey = KEY0_PRES;
					   key_event = KEY0_PRES;
					   OSTaskSuspend(ADC_TASK_PRIO);	//����ADC����.
					   OSTaskSuspend(REMOTE_TASK_PRIO);	//����ң������.
					   TIM3->CR1&=~(0x01);    	        //�رն�ʱ��3(ң�������)	 
					   TIM2->CR1&=~(0x01);    //�رն�ʱ��2(�������ʾ) 
					   for(i=0;i<8;i++)
					   {
							LED_Write_Data(0x00,i);//�������ʾ
							LED_Refresh();//ˢ����ʾ
					   }
					   OSTaskResume(LEDS_TASK_PRIO);//�ָ���ˮ��LED����
					 }
					 break;	
				   
				case KEY1_PRES://����RTCʱ����ʾ
					 if(oldkey!=key)
					 {
						oldkey = KEY1_PRES;
						key_event = KEY1_PRES;
						OSTaskSuspend(ADC_TASK_PRIO);	//����ADC����.
						OSTaskSuspend(REMOTE_TASK_PRIO);//����ң����������. 
						TIM3->CR1&=~(0x01);    	        //�رն�ʱ��3(ң�������) 
						smg_duan=0x00;
						smg_wei=0;
						OSTaskSuspend(LEDS_TASK_PRIO);	//������ˮLED����.
						GPIOC->BSRR = 0xff;//�ر���ˮ��				 
						TIM2->CR1|=0x01; //ʹ�ܶ�ʱ��2(�������ʾ)
					  
					}
					break;	

				case KEY2_PRES://����ң��������	
					 if(oldkey!=key)
					 { 
						oldkey = KEY2_PRES;
						key_event = KEY2_PRES;
						OSTaskSuspend(ADC_TASK_PRIO);//����ADC����.
						OSTaskSuspend(LEDS_TASK_PRIO);//������ˮLED����.
						GPIOC->BSRR = 0xff;//�ر���ˮ��
						TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2(�������ʾ)			
						num1=0x00;
						num=0x00;
						smg_wei=0;
						TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3//ʹ�ܶ�ʱ��3(ң�������)
						OSTaskResume(REMOTE_TASK_PRIO);//�ָ�ң��������
					 }
					 break;
					
				case WKUP_PRES://����ADC�ɼ�
					if(oldkey!=key)	
					{
					   oldkey = WKUP_PRES;
					   key_event = WKUP_PRES;
					   OSTaskSuspend(REMOTE_TASK_PRIO);//����ң����������.
					   TIM3->CR1&=~(0x01);    	       //�رն�ʱ��3(ң�������) 
					   smg_duan=0x00;
					   smg_wei=0;
					   OSTaskSuspend(LEDS_TASK_PRIO);//������ˮLED����.
					   GPIOC->BSRR = 0xff;//�ر���ˮ��
					   OSTaskResume(ADC_TASK_PRIO);//�ָ�ADC����
					   TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2(�������ʾ)
					}				   
					break;
		}  
		delay_ms(10);
				
		
	}
}		   
   		    
//����ɨ������
void key_task(void *pdata)
{	
	u8 key;		    						 
	while(1)
	{
		key=KEY_Scan(0);   
		if(key) OSMboxPost(msg_key,(void*)key);//������Ϣ
 		delay_ms(5);
	}
}

//ϵͳ��ʼ��
void system_init(void)
{
	u8 error=0;
	Stm32_Clock_Init(9);    //ϵͳʱ������
	delay_init(72);	        //��ʱ��ʼ��
	uart_init(72,115200);   //���ڳ�ʼ�� 
	usmart_dev.init(72);	//��ʼ��USMART
	LED_Init();		  	    //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ�� 
	BEEP_Init();            //��������ʼ��
	mem_init();				//��ʼ���ڴ�� 
	LED_SMG_Init();         //����ܳ�ʼ��
	Remote_Init();          //����ң�س�ʼ��
	Adc_Init();		  		//ADC��ʼ��
	TIM2_Init(19,7199);     //2ms��ʱ��ʾ	
	printf("ALIENTEK NANO STM32F1 V1\r\n");
	printf("Copyright(C) 2018-2028\r\n"); 
	printf("HARDWARE: V1.30 ,SOFTWARE: V1.00\r\n");
	printf("CPU:STM32F103RBT6 72Mhz\r\n");
   	printf("FLASH:128KB  SRAM:20KB\r\n"); 
	W25QXX_Init();       //SPI��ʼ��
	if(W25QXX_ReadID()!=W25Q16)//��ⲻ��W25Q16
	{
		printf("Ex Flash Error!!\r\n");
		LED5=0;//SPI FLASH����
		error=1;
	}else
    { 
	    Mass_Memory_Size[0]=2048*1024;//ǰ2M�ֽ� 
	    Mass_Block_Size[0] =512;
	    Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	    printf("Ex Flash: 2048 KB\r\n");
    } 
	printf("RTC Check...\r\n");
	if(RTC_Init())         //RTCʱ�ӳ�ʼ��
	{ 
	  printf("RTC Error!\r\n");
	  LED6=0;//��ʾRTC����
	  error=1;
	}
    else 
	{ 
	  printf("OK\r\n");	
	}	
	AT24CXX_Init();        //IIC��ʼ�� 
	printf("24C02 Check...\r\n");
	if(AT24CXX_Check())		
	{ 
	  printf("24C02 Error!\r\n");
	  LED7=0;//��ʾ24C02����
	  error=1;
	}
	else
	{ 
	  printf("OK\r\n");
	}
	if(!error)//û����
	{
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
		//�������̽�,��ʾ��������
		BEEP=0;
		delay_ms(100);
		BEEP=1; 
	}else//�������
	{
		while(1)
		{
			BEEP=0;
		    delay_ms(500);
		    BEEP=1; 
			delay_ms(500);
		}
	}
}
//������
int main(void)
{	
	system_init();          //ϵͳ��ʼ��
	OSInit();  	 			//��ʼ��UCOSII
  	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	            //OS����
}

