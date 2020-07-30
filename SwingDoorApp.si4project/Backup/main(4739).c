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
 #define ULONG_MAX 0xffffffffUL
 
//�������ȼ�
#define LED_TASK_PRIO	    ( tskIDLE_PRIORITY)
#define HANDSHAKE_TASK_PRIO	( tskIDLE_PRIORITY)
#define QUERYMOTOR_TASK_PRIO ( tskIDLE_PRIORITY)

#define QR_TASK_PRIO	    ( tskIDLE_PRIORITY + 1)
#define KEY_TASK_PRIO	    ( tskIDLE_PRIORITY + 2)
#define INFRARED_TASK_PRIO	( tskIDLE_PRIORITY + 2)
#define MOTOR_TASK_PRIO		( tskIDLE_PRIORITY + 3)
#define CMD_TASK_PRIO		( tskIDLE_PRIORITY + 4)
#define START_TASK_PRIO		( tskIDLE_PRIORITY + 5)
#define MONITOR_TASK_PRIO	( tskIDLE_PRIORITY + 4)



//�����ջ��С   
#define LED_STK_SIZE 		(1024)
#define MOTOR_STK_SIZE 		(1024) 
#define CMD_STK_SIZE 		(1024*1)
#define INFRARED_STK_SIZE 	(1024)
#define START_STK_SIZE 	    (1024)
#define QR_STK_SIZE 		(1280)
#define READER_STK_SIZE     (1024)
#define HANDSHAKE_STK_SIZE  (1024)
#define KEY_STK_SIZE        (1024)
#define QUERYMOTOR_STK_SIZE      (1024)
#define MONITOR_STK_SIZE   (1024)


//�¼���־
#define TASK_BIT_0	 (1 << 0)
#define TASK_BIT_1	 (1 << 1)
#define TASK_BIT_2	 (1 << 2)
#define TASK_BIT_3	 (1 << 3)
#define TASK_BIT_4	 (1 << 4)
#define TASK_BIT_5	 (1 << 5)
#define TASK_BIT_6	 (1 << 6)
//#define TASK_BIT_7	 (1 << 7)
//#define TASK_BIT_8	 (1 << 8)

//��ȡ���״̬������
#define READ_MOTOR_STATUS_TIMES 20



//#define TASK_BIT_ALL (TASK_BIT_0 | TASK_BIT_1 | TASK_BIT_2 | TASK_BIT_3|TASK_BIT_4 | TASK_BIT_5 | TASK_BIT_6 )
#define TASK_BIT_ALL ( TASK_BIT_0 | TASK_BIT_1 | TASK_BIT_2 |TASK_BIT_3|TASK_BIT_4|TASK_BIT_5|TASK_BIT_6)

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
//������
static TaskHandle_t xHandleTaskMotor = NULL;    //�������
static TaskHandle_t xHandleTaskCmd = NULL;      //androidͨѶ
static TaskHandle_t xHandleTaskInfrared = NULL; //�����ӳ
static TaskHandle_t xHandleTaskReader = NULL;   //Τ��������
static TaskHandle_t xHandleTaskQr = NULL;       //��ά���ͷ
//static TaskHandle_t xHandleTaskRs485 = NULL;    //B�ŵ��
static TaskHandle_t xHandleTaskStart = NULL;    //���Ź�
static TaskHandle_t xHandleTaskHandShake = NULL;    // ����
//static TaskHandle_t xHandleTaskKey = NULL;      //B�Ű���




static EventGroupHandle_t xCreatedEventGroup = NULL; //�����¼�֪ͨ


//������Ϊ�ѻ�ģʽ����£����⣬����������ά��������
//�ѻ�ģʽ���ж��������ı��뷶Χ���Լ���ά��ļ������
#ifdef USEQUEUE
#define MONITOR_TASK_PRIO	( tskIDLE_PRIORITY + 4)
#define MONITOR_STK_SIZE   (1024)
static TaskHandle_t xHandleTaskMonitor = NULL;    //�������
static void vTaskMonitor(void *pvParameters);
static QueueHandle_t xTransQueue = NULL;
#endif


/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

//������
static void vTaskLed(void *pvParameters);
static void vTaskMotorToHost(void *pvParameters);
//static void vTaskKey(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskInfrared(void *pvParameters);
//static void vTaskRs485(void *pvParameters);
static void vTaskReader(void *pvParameters);
static void vTaskQR(void *pvParameters);
static void vTaskStart(void *pvParameters);




//���Ϳ�������
static void vTaskHandShake(void *pvParameters);
static void vTaskQueryMotor(void *pvParameters);



static void AppTaskCreate(void);
static void AppObjCreate (void);
static void App_Printf(char *format, ...);

static void MotorInit(void);

//static void AppEventCreate (void);



int main(void)
{   
    //Ӳ����ʼ��
    bsp_Init();  

	/* ��������ͨ�Ż��� */
	AppObjCreate();

	//�����ʼ��(����)
    MotorInit();

	/* �������� */
	AppTaskCreate();
    
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


    //����LED����
    xTaskCreate((TaskFunction_t )vTaskLed,         
                (const char*    )"vTaskLed",       
                (uint16_t       )LED_STK_SIZE, 
                (void*          )NULL,              
                (UBaseType_t    )LED_TASK_PRIO,    
                (TaskHandle_t*  )&xHandleTaskLed);                   

    //��ѯ���״̬
    xTaskCreate((TaskFunction_t )vTaskQueryMotor,
                (const char*    )"vQueryMotor",       
                (uint16_t       )QUERYMOTOR_STK_SIZE, 
                (void*          )NULL,              
                (UBaseType_t    )QUERYMOTOR_TASK_PRIO,    
                (TaskHandle_t*  )&xHandleTaskQueryMotor);  
    
    //���������Ϣ��������
    xTaskCreate((TaskFunction_t )vTaskMotorToHost,     
                (const char*    )"vTMTHost",   
                (uint16_t       )MOTOR_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MOTOR_TASK_PRIO,
                (TaskHandle_t*  )&xHandleTaskMotor);     

    //��androidͨѶ�������ݽ���
    xTaskCreate((TaskFunction_t )vTaskMsgPro,     
                (const char*    )"cmd",   
                (uint16_t       )CMD_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )CMD_TASK_PRIO,
                (TaskHandle_t*  )&xHandleTaskCmd);      

              

    #ifdef USEQUEUE
    //����߳�
    xTaskCreate((TaskFunction_t )vTaskMonitor,     
                (const char*    )"vTaskMonitor",   
                (uint16_t       )MONITOR_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MONITOR_TASK_PRIO,
                (TaskHandle_t*  )&xHandleTaskMonitor);
    #endif

        

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
        App_Printf("�����¼���־��ʧ��\r\n");
    }

	/* ���������ź��� */
    gxMutex = xSemaphoreCreateMutex();
	
	if(gxMutex == NULL)
    {
        /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
        App_Printf("���������ź���ʧ��\r\n");
    }   
    
    
	/* ���������ź��� */
//    gMutex_Motor = xSemaphoreCreateMutex();
//	
//	if(gMutex_Motor == NULL)
//    {
//        /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
//        App_Printf("���������ź���ʧ��\r\n");
//    }      

    #ifdef USEQUEUE
    /* ������Ϣ���� */
    xTransQueue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* ��Ϣ���еĳ��� */
                              (UBaseType_t ) sizeof(QUEUE_TO_HOST_T *));/* ��Ϣ�Ĵ�С */
    if(xTransQueue == NULL)
    {
        App_Printf("����xTransQueue��Ϣ����ʧ��!\r\n");
    }	
    #endif

}





//motor to host ������
void vTaskMotorToHost(void *pvParameters)
{     
    uint8_t buf[8] = {0};
    uint16_t readLen = 0;
    uint16_t iCRC = 0;
    uint8_t crcBuf[2] = {0};

    #ifdef USEQUEUE
    QUEUE_TO_HOST_T *ptMotorToHost;  
    ptMotorToHost = &gQueueToHost;
    #endif
    
    while (1)
    {   
        readLen = RS485_Recv(COM4,buf,8); 

        if(readLen == 7 || readLen == 8)
        {            
            iCRC = CRC16_Modbus(buf, readLen-2);  

            crcBuf[0] = iCRC >> 8;
            crcBuf[1] = iCRC & 0xff;  

            if(crcBuf[1] == buf[readLen-2] && crcBuf[0] == buf[readLen-1])
            { 
                #ifdef USEQUEUE
                ptMotorToHost->cmd = CONTROLMOTOR;
                memcpy(ptMotorToHost->data,buf,readLen);

    			/* ʹ����Ϣ����ʵ��ָ������Ĵ��� */
    			if(xQueueSend(xTransQueue,              /* ��Ϣ���о�� */
    						 (void *) &ptMotorToHost,   /* ���ͽṹ��ָ�����ptQueueToHost�ĵ�ַ */
    						 (TickType_t)10) != pdPASS )
    			{
                    DBG("��xTransQueue��������ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ���\r\n");                
                } 
                else
                {
//                    DBG("��xTransQueue�������ݳɹ�\r\n");
                      dbh("CONTROLMOTOR",(char *)buf,readLen);
                }    
                #endif

//                dbh("RECV A",(char *)buf,readLen);
                send_to_host(CONTROLMOTOR,buf,readLen);
                vTaskResume(xHandleTaskQueryMotor);//����״̬��ѯ�߳�
                Motro_A = 0;
            }            
        }           

        
        //�����¼���־����ʾ������������      
        xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_1);
        
        vTaskDelay(100);
    }

}









   


   



/*
*********************************************************************************************************
*	�� �� ��: App_Printf
*	����˵��: �̰߳�ȫ��printf��ʽ		  			  
*	��    ��: ͬprintf�Ĳ�����
*             ��C�У����޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void  App_Printf(char *format, ...)
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
//    uint8_t OpenDoor_L[8] =  { 0x01,0x06,0x08,0x0C,0x00,0x02,0xCA,0x68 };
//    uint8_t OpenDoor_R[8] =  { 0x01,0x06,0x08,0x0C,0x00,0x03,0x0B,0xA8 };
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


