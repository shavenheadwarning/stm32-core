#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "malloc.h" 
#include "usmart.h" 
/************************************************
 ALIENTEK NANO STM32开发板实验24
 内存管理实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 int main(void)
 { 
	u8 key;		 
 	u8 i=0;	    
	u8 *p=0;
	u8 *tp=0;
	u8 paddr[18];			//存放P Addr:+p地址的ASCII值 

	Stm32_Clock_Init(9);    //系统时钟设置
	delay_init(72);	        //延时初始化
	uart_init(72,115200);     //串口初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口
	usmart_dev.init(72);	//初始化USMART	
 	KEY_Init();				//按键初始化   
	mem_init();				//初始化内存池	    
	printf("NANO STM32\r\n");
	printf("MALLOC TEST\r\n");
    printf("KEY0:Malloc\r\n");
    printf("KEY1:Write Data\r\n");
	printf("KEY2:Free\r\n");
  	while(1)
	{	
		key=KEY_Scan(0);//不支持连按	
		switch(key)
		{
			case 0:		//没有按键按下	
				break;
			case KEY0_PRES:	 //KEY0按下
				p=mymalloc(2048);//申请2K字节
				if(p!=NULL)sprintf((char*)p,"Memory Malloc Test%03d",i);//向p写入一些内容
				break;
			case KEY1_PRES:	 //KEY1按下	   
				if(p!=NULL)
				{
					sprintf((char*)p,"Memory Malloc Test%03d",i);//更新显示内容 	 
					printf("%s\r\n",p);//显示P的内容
				}
				break;
			case KEY2_PRES:	//KEY2按下	  
				myfree(p);	//释放内存
				p=0;		//指向空地址
				break; 
		}
		if(tp!=p)
		{
			tp=p;
			printf("\r\nSRAM USED:%d%%\r\n",mem_perused());//显示内存使用率
			sprintf((char*)paddr,"P Addr:0X%08X",(u32)tp);
			printf("%s\r\n",paddr);//显示p的地址
			if(p) printf("%s\r\n",p);//显示P的内容

		}
		delay_ms(10);   
		i++;
		if((i%20)==0)//DS0闪烁.
		{
 			LED0=!LED0;
 		}
	}	 	   
}




