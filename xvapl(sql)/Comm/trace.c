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
#include "yftrace.h"
/********************************************************************
�������ƣ�EnableTrace
��������: ��������
��������: ��
���ض���: ��
����ʱ��: 2008-4-12 21:32
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL EnableTrace(BYTE fileType,BOOL bTrace)
{
	if(!bTrace)//ʹ����Ҫ��־
	{
	//	g_Trace = TRUE;
	//	Openfile(fileType);//�ȴ��ļ�
		FileTypelog = fileType;
		return g_Trace;
	}
	if(g_Trace)//�Ѿ����ڸ���״̬
		return g_Trace;
	if(ReadTraceIni())//����������
	{
		if(g_Trace_Infor.bTrace)
		{
			Openfile(fileType);//�ȴ��ļ�
			g_Trace = TRUE;
//			SetTraceCondition(&g_Trace_Infor);
		}
		else if(g_Trace_Infor.bGetSysInfor)//�õ�ϵͳ��Ϣ
		{
			GetSysInfor();
		}
	}
	return g_Trace;
	
}
/********************************************************************
�������ƣ�DisaleTrace
��������: ֹͣ����
��������: ��
���ض���: ��
����ʱ��: 2008-4-12 21:33
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL DisaleTrace(BOOL bTrace)
{
	if(!bTrace)//�ر���Ҫ��־�ļ�
	{
		Closefile(FileTypelog);//�ȴ��ļ�
		return g_Trace; 
	}
	g_Trace = FALSE;
	Closefile(FileTypeTrace);//�ر��ļ�
	memset(&g_Trace_Infor,0,sizeof(TRACE_CONDITION));
	return g_Trace;
}

/********************************************************************
�������ƣ�TRACE
��������: ��Ҫ��Ϣ���߸�����Ϣ��ʾ
��������: sid:�Ự�ţ�filetype:�ļ����ͣ�msg:������Ϣ����
���ض���: ��
����ʱ��: 2008-4-12 21:33
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT void TRACE(WORD sid,BOOL bImportInfor,const char *function_name,const char *msg)
{
	FILE		*fp=NULL;
	if(bImportInfor)//д��Ҫ��Ϣ
	{
		WriteLog(FileTypelog,function_name,msg);
	}
	else//д���Ը�����־
	{
		WriteTrace(sid,FileTypeTrace,function_name,msg);
	}
}

/********************************************************************
�������ƣ�SetTraceCondition
��������: ���ø�������
��������: pInfor:��������
���ض���: ��
����ʱ��: 2008-4-12 21:47
��������: ������
ע������: ��	
*********************************************************************/

//DLLIMPORT void SetTraceCondition(TRACE_CONDITION *pInfor)
//{
//	if(pInfor == NULL)
//	{
//		g_Trace = FALSE;
//	}
//	else
//	{
//		memcpy(&g_Trace_Infor,pInfor,sizeof(TRACE_CONDITION));
//		g_Trace = TRUE;
//	}
//	
//}
/********************************************************************
�������ƣ�ReadTraceIni
��������: ��ȡ���������ļ�
��������: ��
���ض���: ��
����ʱ��: 2008-4-12 22:08
��������: ������
ע������: ��	
*********************************************************************/

BOOL ReadTraceIni()
{
	SECTION *sec;
	INI ini;
	char TraceName[32];
	char tmpSection[LENGTH_SECTION];
	sprintf(TraceName,"C:\\yfcomm\\ini\\%s",TRANCE_INI);
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
�������ƣ�GetSysInfor
��������: �õ�ϵͳ��Ϣ
��������: ��
���ض���: ��
����ʱ��: 2008-4-12 22:08
��������: ������
ע������: ��	
*********************************************************************/
void GetSysInfor()
{
	int i;
	i=0;
}
/********************************************************************
�������ƣ�WriteLog
��������: ��¼��Ҫ��Ϣ
��������: filetype�����ͣ�msg����¼��Ϣ
���ض���: ��
����ʱ��: 2008-4-12 22:14
��������: ������
ע������: ��	
*********************************************************************/

void WriteLog(BYTE filetype,const char *function_name,const char *msg)
{
	FILE	       *fp=NULL;
	SYSTEMTIME     Clock;
	char		   Title[255];
	switch(filetype)
	{
	case SPP_LOG:
		{
			sprintf(Title,"C:\\yfcomm\\log\\%s",SPP_LOG_FILE);
			fp = fopen (Title,"a");
		}
		break;
	case PXSSM_LOG:
		{
			sprintf(Title,"C:\\yfcomm\\log\\%s",PXSSM_LOG_FILE);
			fp = fopen (Title,"a");
		}
		break;
	default:
		return;
		break;
	}
	GetLocalTime(&Clock);
	if ( fp == NULL ) return;
	fprintf (fp, "%4d-%02d-%02d %02d:%02d:%02d:%03d [%s] %s\n",Clock.wYear,Clock.wMonth, Clock.wDay,
		Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,function_name,msg);
	fclose(fp);
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
void WriteTrace(WORD sid,BYTE filetype,const char *function_name,const char *msg)
{
	FILE			*fp=NULL;
	SYSTEMTIME      Clock;
	BYTE			errno_type ;
	char			telephone[32];
	memset(telephone,0,sizeof(telephone));
	switch(filetype)
	{
	case SPP_TRANCE:
		{
			fp = fp_sppTrace;			
		}
		break;
	case PXSSM_TRANCE:
		{
			fp = fp_pXSSMTrace;
		}
		break;
	default:
		return ;
		break;
	}
	errno_type = GetErrorType(sid);
	GetLocalTime(&Clock);
	if ( fp == NULL ) return;
	switch(errno_type)
	{
	case 0://����������
		break;
	case 1://��������
		{
			fprintf (fp, "%4d-%02d-%02d %02d:%02d:%02d:%03d [%s] [sid]=%d : %s\n",Clock.wYear,Clock.wMonth, Clock.wDay,
		Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,function_name,sid,msg);
		}
		break;
	case 2://���������㵫��sidԽ��,sid==MESSAGE_MAX
		{
			fprintf (fp, "%4d-%02d-%02d %02d:%02d:%02d:%03d [%s] [sid]=%d is uneffect: %s\n",Clock.wYear,Clock.wMonth, Clock.wDay,
		Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,function_name,sid,msg);
		}
		break;
	case 3://������������sid>MESSAGE_MAX
		{
			fprintf (fp, "%4d-%02d-%02d %02d:%02d:%02d:%03d [%s] [sid]=%d is overflow: %s\n",Clock.wYear,Clock.wMonth, Clock.wDay,
		Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,function_name,sid,msg);
		}
	default :
		break;
	}
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
BYTE GetErrorType(WORD sid)
{
	char  telephone[32];
	MESSAGE pMessage;
	BYTE bWrite;
	if(sid==MESSAGE_MAX)
		return 2;
	if(sid >MESSAGE_MAX)
		return 3;
	if(!Read_Message(sid,&pMessage))//��ȡ������Ϣ����
		return 3;

	if(g_Trace_Infor.Tel_Type==1)//���з�ʽ�ж�
	{
		strcpy(telephone,pMessage.head.CallingNum);
	}
	else
	{
		strcpy(telephone,pMessage.head.CalledNum);
	}
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
�������ƣ�Openfile
��������: ���ļ�
��������: filetype:�ļ�����
���ض���: ��
����ʱ��: 2008-4-13 11:13
��������: ������
ע������: ��	
*********************************************************************/
void Openfile(BYTE filetype)
{
	char        Title[255];
	switch(filetype)
	{
	case SPP_LOG:
		{
			FileTypelog = filetype;
			sprintf(Title,"C:\\yfcomm\\log\\%s",SPP_LOG_FILE);
			fp_spplog = fopen (Title,"a");
		}
		break;
	case SPP_TRANCE:
		{
			FileTypeTrace = filetype;
			sprintf(Title,"C:\\yfcomm\\log\\%s",SPP_TRANCE_FILE);
			fp_sppTrace = fopen (Title,"a");
		}
		break;
	case PXSSM_LOG:
		{
			FileTypelog = filetype;
			sprintf(Title,"C:\\yfcomm\\log\\%s",PXSSM_LOG_FILE);
			fp_pXSSMlog = fopen (Title,"a");
		}
		break;
	case PXSSM_TRANCE:
		{
			FileTypeTrace = filetype;
			sprintf(Title,"C:\\yfcomm\\log\\%s",PXSSM_TRANCE_FILE);
			fp_pXSSMTrace = fopen (Title,"a");
		}
		break;
	default:
		return ;
		break;
	}
}
/********************************************************************
�������ƣ�Closefile
��������: �ر��ļ�
��������: filetype:�ļ�����
���ض���: ��
����ʱ��: 2008-4-13 11:14
��������: ������
ע������: ��	
*********************************************************************/
void Closefile(BYTE filetype)
{
	switch(filetype)
	{
	case SPP_LOG:
		{
			fclose(fp_spplog); 
		}
		break;
	case SPP_TRANCE:
		{
			fclose(fp_sppTrace);
		}
		break;
	case PXSSM_LOG:
		{
			fclose(fp_pXSSMlog);
		}
		break;
	case PXSSM_TRANCE:
		{
			fclose(fp_pXSSMTrace);
		}
		break;
	default:
		return ;
		break;
	}
}