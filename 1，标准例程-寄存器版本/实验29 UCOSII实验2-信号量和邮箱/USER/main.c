#include "led.h"
#include "delay.h"
#include "sys.h"
#include "includes.h" 			 	
#include "key.h"
#include "usart.h"
#include "smg.h"

/************************************************
 ALIENTEK NANO STM32������ʵ��29
 UCOSIIʵ��2-�ź���������  
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
 			   
//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO       			7 
//���������ջ��С
#define LED0_STK_SIZE  		    		64
//�����ջ	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *pdata);

//�������ʾ����
//�����������ȼ�
#define SMG_TASK_PRIO       		 	6
//���������ջ��С
#define SMG_STK_SIZE  				    64
//�����ջ	
OS_STK SMG_TASK_STK[SMG_STK_SIZE];
//������
void smg_task(void *pdata);

//LED1����
//�����������ȼ�
#define LED1_TASK_PRIO       			5 
//���������ջ��С
#define LED1_STK_SIZE  					64
//�����ջ	
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *pdata);

//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			4 
//���������ջ��С
#define MAIN_STK_SIZE  					128
//�����ջ	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

//����ɨ������
//�����������ȼ�
#define KEY_TASK_PRIO       			3 
//���������ջ��С
#define KEY_STK_SIZE  					64
//�����ջ	
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//������
void key_task(void *pdata);
//////////////////////////////////////////////////////////////////////////////
OS_EVENT * msg_key;			//���������¼���ָ��
OS_EVENT * sem_led1;		//LED1�ź���ָ��	 	  
 									
//������������
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
u8 smg_duan=0;//����ܶ�ѡ

 int main(void)
 {	
	Stm32_Clock_Init(9);    //ϵͳʱ������
	delay_init(72);	        //��ʱ��ʼ��
	uart_init(72,115200);   //���ڳ�ʼ�� 
	LED_Init();		  	    //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ��
	LED_SMG_Init();         //����ܳ�ʼ��
  	OSInit();  	 			//��ʼ��UCOSII
  	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
 }
 
 
 //��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 		  
	msg_key=OSMboxCreate((void*)0);	//������Ϣ����
	sem_led1=OSSemCreate(0);		//�����ź���		 			  
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(smg_task,(void *)0,(OS_STK*)&SMG_TASK_STK[SMG_STK_SIZE-1],SMG_TASK_PRIO);	 				   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	 				   
 	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}	  

//LED0����
void led0_task(void *pdata)
{
	u8 t;
	while(1)
	{
		t++;
		delay_ms(10);
		if(t==8)LED0=1;	//LED0��
		if(t==100)		//LED0��
		{
			t=0;
			LED0=0;
		}
	}									 
}	   

//LED1����
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
//�������ʾ����
void smg_task(void *pdata)
{	  	
	while(1)
	{		
		LED_Write_Data(smg_num[smg_duan],7);//�������ʾ
	    LED_Refresh();//ˢ����ʾ
        delay_ms(10);			
	}
}
//������
void main_task(void *pdata)
{							 
	u32 key=0;	
	u8 err;					 
	while(1)
	{
		key=(u32)OSMboxPend(msg_key,10,&err);   
		switch(key)
		{ 
			case KEY0_PRES://�����ź���
				 OSSemPost(sem_led1);
				 break;
			case KEY1_PRES://�������ʾ��1
			     smg_duan++;
			     if(smg_duan==17) smg_duan=0;
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
		if(key)OSMboxPost(msg_key,(void*)key);//������Ϣ
 		delay_ms(10);
	}
}





