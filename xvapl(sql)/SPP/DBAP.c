#include "yftrace.h"
#include "GLM_Log.h"
#include "DBAP.h"


char g_sDBUser[50],g_sDBPwd[50];
BYTE g_DBtns[150];
DBCONNSTATE DBConnState[DBQUERY_THREAD_NUM] = {0};
BOOL g_bDbapTrace = TRUE;
/**************************************************************************
��������DBInitial
��������: ��ʼ���������ݿ�ֲ�����ģ��
��������: ��
���ض���: �ɹ�����TRUE,���쳣����FALSE
����ʱ��: 2008-6-10 
��������: ������
ע������: 
**************************************************************************/
BOOL DBInitial(MESSAGE* pyfMsg,WORD wNum)
{
	int i = 0;
	
	for(i = 0; i < DBQUERY_THREAD_NUM; i++)//����ע��DBM,DBQ�߳�
	{
		DBConnState[i].mId = i + MID_DBQ;
		DBConnState[i].iConnState = 1;  //���ݿ����ӳ�ʼ��Ϊ������״̬
		DBConnState[i].bIsBusy = FALSE;
	}
	
//	Update_Message(pyfMsg,wNum,TRUE);
	
	return TRUE;
}

/**************************************************************************
��������InitThread
��������: ��ʼ����ѯ�߳�DBQ��DBM����̣߳�ע��DBD�߳�
��������: ��
���ض���: 
����ʱ��: 2008-6-13 
��������: ������
ע������: 
**************************************************************************/
void InitThread()
{
	DWORD dwHandle = 0;
	int i = 0;
	BOOL ret = FALSE;
	
	for(i = 0; i < DBQUERY_THREAD_NUM; i++)
	{
		if(_beginthreadex(NULL,0,DBIProc,&DBConnState[i].mId,0,NULL) == 0)
		{
			GLM_WARN "start DBIProc failed");
		}
	}
	if(_beginthreadex(NULL,0,DBMonitorProc,NULL,0,NULL) == 0)
	{
		GLM_WARN "start DBMonitorProc failed");
	}
	
	ret = RegisterThread(MID_DBD);
	
	return;
}
/**************************************************************************
��������DBConnect
��������:��ÿ��DBQ��������.����ǰ�Ѿ��������Ӳ�����,���ؽ�.
��������: no
���ض���: �ɹ�����TRUE,���쳣����FALSE
����ʱ��: 2008-6-10 
��������: ������
ע������: 
**************************************************************************/
BOOL DBConnect(MESSAGE* pyfMsg,WORD wNum)
{
	int i = 0;
	TID * tid = NULL;
	
	for(i = 0; i < DBQUERY_THREAD_NUM; i++)
	{
		tid = GetTID(MID_DBQ + i);
		if(tid == NULL) 
			continue;
		else
			PostThreadMessage(tid->module,evDBQConnection,MID_DBQ+i,0);
	}
	
//	Update_Message(pyfMsg,wNum,TRUE);

	return TRUE;
}


/**************************************************************************
��������DBAvailCheck
��������: ���ָ��ģ���������Ч��
��������: mId,���ݿ����ģ���
���ض���: �ɹ�����TRUE,���쳣����FALSE
����ʱ��: 2008-6-10 
��������: ������
ע������: 
**************************************************************************/
BOOL DBAvailCheck(MESSAGE* pyfMsg,WORD wNum)
{
	TID src;


	UINT i ;
	if(!GetTid(MID_DBD,&src))
	{
		GLM_WARN "GetTID(MID_DBD) or GetTID(MID_SLP)  failed");
		return FALSE;
	}
	pyfMsg->head.event = evDBDConnAvailCheckAck;
	pyfMsg->head.receiver = pyfMsg->head.sender;
	pyfMsg->head.sender = src;
	
	((DBCONNINFO*)(pyfMsg->data))->iConnectionNum = DBQUERY_THREAD_NUM;
	for(i = 0; i < DBQUERY_THREAD_NUM; i++)
	{
		(((DBCONNINFO*)(pyfMsg->data))->sConnInfo)[i] = DBConnState[i].iConnState;
	}
	pyfMsg->head.len = sizeof(DBCONNINFO);

	SEND_MSG(evThreadMessage,pyfMsg->head.receiver.mId,(BYTE*)pyfMsg,sizeof(MESSAGE),wNum,FALSE,NULL,0);

	return TRUE;
}

/**************************************************************************
��������DBDistrQuery
��������: ���ݿ�ֲ�ʽ��ѯ����
��������: ��ѯ��Ϣ
���ض���: �ɹ�����TRUE,���쳣����FALSE
����ʱ��: 2008-6-12 
��������: ������
ע������: 
**************************************************************************/
BOOL DBDistrQuery(MESSAGE* pyfMsg,WORD wNum)
{
	static UINT iCursor = 0; 
	UINT j;
	BOOL bFind = FALSE;
	TID dst ;
	
	YF_TRACE_DBAP -1,"DBDistrQuery","sqlSrc:req row:%d,sql:%s",((SQLSrcMsg*)(pyfMsg->data))->iRow,((SQLSrcMsg*)(pyfMsg->data))->sSql);
	for(j = 0; j < DBQUERY_THREAD_NUM; j++)//��������,���������Ҳ�æ����������в�ѯ,���ҵ�,��bFind = TRUE;
	{
		if((DBConnState[iCursor].iConnState == 1) && (!DBConnState[iCursor].bIsBusy))
		{
			if(!GetTid(DBConnState[iCursor].mId,&dst))
			{
				GLM_WARN "GetTID(MID_DBD,MID_DBQ) execute failed,conn:%d",DBConnState[iCursor].mId);
				return FALSE;
			}
			pyfMsg->head.event = evDBQuery;
			pyfMsg->head.receiver = dst ;

			SEND_MSG(evThreadMessage,DBConnState[iCursor].mId,(BYTE*)pyfMsg,sizeof(MESSAGE),wNum,FALSE,NULL,0);
			iCursor=++iCursor%DBQUERY_THREAD_NUM;
			bFind = TRUE;
			break;
		}
		else
			iCursor=++iCursor%DBQUERY_THREAD_NUM;		
	}	
	if(!bFind)//δ�ҵ������Ҳ�æ������,����һ����������,���������Ƿ�æµ
	{
		for(j = 0; j < DBQUERY_THREAD_NUM; j++)
		{
			if(DBConnState[iCursor].iConnState == 1)
			{
				if(!GetTid(DBConnState[iCursor].mId,&dst))
				{
					GLM_WARN "GetTID(MID_DBD,MID_DBQ) execute failed,conn:%d",DBConnState[iCursor].mId);
					return FALSE;
				}
				pyfMsg->head.event = evDBQuery;
				pyfMsg->head.receiver = dst ;
				
				SEND_MSG(evThreadMessage,DBConnState[iCursor].mId,(BYTE*)pyfMsg,sizeof(MESSAGE),wNum,FALSE,NULL,0);
				iCursor=++iCursor%DBQUERY_THREAD_NUM;
				bFind = TRUE;
				break;
			}
			else
				iCursor=++iCursor%DBQUERY_THREAD_NUM;		
		}
	}

	if(!bFind)//û���ҵ�һ����������.����SLP��ѯʧ��
	{
		dbi_MsgAck_NoConn(MID_DBD,pyfMsg,wNum);
	}
	return TRUE;
}

/**************************************************************************
��������DBInfoMag
��������: 
��������: 
���ض���: 
����ʱ��: 2008-6-10 
��������: ������
ע������: 
**************************************************************************/
void  DBInfoMag(BOOL bDirection)
{
	memset(g_sDBUser,0,sizeof(g_sDBUser));
	memset(g_sDBPwd,0,sizeof(g_sDBPwd));
	dbi_GetDBIni(g_sDBUser,g_sDBPwd);
	return;
}

unsigned  __stdcall /*void*/ DBAPThreadProc(LPVOID lpP)
{
	MESSAGE yfMsg;
	MSG msg;
	int i;
	
	for(i = 0; i < DBQUERY_THREAD_NUM; i++)
	{
		DBConnState[i].mId = i + MID_DBQ;
		DBConnState[i].iConnState = 0;  //���ݿ����ӳ�ʼ��Ϊ������״̬
		DBConnState[i].bIsBusy = FALSE;
	}
	
	DBInfoMag(TRUE);
	
	if(dbi_InitialDB())
		InitThread();
//	g_bDbapTrace = TRUE;
	while(bRun)
	{
		memset(&msg,0,sizeof(MSG));
		memset(&yfMsg,0,sizeof(MESSAGE));
		GetMessage(&msg,NULL,0,0);
		if(msg.message != evThreadMessage)		continue;
		if(!COM_Read_Message((WORD)msg.wParam,&yfMsg))		continue;
		switch ((yfMsg.head).event)
		{
		case  evInitial:  //�̳߳�ʼ���͸�λ��
			DBInitial(&yfMsg,(WORD)msg.lParam);
			break;
		case  evState:   //�߳�״̬��⣬�������������޷�ӳ����ؽ��̽���ֹ���������ý���
			//SEND_MSG(evState,msgPtr->sender,NULL, 0);
			break;

		case evStateReport:
			DBStateReport(&yfMsg);
			break;

		case evTraceStatus:
			DBTraceStatus(&yfMsg);
			break;
			
		case evDBDInital:
			DBInitial(&yfMsg,(WORD)msg.lParam);
			break;
			
		case evDBDConnection:
			DBConnect(&yfMsg,(WORD)msg.lParam);
			break;
			
		case evDBDConnAvailCheck:
			DBAvailCheck(&yfMsg,(WORD)msg.lParam);
			break;
			
		case evDBDAsk://�ֲ�ʽ��ѯ
			DBDistrQuery(&yfMsg,(WORD)msg.lParam);
			break;
			
		default:
			break;
		}
	}  //end of while(1)
	dbi_DBExit();
	return 0;
}
unsigned  __stdcall /*void*/ DBIProc(LPVOID lpP)
{
	
	int mId;
	MESSAGE yfMsg;
	MSG msg;
	mId=*((UINT*)lpP);
	RegisterThread(mId);
	dbi_InitialConn(mId);	
	dbi_CreateConn(mId);
	
	while(bRun)
	{
//		GLM_WARN "DBIProc:mId1=%d",mId);
		memset(&msg,0,sizeof(MSG));
		GetMessage(&msg,NULL,0,0);
		memset(&yfMsg,0,sizeof(MESSAGE));
		if(msg.message != evThreadMessage)		
		{
			if(msg.message == evDBTest)
			{
				dbi_TestConn(msg.wParam);
		//		continue;
			}
			else if(msg.message == evDBQConnection )
			{
				dbi_ReleaseConn(msg.wParam);
				dbi_CreateConn(msg.wParam);
			}
		//	else
//			GLM_WARN "DBIProc:mId2=%d",mId);

			continue;
		}
		if(!COM_Read_Message((WORD)msg.wParam,&yfMsg))	
		{
//			GLM_WARN "DBIProc:mId2=%d",mId);
			continue;
		}
		switch ((yfMsg.head).event)
		{
		case evDBQInital: /*:���ݿ��ʼ��DBQ*/
			dbi_InitialConn(mId);
			break;
			
		case evDBQConnection: /*�������ݿ�����DBQ*/
			dbi_CreateConn(mId);
			break;
			
		case evDBQuery:/*:��DBQ�������ݿ��ѯ���� �������:evDBQueryAck*/

		  //if(1 == DBConnState[*((UINT*)lpP))-MID_DBQ].iConnState)
			if(dbi_ConnAvailCheck(mId))
			{
//				GLM_WARN "DBIProc:mId3=%d",mId);
				if(mId>=MID_DBQ)
					DBConnState[mId - MID_DBQ].bIsBusy = TRUE;
//				GLM_WARN "DBIProc:mId4=%d",mId);
				dbi_ExecSql(mId,&yfMsg,(WORD)msg.lParam);
//				GLM_WARN "DBIProc:mId5=%d",mId);
				if(mId>=MID_DBQ)
					DBConnState[mId - MID_DBQ].bIsBusy = FALSE;
//				GLM_WARN "DBIProc:mId6=%d",mId);
			}
			else
				dbi_MsgAck_NoConn(mId,&yfMsg,(WORD)msg.lParam);
			break;
			
		default:
			break;
		}
//		GLM_WARN "DBIProc:mId2=%d",mId);

	}  //end of while(1)
	return 0;
}

unsigned  __stdcall /*void*/ DBMonitorProc(LPVOID lpP)
{
	int i=0;
	TID  tid ;
    Sleep(10000);
	while(bRun)
	{
		if(!GetTid(MID_DBQ+i,&tid))
			continue;
		else
			PostThreadMessage(tid.module,evDBTest,MID_DBQ+i,0);
		
		Sleep(10000);
		i = ++i % DBQUERY_THREAD_NUM;
	}
	return 0;
}

void DBStateReport(MESSAGE* pyfMsg)
{
//	TID* src = GetTID(MID_DBD);
	TID src;
	if(!GetTid(MID_DBD,&src))
	{
		GLM_WARN "dbi_MsgAck_NoConn:GetTID is NULL");
		return;
	}

	pyfMsg->head.event = evStateReportAck;
	pyfMsg->head.receiver = pyfMsg->head.sender;
	pyfMsg->head.sender =  src;
	
	pyfMsg->head.len = sizeof(DWORD);
	SEND_MSG(evThreadMessage,pyfMsg->head.receiver.mId,(BYTE*)pyfMsg,sizeof(MESSAGE),0,FALSE,NULL,0);
}

void DBTraceStatus(MESSAGE* pyfMsg)
{
	if( pyfMsg->data[0] != 0)
	{
		g_bDbapTrace = TRUE;
	}
	else
	{
		g_bDbapTrace = FALSE;
	}
}