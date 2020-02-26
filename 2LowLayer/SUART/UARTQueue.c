/*
 ********************************************************************************
 *
 *                                 UARTQueue.c
 *
 * File          : UARTQueue.c
 * Version       : V1.0
 * Author        : whq
 * Mode          : Thumb2
 * Toolchain     : 
 * Description   : ���в���
 *                
 * History       :
 * Date          : 2013.07.22
 *******************************************************************************/
 
 #include <string.h>
 
 #include "UARTQueue.h"
 /*******************************************************************************
 * Function Name : uint32_t QUEUE_PacketCreate(QUEUE8_TYPE *pQ8, uint8_t *pBuf, uint32_t lenSize)
 * Description   : ���н���
 * Input         :   pQ8:    ����
                     pBuf:   ���л�������ַ
                     bufSize:���л�������С
 * Output        : 
 * Other         : 
 * Date          : 2013.08.29
 *******************************************************************************/
 uint32_t QUEUE_PacketCreate(QUEUE8_TYPE *pQ8, uint8_t *pBuf, uint32_t bufSize)
 {
     ASSERT_PARAM(pQ8);
     ASSERT_PARAM(pBuf);
     ASSERT_PARAM(bufSize);
 
     pQ8->bufSize    = bufSize;
     pQ8->pBuf       = pBuf;
     pQ8->pStart     = pBuf;
     pQ8->pEnd       = pBuf;
 
     return 0;
 }
 
 /*******************************************************************************
 * Function Name : uint32_t QUEUE_PacketIn(QUEUE8_TYPE *pQ8, uint8_t *pData, uint32_t len)
 * Description   : �����������
 * Input         :   pQ8:    ����
                     pData:  Ҫ�����е�����
                     len:    ���ݳ���
 * Output        : 
 * Other         : 
 * Date          : 2013.08.29
 *******************************************************************************/
 uint32_t QUEUE_PacketIn(QUEUE8_TYPE *pQ8, uint8_t *pData, uint32_t len)
 {
     uint32_t dataLen = len;
     
     ASSERT_PARAM(pData);
     ASSERT_PARAM(pQ8);
     ASSERT_PARAM(pQ8->pStart);
     ASSERT_PARAM(pQ8->pEnd);
 
     while (dataLen--)
     {
         *pQ8->pEnd++ = *pData++;
         
         if (pQ8->pEnd >= pQ8->pBuf + pQ8->bufSize)      //ָ��ָ��ջβ
         {
             pQ8->pEnd = pQ8->pBuf;
         }
         
         if (pQ8->pEnd == pQ8->pStart)                   //���������� �������������
         {
             pQ8->pStart++;
             if (pQ8->pStart >= pQ8->pBuf + pQ8->bufSize) 
             {
                 pQ8->pStart = pQ8->pBuf;
             }
         }
     }
     
     return len;
 }
 
 /*******************************************************************************
 * Function Name : uint32_t QUEUE_PacketOut(QUEUE8_TYPE *pQ8, uint8_t *pData, uint32_t dataLen)
 * Description   : ������ȡ����
 * Input         :   pQ8:    ����
                     pData:  ������
                     dataLen:��������С
 * Output        : 
 * Other         : 
 * Date          : 2013.08.29
 *******************************************************************************/
 uint32_t QUEUE_PacketOut(QUEUE8_TYPE *pQ8, uint8_t *pData, uint32_t dataLen)
 {
     uint32_t index = 0;
     
     ASSERT_PARAM(pData);
     ASSERT_PARAM(pQ8);
     ASSERT_PARAM(pQ8->pStart);
     ASSERT_PARAM(pQ8->pEnd);
 
     while ((pQ8->pStart != pQ8->pEnd) && (index < dataLen) && (index < pQ8->bufSize))
     {
         pData[index++] = *pQ8->pStart++;
         if (pQ8->pStart >= pQ8->pBuf + pQ8->bufSize) 
         {
             pQ8->pStart = pQ8->pBuf;
         }
     }
 
     return index;
 }
 
 /*******************************************************************************
 * Function Name : uint32_t QUEUE_PacketSplit(QUEUE8_TYPE *pQ8, uint8_t startChar, uint8_t endChar, uint8_t *pData, uint32_t dataLen)
 * Description   : ����ʼ���ͽ�����ȡ�����е����� (ȡ�������� ������ʼ�� �ͷָ���)
 * Input         :   pQ8:        ����
                     startChar:  ��ʼ��
                     endChar:    ������
                     pData:      ������
                     dataLen:    ��������С
 * Output        : 
 * Other         : 
 * Date          : 2013.08.29
 *******************************************************************************/
 uint32_t QUEUE_PacketSplit(QUEUE8_TYPE *pQ8, uint8_t startChar, uint8_t endChar, uint8_t *pData, uint32_t dataLen)
 {
     int32_t count;
     int32_t index;
     volatile uint8_t *pStart;
     volatile uint8_t *pEnd;
     
     ASSERT_PARAM(pData);
     ASSERT_PARAM(pQ8);
     ASSERT_PARAM(pQ8->pStart);
     ASSERT_PARAM(pQ8->pEnd);
 
     pStart      = pQ8->pStart;
     count       = pQ8->bufSize;
     
     while ((pStart != pQ8->pEnd) && count--)        //������ʼ�ַ�
     {
         if (startChar == *pStart) break;
         if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
     }
 
     if (pStart == pQ8->pEnd) return 0;              //δ�ҵ���ʼ��
     if (count == -1) return 0;
     pEnd = pStart;
     if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
 
     while ((pEnd != pQ8->pEnd) && count--)          //���ҽ����ַ�
     {
         if (endChar == *pEnd) break;
         if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
     }
     
     if (pEnd == pQ8->pEnd) return 0;                //δ�ҽ�����
     if (count == -1) return 0;
     if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
     
     count   = pQ8->bufSize - count;   
     index   = 0;
      //��ȡ����ʼ�ַ��������ַ�������
     while ((pStart != pEnd) && (index < dataLen) && (index < pQ8->bufSize) && count--)
     {
         pData[index++] = *pStart++;
         if (pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
     }
 
     pQ8->pStart = pEnd;
     return index;
 }
 
 /*******************************************************************************
 * Function Name : uint32_t QUEUE_PacketCharSplit(QUEUE8_TYPE *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
 * Description   : ��ȡ��β˫�ָ����ڵ�����(�����ָ���)
 * Input         :   pQ8:        ����
                     startChar:  ��ʼ��
                     endChar:    ������
                     pData:      ������
                     dataLen:    ��������С
 * Output        : 
 * Other         : 
 * Date          : 2013.08.30
 *******************************************************************************/
 uint32_t QUEUE_PacketDoubleByteSplit(QUEUE8_TYPE *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
 {
     int32_t count;
     int32_t index;
     volatile uint8_t *pStart;
     volatile uint8_t *pEnd;
     
     ASSERT_PARAM(pData);
     ASSERT_PARAM(pQ8);
     ASSERT_PARAM(pQ8->pStart);
     ASSERT_PARAM(pQ8->pEnd);
 
     pStart      = pQ8->pStart;
     count       = pQ8->bufSize;
     
     while ((pStart != pQ8->pEnd) && count--)        //������ʼ�ַ�
     {
         if (splitChar == *pStart) break;
         if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
     }
 
     if (pStart == pQ8->pEnd) return 0;              //δ�ҵ���ʼ��
     if (count == -1) return 0;
     pEnd = pStart;
     if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
 
     while ((pEnd != pQ8->pEnd) && count--)          //���ҽ����ַ�
     {
         if (splitChar == *pEnd) break;
         if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
     }
     
     if (pEnd == pQ8->pEnd) return 0;                //δ�ҽ�����
     if (count == -1) return 0;
     if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
     
     count   = pQ8->bufSize - count;   
     index   = 0;
      //��ȡ����ʼ�ַ��������ַ�������
     while ((pStart != pEnd) && (index < dataLen) && (index < pQ8->bufSize) && count--)
     {
         pData[index++] = *pStart++;
         if (pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
     }
 
     //���ȡ��������ֻ�����ָ������п������ϴν��������´���ʼ������˷����ϴν�������
     if (index <= 2)                                  
     {
         index = 0;
         if (--pStart < pQ8->pBuf) pStart = pQ8->pBuf + pQ8->bufSize - 1;
     }
 
     pQ8->pStart = pStart;
     return index;
 }
 
 /*******************************************************************************
 * Function Name : uint32_t QUEUE_PacketCharSplit(QUEUE8_TYPE *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
 * Description   : ��ȡ�������ָ��������� (�����ָ���)
 * Input         : 
 * Output        : 
 * Other         : 
 * Date          : 2013.10.20
 *******************************************************************************/
 uint32_t QUEUE_PacketCharSplit(QUEUE8_TYPE *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
 {
     int32_t count;
     int32_t index;
     volatile uint8_t *pStart;
     volatile uint8_t *pEnd;
     
     ASSERT_PARAM(pData);
     ASSERT_PARAM(pQ8);
     ASSERT_PARAM(pQ8->pStart);
     ASSERT_PARAM(pQ8->pEnd);
 
     pStart      = pQ8->pStart;
     count       = pQ8->bufSize;
 
     while ((pStart != pQ8->pEnd) && count--)        //������ʼ�ַ�
     {
         if (splitChar == *pStart) break;
         if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
     }
 
     if (pStart == pQ8->pEnd) return 0;              //δ�ҵ���ʼ��
     if (count == -1) return 0;
     pEnd = pStart;
     if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
     
     pStart      = pQ8->pStart;
     count       = pQ8->bufSize;
     index       = 0;
     while ((pStart != pEnd) && (index < dataLen) && count--)        //������ʼ�ַ�
     {
         pData[index++] = *pStart;
         if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
     }
     
     pQ8->pStart = pStart;
     return index;
 }
 
 /*******************************************************************************
 * Function Name :QUEUE_PacketDoubleCharSplit
 * Description   :��ȡ˫�����ָ��������� (�����ָ���)
 * Input         :QUEUE8_TYPE * pQ8
 * Input         :uint8_t splitChar1
 * Input         :uint8_t splitChar2
 * Input         :uint8_t * pData
 * Input         :uint32_t dataLen
 * Output        :uint32_t
 * Other         :
 * Date          :2014/03/27
 *******************************************************************************/
 uint32_t QUEUE_PacketDoubleCharSplit(QUEUE8_TYPE *pQ8, uint8_t splitChar1, uint8_t splitChar2, uint8_t *pData, uint32_t dataLen)
 {
     int32_t count;
     int32_t index;
     volatile uint8_t *pStart;
     volatile uint8_t *pEnd;
     uint8_t lastChar = 0;
 
     ASSERT_PARAM(pData);
     ASSERT_PARAM(pQ8);
     ASSERT_PARAM(pQ8->pStart);
     ASSERT_PARAM(pQ8->pEnd);
 
     pStart      = pQ8->pStart;
     count       = pQ8->bufSize;
 
     while ((pStart != pQ8->pEnd) && count--)        //������ʼ�ַ�
     {
         if ((splitChar1 == lastChar) && (splitChar2 == *pStart)) break;
 
         lastChar = *pStart;
 
         if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
     }
 
     if (pStart == pQ8->pEnd) return 0;              //δ�ҵ���ʼ��
     if (count == -1) return 0;
     pEnd = pStart;
     if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
 
     pStart      = pQ8->pStart;
     count       = pQ8->bufSize;
     index       = 0;
     while ((pStart != pEnd) && (index < dataLen) && count--)        //������ʼ�ַ�
     {
         pData[index++] = *pStart;
         if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
     }
 
     pQ8->pStart = pStart;
     return index;
 }
 
 
 /*******************************************************************************
 * Function Name : void ASSERT_FAILED(uint8_t* file, uint32_t line)
 * Description   : �쳣
 * Input         : 
 * Output        : 
 * Other         : 
 * Date          : 2013.08.29
 *******************************************************************************/
 void ASSERT_FAILED(uint8_t* file, uint32_t line)
 {
     uint8_t flg = 1;
 
     while (flg);
 }

