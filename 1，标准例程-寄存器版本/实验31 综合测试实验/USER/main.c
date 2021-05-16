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
 ALIENTEK NANO STM32开发板实验31
 综合测试实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/ 

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	

//ADC任务
//设置任务优先级
#define ADC_TASK_PRIO       			7 
//设置任务堆栈大小
#define ADC_STK_SIZE  		    		64
//任务堆栈
OS_STK ADC_TASK_STK[ADC_STK_SIZE];
//任务函数
void adc_task(void *pdata);


//遥控控制任务
//设置任务优先级
#define REMOTE_TASK_PRIO    			6 
//设置任务堆栈大小
#define REMOTE_STK_SIZE  		 		64
//任务堆栈	
OS_STK REMOTE_TASK_STK[REMOTE_STK_SIZE];
//任务函数
void remote_task(void *pdata);

//流水灯任务
//设置任务优先级
#define LEDS_TASK_PRIO       		 	5
//设置任务堆栈大小
#define LEDS_STK_SIZE  				    64
//任务堆栈	
OS_STK LEDS_TASK_STK[LEDS_STK_SIZE];
//任务函数
void leds_task(void *pdata);

//主任务
//设置任务优先级
#define MAIN_TASK_PRIO       			4 
//设置任务堆栈大小
#define MAIN_STK_SIZE  					128
//任务堆栈	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

//USB读卡器任务
//设置任务优先级
#define USB_TASK_PRIO       			3
//设置任务堆栈大小
#define USB_STK_SIZE  		    		128
//任务堆栈
OS_STK USB_TASK_STK[USB_STK_SIZE];
//任务函数
void usb_task(void *pdata);

//按键扫描任务
//设置任务优先级
#define KEY_TASK_PRIO       			2 
//设置任务堆栈大小
#define KEY_STK_SIZE  					64
//任务堆栈	
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
void key_task(void *pdata);
//////////////////////////////////////////////////////////////////////////////
    
OS_EVENT * msg_key;	//按键邮箱事件块	 

u8 key_event=0;//按键事件 1:流水灯 2:RTC时钟 3:红外遥控 4:ADC采集 5:USB事件

//共阴数字数组
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,全灭
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
u8 smg_duan=0;//数码管段选
u8 smg_wei=0;//数码管位选

//遥控按键
u8 remote_key=0;
u8 num1=0;
u8 num=0;

//ADC电压采样
u16 adcx=0;
u16 adcx1=0;
float temp=0;

//每2ms执行一次,用于扫描显示管
void TIM2_IRQHandler(void)//TIM3中断
{
	static u16 led_t=0;
	
	if(TIM2->SR&0X0001)//溢出中断
	{   
        if(++led_t==250)//500ms闪烁 
	    { 
		  led_t=0;
		  LED0=!LED0;
	    }
		if(key_event==2)//RTC时钟
		{
			switch(smg_wei)
			{	
			   case 0:  smg_duan = smg_num[calendar.hour/10]; break; //时
			   case 1:  smg_duan = smg_num[calendar.hour%10]; break; 
			   case 2:   
			   case 5:  smg_duan = 0x02;                     break; 
			   case 3:  smg_duan = smg_num[calendar.min/10]; break; //分
			   case 4:  smg_duan = smg_num[calendar.min%10]; break; 
			   case 6:  smg_duan = smg_num[calendar.sec/10]; break; //秒
			   case 7:  smg_duan = smg_num[calendar.sec%10]; break;    
			}
		}
		else if(key_event==3)//红外遥控
		{

			 switch(remote_key)
			 {
					case 104:num1=0x00; num = smg_num[1]; BEEP=0;break; //按键'1'
					case 152:num1=0x00;num=smg_num[2];BEEP=0;break;     //按键'2'	   
					case 176:num1=0x00;num=smg_num[3];BEEP=0;break;     //按键'3'	    
					case 48:num1=0x00;num=smg_num[4];BEEP=0;break;      //按键'4'		    
					case 24:num1=0x00;num=smg_num[5];BEEP=0;break;      //按键'5'		    
					case 122:num1=0x00;num=smg_num[6];BEEP=0;break;     //按键'6'		  
					case 16:num1=0x00;num=smg_num[7];BEEP=0;break;      //按键'7'			   					
					case 56:num1=0x00;num=smg_num[8];BEEP=0;break;      //按键'8'	 
					case 90:num1=0x00;num=smg_num[9];BEEP=0;break;      //按键'9'
					case 66:num1=0x00;num=smg_num[0];BEEP=0;break;      //按键'0'
					case 82:num1=0x00;num=smg_num[17];BEEP=0;break;     //按键'DELETE'
					case 162:num1= smg_num[1];num = smg_num[0];BEEP =0; break;//按键'POWER'    
					case 98:num1= smg_num[1];num = smg_num[1];BEEP =0;  break;//按键'UP'
					case 226:num1= smg_num[1];num = smg_num[2];BEEP =0; break;//按键'ALIENTEK'
					case 34:num1= smg_num[1];num = smg_num[3];BEEP =0;  break;//按键'LEFT'
					case 2:num1= smg_num[1];num = smg_num[4];BEEP =0;   break;//按键'PLAY'
					case 194:num1= smg_num[1];num = smg_num[5];BEEP =0; break;//按键'RIGHT'
					case 224:num1= smg_num[1];num = smg_num[6];BEEP =0; break;//按键'VOL-'
					case 168:num1= smg_num[1];num = smg_num[7];BEEP =0; break;//按键'DOWN'
					case 144:num1= smg_num[1];num = smg_num[8];BEEP =0; break;//按键'VOL+'		
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
		else if(key_event==4)//ADC电压采集
		{
			 switch(smg_wei)
			 {
				   //ADC采样值
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
				   //ADC换算后的电压值
				   case 5:  smg_duan = smg_num[adcx1]|0x01;break;
				   case 6:  smg_duan = smg_num[(u16)temp/100]; break; 
				   case 7:  smg_duan = smg_num[(u16)temp%100/10];break; 
					 
			 }
		}
		if((key_event>1)&&(key_event<5))
		{
			LED_Write_Data(smg_duan,smg_wei);//数码管显示
			LED_Refresh();//刷新显示
		
			smg_wei++;
			if(key_event==3) 
			{
				if(smg_wei==4) smg_wei=6;
			}
			if(smg_wei>7) smg_wei=0;		
		}				   	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 
}

//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;   	    
	pdata = pdata; 	
	msg_key=OSMboxCreate((void*)0);	//创建消息邮箱(按键)
	  	  
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)	
    OSTaskCreate(usb_task,(void *)0,(OS_STK*)&USB_TASK_STK[USB_STK_SIZE-1],USB_TASK_PRIO);//USB读卡器		    
 	OSTaskCreate(adc_task,(void *)0,(OS_STK*)&ADC_TASK_STK[ADC_STK_SIZE-1],ADC_TASK_PRIO);//ADC电压采集						   
 	OSTaskCreate(leds_task,(void *)0,(OS_STK*)&LEDS_TASK_STK[LEDS_STK_SIZE-1],LEDS_TASK_PRIO);//流水LED显示	 				    				   
	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);//按键任务
    OSTaskCreate(remote_task,(void *)0,(OS_STK*)&REMOTE_TASK_STK[REMOTE_STK_SIZE-1],REMOTE_TASK_PRIO);//遥控控制
	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);//消息邮箱检测任务			   
 	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}

//ADC检测任务
void adc_task(void *pdata)
{
	while(1)
	{
        if(key_event==4)
		{
			adcx=Get_Adc_Average(ADC_CH9,10);//ADC原始值 
			temp=(float)adcx*(3.3/4096);//ADC电压值
			adcx1=temp;
			temp-=adcx1;
			temp*=1000; 
		}
		delay_ms(100);
	}									 
}

#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8	//高低字节交换宏定义
#define led_time 70  //延迟时间 (单位:ms)
//流水灯显示任务
void leds_task(void *pdata)
{	 
   	u16 LED=0xffff;
	u16 led=0xffff;
	u8 i=0;	
	u8 flag=1;//LED显示标志 1:上往下 0:下往上
	while(1)
	{
	   if(key_event==1)
	   {
		  LED=0xffff;//LED重新赋值 
		  for(i=0;i<8;i++)
		  {
			  if(flag)
			  {
				LED<<=1;//LED从上往下						
				GPIOC->BRR = ~LED;
			  }else
			  {
				  LED>>=1;//LED从下往上
				  led = swap16(LED);
				  GPIOC->BRR = ~led;
			  }
			 delay_ms(led_time);	
		  }
		  LED=0x00ff;//LED全灭
		  GPIOC->BSRR = LED;
		  flag=!flag;
	    }
	   delay_ms(15);
	}
}     
//遥控器任务
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
//USB读卡器任务
void usb_task(void *pdata)
{
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA; 
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
			if(bDeviceState==CONFIGURED) 
			{				
				printf("USB Connected\r\n");//提示USB连接已经建立 
			}
			else
			{
				printf("USB DisConnected\r\n");//提示USB被拔出了					
			}				
			Divece_STA=bDeviceState;
		}
		tct++;
		if(tct==200)
		{
			tct=0;
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
//主任务
void main_task(void *pdata)
{							 
	u32 key=0;	
	u8 err;	  
	u8 i=0;
	u8 oldkey=1;
				 
	TIM2->CR1|=0x01;    //使能定时器2
	key=KEY1_PRES;	
	OSMboxPost(msg_key,(void*)key);//发送消息
 	while(1)
	{
		key=(u32)OSMboxPend(msg_key,10,&err);
		if(key) BEEP=1;//关闭蜂鸣器
		switch(key)
		{ 
				case KEY0_PRES://LED灯显示
					 if(oldkey!=key)
					 {
					   oldkey = KEY0_PRES;
					   key_event = KEY0_PRES;
					   OSTaskSuspend(ADC_TASK_PRIO);	//挂起ADC任务.
					   OSTaskSuspend(REMOTE_TASK_PRIO);	//挂起遥控任务.
					   TIM3->CR1&=~(0x01);    	        //关闭定时器3(遥控器检测)	 
					   TIM2->CR1&=~(0x01);    //关闭定时器2(数码管显示) 
					   for(i=0;i<8;i++)
					   {
							LED_Write_Data(0x00,i);//数码管显示
							LED_Refresh();//刷新显示
					   }
					   OSTaskResume(LEDS_TASK_PRIO);//恢复流水灯LED任务
					 }
					 break;	
				   
				case KEY1_PRES://启动RTC时钟显示
					 if(oldkey!=key)
					 {
						oldkey = KEY1_PRES;
						key_event = KEY1_PRES;
						OSTaskSuspend(ADC_TASK_PRIO);	//挂起ADC任务.
						OSTaskSuspend(REMOTE_TASK_PRIO);//挂起遥控任务任务. 
						TIM3->CR1&=~(0x01);    	        //关闭定时器3(遥控器检测) 
						smg_duan=0x00;
						smg_wei=0;
						OSTaskSuspend(LEDS_TASK_PRIO);	//挂起流水LED任务.
						GPIOC->BSRR = 0xff;//关闭流水灯				 
						TIM2->CR1|=0x01; //使能定时器2(数码管显示)
					  
					}
					break;	

				case KEY2_PRES://启动遥控器控制	
					 if(oldkey!=key)
					 { 
						oldkey = KEY2_PRES;
						key_event = KEY2_PRES;
						OSTaskSuspend(ADC_TASK_PRIO);//挂起ADC任务.
						OSTaskSuspend(LEDS_TASK_PRIO);//挂起流水LED任务.
						GPIOC->BSRR = 0xff;//关闭流水灯
						TIM2->CR1|=0x01;    //使能定时器2(数码管显示)			
						num1=0x00;
						num=0x00;
						smg_wei=0;
						TIM3->CR1|=0x01;    //使能定时器3//使能定时器3(遥控器检测)
						OSTaskResume(REMOTE_TASK_PRIO);//恢复遥控器任务
					 }
					 break;
					
				case WKUP_PRES://启动ADC采集
					if(oldkey!=key)	
					{
					   oldkey = WKUP_PRES;
					   key_event = WKUP_PRES;
					   OSTaskSuspend(REMOTE_TASK_PRIO);//挂起遥控任务任务.
					   TIM3->CR1&=~(0x01);    	       //关闭定时器3(遥控器检测) 
					   smg_duan=0x00;
					   smg_wei=0;
					   OSTaskSuspend(LEDS_TASK_PRIO);//挂起流水LED任务.
					   GPIOC->BSRR = 0xff;//关闭流水灯
					   OSTaskResume(ADC_TASK_PRIO);//恢复ADC任务
					   TIM2->CR1|=0x01;    //使能定时器2(数码管显示)
					}				   
					break;
		}  
		delay_ms(10);
				
		
	}
}		   
   		    
//按键扫描任务
void key_task(void *pdata)
{	
	u8 key;		    						 
	while(1)
	{
		key=KEY_Scan(0);   
		if(key) OSMboxPost(msg_key,(void*)key);//发送消息
 		delay_ms(5);
	}
}

//系统初始化
void system_init(void)
{
	u8 error=0;
	Stm32_Clock_Init(9);    //系统时钟设置
	delay_init(72);	        //延时初始化
	uart_init(72,115200);   //串口初始化 
	usmart_dev.init(72);	//初始化USMART
	LED_Init();		  	    //初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化 
	BEEP_Init();            //蜂鸣器初始化
	mem_init();				//初始化内存池 
	LED_SMG_Init();         //数码管初始化
	Remote_Init();          //红外遥控初始化
	Adc_Init();		  		//ADC初始化
	TIM2_Init(19,7199);     //2ms定时显示	
	printf("ALIENTEK NANO STM32F1 V1\r\n");
	printf("Copyright(C) 2018-2028\r\n"); 
	printf("HARDWARE: V1.30 ,SOFTWARE: V1.00\r\n");
	printf("CPU:STM32F103RBT6 72Mhz\r\n");
   	printf("FLASH:128KB  SRAM:20KB\r\n"); 
	W25QXX_Init();       //SPI初始化
	if(W25QXX_ReadID()!=W25Q16)//检测不到W25Q16
	{
		printf("Ex Flash Error!!\r\n");
		LED5=0;//SPI FLASH错误
		error=1;
	}else
    { 
	    Mass_Memory_Size[0]=2048*1024;//前2M字节 
	    Mass_Block_Size[0] =512;
	    Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	    printf("Ex Flash: 2048 KB\r\n");
    } 
	printf("RTC Check...\r\n");
	if(RTC_Init())         //RTC时钟初始化
	{ 
	  printf("RTC Error!\r\n");
	  LED6=0;//提示RTC错误
	  error=1;
	}
    else 
	{ 
	  printf("OK\r\n");	
	}	
	AT24CXX_Init();        //IIC初始化 
	printf("24C02 Check...\r\n");
	if(AT24CXX_Check())		
	{ 
	  printf("24C02 Error!\r\n");
	  LED7=0;//提示24C02错误
	  error=1;
	}
	else
	{ 
	  printf("OK\r\n");
	}
	if(!error)//没错误
	{
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
		//蜂鸣器短叫,提示正常启动
		BEEP=0;
		delay_ms(100);
		BEEP=1; 
	}else//外设错误
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
//主函数
int main(void)
{	
	system_init();          //系统初始化
	OSInit();  	 			//初始化UCOSII
  	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	            //OS启动
}

