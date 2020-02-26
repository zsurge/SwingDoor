#include "bsp_infrared.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "tool.h"

uint32_t readData = 0;

//�½��ش���
uint32_t keyDown = 0;
//������������
uint32_t keyContinue = 0;
//�����ش���
uint32_t keyUp = 0;

void bsp_infrared_init ( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd ( RCC_ALL_SENSOR, ENABLE );	//ʹ��GPIOFʱ��


	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SENSOR1|GPIO_PIN_SENSOR2|GPIO_PIN_SENSOR3|GPIO_PIN_SENSOR4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //����
	GPIO_Init ( GPIO_PORT_SENSOR14, &GPIO_InitStructure );		//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SENSOR5 |GPIO_PIN_SENSOR6;	    //LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //����
	GPIO_Init ( GPIO_PORT_SENSOR56, &GPIO_InitStructure );		//��ʼ��GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SENSOR7 |GPIO_PIN_SENSOR8;	    //LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //����
	GPIO_Init ( GPIO_PORT_SENSOR78, &GPIO_InitStructure );		//��ʼ��GPIO

	GPIO_SetBits ( GPIO_PORT_SENSOR14,GPIO_PIN_SENSOR1|GPIO_PIN_SENSOR2|GPIO_PIN_SENSOR3|GPIO_PIN_SENSOR4 );
	GPIO_SetBits ( GPIO_PORT_SENSOR56,GPIO_PIN_SENSOR5 |GPIO_PIN_SENSOR6 );
	GPIO_SetBits ( GPIO_PORT_SENSOR78,GPIO_PIN_SENSOR7 |GPIO_PIN_SENSOR8 );


}




//��ȡ�����˿�״̬���ذ���ID
static uint32_t bsp_getKeyState ( void )
{
    static uint32_t code = 0;   

    if(GETS1 == 0)
    {
        code |= (0x01<<0);
    }
    else
    {
        code &= ~(0x01<<0);
    }

    if(GETS2 == 0)
    {
        code |= (0x01<<1);
    }
    else
    {
        code &= ~(0x01<<1);
    }
    
    if(GETS3 == 0)
    {
        code |= (0x01<<2);
    }
    else
    {
        code &= ~(0x01<<2);
    }
    

    if(GETS4 == 0)
    {
        code |= (0x01<<3);
    }
    else
    {
        code &= ~(0x01<<3);
    }
    

    if(GETS5 == 0)
    {
        code |= (0x01<<4);
    }
    else
    {
        code &= ~(0x01<<4);
    }

    if(GETS6 == 0)
    {
        code |= (0x01<<5);
    }
    else
    {
        code &= ~(0x01<<5);
    }
    
    if(GETS7 == 0)
    {
        code |= (0x01<<6);
    }
    else
    {
        code &= ~(0x01<<6);
    }
    

    if(GETS8 == 0)
    {
        code |= (0x01<<7);
    }    
    else
    {
        code &= ~(0x01<<7);
    }

////////////////////////////////////
    if(GETS9 == 0)
    {
        code |= (0x01<<8);
    }
    else
    {
        code &= ~(0x01<<8);
    }

    if(GETS10 == 0)
    {
        code |= (0x01<<9);
    }
    else
    {
        code &= ~(0x01<<9);
    }
    
    if(GETS11 == 0)
    {
        code |= (0x01<<10);
    }
    else
    {
        code &= ~(0x01<<10);
    }
    

    if(GETS12 == 0)
    {
        code |= (0x01<<11);
    }
    else
    {
        code &= ~(0x01<<11);
    }
    

    if(GETS13 == 0)
    {
        code |= (0x01<<12);
    }
    else
    {
        code &= ~(0x01<<12);
    }

    if(GETS14 == 0)
    {
        code |= (0x01<<13);
    }
    else
    {
        code &= ~(0x01<<13);
    }
    
    if(GETS15 == 0)
    {
        code |= (0x01<<14);
    }
    else
    {
        code &= ~(0x01<<14);
    }
    

    if(GETS16 == 0)
    {
        code |= (0x01<<15);
    }    
    else
    {
        code &= ~(0x01<<15);
    }
/////////////////////////////////////////////
    if(GETS17 == 0)
    {
        code |= (0x01<<16);
    }
    else
    {
        code &= ~(0x01<<16);
    }

    if(GETS18 == 0)
    {
        code |= (0x01<<17);
    }
    else
    {
        code &= ~(0x01<<17);
    }
    
    if(GETS19 == 0)
    {
        code |= (0x01<<18);
    }
    else
    {
        code &= ~(0x01<<18);
    }
    

    if(GETS20 == 0)
    {
        code |= (0x01<<19);
    }
    else
    {
        code &= ~(0x01<<19);
    }
    

    if(GETS21 == 0)
    {
        code |= (0x01<<20);
    }
    else
    {
        code &= ~(0x01<<20);
    }

    if(GETS22 == 0)
    {
        code |= (0x01<<21);
    }
    else
    {
        code &= ~(0x01<<21);
    }
    
    if(GETS23 == 0)
    {
        code |= (0x01<<22);
    }
    else
    {
        code &= ~(0x01<<22);
    }
    

    if(GETS24 == 0)
    {
        code |= (0x01<<23);
    }    
    else
    {
        code &= ~(0x01<<23);
    }

    return code;

}

#if 0
uint32_t bsp_infrared_scan ( void )
{	
    static uint32_t iret = 0;

	//������Ĳ��֣������������ʵ�ְ���ɨ��
	readData = bsp_getKeyState();

	//��¼�½��ش��� ��������һ�Σ����ֺ�������������ʱ�� = ɨ��ʱ�䣬�൱��һ������
	keyDown  = readData & ( readData ^ keyContinue );
    
	//��¼�����ش��� ��������һ�Σ����ֺ�������������ʱ�� = ɨ��ʱ�� ���൱��һ������
	keyUp =  readData ^ ( readData | keyContinue );	
    
	//�����������¼�¼
	keyContinue = readData;

//ɨ���������ʵ������Զ���
	//�������´���
	if ( keyDown )
	{
		switch ( keyDown )
		{
			case KEY1_ID:
                return iret |= (0x01<<0);
			case KEY2_ID:
				return iret |= (0x01<<1);
			case KEY3_ID:
				return iret |= (0x01<<2);
			case KEY4_ID:
				return iret |= (0x01<<3);
			case KEY5_ID:
				return iret |= (0x01<<4);
			case KEY6_ID:
				return iret |= (0x01<<5);
			case KEY7_ID:
				return iret |= (0x01<<6);
			case KEY8_ID:
				return iret |= (0x01<<7);
			case KEY9_ID:
				return iret |= (0x01<<8);
			case KEY10_ID:
				return iret |= (0x01<<9);
			case KEY11_ID:
				return iret |= (0x01<<10);
			case KEY12_ID:
				return iret |= (0x01<<11);
			case KEY13_ID:
				return iret |= (0x01<<12);
			case KEY14_ID:
				return iret |= (0x01<<13);
			case KEY15_ID:
				return iret |= (0x01<<14);
			case KEY16_ID:
				return iret |= (0x01<<15);
			case KEY17_ID:
				return iret |= (0x01<<16);
			case KEY18_ID:
				return iret |= (0x01<<17);
			case KEY19_ID:
				return iret |= (0x01<<18);
			case KEY20_ID:
				return iret |= (0x01<<19);
			case KEY21_ID:
				return iret |= (0x01<<20);
			case KEY22_ID:
				return iret |= (0x01<<21);
			case KEY23_ID:
				return iret |= (0x01<<22);
			case KEY24_ID:
				return iret |= (0x01<<23);
            default: 
                break;
		}
	}
	//�����������
	else if ( keyUp )
	{
		switch ( keyUp )
		{            
			case KEY1_ID:
				return iret &= ~(0x01<<0);
			case KEY2_ID:
				return iret &= ~(0x01<<1);
			case KEY3_ID:
				return iret &= ~(0x01<<2);
			case KEY4_ID:
				return iret &= ~(0x01<<3);
			case KEY5_ID:
				return iret &= ~(0x01<<4);
			case KEY6_ID:
				return iret &= ~(0x01<<5);
			case KEY7_ID:
				return iret &= ~(0x01<<6);
			case KEY8_ID:
				return iret &= ~(0x01<<7);
			case KEY9_ID:
				return iret &= ~(0x01<<8);
			case KEY10_ID:
				return iret &= ~(0x01<<9);
			case KEY11_ID:
				return iret &= ~(0x01<<10);
			case KEY12_ID:
				return iret &= ~(0x01<<11);
			case KEY13_ID:
				return iret &= ~(0x01<<12);
			case KEY14_ID:
				return iret &= ~(0x01<<13);
			case KEY15_ID:
				return iret &= ~(0x01<<14);
			case KEY16_ID:
				return iret &= ~(0x01<<15);
			case KEY17_ID:
				return iret &= ~(0x01<<16);
			case KEY18_ID:
				return iret &= ~(0x01<<17);
			case KEY19_ID:
				return iret &= ~(0x01<<18);
			case KEY20_ID:
				return iret &= ~(0x01<<19);
			case KEY21_ID:
				return iret &= ~(0x01<<20);
			case KEY22_ID:
				return iret &= ~(0x01<<21);
			case KEY23_ID:
				return iret &= ~(0x01<<22);
			case KEY24_ID:
				return iret &= ~(0x01<<23);
            default:
                break;                
		}

	}

    return ERR_INFRARED;// �ް�������
    
}

#endif

uint32_t bsp_infrared_scan ( void )
{	
    static uint32_t iret = 0;
    static uint32_t Last;

    Last = readData;

//    printf("1.last = %06x,readdata = %06x\r\n",Last,readData);

	//������Ĳ��֣������������ʵ�ְ���ɨ��
	readData = bsp_getKeyState();

//   printf("2.last = %06x,readdata = %06x\r\n",Last,readData);

    if (readData^Last)  return ERR_INFRARED;

	//��¼�½��ش��� ��������һ�Σ����ֺ�������������ʱ�� = ɨ��ʱ�䣬�൱��һ������
	keyDown  = readData & ( readData ^ keyContinue );
    
//   printf("3.readData = %06x,keyContinue = %06x,keyDown = %06x\r\n",readData,keyContinue,keyDown);    
    
	//��¼�����ش��� ��������һ�Σ����ֺ�������������ʱ�� = ɨ��ʱ�� ���൱��һ������
	keyUp =  keyContinue & ( readData ^ keyContinue );	

//   printf("4.readData = %06x,keyContinue = %06x,keyUp = %06x\r\n",readData,keyContinue,keyUp);   
       
	//�����������¼�¼
	keyContinue = readData;

//ɨ���������ʵ������Զ���
	//�������´���
	if ( keyDown )
	{
		switch ( keyDown )
		{
			case KEY1_ID:
                return iret |= (0x01<<0);
			case KEY2_ID:
				return iret |= (0x01<<1);
			case KEY3_ID:
				return iret |= (0x01<<2);
			case KEY4_ID:
				return iret |= (0x01<<3);
			case KEY5_ID:
				return iret |= (0x01<<4);
			case KEY6_ID:
				return iret |= (0x01<<5);
			case KEY7_ID:
				return iret |= (0x01<<6);
			case KEY8_ID:
				return iret |= (0x01<<7);
			case KEY9_ID:
				return iret |= (0x01<<8);
			case KEY10_ID:
				return iret |= (0x01<<9);
			case KEY11_ID:
				return iret |= (0x01<<10);
			case KEY12_ID:
				return iret |= (0x01<<11);
			case KEY13_ID:
				return iret |= (0x01<<12);
			case KEY14_ID:
				return iret |= (0x01<<13);
			case KEY15_ID:
				return iret |= (0x01<<14);
			case KEY16_ID:
				return iret |= (0x01<<15);
			case KEY17_ID:
				return iret |= (0x01<<16);
			case KEY18_ID:
				return iret |= (0x01<<17);
			case KEY19_ID:
				return iret |= (0x01<<18);
			case KEY20_ID:
				return iret |= (0x01<<19);
			case KEY21_ID:
				return iret |= (0x01<<20);
			case KEY22_ID:
				return iret |= (0x01<<21);
			case KEY23_ID:
				return iret |= (0x01<<22);
			case KEY24_ID:
				return iret |= (0x01<<23);
            default: 
                break;
		}
	}
	//�����������
	else if ( keyUp )
	{
		switch ( keyUp )
		{            
			case KEY1_ID:
				return iret &= ~(0x01<<0);
			case KEY2_ID:
				return iret &= ~(0x01<<1);
			case KEY3_ID:
				return iret &= ~(0x01<<2);
			case KEY4_ID:
				return iret &= ~(0x01<<3);
			case KEY5_ID:
				return iret &= ~(0x01<<4);
			case KEY6_ID:
				return iret &= ~(0x01<<5);
			case KEY7_ID:
				return iret &= ~(0x01<<6);
			case KEY8_ID:
				return iret &= ~(0x01<<7);
			case KEY9_ID:
				return iret &= ~(0x01<<8);
			case KEY10_ID:
				return iret &= ~(0x01<<9);
			case KEY11_ID:
				return iret &= ~(0x01<<10);
			case KEY12_ID:
				return iret &= ~(0x01<<11);
			case KEY13_ID:
				return iret &= ~(0x01<<12);
			case KEY14_ID:
				return iret &= ~(0x01<<13);
			case KEY15_ID:
				return iret &= ~(0x01<<14);
			case KEY16_ID:
				return iret &= ~(0x01<<15);
			case KEY17_ID:
				return iret &= ~(0x01<<16);
			case KEY18_ID:
				return iret &= ~(0x01<<17);
			case KEY19_ID:
				return iret &= ~(0x01<<18);
			case KEY20_ID:
				return iret &= ~(0x01<<19);
			case KEY21_ID:
				return iret &= ~(0x01<<20);
			case KEY22_ID:
				return iret &= ~(0x01<<21);
			case KEY23_ID:
				return iret &= ~(0x01<<22);
			case KEY24_ID:
				return iret &= ~(0x01<<23);
            default:
                break;                
		}

	}

    return ERR_INFRARED;// �ް�������
    
}


void bsp_GetSensorStatus ( uint8_t* dat )
{

    static uint32_t code = 0;
    uint8_t bcd[3] = {0};
    uint8_t buf[6] = {0};
    

    code = bsp_getKeyState();

    bcd[0] = code>>16;    
    bcd[1] = code>>8;
    bcd[2] = code&0xff;
    
    memset(buf,0x00,sizeof(buf));

    bcd2asc(buf, bcd, 6, 0);
    
    memcpy(dat,buf,6);
}







