实验器材:
	NANO STM32F103开发板V1版本
	
实验目的:
	学习STM32 DMA功能的使用
	
硬件资源:
	1,DS0(连接在PC0),DS2(连接在PC2)
	2,串口1(波特率:115200)
	4,按键KEY0(PC8) 
	5,DMA(DMA1通道4) 
	
实验现象:
	本实验通过按键KEY0控制串口1以DMA方式发送数据，按下KEY0，就开始DMA传送，DS2闪烁表示DMA正在传输。
	打开串口调试助手，可以收到DMA发送的内容，DS0闪烁表示程序正在运行。
	
注意事项:  
	1,电脑端串口调试助手波特率必须是115200.
	2,请使用XCOM/SSCOM串口调试助手。
	6,请用USB线连接在USB_JTAG,找到USB转串口后测试本例程.
	7,PA9/PA10必须通过跳线帽连接在RXD/TXD上.
	 

					正点原子@ALIENTEK
					2018-3-28
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					