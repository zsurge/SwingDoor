/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : key.h
  �� �� ��   : ����
  ��    ��   : �Ŷ�
  ��������   : 2019��5��25��
  ����޸�   :
  ��������   : ȫ����(B��)���Ű���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��5��25��
    ��    ��   : �Ŷ�
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __bsp_KEY_H
#define __bsp_KEY_H	 

#include "sys.h" 
#include "delay.h"


/* �����ڶ�Ӧ��RCCʱ�� */
#define RCC_ALL_KEY             (RCC_AHB1Periph_GPIOE)
#define GPIO_PORT_KEY           GPIOE
#define GPIO_PIN_KEY_DOOR_B     GPIO_Pin_9

#define KEY_DOOR_B  PEin(9)   	



#define KEY_NONE            0   //�ް���
#define KEY_DOOR_B_PRES 	1	//B�ſ��ż�����



void bsp_key_Init(void);	//IO��ʼ��
uint8_t bsp_key_Scan(uint8_t);  		//����ɨ�躯��		


#endif

