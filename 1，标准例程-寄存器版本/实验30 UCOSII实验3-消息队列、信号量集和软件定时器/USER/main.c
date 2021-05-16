#include "led.h"
#include "delay.h"
#include "sys.h"
#include "includes.h" 
#include "key.h"
#include "usart.h"
#include "malloc.h"	
#include "smg.h"
#include "beep.h"

/************************************************
 ALIENTEK NANO STM32开发板实验30
 UCOSII实验3-消息队列、信号量集和软件定时器 
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
 			   
//LED任务
//设置任务优先级
#define LED_TASK_PRIO       			7 
//设置任务堆栈大小
#define LED_STK_SIZE  		    		64
//任务堆栈
OS_STK LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);

//数码管显示任务
//设置任务优先级
#define SMG_TASK_PRIO       		 	6
//设置任务堆栈大小
#define SMG_STK_SIZE  				    64
//任务堆栈	
OS_STK SMG_TASK_STK[SMG_STK_SIZE];
//任务函数
void smg_task(void *pdata);

//队列消息显示任务
//设置任务优先级
#define QMSGSHOW_TASK_PRIO    			5 
//设置任务堆栈大小
#define QMSGSHOW_STK_SIZE  		 		64
//任务堆栈	
OS_STK QMSGSHOW_TASK_STK[QMSGSHOW_STK_SIZE];
//任务函数
void qmsgshow_task(void *pdata);

//主任务
//设置任务优先级
#define MAIN_TASK_PRIO       			4 
//设置任务堆栈大小
#define MAIN_STK_SIZE  					128
//任务堆栈	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

//信号量集任务
//设置任务优先级
#define FLAGS_TASK_PRIO       			3 
//设置任务堆栈大小
#define FLAGS_STK_SIZE  		 		64
//任务堆栈	
OS_STK FLAGS_TASK_STK[FLAGS_STK_SIZE];
//任务函数
void flags_task(void *pdata);

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
    
OS_EVENT * msg_key;			//按键邮箱事件块	  
OS_EVENT * q_msg;			//消息队列
OS_TMR   * tmr1;			//软件定时器1
OS_TMR   * tmr2;			//软件定时器2
OS_TMR   * tmr3;			//软件定时器3
OS_FLAG_GRP * flags_key;	//按键信号量集
void * MsgGrp[256];			//消息队列存储地址,最大支持256个消息

//共阴数字数组
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,全灭
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
u8 smg_duan=0;//数码管段选

//软件定时器1的回调函数	
//每100ms执行一次,用于显示CPU使用率和内存使用率		   
void tmr1_callback(OS_TMR *ptmr,void *p_arg) 
{
 	static u16 cpuusage=0;
	static u8 tcnt=0;	    
	if(tcnt==5)
	{
		printf("CPU:%d%%\r\n",cpuusage/5); //显示CPU使用率
		cpuusage=0;
		tcnt=0; 
	}
	cpuusage+=OSCPUUsage;
	tcnt++;	

    printf("MEM:%d%%\r\n",mem_perused());//显示内存使用率	
    printf("Q:%d\r\n\r\n",((OS_Q*)(q_msg->OSEventPtr))->OSQEntries); //显示队列当前的大小				   
 }

//软件定时器2的回调函数				  	   
void tmr2_callback(OS_TMR *ptmr,void *p_arg) 
{	
    smg_duan++;
	if(smg_duan>16) smg_duan=0;
}
//软件定时器3的回调函数				  	   
void tmr3_callback(OS_TMR *ptmr,void *p_arg) 
{	
	u8* p;	 
	u8 err; 
	static u8 msg_cnt=0;	//msg编号	  
	p=mymalloc(13);	//申请13个字节的内存
	if(p)
	{
	 	sprintf((char*)p,"ALIENTEK %03d",msg_cnt);
		msg_cnt++;
		err=OSQPost(q_msg,p);	//发送队列
		if(err!=OS_ERR_NONE) 	//发送失败
		{
			myfree(p);	//释放内存
			OSTmrStop(tmr3,OS_TMR_OPT_NONE,0,&err);	//关闭软件定时器3
 		}
	}
} 

int main(void)
 {	
	Stm32_Clock_Init(9);    //系统时钟设置
	delay_init(72);	        //延时初始化
	uart_init(72,115200);   //串口初始化 
	LED_Init();		  	    //初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化 
	mem_init();				//初始化内存池 
	LED_SMG_Init();         //数码管初始化
	BEEP_Init();            //蜂鸣器初始化
	OSInit();  	 			//初始化UCOSII
  	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	
 }
 
 
//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	u8 err;	    	    
	pdata = pdata; 	
	msg_key=OSMboxCreate((void*)0);		//创建消息邮箱
	q_msg=OSQCreate(&MsgGrp[0],256);	//创建消息队列
 	flags_key=OSFlagCreate(0,&err); 	//创建信号量集		  
	  
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
 	OSTaskCreate(led_task,(void *)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);						   
 	OSTaskCreate(smg_task,(void *)0,(OS_STK*)&SMG_TASK_STK[SMG_STK_SIZE-1],SMG_TASK_PRIO);	 				   
 	OSTaskCreate(qmsgshow_task,(void *)0,(OS_STK*)&QMSGSHOW_TASK_STK[QMSGSHOW_STK_SIZE-1],QMSGSHOW_TASK_PRIO);	 				   
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 				   
 	OSTaskCreate(flags_task,(void *)0,(OS_STK*)&FLAGS_TASK_STK[FLAGS_STK_SIZE-1],FLAGS_TASK_PRIO);	 				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	 				   
 	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}
//LED任务
void led_task(void *pdata)
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
//队列消息显示任务
void qmsgshow_task(void *pdata)
{
	u8 *p;
	u8 err;
	while(1)
	{
		p=OSQPend(q_msg,0,&err);//请求消息队列
		printf("%s\r\n",p);//串口打印消息
 		myfree(p);	  
		delay_ms(500);	 
	}									 
}
//主任务
void main_task(void *pdata)
{							 
	u32 key=0;	
	u8 err;	
 	u8 tmr2sta=1;	//软件定时器2开关状态   
 	u8 tmr3sta=0;	//软件定时器3开关状态 
 	tmr1=OSTmrCreate(10,50,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr1_callback,0,"tmr1",&err);		//500ms执行一次
	tmr2=OSTmrCreate(10,100,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr2_callback,0,"tmr2",&err);		//1000ms执行一次
	tmr3=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr3_callback,0,"tmr3",&err);		//100ms执行一次
	OSTmrStart(tmr1,&err);//启动软件定时器1				 
	OSTmrStart(tmr2,&err);//启动软件定时器2				 
 	while(1)
	{
		key=(u32)OSMboxPend(msg_key,10,&err); 
		if((key==KEY0_PRES)||(key==KEY1_PRES)||(key==KEY2_PRES))
 			OSFlagPost(flags_key,1<<(key-1),OS_FLAG_SET,&err);//设置对应的信号量为1
		switch(key)
		{ 
			case KEY0_PRES://软件定时器3开关	 
				tmr3sta=!tmr3sta;
				if(tmr3sta)
				{
					printf("TMR3 START\r\n");//提示定时器3打开了	
					OSTmrStart(tmr3,&err);
				}  
				else 
				{
					OSTmrStop(tmr3,OS_TMR_OPT_NONE,0,&err);		//关闭软件定时器3
					printf("TMR3 STOP\r\n");//提示定时器3关闭了
				}
 				break; 
			case KEY1_PRES://软件定时器2开关
				tmr2sta=!tmr2sta;
				if(tmr2sta)
				{
					printf("TMR2 START\r\n");//提示定时器2打开了	
					OSTmrStart(tmr2,&err);			  	//开启软件定时器2
				}
				else 
				{		    		    
  					OSTmrStop(tmr2,OS_TMR_OPT_NONE,0,&err);	//关闭软件定时器2
                    printf("TMR2 STOP\r\n");//提示定时器2关闭了	
				}
				break;	
				
			case KEY2_PRES://控制DS1
				 LED1=!LED1;
				 break;
		}  
		delay_ms(10);
	}
}		   
//信号量集处理任务
void flags_task(void *pdata)
{	
	u16 flags;	
	u8 err;	    						 
	while(1)
	{
		flags=OSFlagPend(flags_key,0X001F,OS_FLAG_WAIT_SET_ANY,0,&err);//等待信号量
		if(flags&0X0001) printf("KEY0 DOWN\r\n");
		else if(flags&0X0002) printf("KEY1 DOWN\r\n");
		else if(flags&0X0004) printf("KEY2 DOWN\r\n");
        BEEP=0;
		delay_ms(50);
		BEEP=1;
		OSFlagPost(flags_key,0X0007,OS_FLAG_CLR,&err);//全部信号量清零
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



