/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : MsgParse_Task.c
  �� �� ��   : ����
  ��    ��   :  
  ��������   : 2020��2��27��
  ����޸�   :
  ��������   : ������λ��ָ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2020��2��27��
    ��    ��   :  
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "MsgParse_Task.h"
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define MSGPARSE_TASK_PRIO		( tskIDLE_PRIORITY + 4)
#define MSGPARSE_STK_SIZE 		(configMINIMAL_STACK_SIZE*8)

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const char *MsgParseTaskName = "vMsgParseTask";  

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
TaskHandle_t xHandleTaskMsgParse = NULL;

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
static void vTaskMsgParse(void *pvParameters);

void CreateMsgParseTask(void *pvParameters)
{
    //��androidͨѶ�������ݽ���
    xTaskCreate((TaskFunction_t )vTaskMsgParse,     
                (const char*    )MsgParseTaskName,   
                (uint16_t       )MSGPARSE_STK_SIZE, 
                (void*          )pvParameters,
                (UBaseType_t    )MSGPARSE_TASK_PRIO,
                (TaskHandle_t*  )&xHandleTaskMsgParse);
}



static void vTaskMsgParse(void *pvParameters)
{
    while(1)
    {          
        deal_Serial_Parse();    
	    deal_rx_data();

		/* �����¼���־����ʾ������������ */        
		xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_2);
        vTaskDelay(100);
    }

}
