/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : bsp_sensor.h
  �� �� ��   : ����
  ��    ��   :  
  ��������   : 2020��5��22��
  ����޸�   :
  ��������   : bsp_sensor.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2020��5��22��
    ��    ��   :  
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __BSP_SENSOR_H__
#define __BSP_SENSOR_H__

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "stm32f4xx.h"
#define ERR_INFRARED    (0xF0000000)
    
#define RCC_ALL_SENSOR     (RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC\
                            |RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG)
    
#define GPIO_PORT_SENSOR14    GPIOA
#define GPIO_PORT_SENSOR56    GPIOC
#define GPIO_PORT_SENSOR78    GPIOB
#define GPIO_PORT_SENSOR914    GPIOG
#define GPIO_PORT_SENSOR1516    GPIOD
#define GPIO_PORT_SENSOR1718    GPIOC
#define GPIO_PORT_SENSOR1920    GPIOF
#define GPIO_PORT_SENSOR2122    GPIOA
#define GPIO_PORT_SENSOR23    GPIOC
#define GPIO_PORT_SENSOR24    GPIOG

//PORTA    
#define GPIO_PIN_SENSOR1    GPIO_Pin_4
#define GPIO_PIN_SENSOR2    GPIO_Pin_5
#define GPIO_PIN_SENSOR3    GPIO_Pin_6
#define GPIO_PIN_SENSOR4    GPIO_Pin_7

//PORTC
#define GPIO_PIN_SENSOR5    GPIO_Pin_4
#define GPIO_PIN_SENSOR6    GPIO_Pin_5

//PORTB
#define GPIO_PIN_SENSOR7    GPIO_Pin_0
#define GPIO_PIN_SENSOR8    GPIO_Pin_1

//PORTG
#define GPIO_PIN_SENSOR9    GPIO_Pin_7
#define GPIO_PIN_SENSOR10   GPIO_Pin_6
#define GPIO_PIN_SENSOR11   GPIO_Pin_5
#define GPIO_PIN_SENSOR12   GPIO_Pin_4
#define GPIO_PIN_SENSOR13   GPIO_Pin_3
#define GPIO_PIN_SENSOR14   GPIO_Pin_2

//PORTD
#define GPIO_PIN_SENSOR15   GPIO_Pin_15
#define GPIO_PIN_SENSOR16   GPIO_Pin_14

//PORTC
#define GPIO_PIN_SENSOR17   GPIO_Pin_1
#define GPIO_PIN_SENSOR18   GPIO_Pin_0

//PORTF
#define GPIO_PIN_SENSOR19   GPIO_Pin_2
#define GPIO_PIN_SENSOR20   GPIO_Pin_1

//PORTA
#define GPIO_PIN_SENSOR21   GPIO_Pin_9
#define GPIO_PIN_SENSOR22   GPIO_Pin_8

//PORTC
#define GPIO_PIN_SENSOR23   GPIO_Pin_9

//PORTG
#define GPIO_PIN_SENSOR24   GPIO_Pin_8


#define GETS1 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR14,GPIO_PIN_SENSOR1) 
#define GETS2 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR14,GPIO_PIN_SENSOR2)
#define GETS3 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR14,GPIO_PIN_SENSOR3) 
#define GETS4 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR14,GPIO_PIN_SENSOR4)
#define GETS5 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR56,GPIO_PIN_SENSOR5) 
#define GETS6 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR56,GPIO_PIN_SENSOR6)
#define GETS7 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR78,GPIO_PIN_SENSOR7) 
#define GETS8 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR78,GPIO_PIN_SENSOR8)

#define GETS9 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR914,GPIO_PIN_SENSOR9) 
#define GETS10 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR914,GPIO_PIN_SENSOR10)
#define GETS11	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR914,GPIO_PIN_SENSOR11) 
#define GETS12	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR914,GPIO_PIN_SENSOR12)
#define GETS13	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR914,GPIO_PIN_SENSOR13) 
#define GETS14	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR914,GPIO_PIN_SENSOR14)

#define GETS15	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR1516,GPIO_PIN_SENSOR15) 
#define GETS16 	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR1516,GPIO_PIN_SENSOR16)

#define GETS17	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR1718,GPIO_PIN_SENSOR17) 
#define GETS18	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR1718,GPIO_PIN_SENSOR18)

#define GETS19	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR1920,GPIO_PIN_SENSOR19) 
#define GETS20	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR1920,GPIO_PIN_SENSOR20)

#define GETS21	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR2122,GPIO_PIN_SENSOR21) 
#define GETS22	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR2122,GPIO_PIN_SENSOR22)

#define GETS23	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR23,GPIO_PIN_SENSOR23) 
#define GETS24	GPIO_ReadInputDataBit(GPIO_PORT_SENSOR24,GPIO_PIN_SENSOR24)


/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define SENSOR_NUM_MAX      24

#define TURN_ON	    0		//���ⴥ��
#define TURN_OFF	1		//�޴���

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
typedef struct SENSORVALUE
{	
    uint8_t state[SENSOR_NUM_MAX];		        //��һ��ɨ��ʱ�����ֵ
	uint8_t curSensorVal[SENSOR_NUM_MAX];		//��ǰ�����ֵ
	uint32_t code;			        //��ǰ���к���״̬��ֵ
	uint32_t preCode;		        //��һ�κ����ֵ	          
}SENSORVALUE_STRU;

extern SENSORVALUE_STRU gSensorValue;


/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
void bsp_GetSensorValue(uint8_t *dat);
void bsp_sensor_init(void);
uint32_t bsp_sensor_scan(void);



#endif /* __BSP_SENSOR_H__ */
