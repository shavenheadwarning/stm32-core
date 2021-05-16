#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "usmart.h"
#include "24cxx.h" 
#include "myiic.h"
/************************************************
 ALIENTEK NANO STM32开发板实验18
 IIC实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
   	
//要写入到24c02的字符串数组
const u8 TEXT_Buffer[]={"NANO STM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	
int main(void)
{ 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	uart_init(72,115200);   //串口初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化		 	
	AT24CXX_Init();			//IIC初始化 
    usmart_dev.init(72);	//初始化USMART
	printf("NANO STM32\r\n");	
    printf("IIC TEST\r\n"); 	
 	while(AT24CXX_Check())//检测不到24c02
	{
		printf("24C02 Check Failed!\r\n");
		delay_ms(500);
		printf("Please Check!\r\n");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	printf("24C02 Ready!\r\n");
	printf("WK_UP:Write  KEY1:Read\r\n");//显示提示信息	  
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)//WK_UP 按下,写入24C02
		{
			LED2=0;
		    printf("\r\nStart Write 24C02....\r\n");
			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
			printf("24C02 Write Finished!\r\n");//提示传送完成
			LED2=1;
		}
		if(key==KEY1_PRES)//KEY1 按下,读取字符串并显示
		{
			LED2=0;
			printf("\r\nStart Read 24C02....\r\n");
			AT24CXX_Read(0,datatemp,SIZE);
			printf("The Data Readed Is:\r\n");//提示传送完成
			printf("%s\r\n",datatemp);//显示读到的字符串
			LED2=1;
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//提示系统正在运行	
			i=0;
		}		   
	}
	
}
