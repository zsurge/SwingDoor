/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : key.c
  �� �� ��   : ����
  ��    ��   : �Ŷ�
  ��������   : 2019��5��25��
  ����޸�   :
  ��������   : ��������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��5��25��
    ��    ��   : �Ŷ�
    �޸�����   : �����ļ�

******************************************************************************/

#include "bsp_key.h"


//������ʼ������
void bsp_key_Init ( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd ( RCC_ALL_KEY, ENABLE ); //ʹ��GPIOE

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_DOOR_B; //��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//
	GPIO_Init ( GPIO_PORT_KEY, &GPIO_InitStructure ); //��ʼ��GPIOE9
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY_DOOR_B����

u8 bsp_key_Scan ( u8 mode )
{
	static u8 key_up=1;//�������ɿ���־
	if ( mode )
	{
		key_up=1;    //֧������
	}
	if ( key_up&& ( KEY_DOOR_B==0 ) )
	{
		delay_ms ( 10 ); //ȥ����
		
		key_up=0;
        
		if ( KEY_DOOR_B == 0 )
		{
			return KEY_DOOR_B_PRES;
		}

	}
	else if ( KEY_DOOR_B == 1 )
	{
		key_up=1;
	}
    
	 return KEY_NONE;// �ް�������
}


