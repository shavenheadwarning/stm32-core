#include "led.h"
#include "delay.h"
#include "sys.h"
#include "includes.h" 			 	
#include "key.h"
#include "usart.h"
#include "smg.h"

/************************************************
 ALIENTEK NANO STM32开发板实验29
 UCOSII实验2-信号量和邮箱  
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
 			   
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO       			7 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		64
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);

//数码管显示任务
//设置任务优先级
#define SMG_TASK_PRIO       		 	6
//设置任务堆栈大小
#define SMG_STK_SIZE  				    64
//任务堆栈	
OS_STK SMG_TASK_STK[SMG_STK_SIZE];
//任务函数
void smg_task(void *pdata);

//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO       			5 
//设置任务堆栈大小
#define LED1_STK_SIZE  					64
//任务堆栈	
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *pdata);

//主任务
//设置任务优先级
#define MAIN_TASK_PRIO       			4 
//设置任务堆栈大小
#define MAIN_STK_SIZE  					128
//任务堆栈	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

//按键扫描任务
//设置任务优先级
#define KEY_TASK_PRIO       			3 
//设置任务堆栈大小
#define KEY_STK_SIZE  					64
//任务堆栈	
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
void key_task(void *pdata);
//////////////////////////////////////////////////////////////////////////////
OS_EVENT * msg_key;			//按键邮箱事件块指针
OS_EVENT * sem_led1;		//LED1信号量指针	 	  
 									
//共阴数字数组
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,全灭
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
u8 smg_duan=0;//数码管段选

 int main(void)
 {	
	Stm32_Clock_Init(9);    //系统时钟设置
	delay_init(72);	        //延时初始化
	uart_init(72,115200);   //串口初始化 
	LED_Init();		  	    //初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化
	LED_SMG_Init();         //数码管初始化
  	OSInit();  	 			//初始化UCOSII
  	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	
 }
 
 
 //开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 		  
	msg_key=OSMboxCreate((void*)0);	//创建消息邮箱
	sem_led1=OSSemCreate(0);		//创建信号量		 			  
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(smg_task,(void *)0,(OS_STK*)&SMG_TASK_STK[SMG_STK_SIZE-1],SMG_TASK_PRIO);	 				   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	 				   
 	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}	  

//LED0任务
void led0_task(void *pdata)
{
	u8 t;
	while(1)
	{
		t++;
		delay_ms(10);
		if(t==8)LED0=1;	//LED0灭
		if(t==100)		//LED0亮
		{
			t=0;
			LED0=0;
		}
	}									 
}	   

//LED1任务
void led1_task(void *pdata)
{
	u8 err;
	while(1)
	{
		OSSemPend(sem_led1,0,&err);
		LED1=0;
		delay_ms(200);
		LED1=1;
		delay_ms(800);
	}									 
}
//数码管显示任务
void smg_task(void *pdata)
{	  	
	while(1)
	{		
		LED_Write_Data(smg_num[smg_duan],7);//数码管显示
	    LED_Refresh();//刷新显示
        delay_ms(10);			
	}
}
//主任务
void main_task(void *pdata)
{							 
	u32 key=0;	
	u8 err;					 
	while(1)
	{
		key=(u32)OSMboxPend(msg_key,10,&err);   
		switch(key)
		{ 
			case KEY0_PRES://发送信号量
				 OSSemPost(sem_led1);
				 break;
			case KEY1_PRES://数码管显示加1
			     smg_duan++;
			     if(smg_duan==17) smg_duan=0;
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
		if(key)OSMboxPost(msg_key,(void*)key);//发送消息
 		delay_ms(10);
	}
}





