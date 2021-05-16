#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "smg.h"
#include "remote.h" 
#include "timer.h"
#include "beep.h"

/************************************************
 ALIENTEK NANO STM32开发板实验20
 红外遥控实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

// 共阴数字数组
// 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F, .，全灭
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};
u8 key=0;  //按键值
u8 num=0x00;//数值
u8 num1=0x00;//数值
u8 smg_wei=6;//数码管位选
u8 smg_duan=0;//数码管段选
u8 smg_flag=0;//数码管显示标志 0:正常显示 1:不显示（消除鬼影）
u8 t=0;
 int main(void)
 { 
	
	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,115200);	//串口初始化为115200
	delay_init(72);	   	 	//延时初始化 
	BEEP_Init();            //蜂鸣器初始化
	LED_Init();		  		//初始化与LED连接的硬件接口
    LED_SMG_Init();         //数码管初始化
    TIM4_Init(19,7199);     //数码管2ms定时显示
    Remote_Init();			//红外接收初始化		 
	while(1);
}

void TIM4_IRQHandler(void) //TIM4中断
{
	
	if(TIM4->SR&0X0001)//溢出中断
	{
        key = Remote_Scan();//获取红外遥控键值
		 if(key)
		 {
			switch(key)
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
				case 162:num1=smg_num[1];num=smg_num[0];BEEP=0; break;//按键'POWER'    
				case 98:num1=smg_num[1];num=smg_num[1];BEEP=0;  break;//按键'UP'
				case 226:num1=smg_num[1];num=smg_num[2];BEEP=0; break;//按键'ALIENTEK'
				case 34:num1=smg_num[1];num=smg_num[3];BEEP=0;  break;//按键'LEFT'
				case 2:num1=smg_num[1];num=smg_num[4];BEEP=0;   break;//按键'PLAY'
				case 194:num1=smg_num[1];num=smg_num[5];BEEP=0; break;//按键'RIGHT'
				case 224:num1=smg_num[1];num=smg_num[6];BEEP=0; break;//按键'VOL-'
				case 168:num1=smg_num[1];num=smg_num[7];BEEP=0; break;//按键'DOWN'
				case 144:num1=smg_num[1];num=smg_num[8];BEEP=0; break;//按键'VOL+'
			}
		 }else
		 {
			BEEP=1;
		 }
		 if(smg_wei==6)//数码管位
		 {
			 smg_duan = num1;
		 }
		 else if(smg_wei==7)//数码管位
		 {
			 smg_duan = num;
		 }
		 if(smg_flag) LED_Write_Data(0x00,smg_wei);//消除鬼影(段码不显示)
		 else 	  LED_Write_Data(smg_duan,smg_wei);//正常显示 
		 LED_Refresh();//数码管数据更新
		 smg_flag=!smg_flag;
		 if(smg_flag==0)//正常显示才更新位码
		 {
			 smg_wei++;
		     if(smg_wei==8) smg_wei=6;
		 }
		 t++;
		 if(t==250)//LED0每500MS闪烁
		 {
			t=0;
			LED0=!LED0;
		 } 
	}
	TIM4->SR&=~(1<<0);//清除中断标志位 
		
}
