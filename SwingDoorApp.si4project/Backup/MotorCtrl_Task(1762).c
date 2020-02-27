/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : MotorCtrl_Task.c
  �� �� ��   : ����
  ��    ��   :  
  ��������   : 2020��2��26��
  ����޸�   :
  ��������   : �����������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2020��2��26��
    ��    ��   :  
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include"MotorCtrl_Task.h"
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define MOTOR_TASK_PRIO		( tskIDLE_PRIORITY + 3)
#define MOTOR_STK_SIZE 		(configMINIMAL_STACK_SIZE*8)



/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const char *MotorCtrlTaskName = "vMotorCtrlTask";      

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
TaskHandle_t xHandleTaskMotorCtrl = NULL;     




/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
static void vTaskMotorCtrl(void *pvParameters);



void CreateMotorCtrlTask(void *pvParameters)
{
    //���������Ϣ��������
    xTaskCreate((TaskFunction_t )vTaskMotorToHost,     
                (const char*    )MotorCtrlTaskName,   
                (uint16_t       )MOTOR_STK_SIZE, 
                (void*          )pvParameters,
                (UBaseType_t    )MOTOR_TASK_PRIO,
                (TaskHandle_t*  )&xHandleTaskMotor);     

}

static void vTaskMotorCtrl(void *pvParameters)
{

}


