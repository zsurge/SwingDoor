/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, ���ڲ�˼�߿Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : main.c
  �� �� ��   : ����
  ��    ��   : �Ŷ�
  ��������   : 2019��7��9��
  ����޸�   :
  ��������   : ������ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��9��
    ��    ��   : �Ŷ�
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "def.h"

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define APPCREATE_TASK_PRIO		(tskIDLE_PRIORITY)
#define APPCREATE_STK_SIZE 		(configMINIMAL_STACK_SIZE*16)
    


/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const char *AppCreateTaskName = "vAppCreateTask";      

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
static TaskHandle_t xHandleTaskAppCreate = NULL;     

SemaphoreHandle_t gxMutex = NULL;
EventGroupHandle_t xCreatedEventGroup = NULL;
QueueHandle_t gxMotorCtrlQueue = NULL; 



static void AppTaskCreate(void);
static void AppObjCreate (void);
static void AppPrintf(char *format, ...);
static void MotorInit(void);


int main(void)
{   
    //Ӳ����ʼ��
    bsp_Init();

    //��������ͨ�Ż���  
    AppObjCreate();
    
	//�����ʼ��(����)
    MotorInit();

    //����AppTaskCreate����
    AppTaskCreate();
//    xTaskCreate((TaskFunction_t )AppTaskCreate,     
//                (const char*    )AppCreateTaskName,   
//                (uint16_t       )APPCREATE_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )APPCREATE_TASK_PRIO,
//                (TaskHandle_t*  )&xHandleTaskAppCreate);   

    
    /* �������ȣ���ʼִ������ */
    vTaskStartScheduler();
    
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
    //�����ٽ���
//    taskENTER_CRITICAL(); 
    
    //����
    CreateHandShakeTask();

    //����λ��ͨѶ����
    CreateMsgParseTask();
    
    //������ƴ���
    CreateMotorCtrlTask();

    //����ָʾ��
    CreateLedTask();

    //�����������ռ�
    CreateReaderTask();

    //����ɨ�����ݴ���
    CreateBarCodeTask();

    //���⴫������������
    CreateSensorTask();

    //���Ź�
    CreateWatchDogTask();

    //ɾ������
//    vTaskDelete(xHandleTaskAppCreate); //ɾ��AppTaskCreate����

    //�˳��ٽ���
//    taskEXIT_CRITICAL();            

}


/*
*********************************************************************************************************
*	�� �� ��: AppObjCreate
*	����˵��: ��������ͨ�Ż���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppObjCreate (void)
{
	/* �����¼���־�� */
	xCreatedEventGroup = xEventGroupCreate();
	
	if(xCreatedEventGroup == NULL)
    {
        /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
        AppPrintf("�����¼���־��ʧ��\r\n");
    }

	/* ���������ź��� */
    gxMutex = xSemaphoreCreateMutex();
	
	if(gxMutex == NULL)
    {
        /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
        AppPrintf("���������ź���ʧ��\r\n");
    }     
    
  
    /* ������Ϣ���� */
    gxMotorCtrlQueue = xQueueCreate((UBaseType_t ) MOTORCTRL_QUEUE_LEN,/* ��Ϣ���еĳ��� */
                              (UBaseType_t ) sizeof(MOTORCTRL_QUEUE_T *));/* ��Ϣ�Ĵ�С */
    if(gxMotorCtrlQueue == NULL)
    {
        AppPrintf("����xTransQueue��Ϣ����ʧ��!\r\n");
    }	


}

/*
*********************************************************************************************************
*	�� �� ��: AppPrintf
*	����˵��: �̰߳�ȫ��printf��ʽ		  			  
*	��    ��: ͬprintf�Ĳ�����
*             ��C�У����޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void  AppPrintf(char *format, ...)
{
    char  buf_str[512 + 1];
    va_list   v_args;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);    

	/* �����ź��� */
	xSemaphoreTake(gxMutex, portMAX_DELAY);

    printf("%s", buf_str);

   	xSemaphoreGive(gxMutex);
}


static void MotorInit(void)
{
    uint8_t CloseDoor[8] = { 0x01,0x06,0x08,0x0C,0x00,0x01,0x8A,0x69 };
    uint8_t QuestStatus[8] =  { 0x01,0x03,0x07,0x0C,0x00,0x01,0x45,0x7D };
    uint8_t MotorReset[8] =  { 0x01,0x06,0x08,0x0C,0x00,0x07,0x0A,0x6B };


    uint8_t buf[8] = {0};
    uint8_t readLen = 0;
    uint16_t iCRC = 0;
    uint8_t crcBuf[2] = {0};
    uint8_t flag = 100;
    uint8_t cnt = 1;  

    do
    {   
        RS485_SendBuf(COM4, QuestStatus,8);
        delay_ms(50);
        readLen = RS485_Recv(COM4,buf,8);  
        
        if(readLen >= 6 )
        {
            iCRC = CRC16_Modbus(buf, readLen-2);
            crcBuf[0] = iCRC >> 8;
            crcBuf[1] = iCRC & 0xff;  

            if(crcBuf[1] == buf[readLen-2] && crcBuf[0] == buf[readLen-1])
            {    
                 if(buf[3] == 6 ||  buf[3] == 7 ||  buf[3] == 8)
                 {
                    flag = buf[3];
                    readLen = 7;
                 }
                 else
                 {
                    readLen = 0;
                 }      
            }  
            else
            {
                readLen = 0;//������ѯ
            }            
        }
        else //���и�λ
        {
            if(cnt % 10 == 0)
            {
               RS485_SendBuf(COM4, MotorReset,8);
               delay_ms(2000);
            }
        }

        cnt++;
        dbh("buf", (char * )buf, readLen);
    }
    while (readLen != 7 && cnt != READ_MOTOR_STATUS_TIMES);

    if(readLen !=7 && cnt == READ_MOTOR_STATUS_TIMES)
    {
        SendAsciiCodeToHost(ERRORINFO,MOTOR_RESET_ERR,(uint8_t *)"motor initial error");
        return;
    }

    if(flag == 6 || flag == 7)
    {
        RS485_SendBuf(COM4, CloseDoor,8);
    } 
}


#ifdef USEQUEUE
static void vTaskMonitor(void *pvParameters)
{
  BaseType_t xReturn = pdTRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdTRUE */
  QUEUE_TO_HOST_T *ptMsg;
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* �������ȴ�ʱ��Ϊ200ms */  

  while (1)
  {
    xReturn = xQueueReceive( xTransQueue,    /* ��Ϣ���еľ�� */
                             (void *)&ptMsg,  /*�����ȡ���ǽṹ��ĵ�ַ */
                             xMaxBlockTime); /* ��������ʱ�� */
    if(pdPASS == xReturn)
    {
//        DBG("ptMsg->cmd = %02x\r\n", ptMsg->cmd);
//        dbh("ptMsg->data ", (char *)ptMsg->data,QUEUE_BUF_LEN);

        switch (ptMsg->cmd)
        {
            case GETSENSOR:
                 DBG("��������\r\n");
                break;
             case CONTROLMOTOR:
                 DBG("A�ŵ������\r\n");
                break;
            case DOOR_B:
                 DBG("B�ŵ������\r\n");
                break;
            case WGREADER:
                 DBG("����������\r\n");
                break;            
            
        }
    }    

  }    
}
#endif


