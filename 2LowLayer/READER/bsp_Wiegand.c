/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : bsp_Wiegand.c
  �� �� ��   : ����
  ��    ��   : �Ŷ�
  ��������   : 2019��7��29��
  ����޸�   :
  ��������   : Τ������������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��29��
    ��    ��   : �Ŷ�
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "bsp_Wiegand.h"

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

static uint8_t WG_Rx_Pro ( void );

static uint8_t ECC_ODD_Check ( uint32_t EvevData,uint8_t Bitlen );
static uint8_t ECC_Even_Check ( uint32_t EvevData,uint8_t Bitlen );


WG_RX_T WG_Rx_Str;   //Τ�����սṹ��


static uint8_t g_WGCardId[4] = {0};
static uint8_t CheckBitStart = 0;


/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/


void bsp_WiegandInit ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_AHB1PeriphClockCmd ( WG1_RCC, ENABLE ); //GPIODʱ��ʹ��

	//GPIO
	GPIO_InitStructure.GPIO_Pin = WG1_IN_D0 | WG1_IN_D1;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //�ܽ�����ѡ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//�ܽŹ�������
	GPIO_Init ( WG1_GPIO_PORT, &GPIO_InitStructure );   //�˿�ѡ����PA��PB��

//    GPIO_SetBits(WG1_GPIO_PORT,WG1_IN_D0);
//    GPIO_SetBits(WG1_GPIO_PORT,WG1_IN_D1);

	/* EXTI line gpio config(PD10 PD11) */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SYSCFG, ENABLE ); //ʹ��SYSCFGʱ��

	/* IO3_1 ���� EXTI Line10 �� PD10 ���� */
	SYSCFG_EXTILineConfig ( WG1_PortSource, WG1_IN_D0_PinSource );

	/* IO3_1���� EXTI Line11 �� PD11 ���� */
	SYSCFG_EXTILineConfig ( WG1_PortSource, WG1_IN_D1_PinSource );


	EXTI_InitStructure.EXTI_Line = WG1_IN_D0_EXTI | WG1_IN_D1_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init ( &EXTI_InitStructure );

	/* ����PD10 11Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init ( &NVIC_InitStructure );
}


void EXTI15_10_IRQHandler ( void )
{

	if ( EXTI_GetITStatus ( WG1_IN_D0_EXTI ) != RESET ) //D0
	{
		//Τ��ȡD0�ܽ�����
        delay_us(30);//����
        if(WG_IN_D0==0)     
        {                
            WeigenInD0();
        }

		/* Clear the EXTI line 0 pending bit */
		EXTI_ClearITPendingBit ( WG1_IN_D0_EXTI );
	}



	if ( EXTI_GetITStatus ( WG1_IN_D1_EXTI ) != RESET ) //D1
	{
		//Τ��ȡD1�ܽ�����		
        delay_us(30);//����
        
        if(WG_IN_D1==0)     
        {                
            WeigenInD1();
        }

		/* Clear the EXTI line 1 pending bit */
		EXTI_ClearITPendingBit ( WG1_IN_D1_EXTI );
	}

}



void WeigenInD0 ( void)
{
	WG_Rx_Str.WG_Bit=0;

	WG_Rx_Str.End_TIME=WG_TIMEOUT;   //40~50ms

	if ( WG_Rx_Str.WG_Rx_Len<34 )
	{
		WG_Rx_Str.WG_Rx_Len++;
	}

	else if ( WG_Rx_Str.WG_Rx_Len==34 )
	{
		WG_Rx_Str.WG_Rx_End=1;
		WG_Rx_Str.END_TIME_C_EN=0;
	}
	else
	{
		WG_Rx_Str.WG_Rx_Len=0;
		WG_Rx_Str.WG_Rx_End=0;
		WG_Rx_Str.END_TIME_C_EN=0;
	}

	if ( WG_Rx_Str.WG_Rx_Len==1 )
	{
		WG_Rx_Str.WG_Rx_Data=0;
		WG_Rx_Str.END_TIME_C_EN=1;
		CheckBitStart = WG_Rx_Str.WG_Bit;  //2012-10-13 ,������żЧ��λ;
	}

	if ( ( 1<WG_Rx_Str.WG_Rx_Len ) && ( WG_Rx_Str.WG_Rx_Len<34 ) ) 	//2012-09-19 Leo �޸ģ����Wiegand���������ж����������bug
	{
		if ( WG_Rx_Str.WG_Bit )
		{
			WG_Rx_Str.WG_Rx_Data|=0x01;
		}
		else
		{
			WG_Rx_Str.WG_Rx_Data&=0xFFFFFFFE;
		}
		if ( WG_Rx_Str.WG_Rx_Len<33 )
		{
			WG_Rx_Str.WG_Rx_Data<<=1;
		}
	}
}


void WeigenInD1 ( void )
{
	WG_Rx_Str.WG_Bit=1;
	WG_Rx_Str.End_TIME=WG_TIMEOUT;   //40~50ms
	if ( WG_Rx_Str.WG_Rx_Len<34 )
	{
		WG_Rx_Str.WG_Rx_Len++;
	}
	else if ( WG_Rx_Str.WG_Rx_Len==34 )
	{
		WG_Rx_Str.WG_Rx_End=1;
		WG_Rx_Str.END_TIME_C_EN=0;

	}
	else
	{
		WG_Rx_Str.WG_Rx_Len=0;
		WG_Rx_Str.WG_Rx_End=0;
		WG_Rx_Str.END_TIME_C_EN=0;
	}
	if ( WG_Rx_Str.WG_Rx_Len==1 )
	{
		WG_Rx_Str.WG_Rx_Data=0;
		WG_Rx_Str.END_TIME_C_EN=1;
		CheckBitStart = WG_Rx_Str.WG_Bit; ;  //2012-10-13 ,������żЧ��λ;
	}

	if ( ( 1<WG_Rx_Str.WG_Rx_Len ) && ( WG_Rx_Str.WG_Rx_Len<34 ) )		//2012-09-19 Leo �޸ģ����Wiegand���������ж����������bug
	{
		if ( WG_Rx_Str.WG_Bit )
		{
			WG_Rx_Str.WG_Rx_Data|=0x01;
		}
		else
		{
			WG_Rx_Str.WG_Rx_Data&=0xFFFFFFFE;
		}
		if ( WG_Rx_Str.WG_Rx_Len<33 )
		{
			WG_Rx_Str.WG_Rx_Data<<=1;
		}
	}
}


uint8_t WG_Rx_Pro ( void )
{
	uint8_t wg_len=0;
	uint8_t WGBit;

	wg_len=WG_Rx_Str.WG_Rx_Len;


	WG_Rx_Str.WG_Rx_Len=0;
	if ( wg_len!=26&&wg_len!=34 )
	{
		return 0;
	}
	if ( wg_len==34 ) //wg34
	{
		g_WGCardId[0] = ( WG_Rx_Str.WG_Rx_Data&0xFF000000 ) >>24;	//wg34
	}
	else //wg26
	{
		g_WGCardId[0]=0;
		WGBit = ( WG_Rx_Str.WG_Rx_Data>>1 ) &0x01;
		if ( ECC_ODD_Check ( WG_Rx_Str.WG_Rx_Data>>2,12 ) != WGBit )
		{
			return 0;
		}

		WG_Rx_Str.WG_Rx_Data>>=2;
		if ( ECC_Even_Check ( WG_Rx_Str.WG_Rx_Data>>12,12 ) != CheckBitStart )
		{
			return 0;
		}
	}


	g_WGCardId[1]= ( WG_Rx_Str.WG_Rx_Data&0xFF0000 ) >>16;
	g_WGCardId[2]= ( WG_Rx_Str.WG_Rx_Data&0xFF00 ) >>8;
	g_WGCardId[3]=WG_Rx_Str.WG_Rx_Data&0xFF;
	WG_Rx_Str.WG_Rx_Data=0;
	return wg_len;
}


uint32_t bsp_WeiGenScanf (void)
{
	uint8_t len=0;
	uint32_t Serial_Number = 0;
	/*wiegand����*/

	if ( WG_Rx_Str.END_TIME_C_EN )
	{
		if ( !WG_Rx_Str.End_TIME )
		{
			WG_Rx_Str.END_TIME_C_EN=0;
			if ( ( WG_Rx_Str.WG_Rx_Len==26 ) || ( WG_Rx_Str.WG_Rx_Len==34 ) )
			{
				WG_Rx_Str.WG_Rx_End=1;
			}
			else
			{
				WG_Rx_Str.WG_Rx_End=0;
				WG_Rx_Str.WG_Rx_Len = 0;
				WG_Rx_Str.End_TIME = WG_TIMEOUT;
			}
		}
	}
	else
	{
		if ( !WG_Rx_Str.End_TIME )
		{
			WG_Rx_Str.WG_Rx_Len=0;
			WG_Rx_Str.WG_Rx_End=0;
			WG_Rx_Str.End_TIME = WG_TIMEOUT;
		}
	}

	if ( WG_Rx_Str.WG_Rx_End )
	{
		WG_Rx_Str.WG_Rx_End=0;

		len = WG_Rx_Pro();

		if ( len==34 )
		{
			Serial_Number |= g_WGCardId[0]<<24;
			Serial_Number |= g_WGCardId[1]<<16;
			Serial_Number |= g_WGCardId[2]<<8;
			Serial_Number |= g_WGCardId[3];

			g_WGCardId[0] = 0;
			g_WGCardId[1] = 0;
			g_WGCardId[2] = 0;
			g_WGCardId[3] = 0;
		}
		else if ( len==26 )
		{
			Serial_Number |= g_WGCardId[1]<<16;
			Serial_Number |= g_WGCardId[2]<<8;
			Serial_Number |= g_WGCardId[3];

			if ( ( g_WGCardId[1] == 0xFF ) && ( g_WGCardId[2] == 0xFF ) )
			{
				if ( g_WGCardId[3] >= 0xF0 )
				{
					len = 4;
				}
				else
				{
                    len = 8;
				}
			}

			g_WGCardId[0] = 0;
			g_WGCardId[1] = 0;
			g_WGCardId[2] = 0;
			g_WGCardId[3] = 0;
		}

	}
    


	return Serial_Number;
}

uint8_t ECC_Even_Check ( uint32_t EvevData,uint8_t Bitlen )
{
	uint8_t i,Num=0;
	uint32_t Data,EccEven = 0x00000001;

	for ( i=0; i<Bitlen; i++ )
	{
		Data = EccEven&EvevData;
		if ( Data==1 )
		{
			Num ++;
		}
		EvevData>>=1;
	}
	Num = Num%2;

	if ( Num )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


uint8_t ECC_ODD_Check ( uint32_t EvevData,uint8_t Bitlen )
{
	uint8_t i,Num=0;
	uint32_t Data,EccEven = 0x00000001;

	for ( i=0; i<Bitlen; i++ )
	{
		Data = EccEven&EvevData;
		if ( Data==1 )
		{
			Num ++;
		}
		EvevData>>=1;
	}
	Num = Num%2;

	if ( Num )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

