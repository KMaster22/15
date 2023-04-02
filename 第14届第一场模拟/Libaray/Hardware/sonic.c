#include "sonic.h"

sbit Trig = P1^0;
sbit RX = P1^1;


void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x00;		//���ö�ʱ��ֵ
	
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//�Ȳ�����
}


void Sendwave()
{
	char i ;
	for(i =0 ;i<8;i++)
	{
		Trig = 1;
		Delay13us();
		Trig = 0;
		Delay13us();
	}
}


//float GetDistance()
//{
//	float distance = 0;
//	
//	Sendwave();
//	TR0 = 1;
//	while((RX==1)&&(TF0==0));
//	TR0 = 0;	
//	
//	distance = ((TH0<<8)|TL0)*0.017;//cm
//	
//	TF0 = 0;
//	TL0 = 0x00;		//���ö�ʱ��ֵ
//	TH0 = 0x00;		//���ö�ʱ��ֵ
//	
//	return distance;
//}

u16 GetDistance()
{
	float distance = 0;
	
	TL0 = 0x00;		
	TH0 = 0x00;	
	
	Sendwave();
	TR0 = 1;
	while(RX==1&&TF0==0);
	TR0 = 0;	
	if(TF0==1){
		TF0 = 0;
	}else{
		distance = ((TH0<<8)|TL0)*0.017;//cm
	}
	
	return distance;
}
