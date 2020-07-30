/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : Monitor_Task.c
  版 本 号   : 初稿
  作    者   :  
  生成日期   : 2020年5月25日
  最近修改   :
  功能描述   : 监控线程，做为防夹使用
  函数列表   :
  修改历史   :
  1.日    期   : 2020年5月25日
    作    者   :  
    修改内容   : 创建文件

    监控到关门时，启用该任务，读取红外状态，执行防夹功能
    收到关门到位或者开门到位，挂起该任务

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include"Monitor_Task.h"
#include "comm.h"
#include "bsp_sensor.h"
#include "stdlib.h"
#include "bsp_uart_fifo.h"

    
    
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define MONITOR_TASK_PRIO		(tskIDLE_PRIORITY + 4)
#define MONITOR_STK_SIZE 		(configMINIMAL_STACK_SIZE*8)

#define MON_ENABLE  0x55
#define MON_DISABLE  0xAA



/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
const char *MonitorTaskName = "vMonitorTask";    

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
TaskHandle_t xHandleTaskMonitor = NULL; 

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
static void vTaskMonitor(void *pvParameters);



void CreateMonitorTask(void)
{
    //创建电机信息返回任务
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
    uint8_t OpenDoor[8] = { 0x01,0x06,0x08,0x0C,0x00,0x02,0xCA,0x68 }; //正向开门    
    uint8_t ReverseOpenDoor[8] = { 0x01,0x06,0x08,0x0C,0x00,0x03,0x0B,0xA8 }; //正向开门
    
    static uint32_t flag = MON_DISABLE;

    while (1)
    {
        if(flag == MON_DISABLE )
        {
            flag = MON_ENABLE ;
            vTaskSuspend(xHandleTaskMonitor);
        }

        bsp_GetSensorValue(buf);


        //这里需要判定红外的值，来确定是正向开门或者反向开门
        if(atoi((const char*)buf) != 0) 
        {
            printf("waring!!! open door\r\n");
            RS485_SendBuf(COM4, OpenDoor,sizeof(OpenDoor));//打开A电机
            vTaskSuspend(xHandleTaskMonitor);
        }
        
        vTaskDelay(10);
        
    }    
    
}












