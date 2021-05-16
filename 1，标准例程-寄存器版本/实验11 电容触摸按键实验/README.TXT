实验器材:
	NANO STM32F103开发板V1版本
	
实验目的:
	进一步学习定时器的输入捕获功能
	
硬件资源:
	1,DS0(连接在PC0)、DS5(连接在PC5)
	2,TPAD电容触摸按键(右下角LOGO,即TPAD,连接在PB1)
	3,定时器3(TIM3),TIM3的通道4(TIM3_CH4,连接在PA1上面).
	
实验现象:
	本实验,利用开发板板载的电容触摸按键(右下角白色LOGO,即TPAD)，通过TIM3_CH4（PB1）对
	电容触摸按键的检测，实现对DS5的控制,下载本代码后，我们通过按开发板右下角的TPAD按钮
	，就可以控制DS5的亮灭了，DS0闪烁表示程序正在运行。
	
注意事项:
	1,本实验需要将P3端子用跳线帽连接ADC&TPAD. 


					正点原子@ALIENTEK
					2018-3-28
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com


































