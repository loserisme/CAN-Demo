//DAC(STM32内部DAC通道1,通过PA4输出)
//ADC(STM32内部ADC1_CH1,连接在PA1).
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "can.h" 
#include "dac.h"
#include "lcd_driver.h"
#include "gui.h"
 
 //void ShowState(u8 s);
 uint8_t *floatToChar(float x);


 int main(void)
 {
	u8 length;
	u8 i=0;
	u8 canbuf[8];
	//uint8_t a=66;

	 
	delay_init();	    	 //延时函数初始化	  
	//uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级 	
	Dac1_Init();				//DAC初始化
  TFT_Init();	 //TFT彩屏初始化
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);    
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);//初始值为0
	TFT_ClearScreen(BLACK);	   //清屏
	GUI_Show12ASCII(10,10,"This is a CAN DEMO!",YELLOW,BLACK);
	GUI_Show12ASCII(10,100,"The Value is:",YELLOW,BLACK);
	 
 // GUI_Show12ASCII(130,100,floatToChar(a),YELLOW,BLACK);
	 
 	while(1)
	{
		length=Can_Receive_Msg(canbuf);
		if(length)//接收到有数据
		{
			//GUI_Box(10,150,150,170,BLACK);
			for(i=0;i<=7;i++)
			{
				GUI_Show12ASCII(130,100,floatToChar(canbuf[i]),YELLOW,BLACK);
				Dac1_Set_Vol(canbuf[i]);			
			}
		}
		else
		{
			//GUI_Show12ASCII(10,150,"This is no device!",RED,BLACK);
		}
		delay_ms(10);
		
	}
}

uint8_t *floatToChar(float x)
{
	static uint8_t len[7];
	uint32_t plus;
	plus=x*1000;
	len[6]='0'+plus%10;
	len[5]='0'+plus/10%10;
	len[4]='0'+plus/100%10;
	len[3]='.';
	if(x>0.000&&x<10.000)
	{
		len[2]='0'+plus/1000%10;
		len[1]='0';
		len[0]='0';
	}
	
	if(x>9.000&&x<100.000)
	{
		len[2]='0'+plus/1000%10;
		len[1]='0'+plus/10000%10;
		len[0]='0';
	}
	if(x>99.000&&x<1000.000)
	{
		len[2]='0'+plus/1000%10;
		len[1]='0'+plus/10000%10;
		len[0]='0'+plus/100000%10;
	}

//for(plus=0;plus<7;plus++)
//{
//	len[plus]='0'+plus;
//}
//len[3]='.';
	return len;
}

