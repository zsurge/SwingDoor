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
#define APPCREATE_TASK_PRIO		(tskIDLE_PRIORITY)
#define APPCREATE_STK_SIZE 		(configMINIMAL_STACK_SIZE*16)
    


/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
const char *AppCreateTaskName = "vAppCreateTask";      

/*----------------------------------------------*
 * 模块级变量                                   *
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
    //硬件初始化
    bsp_Init();

    //创建任务通信机制  
    AppObjCreate();
    
	//电机初始化(关门)
    MotorInit();

    //创建AppTaskCreate任务
    AppTaskCreate();
//    xTaskCreate((TaskFunction_t )AppTaskCreate,     
//                (const char*    )AppCreateTaskName,   
//                (uint16_t       )APPCREATE_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )APPCREATE_TASK_PRIO,
//                (TaskHandle_t*  )&xHandleTaskAppCreate);   

    
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
    //进入临界区
//    taskENTER_CRITICAL(); 
    
    //握手
    CreateHandShakeTask();

    //与上位机通讯处理
    CreateMsgParseTask();
    
    //电机控制处理
    CreateMotorCtrlTask();

    //方向指示灯
    CreateLedTask();

    //读卡器数据收集
    CreateReaderTask();

    //条码扫描数据处理
    CreateBarCodeTask();

    //红外传感器数据上送
    CreateSensorTask();

    //看门狗
    CreateWatchDogTask();

    //删除本身
//    vTaskDelete(xHandleTaskAppCreate); //删除AppTaskCreate任务

    //退出临界区
//    taskEXIT_CRITICAL();            

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
    
  
    /* 创建消息队列 */
    gxMotorCtrlQueue = xQueueCreate((UBaseType_t ) MOTORCTRL_QUEUE_LEN,/* 消息队列的长度 */
                              (UBaseType_t ) sizeof(MOTORCTRL_QUEUE_T *));/* 消息的大小 */
    if(gxMotorCtrlQueue == NULL)
    {
        AppPrintf("创建xTransQueue消息队列失败!\r\n");
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


