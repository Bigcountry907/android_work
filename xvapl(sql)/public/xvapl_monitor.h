/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2008-12-11   16:50
�ļ�����: E:\XVAPLҵ���߼�ƽ̨\Դ����\PUBLIC\xvapl_monitor.h
�ļ�·��: E:\XVAPLҵ���߼�ƽ̨\Դ����\PUBLIC
file base:xvapl_monitor
file ext: h
author:	  ������

purpose:	��������SvrMan�ӿڵ���Ϣ����
*********************************************************************/
#ifndef _XVAPL_MONITOR_H_
#define _XVAPL_MONITOR_H_
#include "serviceMan_global.h"
void xvapl_monitor_run(PVOID pVoid);

void Monitor_Deal_Message(WPARAM wParam,LPARAM lParam);

BOOL xvapl_svrman_ActivityTestAck(MESSAGE  msg);

BOOL xvapl_svrman_SetStatusAck(MESSAGE msg);

BOOL xvapl_svrman_UpdateService(MESSAGE msg,WORD event);
BOOL SendSvrManAck(MESSAGE msg,BOOL bValue);
#endif