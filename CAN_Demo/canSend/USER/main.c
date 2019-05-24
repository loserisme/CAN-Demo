//DAC(STM32�ڲ�DACͨ��1,ͨ��PA4���)
//ADC(STM32�ڲ�ADC1_CH1,������PA1).
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


	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
  Adc_Init();        //adc ???	 
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������500Kbps    
  uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	

 	while(1)
	{
		
			for(i=0;i<8;i++)
			{
				adcx=Get_Adc_Average(ADC_Channel_1,10);
				canbuf[i]=adcx;//��䷢�ͻ�����
				//SerialPort text
				printf(floatToChar(canbuf[i]));
		    printf("\n");
 			}
			Can_Send_Msg(canbuf,8);
			//res=Can_Send_Msg(canbuf,8);//����8���ֽ�
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




