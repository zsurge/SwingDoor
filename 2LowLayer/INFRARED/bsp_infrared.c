/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : bsp_infrared.c
  �� �� ��   : ����
  ��    ��   : �Ŷ�
  ��������   : 2019��6��28��
  ����޸�   :
  ��������   : ������⴫�����ź�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��6��28��
    ��    ��   : �Ŷ�
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "bsp_infrared.h"
#include "stdio.h"
#include "string.h"
#include "tool.h"
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
TSR_INF SR_INF;
/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/


/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
void bsp_infrared_init ( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd ( RCC_ALL_SENSOR, ENABLE );	//ʹ��GPIOFʱ��


	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SENSOR1|GPIO_PIN_SENSOR2|GPIO_PIN_SENSOR3|GPIO_PIN_SENSOR4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //����
	GPIO_Init ( GPIO_PORT_SENSOR14, &GPIO_InitStructure );		//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SENSOR5 |GPIO_PIN_SENSOR6;	    //LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //����
	GPIO_Init ( GPIO_PORT_SENSOR56, &GPIO_InitStructure );		//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SENSOR7 |GPIO_PIN_SENSOR8;	    //LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //����
	GPIO_Init ( GPIO_PORT_SENSOR78, &GPIO_InitStructure );		//��ʼ��GPIO

	GPIO_SetBits ( GPIO_PORT_SENSOR14,GPIO_PIN_SENSOR1|GPIO_PIN_SENSOR2|GPIO_PIN_SENSOR3|GPIO_PIN_SENSOR4 );
	GPIO_SetBits ( GPIO_PORT_SENSOR56,GPIO_PIN_SENSOR5 |GPIO_PIN_SENSOR6 );
	GPIO_SetBits ( GPIO_PORT_SENSOR78,GPIO_PIN_SENSOR7 |GPIO_PIN_SENSOR8 );

	memset ( &SR_INF,0,sizeof ( SR_INF ) );

}


void bsp_inf_clk ( void )
{
	uint8_t i;
	for ( i = 0; i < 24; ++i )
	{
		if ( SR_INF.NUMBUF[i] > TIM_UP )
		{
			SR_INF.NUMBUF[i]--;
		}
	}
}


uint32_t bsp_infrared_scan ( void )
{
	//static uint32_t iret = 0;
	static uint8_t i=0;

	//����IO�ڵ�ǰ״̬ӳ��������������
	SR_INF.STRBUF[0]=GETS1;
	SR_INF.STRBUF[1]=GETS2;
	SR_INF.STRBUF[2]=GETS3;
	SR_INF.STRBUF[3]=GETS4;
	SR_INF.STRBUF[4]=GETS5;
	SR_INF.STRBUF[5]=GETS6;
	SR_INF.STRBUF[6]=GETS7;
	SR_INF.STRBUF[7]=GETS8;
	SR_INF.STRBUF[8]=GETS9;
	SR_INF.STRBUF[9]=GETS10;
	SR_INF.STRBUF[10]=GETS11;
	SR_INF.STRBUF[11]=GETS12;
	SR_INF.STRBUF[12]=GETS13;
	SR_INF.STRBUF[13]=GETS14;
	SR_INF.STRBUF[14]=GETS15;
	SR_INF.STRBUF[15]=GETS16;
	SR_INF.STRBUF[16]=GETS17;
	SR_INF.STRBUF[17]=GETS18;
	SR_INF.STRBUF[18]=GETS19;
	SR_INF.STRBUF[19]=GETS20;
	SR_INF.STRBUF[20]=GETS21;
	SR_INF.STRBUF[21]=GETS22;
	SR_INF.STRBUF[22]=GETS23;
	SR_INF.STRBUF[23]=GETS24;

	//24����������ж�
	for ( i=0; i<24; i++ )
	{
		if ( SR_INF.STRBUF[i]==INF_T )  //���ⴥ��
		{
			if ( SR_INF.NUMBUF[i]==0 )  //ˢ�·���ʱ��
			{
				SR_INF.NUMBUF[i]=JET_TIM;
			}
			else if ( SR_INF.NUMBUF[i]==TIM_UP )    //�����ź��ȶ�
			{
				SR_INF.code |= ( 0x01<<i );
			}
		}
		else //�뿪
		{
			SR_INF.NUMBUF[i]=0;
			SR_INF.code &= ~ ( 0x01<<i );
		}
	}

	if ( SR_INF.code!=SR_INF.frecode )
	{
		SR_INF.frecode=SR_INF.code;
		return SR_INF.code;
	}

	return ERR_INFRARED;// �ް�������
}





void bsp_GetSensorStatus ( uint8_t* dat )
{
	//static uint32_t code = 0;
	uint8_t bcd[3] = {0};
	uint8_t buf[6] = {0};
	//uint8_t i;

	bcd[0] = SR_INF.code>>16;
	bcd[1] = SR_INF.code>>8;
	bcd[2] = SR_INF.code&0xff;

	memset ( buf,0x00,sizeof ( buf ) );

	bcd2asc ( buf, bcd, 6, 0 );

	memcpy ( dat,buf,6 );

}


//void bsp_putStatus(uint8_t dat)
//{
//    gSensorStatus[gSensorEnd] = dat;

//	if (++gSensorEnd  >= SENSORMAXBUFFSIZE)
//	{
//		gSensorEnd = 0;
//	}
//}


//uint32_t bsp_GetStatus(void)
//{
//	uint32_t ret;

//	if (gSensorTop == gSensorEnd)
//	{
//		return ERR_INFRARED;
//	}
//	else
//	{
//		ret = gSensorStatus[gSensorTop];

//		if (++gSensorTop >= SENSORMAXBUFFSIZE)
//		{
//			gSensorTop = 0;
//		}
//		return ret;
//	}
//}


