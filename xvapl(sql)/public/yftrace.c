/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2008-04-12   14:26
�ļ�����: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\SPP\trace.c
�ļ�·��: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\SPP
file base:SPP_SLP
file ext: c
author:	  ������

purpose:	���Ը��ٴ���
*********************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include "Commdll.h"
#include "WatchDefine.h"
#include "event.h"
#include "log.h"
#include "yftrace.h"


BOOL ReadTraceIni()
{
	SECTION *sec;
	INI ini;
	char TraceName[32];
	char tmpSection[LENGTH_SECTION];
	sprintf(TraceName,"C:\\yfcomm\\ini\\%s",TRACE_INI);
	loadINI(&ini,TraceName);
	/*�õ�[TRACE]*/
	sprintf(tmpSection,"TRACE");
	sec=getSection(&ini,tmpSection);
	if(sec==NULL)
	{
		return FALSE;
	}
	{
		g_Trace_Infor.Tel_Type=atoi(GetKeyList(sec,0));//���к����жϻ��Ǳ��к����жϣ�1Ϊ���У�2Ϊ����
		g_Trace_Infor.Tel_Match=atoi(GetKeyList(sec,1));//1��ʾ��ƥ�䣬2��ʾ��ƥ�䣬3��ʾȫƥ�䣬4��ʾȫ���绰����
		strcpy(g_Trace_Infor.Telephone,GetKeyList(sec,2));//ƥ�����
		g_Trace_Infor.bGetSysInfor=(BOOL)atoi(GetKeyList(sec,3));	
		g_Trace_Infor.bTrace=(BOOL)atoi(GetKeyList(sec,4));	
	}

	/*end*/
	freeINI(&ini);
	return TRUE;	
}

/********************************************************************
�������ƣ�EnableTrace
��������: ��������
��������: bTrace : true : �򿪸��٣�false ���رո���
���ض���: ����״̬
����ʱ��: 2008-4-12 21:32
��������: ������
ע������: ��	
*********************************************************************/
BOOL EnableTrace(BOOL bTrace)
{
	if(bTrace)
	{
		ReadTraceIni();  //����������
		g_Trace = TRUE;
	}
	else
		g_Trace = FALSE;
	return g_Trace;
}
BOOL EnableTrace_Spp(BOOL bTrace)
{
	if(bTrace)
	{
		ReadTraceIni();  //����������
		g_Trace_Spp = TRUE;
	}
	else
		g_Trace_Spp = FALSE;
	return g_Trace_Spp;
}
BOOL EnableTrace_Slp(BOOL bTrace)
{
	if(bTrace)
	{
		ReadTraceIni();  //����������
		g_Trace_Slp = TRUE;
	}
	else
		g_Trace_Slp = FALSE;
	return g_Trace_Slp;
}
/********************************************************************
�������ƣ�GetErrorType
��������: �����ж�
��������: sid:�Ự��
���ض���: ��
����ʱ��: 2008-4-13 9:43
��������: ������
ע������: ��	
*********************************************************************/
BYTE GetErrorType(int sid)
{
	char  telephone[32];
//	MESSAGE pMessage;
	BYTE bWrite;
	if(sid==MESSAGE_MAX)
		return 2;
	if(sid >MESSAGE_MAX)
		return 3;
//	if(!Read_Message((WORD)sid,&pMessage))//��ȡ������Ϣ����
//		return 3;

//	if(g_Trace_Infor.Tel_Type==1)//���з�ʽ�ж�
//	{
//		strcpy(telephone,pMessage.head.CallingNum);
//	}
//	else
//	{
//		strcpy(telephone,pMessage.head.CalledNum);
//	}
	switch(g_Trace_Infor.Tel_Match)
	{
	case MATCH_LEFT: // ��ƥ��
		{
			if(strcspn(telephone,g_Trace_Infor.Telephone)==0)
				bWrite = 1;
			else
				bWrite = 0;
		}
		break;
	case MATCH_RIGHT://��ƥ��
		{
			if((strcspn(telephone,g_Trace_Infor.Telephone)+strlen(g_Trace_Infor.Telephone)+1)
				==strlen(telephone))
				bWrite = 1;
			else
				bWrite = 0;
		}
		break;
	case MATCH_ALL://ȫ��ƥ��
		{
			if(strcmp(telephone,g_Trace_Infor.Telephone)==0)
			{
				bWrite = 1;
			}
			else
			{
				bWrite = 0;
			}
		}
		break;
	case MATCH_NONE://����ƥ�䣬ȫ������
		{
			bWrite = 1;
		}
		break;
	default:
		{
			bWrite = 0;
		}
		break;
	}
	return bWrite;
	
}


/********************************************************************
�������ƣ�WriteTrace
��������: ��¼������Ϣ
��������: sid:�Ự�ţ�filetype:�ļ����ͣ�msg:������Ϣ����
���ض���: ��
����ʱ��: 2008-4-12 22:17
��������: ������
ע������: ��	
*********************************************************************/
void WriteTraceInf(const char * tracefile,const char *cfile, const int line,int sid,const char *function_name,const char *fmt,...)
{
	va_list	args;
	char	        msg[1024];
	int		        buf_size=sizeof(msg);
	memset(msg,0,buf_size);
	
	va_start(args,fmt);
	_vsnprintf(msg,1023,fmt,args);
	va_end(args);
	WriteFileLog_Trace(tracefile,cfile, line,"sid=%4d : [%10s] %s",sid,function_name,msg);
}

#define MAXFILELEN 10*1024*1024

/******************************************************************************
*  Function Name:  WriteFilelog()
*  Description	:  write a log information to the log file. 
*  INPUT	:      error messages--errmsg,filename--file, and line count--line.
*  OUTPUT	:      if error return false,else return true. 
*  Note		:
*  Modify	:  
*******************************************************************************/

BOOL WriteFileLog(const char * logname, const char *cfile, const int line,const char *fmt,...)
{
	Log_Message log_msg;
	va_list	args;	
	SYSTEMTIME     Clock;
	char	 	fileName[64];
	char	    buf[1024];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char content[448];
	MESSAGE pMessage;
	TID pTid ;
 	memset(buf,0,sizeof(buf));
	memset(content,0,sizeof(content));
	va_start(args,fmt);
	_vsnprintf(buf,sizeof(buf)-1,fmt,args);
	va_end(args);

	if (!logname) 
		return FALSE;
	memset(&log_msg,0,sizeof(Log_Message));
	log_msg.type = 0;
	log_msg.warnstep = 0;
	log_msg.lines = line;
	
	GetLocalTime(&Clock);

	sprintf(fileName,"%s%04d%02d%02d.log",logname,Clock.wYear,Clock.wMonth, Clock.wDay);
	 _splitpath( cfile, drive, dir, fname, ext ); 
//	strcpy(log_msg.funname,ext);
	sprintf(log_msg.funname,"%8s%4s",fname, ext);
//	sprintf(content, "%02d:%02d:%02d.%03d F[%8s%4s] L[%4d] %s",
//		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,
//			fname,ext,line, buf );
	strcpy(log_msg.content,buf);
	memset(&pMessage,0,sizeof(MESSAGE));
	if(GetTid(MID_COM,&pTid))
	{
		pMessage.head.sender.node = pTid.node;
	
		pMessage.head.Source.node = pTid.node;
		{
			pMessage.head.sender.hWnd= pTid.hWnd;
			pMessage.head.Source.hWnd= pTid.hWnd;
			
		}
		pMessage.head.sender.module=pTid.module;
		pMessage.head.Source.module=pTid.module;
		pMessage.head.Source.mId=pTid.mId;	
	}
	
	if(!GetTid(MID_LOG,&pTid))
		return FALSE;
	
	pMessage.head.receiver.node = pTid.node;
	{
		pMessage.head.receiver.hWnd= pTid.hWnd;
	}

	pMessage.head.receiver.module=pTid.module;
	pMessage.head.receiver.mId=pTid.mId;

	
	pMessage.head.ptrAck=0;
	pMessage.head.ackLen=0;
	pMessage.head.reserve=0;
	pMessage.head.sync=FALSE;
	pMessage.head.event = evLogAsk;
//	memcpy(pMessage.data,fileName,sizeof(fileName));
//	memcpy(pMessage.data+sizeof(fileName),content,strlen(content));
	memcpy(pMessage.data,&log_msg,sizeof(Log_Message));
	pMessage.head.len = sizeof(Log_Message);
	SEND_MSG(evThreadMessage,MID_LOG,&pMessage,sizeof(MESSAGE),/*(WORD)wNumber*/1,FALSE,NULL,0);;
	return TRUE;
}

BOOL WriteFileLog_Trace(const char * logname, const char *cfile, const int line,const char *fmt,...)
{
	Log_Message log_msg;
	va_list	args;	
	SYSTEMTIME     Clock;
	char	 	fileName[64];
	char	    buf[1024];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char content[448];
	MESSAGE pMessage;
	TID pTid ;
 	memset(buf,0,sizeof(buf));
	memset(content,0,sizeof(content));
	va_start(args,fmt);
	_vsnprintf(buf,sizeof(buf)-1,fmt,args);
	va_end(args);

	if (!logname) 
		return FALSE;
	memset(&log_msg,0,sizeof(Log_Message));
	log_msg.type = 2;
	log_msg.warnstep = 0;
	log_msg.lines = line;
	
	GetLocalTime(&Clock);

	sprintf(fileName,"%s%04d%02d%02d.log",logname,Clock.wYear,Clock.wMonth, Clock.wDay);
	 _splitpath( cfile, drive, dir, fname, ext ); 
//	strcpy(log_msg.funname,ext);
	sprintf(log_msg.funname,"%8s%4s",fname, ext);
//	sprintf(content, "%02d:%02d:%02d.%03d F[%8s%4s] L[%4d] %s",
//		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,
//			fname,ext,line, buf );
	strcpy(log_msg.content,buf);
	memset(&pMessage,0,sizeof(MESSAGE));
	if(GetTid(MID_COM,&pTid))
	{
		pMessage.head.sender.node = pTid.node;
	
		pMessage.head.Source.node = pTid.node;
		{
			pMessage.head.sender.hWnd= pTid.hWnd;
			pMessage.head.Source.hWnd= pTid.hWnd;
			
		}
		pMessage.head.sender.module=pTid.module;
		pMessage.head.Source.module=pTid.module;
		pMessage.head.Source.mId=pTid.mId;	
	}
	
	if(!GetTid(MID_LOG,&pTid))
		return FALSE;
	
	pMessage.head.receiver.node = pTid.node;
	{
		pMessage.head.receiver.hWnd= pTid.hWnd;
	}

	pMessage.head.receiver.module=pTid.module;
	pMessage.head.receiver.mId=pTid.mId;

	
	pMessage.head.ptrAck=0;
	pMessage.head.ackLen=0;
	pMessage.head.reserve=0;
	pMessage.head.sync=FALSE;
	pMessage.head.event = evLogAsk;
//	memcpy(pMessage.data,fileName,sizeof(fileName));
//	memcpy(pMessage.data+sizeof(fileName),content,strlen(content));
	memcpy(pMessage.data,&log_msg,sizeof(Log_Message));
	pMessage.head.len = sizeof(Log_Message);
	SEND_MSG(evThreadMessage,MID_LOG,&pMessage,sizeof(MESSAGE),/*(WORD)wNumber*/1,FALSE,NULL,0);;
	return TRUE;
}
BOOL WriteFileLog_WARN(const char * logname, const char *cfile, const int line,const char *fmt,...)
{
	Log_Message log_msg;
	va_list	args;	
	SYSTEMTIME     Clock;
	char	 	fileName[64];
	char	    buf[1024];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char content[448];
	MESSAGE pMessage;
	TID pTid ;
 	memset(buf,0,sizeof(buf));
	memset(content,0,sizeof(content));
	va_start(args,fmt);
	_vsnprintf(buf,sizeof(buf)-1,fmt,args);
	va_end(args);

	if (!logname) 
		return FALSE;
	memset(&log_msg,0,sizeof(Log_Message));
	log_msg.type = 1;
	log_msg.warnstep = 0;
	log_msg.lines = line;
	
	GetLocalTime(&Clock);

	sprintf(fileName,"%s%04d%02d%02d.log",logname,Clock.wYear,Clock.wMonth, Clock.wDay);
	 _splitpath( cfile, drive, dir, fname, ext ); 
//	strcpy(log_msg.funname,ext);
	sprintf(log_msg.funname,"%8s%4s",fname, ext);
//	sprintf(content, "%02d:%02d:%02d.%03d F[%8s%4s] L[%4d] %s",
//		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,
//			fname,ext,line, buf );
	strcpy(log_msg.content,buf);
	memset(&pMessage,0,sizeof(MESSAGE));
	if(GetTid(MID_COM,&pTid))
	{
		pMessage.head.sender.node = pTid.node;
	
		pMessage.head.Source.node = pTid.node;
		{
			pMessage.head.sender.hWnd= pTid.hWnd;
			pMessage.head.Source.hWnd= pTid.hWnd;
			
		}
		pMessage.head.sender.module=pTid.module;
		pMessage.head.Source.module=pTid.module;
		pMessage.head.Source.mId=pTid.mId;	
	}
	
	if(!GetTid(MID_LOG,&pTid))
		return FALSE;
	
	pMessage.head.receiver.node = pTid.node;
	{
		pMessage.head.receiver.hWnd= pTid.hWnd;
	}

	pMessage.head.receiver.module=pTid.module;
	pMessage.head.receiver.mId=pTid.mId;

	
	pMessage.head.ptrAck=0;
	pMessage.head.ackLen=0;
	pMessage.head.reserve=0;
	pMessage.head.sync=FALSE;
	pMessage.head.event = evLogAsk;
//	memcpy(pMessage.data,fileName,sizeof(fileName));
//	memcpy(pMessage.data+sizeof(fileName),content,strlen(content));
	memcpy(pMessage.data,&log_msg,sizeof(Log_Message));
	pMessage.head.len = sizeof(Log_Message);
	SEND_MSG(evThreadMessage,MID_LOG,&pMessage,sizeof(MESSAGE),/*(WORD)wNumber*/1,FALSE,NULL,0);;
	return TRUE;
}
void  WriteErrorInf(const char * logname, const char *cfile, const int line)
{
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    WriteFileLog(logname, cfile, line,"Failed with error %d: %s",dw, lpMsgBuf);
    LocalFree(lpMsgBuf);
}