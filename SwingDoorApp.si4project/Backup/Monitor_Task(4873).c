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


#if 0
static void vTaskMonitor(void *pvParameters)
{
    BaseType_t xReturn = pdTRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    uint32_t r_num = 0;
    uint8_t buf[8] = {0};
    uint8_t OpenDoor[8] = { 0x01,0x06,0x08,0x0C,0x00,0x02,0xCA,0x68 };
    static uint32_t flag = MON_DISABLE;

    while (1)
    {
        //��ȡ����֪ͨ ,û��ȡ����һֱ�ȴ�
    	xReturn=xTaskNotifyWait(0x0,	  //���뺯����ʱ���������bit
                            ULONG_MAX,	  //�˳�������ʱ��������е�bit
                            &r_num,		  //��������ֵ֪ͨ                     
                            portMAX_DELAY);	//����ʱ��
                            
        if( pdTRUE == xReturn || flag == MON_ENABLE)
        {           
            //��һ�λḳֵ��һֱ���ڶ��ν��յ�֪ͨΪֹ
            flag = r_num;
            
            bsp_GetSensorValue(buf);
//            printf("r_num == %x,sensor = %s,flag = %x\r\n",r_num,buf,flag);
            if(atoi((const char*)buf) != 0 && flag != MON_DISABLE) 
            {
                printf("waring!!! open door\r\n");
                RS485_SendBuf(COM4, OpenDoor,sizeof(OpenDoor));//��A���
            }
        }
        
         vTaskDelay(20);
        
    }    
    
}
#endif 


#if 0
static void vTaskMonitor(void *pvParameters)
{
	QueueSetMemberHandle_t xActivatedMember;
    MOTORCTRL_QUEUE_T *ptMotor1; 
    MOTORCTRL_QUEUE_T *ptMotor2; 
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); 

    while (1)
    {
		/* ���¼��ȴ����ȴ���ֵ�ź�������Ϣ���� */
        xActivatedMember = xQueueSelectFromSet(gxQueueSet, xMaxBlockTime);

        /* ����xActivatedMember�жϽ��ܵ�����Ϣ���� */
        if(xActivatedMember == gxMotorCtrlQueue)
        {
			/* ��Ϣ����1���յ���Ϣ */
            xQueueReceive(xActivatedMember, &ptMotor1, 0);
			dbh("ptMotor1", ptMotor1->data,sizeof(ptMotor1->data));
        }
        else if(xActivatedMember == gxMotorSecDoorCtrlQueue)
        {
			/* ��Ϣ����2���յ���Ϣ */
            xQueueReceive(xActivatedMember, &ptMotor2, 0);
			dbh("ptMotor2", ptMotor2->data,sizeof(ptMotor2->data));
        }
        else
        {
            ;
        }        
    }    
    
}

#endif





static void vTaskMonitor(void *pvParameters)
{
    uint8_t buf[8] = {0};
    uint8_t OpenDoor[8] = { 0x01,0x06,0x08,0x0C,0x00,0x02,0xCA,0x68 };
    static uint32_t flag = MON_DISABLE;

    while (1)
    {
        if(flag == MON_DISABLE )
        {
            flag = MON_ENABLE ;
            vTaskSuspend(xHandleTaskMonitor);
        }

        bsp_GetSensorValue(buf);

        if(atoi((const char*)buf) != 0) 
        {
            printf("waring!!! open door\r\n");
            RS485_SendBuf(COM4, OpenDoor,sizeof(OpenDoor));//��A���
            vTaskSuspend(xHandleTaskMonitor);
        }
        
        vTaskDelay(10);
        
    }    
    
}












