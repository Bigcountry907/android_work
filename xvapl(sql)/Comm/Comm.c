/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2007-10-29   17:11
�ļ�����: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\Comm\Comm.c
�ļ�·��: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\Comm
file base:Comm
file ext: c
author:	  ������

  purpose:	ͨѶDLL����
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <process.h>
#include "../public/Commdll.h"
#include "../public/SppComm.h"
#include "../public/init_viriable.h"
#include "../public/yftrace.h"
BOOL    bIsInit = FALSE;
DWORD   dwThreadId = 0;
HANDLE  hMutex;
TMCtrl *pHead = NULL, *pCur = NULL,*pTail = NULL;
HWND    huserWnd;
int     createlable=1;
/********************************************************************

�������ƣ�DllMain
��������: ��ں���
��������: ��
���ض���: ��
����ʱ��: 2007-10-29 17:11
��������: ������
ע������: ��
	
*********************************************************************/

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
	int a=0;

	node_numer=0;
	iHaveNodeNumber=0;
    
    if(createlable)
	{
	  hMutex=CreateMutex(NULL,FALSE,"mutex");
	  createlable=0;
	}
	switch (reason)
    {
	case DLL_PROCESS_ATTACH:
        break;
		
	case DLL_PROCESS_DETACH:
        break;
		
	case DLL_THREAD_ATTACH:
        break;
		
	case DLL_THREAD_DETACH:
        break;
    }
	
    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}

/********************************************************************
�������ƣ�SetWindowsHandle
��������: ���ö�̬�������Ĵ��ھ��
��������: hWnd�����ھ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-11-27 18:02
��������: ������
ע������: ��	
*********************************************************************/
BOOL SetWindowsHandle(HWND hWnd)
{
	if(!Share_Merry())
	{
		return FALSE;
	}
	huserWnd = hWnd;
	return TRUE;
}
/********************************************************************
�������ƣ�GetWindowsHandle
��������: �õ�����̬�������Ľ��̵Ĵ��ھ��
��������: ��
���ض���: ���ر���̬�������Ľ��̵Ĵ��ھ��
����ʱ��: 2008-11-28 10:36
��������: ������
ע������: ��	
*********************************************************************/
HWND GetWindowsHandle()
{
	return huserWnd ;
}
void WriteSendMessageState(WORD sid,int sen_mod,BYTE recv_mod,char *value)
{
	FILE *fp;
	char buffer[512];
	SYSTEMTIME     Clock;
	char data[100];
	
	fp=fopen("C:\\yfcomm\\log\\Commerr.log","a");
	GetLocalTime(&Clock);
	memset(data,0,sizeof(data));
	if(value!=NULL)
	{
		sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d:%03d [sid=%d] [sen_mod=%d] [recv_mod = %d] ,%s\n",Clock.wYear,Clock.wMonth, Clock.wDay,
			Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,sid,sen_mod,recv_mod,value);
	}
	else
	{
		sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d:%03d [sid=%d] [sen_mod=%d] [recv_mod = %d] \n",Clock.wYear,Clock.wMonth, Clock.wDay,
			Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,sid,sen_mod,recv_mod);
	}
	if(fp!=NULL)
	{
		fwrite(buffer,strlen(buffer),1,fp);
		fclose(fp);
	}
}
void BCD_TO_ASCIIString(unsigned char *cSource, unsigned int iSourceLen,BOOL bTx)
{
	char mmmmm[3000];
	char cResult[3000];
	unsigned int i=0;
	char bSign[10];
    int j=0;
	FILE *fp;
	SYSTEMTIME     Clock;
	char content[255];
	memset(mmmmm, 0, 3000);
	memset(bSign,0,sizeof(bSign));
	memset(cResult,0,sizeof(cResult));
    for(i=0;i<iSourceLen;i++) 
    {
        cResult[j+2]=32;
        cResult[j+1] = cSource[i]%16 + '0';
        if(cResult[j+1] > 9+'0')
        {
            cResult[j+1] = cResult[j+1]+7;
        }
        cResult[j] = cSource[i]/16+'0';
        if(cResult[j] > 9+'0')
        {
            cResult[j] = cResult[j]+7;
        }
        j+=3;
    }
    cResult[j] = '\0';
	fp=fopen("C:\\yfcomm\\log\\CommData.log","a");
	GetLocalTime(&Clock);
	if(fp!=NULL)
	{
		memset(content,0,sizeof(content));
		sprintf(content,"\n%04d-%02d-%02d %02d:%02d:%02d:%03d\n",Clock.wYear,Clock.wMonth, Clock.wDay,Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds);
		fwrite(content,strlen(content),1,fp);
		if(bTx)
		{
			sprintf(bSign,"TX:");
		}
		else
		{
			sprintf(bSign,"\nRX:");
		}
		fwrite(bSign,strlen(bSign),1,fp);
		fwrite(cResult,strlen(cResult),1,fp);
		
		fclose(fp);
	}
} 
/********************************************************************
�������ƣ�SEND_MSG
��������:������Ϣ
��������: event:�¼���
receiverModule:������Ϣģ���
data:��Ϣ����
len: ������Ϣ����
ref: ��������
bSync:�Ƿ�ͬ��
ack:��������ָ��
ackLen:�������ݳ���
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-10-20 14:38
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL SEND_MSG(WORD event,UINT receiverModule,void *data,WORD len,WORD ref,BOOL sync, BYTE far *ack,WORD ackLen)
{
	
	BYTE bModNo;
	BOOL bSocketComm = FALSE;
	BOOL bSuc = FALSE;
	TID pTid ;
	int sid;
	HWND   hWnd;
	char data12[30];
	MESSAGE *pMessage;
	int sen_mod = -1;

	memset(data12,0,sizeof(data12));
	hWnd=GetTopWindow(NULL);
	if(receiverModule>=MODULE_MAX)
	{
		bSocketComm = TRUE;
	}
	bModNo = receiverModule;
	if((MODULE_MAX<= bModNo)/*||(ref >= MESSAGE_MAX)*/)
		return FALSE;
	if((g_Module==NULL)||(g_Node==NULL))
	{
		if(!Share_Merry())
		{
			YF_LOG_SPP "E00000003:[SEND_MSG] %s","�����ڴ�û�ҵ�,������Ϣʧ��");
			return FALSE;
		}
	}
	pMessage = (MESSAGE *)data;
	if(pMessage!=NULL)
		sen_mod=pMessage->head.sender.mId;
	else
		sen_mod = -1;

	sid = Add_Message(data,len);

	if(sid < 0)
	{
		YF_LOG_SPP "E00000004:�Ҳ������е���Ϣ�ڴ�ռ䣬��Ϣ����ʧ��");
		return FALSE;
	}
	if(bSocketComm)
	{
		bSuc=SendSocketMessage(event,bModNo,ref,(WORD)sid);//�ڵ�ͨѶ
	}
	else
	{
		if(!GetTid(bModNo,&pTid))
		{
			YF_LOG_SPP "E00000006:mod id=%d,ģ���ûע�ᣬ�ò���ģ�������Ϣ",bModNo);
			return FALSE;
		}
		if(pTid.hWnd==huserWnd)//����ģ��ͷ���ģ����ͬһ������һ��
		{
			
			if(!g_Node[0].isSPP)//is not spp sever
			{
				bSuc=FALSE;
			}
			else
			{
				bSuc=SendThreadMessage(evThreadMessage,bModNo,ref,(WORD)sid);
			}
		}
		else //��ϵͳģ��
		{
			if(g_Module[bModNo].tId.node==g_Public->g_dwNode)
			{
				bSuc=SendForkMessage(evIPCMessage,bModNo,ref,(WORD)sid);//����ͨѶ
			}
			else
			{
				bSuc=SendSocketMessage(evSocketMessage,bModNo,ref,(WORD)sid);//�ڵ�ͨѶ
			}
		}
	}
	if(!bSuc)
	{
		Clean_Message((WORD)sid);
	}
	pMessage = NULL;
	return bSuc;
}
/********************************************************************
�������ƣ�SEND_SOCKET_MSG
��������: ���⴦���������̷߳�����Ϣ
��������: iThreadId���̺߳�
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2009-6-12 16:29
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL SEND_SOCKET_MSG(WORD event,DWORD iThreadId,void *data,WORD len,WORD ref,BOOL sync, BYTE far *ack,WORD ackLen)
{
	BOOL bSuc = FALSE;
	int sid;
	if((g_Module==NULL)||(g_Node==NULL))
	{
		if(!Share_Merry())
		{
			//			YF_LOG "[SEND_MSG] %s","�����ڴ�û�ҵ�,������Ϣʧ��");
			return FALSE;
		}
	}
	
	sid = Add_Message(data,len);
	if(sid < 0)
	{
		return FALSE;
	}
	bSuc = PostThreadMessage(iThreadId,event,ref,sid);
	if(!bSuc)
	{
		Clean_Message((WORD)sid);
	}
	return bSuc;
}
/********************************************************************
�������ƣ�SendThreadMessage
��������: ͬ�����ڷ����߳���Ϣ
��������: event:�¼��ţ�receiverModule:������Ϣģ���
wNumber����Ϣƫ����
���ض���: �ɹ�����TURE�����󷵻�FALSE
����ʱ��: 2007-11-8 18:54
��������: ������
ע������: ��	
*********************************************************************/
BOOL SendThreadMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD sid)
{
	BOOL bSuc=FALSE;
	if(g_Module[receiverModule].state == 1)
	{
		if(g_Module[receiverModule].tId.module == g_Module[receiverModule].tId.mId)//���̷���
			bSuc = PostMessage(g_Module[receiverModule].tId.hWnd,event,sid,SenssionID);
		else
			bSuc = PostThreadMessage(g_Module[receiverModule].tId.module,event,sid,SenssionID);
	}
	return bSuc;
}
/********************************************************************
�������ƣ�SendForkMessage
��������: ���̼䷢���߳���Ϣ
��������: event:�¼��ţ�receiverModule:������Ϣģ���
wNumber����Ϣƫ����
���ض���: �ɹ�����TURE�����󷵻�FALSE
����ʱ��: 2007-11-8 18:54
��������: ������
ע������: ��	
*********************************************************************/
BOOL SendForkMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD sid)
{
	BOOL bSuc = FALSE;
	TID tId;
	tId = g_Module[receiverModule].tId;
	bSuc=PostMessage(tId.hWnd,event,sid,SenssionID);
	return bSuc;
}
/********************************************************************
�������ƣ�SendSocketMessage
��������: �ڵ�֮�䷢���߳���Ϣ
��������: event:�¼��ţ�receiverModule:������Ϣģ���
wNumber����Ϣƫ����
���ض���: �ɹ�����TURE�����󷵻�FALSE
����ʱ��: 2007-11-8 18:54
��������: ������
ע������: ��	
*********************************************************************/
BOOL SendSocketMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD sid)
{
	
	TID tId;
	
	DWORD module;
	int i;
	BYTE bSuc = FALSE;
	int NodeNum= -1;
	BYTE MoudleNum;
	if(receiverModule>=MODULE_MAX)
	{
		NodeNum = receiverModule/0xff;
		MoudleNum = receiverModule%0xff; 
		
	}
	if(receiverModule<MODULE_MAX)
	{
		tId = g_Module[receiverModule].tId;
		for(i=0;i<MODULE_MAX;i++)
		{
			if(g_Node[i].node==tId.node)
			{
				module = g_Node[i].module;
				bSuc = TRUE;
				break;
			}
		}
	}
	else
	{
		bSuc = FALSE;
	}
	if(bSuc)
	{
		if(GetTid(MODULE_MAX,&tId))
		{
			if(tId.hWnd==huserWnd)//����ģ��ͷ���ģ����ͬһ������һ��
			{
				bSuc = PostThreadMessage(module,event,sid,SenssionID);
			}
			else
			{
				bSuc=PostMessage(tId.hWnd,event,sid,SenssionID);
			}
		}
		else
			bSuc = FALSE;
	}
	return bSuc;
}
/********************************************************************
�������ƣ�RegisterIPC
��������: ע��һ������
��������: �߳���
���ض���: TID,����ע��ɹ����TID��Ϣ
����ʱ��: 2007-10-29 17:13
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL RegisterIPC(UINT mId,HWND hwnd)
{
	TID tId;
	BOOL bSysModule = TRUE;
	if(g_Module==NULL)
	{
		if(!Share_Merry())
		{
			//YF_LOG "[RegisterIPC] %s","�����ڴ�û�ҵ�,ע�����ʧ��");
			return FALSE;
		}
	}
	if(mId>=MODULE_MAX)
		return FALSE;
	if(mId>=SYSMODULE)
	{
		bSysModule=FALSE;
	}
	tId.hWnd=hwnd;
	tId.node=g_Public->g_dwNode;
	tId.module=mId;
	tId.mId = mId;
	g_Module[mId].mId=mId;
	g_Module[mId].tId = tId;
	g_Module[mId].isSysModule=bSysModule;
	g_Module[mId].state=1;
	SetWindowsHandle(hwnd);
	return TRUE;	
}

/********************************************************************
�������ƣ�UnRegisterIPC
��������: ȡ������ע��
��������: mId:ģ���
���ض���: ��
����ʱ��: 2007-12-6 10:29
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL UnRegisterIPC(UINT mId)
{
	return UnRegisterThread(mId);
}

/********************************************************************
�������ƣ�RegisterThread
��������: ע��һ���߳�
��������: �߳���
���ض���: TID,����ע��ɹ����TID��Ϣ
����ʱ��: 2007-10-29 17:13
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL RegisterThread(UINT mId)
{
	TID tId;
	BOOL bSysModule = TRUE;
	if(g_Module==NULL)
	{
		if(!Share_Merry())
		{
			//YF_LOG "RegisterThread %s","�����ڴ�û�ҵ�,ע���߳�ʧ��");
			return FALSE;
		}
	}
	if(mId>=(MODULE_MAX+1))
		return FALSE;
	if(mId>=SYSMODULE)
	{
		bSysModule=FALSE;
	}
	
	//	tId.hWnd=g_Public->g_hWnd;
	tId.hWnd=huserWnd;
	tId.node=g_Public->g_dwNode;
	tId.module=GetCurrentThreadId();
	tId.mId = mId;
	g_Module[mId].mId=mId;
	g_Module[mId].tId = tId;
	g_Module[mId].isSysModule=bSysModule;
	g_Module[mId].state=1;
	return TRUE;	
}
DLLIMPORT BOOL RegisterSocketThread(UINT mId)
{
	TID tId;
	BOOL bSysModule = TRUE;
	if(g_Module==NULL)
	{
		if(!Share_Merry())
		{
			return FALSE;
		}
	}
	if(mId!=MODULE_MAX)
		return FALSE;
	
	tId.hWnd=huserWnd;
	tId.node=g_Public->g_dwNode;
	tId.module=GetCurrentThreadId();
	tId.mId = mId;
	g_Module[mId].mId=mId;
	g_Module[mId].tId = tId;
	g_Module[mId].isSysModule=bSysModule;
	g_Module[mId].state=1;
	return TRUE;	
}
DLLIMPORT BOOL UnRegisterThread(UINT mId)
{
	if(mId < MODULE_MAX+1)
	{
		memset(&g_Module[mId].tId,0,sizeof(TID));
		g_Module[mId].state = 0;
	}
	else
	{
		return FALSE;
	}
	return  TRUE;
}
/********************************************************************
�������ƣ�RegisterNodeTread
��������: ע����������ڵ�ģ��
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2009-4-7 9:56
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL RegisterNodeMod(UINT mId,DWORD nodeip)
{
	BOOL bSysModule = FALSE;
	if(g_Module==NULL)
	{
		if(!Share_Merry())
		{
			return FALSE;
		}
	}
	if(mId>=MODULE_MAX)
		return FALSE;
	if(mId>=SYSMODULE)
	{
		bSysModule=FALSE;
	}
	g_Module[mId].tId.node = nodeip;
	g_Module[mId].tId.mId = mId;
	g_Module[mId].mId = mId;
	g_Module[mId].state = 1;
	g_Module[mId].isSysModule = bSysModule;
	return TRUE;
}
DLLIMPORT BOOL UnRegisterNodeMod(UINT mId)
{
	if(mId < MODULE_MAX)
	{
		memset(&g_Module[mId].tId,0,sizeof(TID));
		g_Module[mId].state = 0;
	}
	else
	{
		return FALSE;
	}
	return  TRUE;
}
/********************************************************************
�������ƣ�Comm_SetTraceValue
��������: ���ø��ٿ���
��������: bValue���Ƿ�򿪸��٣�TRUE�򿪸��٣�FALSE�رո���
���ض���: ��
����ʱ��: 2009-4-10 10:55
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT void Comm_SetTraceValue(BOOL bValue)
{
	EnableTrace(bValue);
 
}
/********************************************************************
�������ƣ�GetMid
��������: ��ȡģ���������
��������: mId:ģ���,pMid:��ȡMID������
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-12-4 14:54
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL GetMid(UINT mId,MID *pMid)
{
	BOOL bOk = FALSE;
	if(g_Module==NULL)
		return FALSE;
	if(pMid == NULL)
		return FALSE;
	if(mId<MODULE_MAX)
	{
		if(g_Module[mId].state==1)
		{
			memcpy(pMid,&(g_Module[mId]),sizeof(MID));
			bOk = TRUE;
		}
		else
		{
			pMid = NULL;
		}
	}
	else
	{
		pMid = NULL;
	}
	return bOk;
}
/********************************************************************
�������ƣ�GetTid
��������: ����ģ��ŵõ�ע��ID
��������: mId:ģ���,pTid:��ȡTID������
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-10-21 9:45
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL GetTid(UINT mId,TID *pTid)
{
	BOOL bOk = FALSE;
	if(g_Module==NULL)
		return FALSE;
	if(pTid == NULL)
		return FALSE;
	if(mId<(MODULE_MAX+1))
	{
		if(g_Module[mId].state==1)
		{
			memcpy(pTid,&(g_Module[mId].tId),sizeof(TID));
			bOk = TRUE;
		}
		else
		{
			pTid = NULL;
		}
	}
	else
	{
		pTid = NULL;
	}
	return bOk;
}
/********************************************************************
�������ƣ�GetTID
��������: ����ģ��ŵõ�ע��ID
��������: mId:ģ���
���ض���: pTid
����ʱ��: 2007-10-29 17:30
��������: ������
ע������: ��	
*********************************************************************/

DLLIMPORT TID *GetTID(UINT mId)
//DLLIMPORT BOOL GetTID(UINT mId,TID & pTid)
{
	
	if(g_Module==NULL)
		return NULL;
	if(mId<MODULE_MAX)
	{
		if(g_Module[mId].state==1)
		{
			return &(g_Module[mId].tId);
			
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
/********************************************************************
�������ƣ�Read_Message
��������: ��ȡ��Ϣ��������
��������: ��Ϣƫ����
���ض���: �з�����Ϣ���ݣ��޷��ؿ�ָ��
����ʱ��: 2007-11-8 20:03
��������: ������
ע������: ��	
*********************************************************************/


DLLIMPORT BOOL COM_Read_Message(WORD wNumber,MESSAGE *pMsg)
{
	if(g_Message==NULL)
	{
		if(!Share_Merry())
		{	
			return FALSE;
		}
	}
	if(MESSAGE_MAX>wNumber)
	{	
		memcpy( pMsg,g_Message+wNumber,sizeof(MESSAGE));
		Clean_Message(wNumber);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
/********************************************************************
�������ƣ�GetSenssionID
�������ܣ�����һ���Ự��   					
������������         								
����ֵ  ���ɹ����ػỰ��>=0��ʧ�ܷ���-1								
����ʱ��: 2008-5-5 15:43
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT int GetSenssionID()
{
	int SenssionId = -1;
	
    SenssionId=GetSenssionNumber();
	if(SenssionId < 0)
	{
		return -1;
	}
	return SenssionId;
}
/********************************************************************
�������ƣ�ReleaseSenssionID
�������ܣ��ͷŻỰ��   					
����������sid:�Ự��        								
����ֵ  ���ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2008-5-5 15:45
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL ReleaseSenssionID(WORD SenssionID)
{
	if(SenssionID>=MESSAGE_MAX)
		return FALSE;
	return(SetSenssionId(SenssionID));
}

/********************************************************************
�������ƣ�Add_Message
��������: �����Ϣ
��������: pMsg:��Ϣ����
���ض���: �ɹ�������Ϣ��ƫ����,�Ǳ�����-1
����ʱ��: 2007-11-8 20:05
��������: ������
ע������: ��	
*********************************************************************/
int Add_Message(void *pMsg,int len)
{
	int iNo = -1;

	iNo = GetOffsetId();
	if(iNo < 0)
	{
		return -1;
	}
	else
	{
		if(iNo<MESSAGE_MAX)
		{
			if(len<=sizeof(MESSAGE))
			  memcpy(g_Message+iNo,pMsg,len);
			else
			  memcpy(g_Message+iNo,pMsg,sizeof(MESSAGE));			
		}
		else
		{
			return -1;
		}
	}
	return iNo;
}
/********************************************************************
�������ƣ�Update_Message
��������: ��Ϣ���ݸ��£�����ɾ���͸����ж�
��������: pMsg:��Ϣ����,wNumber:��Ϣƫ������
bDelete:TRUEΪ�����Ϣ��FALSEΪ������Ϣ
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-11-8 20:07
��������: ������
ע������: ��	
*********************************************************************/
BOOL Update_Message_COM(MESSAGE *pMsg,WORD wNumber,BOOL bDelete)
{
	BOOL bTrue=FALSE;
	if(g_Message==NULL)
	{
		if(!Share_Merry())
		{
			//			YF_LOG "[Update_Message]%s ","�����ڴ�û�ҵ�,update��Ϣʧ��");
			return FALSE;
		}
	}
	if(MESSAGE_MAX<=wNumber)
	{
		return bTrue;
	}
	if(bDelete)//�����Ϣ
	{
		//call clean message
		bTrue=Clean_Message(wNumber);
	}
	else//������Ϣ
	{
		//call updatemessage;
		bTrue=UpdateMessage(wNumber,pMsg);
	}
	return  bTrue;
}
/********************************************************************
�������ƣ�FreeDll
��������: �ͷŹ����ڴ�
��������: ��
���ض���: ��
����ʱ��: 2007-11-20 10:24
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT void FreeDll()
{
	Detach_Merry();
}
/********************************************************************
�������ƣ�Clean_Message
��������: �����Ϣ����
��������: wNumber:��Ϣƫ����
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-11-8 20:10
��������: ������
ע������: ��	
*********************************************************************/

BOOL Clean_Message(WORD wNumber)
{
	if(MESSAGE_MAX<=wNumber)
	{
		return FALSE;
	}
	memset((g_Message+wNumber),0,sizeof(MESSAGE));
	SetOffsetId(wNumber);
	return TRUE;
}
/********************************************************************
�������ƣ�UpdateMessage
��������: ������Ϣ
��������: wNumber:��Ϣƫ������pMsg:���µ���Ϣ����
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-11-8 20:11
��������: ������
ע������: ��	
*********************************************************************/
BOOL UpdateMessage(WORD wNumber,const MESSAGE *pMsg)
{
	if(MESSAGE_MAX<=wNumber)
	{
		return FALSE;
	}
	memcpy(g_Message+wNumber,pMsg,sizeof(MESSAGE));
	return TRUE;
}
/********************************************************************
�������ƣ�Share_Merry
��������: ���빲���ڴ�
��������: ��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-11-8 19:48
��������: ������
ע������: ��	
*********************************************************************/

BOOL Share_Merry()
{
	
	hPublicMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_PUBLIC);
	if(hPublicMapping==NULL)
	{
		// 		MessageBox(NULL,"openfile map errno","warn",MB_ICONERROR);
		return FALSE;
	}
	g_Public=(PUBLIC *)MapViewOfFile(hPublicMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Public==NULL)
	{
		CloseHandle(hPublicMapping);
		return FALSE;
		
	}
	hMessageMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_MESSAGE);
	if(hMessageMapping==NULL)
	{
		return FALSE;
	}
	g_Message=(MESSAGE *)MapViewOfFile(hMessageMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Message==NULL)
	{
		CloseHandle(hMessageMapping);
		return FALSE;	
	}
	hNodeMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_NODE);
	if(hNodeMapping==NULL)
	{
		return FALSE;
	}
	g_Node=(NID *)MapViewOfFile(hNodeMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Message==NULL)
	{
		CloseHandle(hNodeMapping);
		return FALSE;	
	}
	hModuleMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_MODULE);
	if(hModuleMapping==NULL)
	{
		return FALSE;
	}
	g_Module=(MID *)MapViewOfFile(hModuleMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Module==NULL)
	{
		CloseHandle(hModuleMapping);
		return FALSE;	
	}
	hSidMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_SID);
	if(hSidMapping==NULL)
	{
		return FALSE;
	}
	g_Sid_Position=(SID_POSITION *)MapViewOfFile(hSidMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Sid_Position==NULL)
	{
		CloseHandle(hSidMapping);
		return FALSE;
	}
	hOffsetMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_OFFSET);
	if(hOffsetMapping==NULL)
	{
		return FALSE;
	}
	g_Offset_Position=(SID_POSITION *)MapViewOfFile(hOffsetMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Offset_Position==NULL)
	{
		CloseHandle(hOffsetMapping);
		return FALSE;
	}
	return TRUE;
	
}
/********************************************************************
�������ƣ�Detach_Merry
��������: �ͷŹ����ڴ�
��������: ��
���ض���: ��
����ʱ��: 2007-11-8 19:51
��������: ������
ע������: ��	
*********************************************************************/
void Detach_Merry()
{
	UnmapViewOfFile(g_Message);
	UnmapViewOfFile(g_Node);
	UnmapViewOfFile(g_Module);
	UnmapViewOfFile(g_Sid_Position);
	UnmapViewOfFile(g_Offset_Position);
	g_Message=NULL;
	g_Node = NULL;
	g_Module = NULL;
	g_Sid_Position = NULL;
	g_Offset_Position = NULL;
	CloseHandle(hMessageMapping);
	CloseHandle(hNodeMapping);
	CloseHandle(hModuleMapping);
	CloseHandle(hReBufferMapping);
	CloseHandle(hSenssionMapping);
	CloseHandle(hSidMapping);
	CloseHandle(hOffsetMapping);
	CloseHandle(hMutex);
}

/************************************************************************/

BOOL SetSenssionId(WORD offset)
{
	if(offset < MESSAGE_MAX)
	{
		g_Sid_Position->bSign[offset] = 0;
		return TRUE;
	}
	else
		return FALSE;
}

int GetSenssionNumber()
{
	int i=0;
	int offset = -1;

	WaitForSingleObject(hMutex,INFINITE);
	while(i<MESSAGE_MAX)
	{
		if(g_Sid_Position->offset<MESSAGE_MAX)
		{
			if(g_Sid_Position->bSign[g_Sid_Position->offset]==0)
			{
				offset = g_Sid_Position->offset;
				g_Sid_Position->bSign[g_Sid_Position->offset] = 1;
				if(g_Sid_Position->offset == (MESSAGE_MAX - 1))
				{
					g_Sid_Position->offset = 0;
				}
				else
				{
					g_Sid_Position->offset ++;
				}
				break;
			}
			if(g_Sid_Position->offset == (MESSAGE_MAX - 1))
			{
				g_Sid_Position->offset = 0;
			}
			else
			{
				g_Sid_Position->offset ++;
			}
		}
		else
		{
			g_Sid_Position->offset = 0;
		}
		
		i++;
	}
   ReleaseMutex(hMutex);
   return offset;
}
void SetOffsetId(WORD offset)
{
	if(offset < MESSAGE_MAX)
	{
		g_Offset_Position->bSign[offset] = 0;
	}
}
int GetOffsetId()
{
	
	int i=0;
	int offset = -1;
	
	/***********2014-03-10 change by dyz*********************/
    WaitForSingleObject(hMutex,INFINITE);
	while(i<MESSAGE_MAX)
	{
		if(g_Offset_Position->offset<MESSAGE_MAX)
		{
			if(g_Offset_Position->bSign[g_Offset_Position->offset]==0)
			{
				offset = g_Offset_Position->offset;
				g_Offset_Position->bSign[g_Offset_Position->offset] = 1;
				if(g_Offset_Position->offset == (MESSAGE_MAX - 1))
				{
					g_Offset_Position->offset = 0;
				}
				else
				{
					g_Offset_Position->offset ++;
				}
				break;
			}
			if(g_Offset_Position->offset == (MESSAGE_MAX - 1))
			{
				g_Offset_Position->offset = 0;
			}
			else
			{
				g_Offset_Position->offset ++;
			}
		}
		else
		{
			g_Offset_Position->offset = 0;
		}
		i++;
	}
	ReleaseMutex(hMutex);
	return offset;
}
