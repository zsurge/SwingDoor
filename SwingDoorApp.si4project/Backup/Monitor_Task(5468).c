/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : Monitor_Task.c
  �� �� ��   : ����
  ��    ��   :  
  ��������   : 2020��5��25��
  ����޸�   :
  ��������   : ����̣߳���Ϊ����ʹ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2020��5��25��
    ��    ��   :  
    �޸�����   : �����ļ�

    ��ص�����ʱ�����ø����񣬶�ȡ����״̬��ִ�з��й���
    �յ����ŵ�λ���߿��ŵ�λ�����������

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include"Monitor_Task.h"
#include "comm.h"
#include "bsp_sensor.h"
#include "stdlib.h"
#include "bsp_uart_fifo.h"

    
    
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define MONITOR_TASK_PRIO		(tskIDLE_PRIORITY + 4)
#define MONITOR_STK_SIZE 		(configMINIMAL_STACK_SIZE*8)

#define MON_ENABLE  0x55
#define MON_DISABLE  0xAA



/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const char *MonitorTaskName = "vMonitorTask";    

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
TaskHandle_t xHandleTaskMonitor = NULL; 

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
static void vTaskMonitor(void *pvParameters);



void CreateMonitorTask(void)
{
    //���������Ϣ��������
    xTaskCreate((TaskFunction_t )vTaskMonitor,     
                (const char*    )MonitorTaskName,   
                (uint16_t       )MONITOR_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MONITOR_TASK_PRIO,
                (TaskHandle_t*  )&xHandleTaskMonitor);
}

static void vTaskMonitor(void *pvParameters)
{
    uint8_t buf[8] = {0};
    uint8_t OpenDoor[8] = { 0x01,0x06,0x08,0x0C,0x00,0x02,0xCA,0x68 }; //������    
    uint8_t ReverseOpenDoor[8] = { 0x01,0x06,0x08,0x0C,0x00,0x03,0x0B,0xA8 }; //������
    
    static uint32_t flag = MON_DISABLE;

    while (1)
    {
        if(flag == MON_DISABLE )
        {
            flag = MON_ENABLE ;
            vTaskSuspend(xHandleTaskMonitor);
        }

        bsp_GetSensorValue(buf);


        //������Ҫ�ж������ֵ����ȷ���������Ż��߷�����
        if(atoi((const char*)buf) != 0) 
        {
            printf("waring!!! open door\r\n");
            RS485_SendBuf(COM4, OpenDoor,sizeof(OpenDoor));//��A���
            vTaskSuspend(xHandleTaskMonitor);
        }
        
        vTaskDelay(10);
        
    }    
    
}












