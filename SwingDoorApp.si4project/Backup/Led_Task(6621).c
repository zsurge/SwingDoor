/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : Led_Task.c
  �� �� ��   : ����
  ��    ��   :  
  ��������   : 2020��2��25��
  ����޸�   :
  ��������   : LED�ƿ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2020��2��25��
    ��    ��   :  
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "led_task.h"
#include "tool.h"
#include "bsp_led.h" 
#include "comm.h"
#include "bsp_beep.h" 
#include "bsp_uart_fifo.h"


/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define LED_TASK_PRIO	    ( tskIDLE_PRIORITY)
#define LED_STK_SIZE 		(configMINIMAL_STACK_SIZE*8)

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const char *ledTaskName = "vLedTask";      //���Ź�������


/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
TaskHandle_t xHandleTaskLed = NULL;      //LED��


/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
static void vTaskLed(void *pvParameters);

void CreateLedTask(void)
{
    //����LED����
    xTaskCreate((TaskFunction_t )vTaskLed,         
                (const char*    )ledTaskName,       
                (uint16_t       )LED_STK_SIZE, 
                (void*          )NULL,              
                (UBaseType_t    )LED_TASK_PRIO,    
                (TaskHandle_t*  )&xHandleTaskLed);   

}


//LED������ 
static void vTaskLed(void *pvParameters)
{   
//    uint8_t pcWriteBuffer[1024];
    uint8_t tmp[15] = {0x00};
    BaseType_t xReturn = pdTRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(1000); /* �������ȴ�ʱ��Ϊ1000ms */  
    char *recvbuff;
    uint8_t i = 0;

    
    BEEP = 0;
    vTaskDelay(300);
    BEEP = 1;
    
    while(1)
    {          
        if(Motro_A== 1)
        {
          LED3=!LED3;   
        }
        else
        {
            LED3 = 0;
        }
        
        if(Motro_B == 1)
        {
          LED2=!LED2;   
        }
        else
        {
            LED2 = 0;
        }    

        //ϵͳ״̬���еƣ�ÿ500ms һ��
        i++;
        if(i == 5)
        {
            i = 0;
            LED4=!LED4; 
        }

        //��ȡ����֪ͨ���ȴ�1000��ʱ����ģ���ȡ������ִ����λ��ָ���ȡ��������ִ��״̬��ѯ
		xReturn=xTaskNotifyWait(0x0,			//���뺯����ʱ���������bit
                            ULONG_MAX,	        //�˳�������ʱ��������е�bit
                            (uint32_t *)&recvbuff,//��������ֵ֪ͨ                    
                            (TickType_t)xMaxBlockTime);	//����ʱ��
                            
        if( pdTRUE == xReturn )
        {            
            memcpy(tmp,recvbuff,MAX_EXLED_LEN);
//            dbh("ex_led recv��",tmp, MAX_EXLED_LEN);
            if(Nonzero(tmp,MAX_EXLED_LEN))
            {
                bsp_Ex_SetLed((uint8_t*)tmp); 
                memset(tmp,0x00,sizeof(tmp));
                send_to_host(SETLED,tmp,1);
            }
            else
            {
                SendAsciiCodeToHost(ERRORINFO,COMM_SEND_ERR,"set led error,try again");
            }

        }
         
        
		/* �����¼���־����ʾ������������ */        
		xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_0);  
        vTaskDelay(100);     
    }
}   


