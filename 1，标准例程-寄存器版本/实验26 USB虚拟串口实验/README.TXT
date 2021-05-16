实验器材:
	NANO STM32F103开发板V1版本
	
实验目的:
	学习STM32的USB功能使用,实现一个USB虚拟串口功能.
	
硬件资源:
	1,DS0(连接在PC0)
	2,串口1(波特率:115200)
	3,USB_SLAVE接口
	
实验现象:
	本实验，利用STM32自带的USB功能,连接电脑USB,虚拟出一个USB串口，实现电脑和开发板的数据通信。
	本例程功能完全同实验4（串口实验），只不过串口变成了STM32的USB虚拟串口。当USB连接电脑（USB
	线插入USB_SLAVE接口），开发板将通过USB和电脑建立连接虚拟出一个串口（注意：需要先安装:光盘
	\6，软件资料\1，软件\STM32 USB虚拟串口驱动\VCP_V1.4.0_Setup.exe这个驱动软件），USB和电脑
	连接成功后，DS1常亮。在找到虚拟串口后，即可打开串口调试助手，实现同实验4一样的功能，即：
	STM32通过USB虚拟串口和上位机对话，STM32在收到上位机发过来的字符串（以回车换行结束）后，原原
	本本的返回给上位机。下载后，DS0闪烁，提示程序在运行，同时每隔一定时间，通过USB虚拟串口输出一
	段信息到电脑。  
	
注意事项:
	1,本实验需安装:光盘\6，软件资料\1，软件\STM32 USB虚拟串口驱动\VCP_V1.4.0_Setup.exe这个驱动
	2,如果电脑显示驱动安装不正常（有惊叹号），请手动选择驱动安装，驱动路径（以WIN7系统为例）：C:\
	  Program Files (x86)\STMicroelectronics\Software\Virtual comport driver\Win7
	3,本例程USB线需要插在:USB_SLAVE.
	
 


					正点原子@ALIENTEK
					2018-3-28
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com




































