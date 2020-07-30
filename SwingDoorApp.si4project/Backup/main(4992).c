/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : main.c
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年7月9日
  最近修改   :
  功能描述   : 主程序模块
  函数列表   :
  修改历史   :
  1.日    期   : 2019年7月9日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "def.h"

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/


static void AppTaskCreate(void);
static void AppObjCreate (void);
static void AppPrintf(char *format, ...);
static void MotorInit(void);


int main(void)
{   
    //硬件初始化
    bsp_Init();  

	/* 创建任务通信机制 */
	AppObjCreate();

	//电机初始化(关门)
    MotorInit();

	/* 创建任务 */
	AppTaskCreate();
    
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
    
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{


}


/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通信机制
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppObjCreate (void)
{
	/* 创建事件标志组 */
	xCreatedEventGroup = xEventGroupCreate();
	
	if(xCreatedEventGroup == NULL)
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
        AppPrintf("创建事件标志组失败\r\n");
    }

	/* 创建互斥信号量 */
    gxMutex = xSemaphoreCreateMutex();
	
	if(gxMutex == NULL)
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
        AppPrintf("创建互斥信号量失败\r\n");
    }   
    
    
	/* 创建互斥信号量 */
//    gMutex_Motor = xSemaphoreCreateMutex();
//	
//	if(gMutex_Motor == NULL)
//    {
//        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
//        AppPrintf("创建互斥信号量失败\r\n");
//    }      

    #ifdef USEQUEUE
    /* 创建消息队列 */
    xTransQueue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* 消息队列的长度 */
                              (UBaseType_t ) sizeof(QUEUE_TO_HOST_T *));/* 消息的大小 */
    if(xTransQueue == NULL)
    {
        AppPrintf("创建xTransQueue消息队列失败!\r\n");
    }	
    #endif

}


//motor to host 任务函数
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

    			/* 使用消息队列实现指针变量的传递 */
    			if(xQueueSend(xTransQueue,              /* 消息队列句柄 */
    						 (void *) &ptMotorToHost,   /* 发送结构体指针变量ptQueueToHost的地址 */
    						 (TickType_t)10) != pdPASS )
    			{
                    DBG("向xTransQueue发送数据失败，即使等待了10个时钟节拍\r\n");                
                } 
                else
                {
//                    DBG("向xTransQueue发送数据成功\r\n");
                      dbh("CONTROLMOTOR",(char *)buf,readLen);
                }    
                #endif

//                dbh("RECV A",(char *)buf,readLen);
                send_to_host(CONTROLMOTOR,buf,readLen);
                vTaskResume(xHandleTaskQueryMotor);//重启状态查询线程
                Motro_A = 0;
            }            
        }           

        
        //发送事件标志，表示任务正常运行      
        xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_1);
        
        vTaskDelay(100);
    }

}


/*
*********************************************************************************************************
*	函 数 名: AppPrintf
*	功能说明: 线程安全的printf方式		  			  
*	形    参: 同printf的参数。
*             在C中，当无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
*	返 回 值: 无
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


    

	/* 互斥信号量 */
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
                readLen = 0;//持续查询
            }            
        }
        else //进行复位
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
  BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdTRUE */
  QUEUE_TO_HOST_T *ptMsg;
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为200ms */  

  while (1)
  {
    xReturn = xQueueReceive( xTransQueue,    /* 消息队列的句柄 */
                             (void *)&ptMsg,  /*这里获取的是结构体的地址 */
                             xMaxBlockTime); /* 设置阻塞时间 */
    if(pdPASS == xReturn)
    {
//        DBG("ptMsg->cmd = %02x\r\n", ptMsg->cmd);
//        dbh("ptMsg->data ", (char *)ptMsg->data,QUEUE_BUF_LEN);

        switch (ptMsg->cmd)
        {
            case GETSENSOR:
                 DBG("红外数据\r\n");
                break;
             case CONTROLMOTOR:
                 DBG("A门电机数据\r\n");
                break;
            case DOOR_B:
                 DBG("B门电机数据\r\n");
                break;
            case WGREADER:
                 DBG("读卡器数据\r\n");
                break;            
            
        }
    }    

  }    
}
#endif


