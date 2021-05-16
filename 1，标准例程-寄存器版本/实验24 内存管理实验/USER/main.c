#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "malloc.h" 
#include "usmart.h" 
/************************************************
 ALIENTEK NANO STM32������ʵ��24
 �ڴ����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 int main(void)
 { 
	u8 key;		 
 	u8 i=0;	    
	u8 *p=0;
	u8 *tp=0;
	u8 paddr[18];			//���P Addr:+p��ַ��ASCIIֵ 

	Stm32_Clock_Init(9);    //ϵͳʱ������
	delay_init(72);	        //��ʱ��ʼ��
	uart_init(72,115200);     //���ڳ�ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	usmart_dev.init(72);	//��ʼ��USMART	
 	KEY_Init();				//������ʼ��   
	mem_init();				//��ʼ���ڴ��	    
	printf("NANO STM32\r\n");
	printf("MALLOC TEST\r\n");
    printf("KEY0:Malloc\r\n");
    printf("KEY1:Write Data\r\n");
	printf("KEY2:Free\r\n");
  	while(1)
	{	
		key=KEY_Scan(0);//��֧������	
		switch(key)
		{
			case 0:		//û�а�������	
				break;
			case KEY0_PRES:	 //KEY0����
				p=mymalloc(2048);//����2K�ֽ�
				if(p!=NULL)sprintf((char*)p,"Memory Malloc Test%03d",i);//��pд��һЩ����
				break;
			case KEY1_PRES:	 //KEY1����	   
				if(p!=NULL)
				{
					sprintf((char*)p,"Memory Malloc Test%03d",i);//������ʾ���� 	 
					printf("%s\r\n",p);//��ʾP������
				}
				break;
			case KEY2_PRES:	//KEY2����	  
				myfree(p);	//�ͷ��ڴ�
				p=0;		//ָ��յ�ַ
				break; 
		}
		if(tp!=p)
		{
			tp=p;
			printf("\r\nSRAM USED:%d%%\r\n",mem_perused());//��ʾ�ڴ�ʹ����
			sprintf((char*)paddr,"P Addr:0X%08X",(u32)tp);
			printf("%s\r\n",paddr);//��ʾp�ĵ�ַ
			if(p) printf("%s\r\n",p);//��ʾP������

		}
		delay_ms(10);   
		i++;
		if((i%20)==0)//DS0��˸.
		{
 			LED0=!LED0;
 		}
	}	 	   
}




