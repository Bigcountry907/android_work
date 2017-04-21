/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2008-12-11   16:57
�ļ�����: E:\XVAPLҵ���߼�ƽ̨\Դ����\SPP\xvapl_monitor.c
�ļ�·��: E:\XVAPLҵ���߼�ƽ̨\Դ����\SPP
file base:xvapl_monitor
file ext: c
author:	  ������

purpose:	��������SvrMan�ӿڵ���Ϣ����
*********************************************************************/
#include "Commdll.h"
#include "yftrace.h"
#include "xvapl_monitor.h"
/********************************************************************
�������ƣ�xvapl_monitor_run
��������: ������ں���
��������: ��
���ض���: wu
����ʱ��: 2008-12-11 17:02
��������: ������
ע������: ��	
*********************************************************************/
void xvapl_monitor_run(PVOID pVoid)
{
	MSG   msg ;
	RegisterThread(MID_MONITOR);
	while(bRun)
	{
		GetMessage(&msg,NULL,0,0);
		if(msg.message!=evThreadMessage)
		{
			continue;
		}
		else//������Ϣ
		{
			Monitor_Deal_Message(msg.wParam,msg.lParam);
		}
	}
}
/********************************************************************
�������ƣ�Deal_Message
��������: ��Ϣ����
��������: wParam:��Ϣ����ƫ������lparam:�Ự��(����)
���ض���: ��
���ض���: ��
����ʱ��: 2008-12-11 17:07
��������: ������
ע������: ��	
*********************************************************************/
void Monitor_Deal_Message(WPARAM wParam,LPARAM lParam)
{
	BOOL bFind = FALSE;
	MESSAGE msg;
	
	
	bFind = COM_Read_Message((WORD)wParam,&msg);
	if(!bFind)
		return;

	switch(msg.head.event)
	{
	case evSerMan_ActivityTest:/*���Ӳ���*/
		{
			
			xvapl_svrman_ActivityTestAck(msg);
		}
		break;
	case evSerMan_SetStatus://����ҵ��״̬
		{
			xvapl_svrman_SetStatusAck(msg);
		}
		break;
	case evSerMan_SetConstValue://���ó���ֵ
	case evSerMan_GetConstValue://��ȡ����ֵ
	case evSerMan_GetConstAttr://��ȡ��������
	case evSerMan_GetAllConstAttr://��ȡ����������������
		{
			xvapl_svrman_UpdateService(msg,msg.head.event);
		}
		break;
	default:
		break;
	}
}
/********************************************************************
�������ƣ�xvapl_svrman_ActivityTestAck
��������: ���Ӳ���ȷ��
��������: msg:��Ϣ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-12-11 17:27
��������: ������
ע������: ��	
*********************************************************************/

BOOL xvapl_svrman_ActivityTestAck(MESSAGE  msg)
{
	TID pTid;
	if(!GetTid(MID_SVRMAN,&pTid))
		return FALSE;
	msg.head.event = evSerMan_ActivityTestAck;
	memcpy(&msg.head.receiver,&pTid,sizeof(TID));
	GetTid(MID_MONITOR,&pTid);
	memcpy(&msg.head.sender,&pTid,sizeof(TID));
	memset(msg.data,0,sizeof(msg.data));
	msg.head.len=0;
	if(!SEND_MSG(evIPCMessage,MID_SVRMAN,(void *)&msg,(WORD)(sizeof(MESSAGE)),(WORD)(MESSAGE_MAX+1),FALSE,NULL,0))
		return FALSE;
	return TRUE;
}
/********************************************************************
�������ƣ�xvapl_svrman_SetStatusAck
��������: ����״̬ȷ��
��������: msg:��Ϣ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-12-11 17:27
��������: ������
ע������: ��	
*********************************************************************/
BOOL xvapl_svrman_SetStatusAck(MESSAGE msg)
{
	BYTE status = 0;
	stSERMAN_STATUS *pStatus = (stSERMAN_STATUS *)msg.data;
	if(pStatus == NULL)
		return FALSE;
	status = pStatus->operStatus;
	switch(status)
	{
	case StartTrace://  ��������
		g_Trace = EnableTrace(TRUE);
		SendSvrManAck(msg,g_Trace);
		break;
	case CancelTrace:   //  ȡ������
		g_Trace = EnableTrace(FALSE);
		SendSvrManAck(msg,g_Trace);
		break;
	case StopServiceRun:  //  ֹͣҵ��
	case ActiveService: //  ����ҵ��
	case WriteConfig://д�����ļ�
	case LoadService://  ����ҵ��
	case UnloadService://  ж��ҵ��
	    xvapl_svrman_UpdateService(msg,evSerMan_SetStatus);
		break;
	default:
		break;
	}
	return TRUE;
}
BOOL SendSvrManAck(MESSAGE msg,BOOL bValue)
{
	TID pTid;
	if(!bValue)
	{
		msg.head.event = evSerMan_ErrorReport;
		msg.head.len = 0;
		memset(msg.data,0,sizeof(msg.data));
	}
	else
	{
		msg.head.event = evSerMan_SetStatusAck;
	}
	if(!GetTid(MID_SVRMAN,&pTid))
		return FALSE;
	memcpy(&msg.head.receiver,&pTid,sizeof(TID));
	GetTid(MID_MONITOR,&pTid);
	memcpy(&msg.head.sender,&pTid,sizeof(TID));
	if(!SEND_MSG(evIPCMessage,MID_SVRMAN,(void *)&msg,(WORD)(sizeof(MESSAGE)),(WORD)(MESSAGE_MAX-1),FALSE,NULL,0))
		return FALSE;
	return TRUE;
}
/********************************************************************
��������:xvapl_svrman_UpdateService
��������:���س���ֵ
��������: msg:��Ϣ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-12-15 10:02
��������: ������
ע������: ��	
*********************************************************************/
BOOL xvapl_svrman_UpdateService(MESSAGE msg,WORD event)
{
	TID pTid;
	stSERMAN_VALUE *pValue=(stSERMAN_VALUE *)msg.data;
	if(pValue == NULL)
		return FALSE;
	if(!GetTid(MID_SLP,&pTid))
		return FALSE;
	msg.head.event = event;
	memcpy(&msg.head.receiver,&pTid,sizeof(TID));
	GetTid(MID_MONITOR,&pTid);
	memcpy(&msg.head.sender,&pTid,sizeof(TID));
	
	if(!SEND_MSG(evThreadMessage,MID_SLP,(void *)&msg,(WORD)(sizeof(MESSAGE)),(WORD)(0),FALSE,NULL,0))
		return FALSE;
	return TRUE;
}