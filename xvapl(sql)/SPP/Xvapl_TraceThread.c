/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2009-3-2   12:01
�ļ�����: E:\XVAPLҵ���߼�ƽ̨\Դ����\SPP\Xvapl_TraceThread.c
�ļ�·��: E:\XVAPLҵ���߼�ƽ̨\Դ����\SPP
file base:Xvapl_TraceThread
file ext: c
author:	  ������

  purpose:	������־��¼
*********************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "public_define.h"
#include "Xvapl_TraceThread.h"
void Trace_ThreadProc(PVOID pVoid)
{
	MSG   msg ;
	
	RegisterThread(MID_TRACE);
	
	while(bRun)
	{
		GetMessage(&msg,NULL,0,0);
		if(msg.message!=evThreadMessage)
		{
			continue;
		}
		else//������Ϣ
		{
			Trace_Deal_Message(msg.wParam,msg.lParam);
		}
	}
}
/********************************************************************
�������ƣ�Trace_Deal_Message
��������: ����Ϣ��������־д���ļ�
��������: wParam����Ϣ��ţ�lParam���Ự��
���ض���: ��
����ʱ��: 2009-3-2 13:58
��������: ������
ע������: ��	
*********************************************************************/

void Trace_Deal_Message(WPARAM wParam,LPARAM lParam)
{
	BOOL bFind = FALSE;
	MESSAGE msg;
	
	
	bFind = COM_Read_Message((WORD)wParam,&msg);
	if(!bFind)
		return;
	
	Trace_WriteFileLog(msg);
}
/********************************************************************
�������ƣ�WriteFileLog
��������:д������־
��������: pMessage����Ϣ
���ض���: �ɹ�����TRUE,ʰܷ���FALSE
����ʱ��: 2009-3-2 14:02
��������: ������
ע������: ��	
*********************************************************************/

BOOL Trace_WriteFileLog(MESSAGE pMessage)
{
	FILE	    *fp=NULL;
	char	 	fileName[64];
	char content[448];
	int len = sizeof(fileName);
	memset(fileName,0,len);
	memset(content,0,sizeof(content));
	memcpy(fileName,pMessage.data,len);
	memcpy(content,pMessage.data+len,pMessage.head.len-len);
	fp = fopen (fileName,"a");
	if ( fp == NULL )
		return FALSE;
	fprintf (fp, "%s\n",content);
	fclose (fp);
	
	return TRUE;
}