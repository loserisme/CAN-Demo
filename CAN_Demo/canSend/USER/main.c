//DAC(STM32内部DAC通道1,通过PA4输出)
//ADC(STM32内部ADC1_CH1,连接在PA1).
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "can.h" 
#include "adc.h"

 void ShowState(u8 s);
 uint8_t *floatToChar(float x);

 int main(void)
 {	 
	//u8 key;
	u8 i=0;
	u8 a=11;
	u8 canbuf[8];
	//u8 res;
	u16 adcx;


	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级	
  Adc_Init();        //adc ???	 
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps    
  uart_init(115200);	 //串口初始化为115200
	

 	while(1)
	{
		
			for(i=0;i<8;i++)
			{
				adcx=Get_Adc_Average(ADC_Channel_1,10);
				canbuf[i]=adcx;//填充发送缓冲区
				//SerialPort text
				printf(floatToChar(canbuf[i]));
		    printf("\n");
 			}
			Can_Send_Msg(canbuf,8);
			//res=Can_Send_Msg(canbuf,8);//发送8个字节
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




