/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2007-11-2   10:51
�ļ�����: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\public\init_global.h
�ļ�·��: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\public
file base:init_global
file ext: h
author:	  ������

purpose:	����ṹ����
*********************************************************************/
#ifndef _INIT_GLOBAL_H_
#define _INIT_GLOBAL_H_
#include "comm.h"
#include "spp_comm.h"
BOOL       bRun;				/*����״̬*/
HWND       g_hWnd;				/*���ھ��*/
DWORD      g_dwNode;			/*�ڵ㣨IP��ַ��*/

SPP_BASIC    s_basic;           /*����������Ϣ*/
SPP_DATABASE s_database;        /*���ݿ���Ϣ*/
PUBLIC     *g_Public;           /*��¼���ںͽڵ���Ϣ*/
MESSAGE    *g_Message;			/*��Ϣ*/
NID        *g_Node;				/*�ڵ���Ϣ*/
MID        *g_Module;			/*ģ����Ϣ*/
SPP_IPP    *g_Ipp;              /*��¼������ģ����Ϣ*/
SID_POSITION *g_Sid_Position;   /*�Ự��ID*/
SID_POSITION  *g_Offset_Position; /*��Ϣƫ����*/
// BOOL g_Trace;//�Ƿ����
BOOL bWritConfig;//дҵ�������ļ�
BOOL bLoadService;//����ҵ��
TRACE_CONDITION g_Trace_Infor;//��������
#endif