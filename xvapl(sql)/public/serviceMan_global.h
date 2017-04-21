/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2008-12-11   10:02
�ļ�����: E:\XVAPLҵ���߼�ƽ̨\Դ����\PUBLIC\serviceMan_global.h
�ļ�·��: E:\XVAPLҵ���߼�ƽ̨\Դ����\PUBLIC
file base:serviceMan_global
file ext: h
author:	  ������

purpose:SPP��ServiceMan�ӿ��¼�����	
*********************************************************************/
#ifndef _SERVICEMAN_GLOBAL_H_
#define _SERVICEMAN_GLOBAL_H_

#include "event.h"
#include "xvapl_define.h"


#pragma pack(1)
typedef enum
{
  StopServiceRun  = 1  ,  //  ֹͣҵ��
  ActiveService   ,  //  ����ҵ��
  LoadService     ,  //  ����ҵ��
  UnloadService   ,  //  ж��ҵ��
  StartTrace      ,  //  ��������
  CancelTrace     ,  //  ȡ������
  WriteConfig        //  д�����ļ�
}emSERMAN_OperType;
typedef struct 
{
	DWORD serviceID;//ҵ��ID
	enum  emSERMAN_OperType operStatus;//��������
}stSERMAN_STATUS;//����ҵ��״̬
typedef struct 
{
	DWORD serviceID;// ҵ��ID
	WORD  no;//�������
	WORD  addr;//��ʼ��ַ
	WORD  datalen;//���ݳ���
	BYTE  pValue[255];//ֵ
}stSERMAN_VALUE;//��������ֵ


typedef struct
{
 DWORD serviceID;// ҵ��ID
}stSERMAN_ASKALLCONSTATTR;//�����ȡ������������
typedef struct
{
 DWORD serviceID;// ҵ��ID
 WORD  ntotal;//��������
}stSERMAN_ASKALLCONSTATTRAck;//�����������Իظ�

typedef struct
{
 DWORD serviceID;// ҵ��ID
 WORD  starteId;//������ʼ���
 BYTE    btotal;//��ȡ�������Ը���  
} stSERMAN_ASKCONSTATTR;//����������

typedef struct
{
 DWORD serviceID;// ҵ��ID
 WORD  starteId;//������ʼ���
 BYTE    btotal;//��ȡ�������Ը��� 
XSCM_CONSTVARIABLE *pConstAttr;//��������
} stSERMAN_ASKCONSTATTRAck;//�������Իظ�


#pragma pack()

#endif