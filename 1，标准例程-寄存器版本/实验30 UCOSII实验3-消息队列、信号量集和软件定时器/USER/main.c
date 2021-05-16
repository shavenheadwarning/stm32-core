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
 ALIENTEK NANO STM32������ʵ��30
 UCOSIIʵ��3-��Ϣ���С��ź������������ʱ�� 
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
 			   
//LED����
//�����������ȼ�
#define LED_TASK_PRIO       			7 
//���������ջ��С
#define LED_STK_SIZE  		    		64
//�����ջ
OS_STK LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *pdata);

//�������ʾ����
//�����������ȼ�
#define SMG_TASK_PRIO       		 	6
//���������ջ��С
#define SMG_STK_SIZE  				    64
//�����ջ	
OS_STK SMG_TASK_STK[SMG_STK_SIZE];
//������
void smg_task(void *pdata);

//������Ϣ��ʾ����
//�����������ȼ�
#define QMSGSHOW_TASK_PRIO    			5 
//���������ջ��С
#define QMSGSHOW_STK_SIZE  		 		64
//�����ջ	
OS_STK QMSGSHOW_TASK_STK[QMSGSHOW_STK_SIZE];
//������
void qmsgshow_task(void *pdata);

//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			4 
//���������ջ��С
#define MAIN_STK_SIZE  					128
//�����ջ	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

//�ź���������
//�����������ȼ�
#define FLAGS_TASK_PRIO       			3 
//���������ջ��С
#define FLAGS_STK_SIZE  		 		64
//�����ջ	
OS_STK FLAGS_TASK_STK[FLAGS_STK_SIZE];
//������
void flags_task(void *pdata);

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
    
OS_EVENT * msg_key;			//���������¼���	  
OS_EVENT * q_msg;			//��Ϣ����
OS_TMR   * tmr1;			//�����ʱ��1
OS_TMR   * tmr2;			//�����ʱ��2
OS_TMR   * tmr3;			//�����ʱ��3
OS_FLAG_GRP * flags_key;	//�����ź�����
void * MsgGrp[256];			//��Ϣ���д洢��ַ,���֧��256����Ϣ

//������������
//0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .,ȫ��
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
u8 smg_duan=0;//����ܶ�ѡ

//�����ʱ��1�Ļص�����	
//ÿ100msִ��һ��,������ʾCPUʹ���ʺ��ڴ�ʹ����		   
void tmr1_callback(OS_TMR *ptmr,void *p_arg) 
{
 	static u16 cpuusage=0;
	static u8 tcnt=0;	    
	if(tcnt==5)
	{
		printf("CPU:%d%%\r\n",cpuusage/5); //��ʾCPUʹ����
		cpuusage=0;
		tcnt=0; 
	}
	cpuusage+=OSCPUUsage;
	tcnt++;	

    printf("MEM:%d%%\r\n",mem_perused());//��ʾ�ڴ�ʹ����	
    printf("Q:%d\r\n\r\n",((OS_Q*)(q_msg->OSEventPtr))->OSQEntries); //��ʾ���е�ǰ�Ĵ�С				   
 }

//�����ʱ��2�Ļص�����				  	   
void tmr2_callback(OS_TMR *ptmr,void *p_arg) 
{	
    smg_duan++;
	if(smg_duan>16) smg_duan=0;
}
//�����ʱ��3�Ļص�����				  	   
void tmr3_callback(OS_TMR *ptmr,void *p_arg) 
{	
	u8* p;	 
	u8 err; 
	static u8 msg_cnt=0;	//msg���	  
	p=mymalloc(13);	//����13���ֽڵ��ڴ�
	if(p)
	{
	 	sprintf((char*)p,"ALIENTEK %03d",msg_cnt);
		msg_cnt++;
		err=OSQPost(q_msg,p);	//���Ͷ���
		if(err!=OS_ERR_NONE) 	//����ʧ��
		{
			myfree(p);	//�ͷ��ڴ�
			OSTmrStop(tmr3,OS_TMR_OPT_NONE,0,&err);	//�ر������ʱ��3
 		}
	}
} 

int main(void)
 {	
	Stm32_Clock_Init(9);    //ϵͳʱ������
	delay_init(72);	        //��ʱ��ʼ��
	uart_init(72,115200);   //���ڳ�ʼ�� 
	LED_Init();		  	    //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ�� 
	mem_init();				//��ʼ���ڴ�� 
	LED_SMG_Init();         //����ܳ�ʼ��
	BEEP_Init();            //��������ʼ��
	OSInit();  	 			//��ʼ��UCOSII
  	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
 }
 
 
//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	u8 err;	    	    
	pdata = pdata; 	
	msg_key=OSMboxCreate((void*)0);		//������Ϣ����
	q_msg=OSQCreate(&MsgGrp[0],256);	//������Ϣ����
 	flags_key=OSFlagCreate(0,&err); 	//�����ź�����		  
	  
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
 	OSTaskCreate(led_task,(void *)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);						   
 	OSTaskCreate(smg_task,(void *)0,(OS_STK*)&SMG_TASK_STK[SMG_STK_SIZE-1],SMG_TASK_PRIO);	 				   
 	OSTaskCreate(qmsgshow_task,(void *)0,(OS_STK*)&QMSGSHOW_TASK_STK[QMSGSHOW_STK_SIZE-1],QMSGSHOW_TASK_PRIO);	 				   
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 				   
 	OSTaskCreate(flags_task,(void *)0,(OS_STK*)&FLAGS_TASK_STK[FLAGS_STK_SIZE-1],FLAGS_TASK_PRIO);	 				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	 				   
 	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}
//LED����
void led_task(void *pdata)
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
//������Ϣ��ʾ����
void qmsgshow_task(void *pdata)
{
	u8 *p;
	u8 err;
	while(1)
	{
		p=OSQPend(q_msg,0,&err);//������Ϣ����
		printf("%s\r\n",p);//���ڴ�ӡ��Ϣ
 		myfree(p);	  
		delay_ms(500);	 
	}									 
}
//������
void main_task(void *pdata)
{							 
	u32 key=0;	
	u8 err;	
 	u8 tmr2sta=1;	//�����ʱ��2����״̬   
 	u8 tmr3sta=0;	//�����ʱ��3����״̬ 
 	tmr1=OSTmrCreate(10,50,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr1_callback,0,"tmr1",&err);		//500msִ��һ��
	tmr2=OSTmrCreate(10,100,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr2_callback,0,"tmr2",&err);		//1000msִ��һ��
	tmr3=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr3_callback,0,"tmr3",&err);		//100msִ��һ��
	OSTmrStart(tmr1,&err);//���������ʱ��1				 
	OSTmrStart(tmr2,&err);//���������ʱ��2				 
 	while(1)
	{
		key=(u32)OSMboxPend(msg_key,10,&err); 
		if((key==KEY0_PRES)||(key==KEY1_PRES)||(key==KEY2_PRES))
 			OSFlagPost(flags_key,1<<(key-1),OS_FLAG_SET,&err);//���ö�Ӧ���ź���Ϊ1
		switch(key)
		{ 
			case KEY0_PRES://�����ʱ��3����	 
				tmr3sta=!tmr3sta;
				if(tmr3sta)
				{
					printf("TMR3 START\r\n");//��ʾ��ʱ��3����	
					OSTmrStart(tmr3,&err);
				}  
				else 
				{
					OSTmrStop(tmr3,OS_TMR_OPT_NONE,0,&err);		//�ر������ʱ��3
					printf("TMR3 STOP\r\n");//��ʾ��ʱ��3�ر���
				}
 				break; 
			case KEY1_PRES://�����ʱ��2����
				tmr2sta=!tmr2sta;
				if(tmr2sta)
				{
					printf("TMR2 START\r\n");//��ʾ��ʱ��2����	
					OSTmrStart(tmr2,&err);			  	//���������ʱ��2
				}
				else 
				{		    		    
  					OSTmrStop(tmr2,OS_TMR_OPT_NONE,0,&err);	//�ر������ʱ��2
                    printf("TMR2 STOP\r\n");//��ʾ��ʱ��2�ر���	
				}
				break;	
				
			case KEY2_PRES://����DS1
				 LED1=!LED1;
				 break;
		}  
		delay_ms(10);
	}
}		   
//�ź�������������
void flags_task(void *pdata)
{	
	u16 flags;	
	u8 err;	    						 
	while(1)
	{
		flags=OSFlagPend(flags_key,0X001F,OS_FLAG_WAIT_SET_ANY,0,&err);//�ȴ��ź���
		if(flags&0X0001) printf("KEY0 DOWN\r\n");
		else if(flags&0X0002) printf("KEY1 DOWN\r\n");
		else if(flags&0X0004) printf("KEY2 DOWN\r\n");
        BEEP=0;
		delay_ms(50);
		BEEP=1;
		OSFlagPost(flags_key,0X0007,OS_FLAG_CLR,&err);//ȫ���ź�������
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



