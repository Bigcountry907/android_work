#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <shellapi.h>
#include <setjmp.h>
#include <signal.h>

#include "comm.h"
#include "define.h"
#include "event.h"
#include "commdll.h"
#include "yftrace.h"
#include "pMsg.h"
#include "TimerDll.h"
#include "pXSSM.h"
HWND    hSppWnd=NULL;//�����Ӷ˵Ĵ��ھ��MONITOR
int		gProgramTime=0;//��������û���ϴ���
#define TIMER_LEN  5000             // ������Ϣ��ⷢ��ʱ����
#define MAX_SEND_CHECK_COUNT 3      // �����ӵĳ���Ӧ��������Ϣ�ﵽ��ֵ���������б����ӳ���
#define SPP_NAME "SPPServer"
#define WM_OPENWND WM_USER+2220
#define SYSTRAY_ICON_BASE 2222
#define FILENAME "pXSSM.ini"
#define TERN_TIME 300

MESSAGE g_Slave_Message[MESSAGE_MAX];
jmp_buf Jump_Buffer; 
#define try if(!setjmp(Jump_Buffer))
#define catch else
#define throw longjmp(Jump_Buffer,1)

void  fphandler(int sig);   /* Prototypes */
LRESULT APIENTRY WndProc ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					
					PSTR szCmdLine, int iCmdShow)
					
{
	
	static TCHAR szAppName[] = TEXT ("pXSSM"/*MAIN_WINDOW_TITLE*/) ;
	
	
	HWND                                 hwnd ;
	
	MSG                                  msg ;
	
	WNDCLASS                      wndclass ;

	BOOL bDll=FALSE;
	HWND hwnd1 = FindWindow(szAppName,szAppName);//������ڣ���������
	g_Trace=EnableTrace(TRUE);//����Ҫ��־��¼�ļ�
	Hdl_SetTrace(TRUE);
	if(hwnd1!=NULL)
	{
		YF_LOG_SMM "pXSSM:�����Ѿ���������ͼ��������ʧ��");
		return 0;
	}
	

	wndclass.style                               = CS_HREDRAW | CS_VREDRAW ;
	
	wndclass.lpfnWndProc                         = WndProc ;
	
	wndclass.cbClsExtra                          = 0 ;
	
	wndclass.cbWndExtra                          = 0 ;
	
	wndclass.hInstance                           = hInstance ;
	
	wndclass.hIcon                              = LoadIcon (NULL, IDI_APPLICATION) ;
	
	wndclass.hCursor                             = LoadCursor (NULL, IDC_ARROW) ;
	
	wndclass.hbrBackground              = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	
	wndclass.lpszMenuName                = NULL ;
	
	wndclass.lpszClassName               = szAppName ;
	

	if (!RegisterClass (&wndclass))
 
	{
        
//		MessageBox (NULL, TEXT ("This program requires Windows NT!"),
//			
//			szAppName, MB_ICONERROR) ;
//        
		return 0 ;
        
	}

	
	hwnd = CreateWindow ( szAppName, TEXT ("pXSSM"/*MAIN_WINDOW_TITLE*/),
        
		WS_OVERLAPPEDWINDOW,
        
		CW_USEDEFAULT, CW_USEDEFAULT,
        
		CW_USEDEFAULT, CW_USEDEFAULT,
        
		NULL, NULL, hInstance, NULL) ;
	ShowWindow (hwnd, iCmdShow) ;
	
	UpdateWindow (hwnd) ;

//////////////////////////////////////////////////////////////////////////
	if(signal(SIGFPE,fphandler)==SIG_ERR)
	{
		YF_LOG_SMM "�¼��쳣��������ʧ��");
	}
//////////////////////////////////////////////////////////////////////////

	pwnd = hwnd;
	GetSppStatus();
//	SetRegisterHotKey(hwnd);
	if(bFindSpp)
	{
//		TRACE(0,FALSE,"WinMain","SPP.exeû�������Ҳ��������ڴ�");
//		RegisterIPC(MID_ACC,hwnd);
//		TRACE(0,TRUE,"WinMain","ע�����");
//		SetRegisterHotKey(hwnd);
//		
//		Start_All();
		StartAllWork();
	}
	else
	{
		YF_LOG_SMM "SPP.exeû�������Ҳ��������ڴ�");
	}
	if(!IsPSTN)
		return 0;
	INIT_TIMER();
//	ShowInfor(hwnd);
	while (GetMessage (&msg, NULL, 0, 0))      
	{
        TranslateMessage (&msg) ;
        
		DispatchMessage (&msg) ;
        
	}
	
	return msg.wParam ;
	
}
void fphandler(int sig )
{
   int j;
   j = sig;
}
void WriteLog_char(char *callnum)
{
	SYSTEMTIME     Clock;
	FILE	    *fp=NULL;
	fp = fopen ("C:\\yfcomm\\log\\pxssmtest.log","a");

	if ( fp == NULL )
		return ;

	GetLocalTime(&Clock);

 
	fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d have call in message,callingnum is=%s\n",
			Clock.wYear,Clock.wMonth, Clock.wDay,
		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,callnum);
	fclose (fp);
}
void WriteLog(BOOL status)
{
	SYSTEMTIME     Clock;
	FILE	    *fp=NULL;
	fp = fopen ("C:\\yfcomm\\log\\pxssmtest.log","a");

	if ( fp == NULL )
		return ;

	GetLocalTime(&Clock);

 
	fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d have call in message,staus=%d\n",
			Clock.wYear,Clock.wMonth, Clock.wDay,
		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,status);
	fclose (fp);
}
void StartAllWork()
{
	RegisterIPC(MID_ACC,pwnd);
	YF_LOG_SMM "IPCע�����");
//	SetRegisterHotKey(pwnd);
	Start_All();
}
BOOL systray_add(HWND hwnd, UINT uID, HICON hIcon, LPSTR lpszTip)
{
    NOTIFYICONDATA tnid;
    tnid.cbSize = sizeof(NOTIFYICONDATA);
    tnid.hWnd = hwnd;
    tnid.uID = uID+SYSTRAY_ICON_BASE;
    tnid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    tnid.uCallbackMessage = WM_OPENWND;     //���巢�͵��������Ϣ
    tnid.hIcon = hIcon;
    lstrcpyn(tnid.szTip,lpszTip,sizeof(tnid.szTip)-1);
    return (Shell_NotifyIcon(NIM_ADD, &tnid));
}
BOOL systray_del(HWND hwnd, UINT uID) {
    NOTIFYICONDATA tnid;
    tnid.cbSize = sizeof(NOTIFYICONDATA);
    tnid.hWnd = hwnd;
    tnid.uID = uID+SYSTRAY_ICON_BASE;
    return(Shell_NotifyIcon(NIM_DELETE, &tnid));
}
/********************************************************************
�������ƣ�WndProc
��������: ��Ϣ����
��������: ��
���ض���: ��
����ʱ��: 2007-11-21 11:11
��������: ������
ע������: ��	
*********************************************************************/

LRESULT APIENTRY WndProc ( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)

{
	ACS_Hdl_EVENT * ppHdlEvent;
	COPYDATASTRUCT *cds;
	switch (message)
	{
	case WM_COPYDATA:
		cds = (COPYDATASTRUCT *)lParam;
		if(cds ==NULL)
		{
			break;
		}
		ppHdlEvent =(ACS_Hdl_EVENT*) malloc(cds->cbData);

		if(ppHdlEvent == NULL)
		{
			YF_TRACE -1,"", "�ڴ�����ʧ�ܣ���Ϣ����");
			break;
		}
		memcpy(ppHdlEvent,cds->lpData,cds->cbData);
//		if (ppHdlEvent->datalen>0)
//		    ppHdlEvent->ioData = (char *)ppHdlEvent+sizeof(ACS_Hdl_EVENT);
		PostMessage(hwnd,evACCMessage,0,(LPARAM)ppHdlEvent);
		break;
   case WM_CREATE:		
		break;
	case WM_TIMER:
		{
			Monitor(WM_TIMER,lParam);
		}
		break;
	case MSG_SPP_ACK:
		{
			Monitor(MSG_SPP_ACK,lParam);
		}
	case  WM_SIZE:
		if (wParam==SIZE_MINIMIZED)
		{
			systray_add(hwnd,1,LoadIcon(NULL,IDI_APPLICATION),"pXSSMģ��V1.0.0");
			ShowWindow(hwnd,SW_HIDE);
		}	
		break;
    case WM_OPENWND:
		{
			if(lParam == WM_LBUTTONDOWN)// ���ϵͳ��������ͼ��ʱ����5.0ǰ�汾�Ƿ���WM_LBUTTONDOWN��Ϣ����ͬ�汾�ıȽ���MSDN�в���Shell_NotifyIcon
			{
				systray_del(hwnd,1);
				ShowWindow(hwnd,SW_SHOW);     //һ��Ҫ����showwindow���β��ܼ���壬������֣���������SW_HIDE״̬һ��ҪSW_SHOWһ�²��а�
				ShowWindow(hwnd,SW_SHOWNORMAL);	
			}
		}
		break;
	case evThreadMessage:
		DealWithMessage(wParam,lParam);	
		break;
	case evIPCMessage:
		DealWithMessage(wParam,lParam);	
		break;
	case evACCMessage:
		{
			EvtHandler(lParam);
		}
		break;
	case WM_DESTROY:
		{
			Hdl_Exit();
			FreeMerroy();
			YF_LOG_SMM "�������˳�");
			EnableTrace(FALSE);//�ر���Ҫ��־��¼�ļ����
			FreeHotKey(hwnd);
			PostQuitMessage(0) ;
			return 0 ;
		}
		break;
	case MSG_TRACE_ON:
		{
			
				g_Trace = EnableTrace(TRUE);
//				SetTraceValue(g_Trace);
				Hdl_SetTrace(g_Trace);
				YF_TRACE -1,"","pXSSM��������");			
		}
		break;
	case MSG_TRACE_OFF:
		{
			YF_TRACE -1,"","pXSSMֹͣ����");
			g_Trace = EnableTrace(FALSE);
//			SetTraceValue(g_Trace);
			Hdl_SetTrace(g_Trace);
			
		}
		break;
	case MSG_WATCHDOG_CHECK:
		{
			Monitor(MSG_WATCHDOG_CHECK,lParam);
		}
		break;
	default:
		break;
        
	}
	
	return DefWindowProc (hwnd, message, wParam, lParam) ;
	
}

BOOL Start_All()
{
	
	Init_XSSM();
	INIT_TIMER();/*��ʼ����ʱ��*/
	Hdl_Init(Hdl_Attr);
//	SetRecallFuction();
	YF_LOG_SMM "��ʼ��Keygoe���");
	return TRUE;
}
/********************************************************************
�������ƣ�DealWithMessage
��������: ���������������̵���Ϣ
��������: lParam����Ϣ��
���ض���: ��
����ʱ��: 2008-1-27 21:28
��������: ������
ע������: ��	
*********************************************************************/

void DealWithMessage(WPARAM wParam,LPARAM lParam)
{
	MESSAGE pMessage ; 
	BOOL bFind = FALSE ;
	bFind = COM_Read_Message((WORD)wParam,&pMessage);
	if(!bFind)
		return ;
	YF_TRACE lParam,"","id=%d",wParam);
	Update_Message((WORD)lParam,&pMessage,FALSE);
	switch(pMessage.head.event)
	{
	case evTraceStatus://���ٿ���
		ControlTrace(pMessage.data);
		break;
	case evPXSSM_ActivityTest:/*����״̬����*/
		XSSM_ActiveTestResponse((WORD)lParam);
		break;
	case evPXSSM_ConnectToResource:/*������*/
		XSSM_RecvConnectToResouce(pMessage.data);
		break;
	case evPXSSM_DisconnectForwardConnection:
		XSSM_RecvDisConnectForwardConnection(pMessage.data);
		break;
	case evPXSSM_ReleaseCall:
		{
			XSSM_RecvReleaseCall(pMessage.data);

		}
		break;
	case evPXSSM_PlayAnnouncement:
		XSSM_RecvPlayAnnouncement(pMessage.data);
		break;
	case evPXSSM_PromptCollectInformation:
		WriteLog(3);
		SetSidEvent((WORD)lParam,pMessage.head.event);
		XSSM_RecvPromptCollectInformation(pMessage.data);
		break;
	case evPXSSM_PromptCollectInformationAndSetFormat:
		SetSidEvent((WORD)lParam,pMessage.head.event);
		XSSM_PromptCollectInformationAndSetFormat(pMessage.data);
		break;
	case evPXSSM_SendFSK:
		WriteLog(6);
		XSSM_RecvSendFSK(pMessage.data);
		break;
	case evPXSSM_SendFSKCollectInformation:
		WriteLog(7);
		SetSidEvent((WORD)lParam,pMessage.head.event);
		XSSM_RecvSendFSKCollectInformation(pMessage.data);
		break;
	case evPXSSM_PromptCollectFSK:
		WriteLog(9);
		XSSM_RecvPlayAnnouncementAndSetFSK(pMessage.data);
		break;
	case evPXSSM_SendFSKCollectFSK:
		WriteLog(8);
		XSSM_RecvSendFSKCollectFSK(pMessage.data);
		SetSidEvent((WORD)lParam,pMessage.head.event);
		break;
	case evPXSSM_InitiateRecord:
		XSSM_RecvInitiateRecord(pMessage.data);
		break;
	case evPXSSM_StopRecord:
		XSSM_RecvStopRecord(pMessage.data);
		break;
	case evPXSSM_Connect:
		XSSM_RecvConect((WORD)lParam,pMessage.data);
		break;
	case evPXSSM_InitiateCallAttempt:
		XSSM_RecvConect((WORD)lParam,pMessage.data);
		break;
	case evPXSSM_TTSConvert:
		XSSM_RecvTTSConvert(pMessage.data);
		break;
	case evPXSSM_TTSPlay:
		XSSM_RecvTTSPlay(pMessage.data);
		break;
	case evTimer1:
		DoTimer(evTimer1,(DWORD)lParam);
		break;
	case evTimer2:
		DoTimer(evTimer2,(DWORD)lParam);
		break;
	case evTimer3:
		DoTimer(evTimer3,(DWORD)lParam);
		break;
	default:
		break;
	}
}
void SetSidEvent(WORD wNumber,DWORD ievent)
{
	
	if(wNumber<MESSAGE_MAX)
	{
		if(g_Sid[wNumber].sid == wNumber)
		{
			g_Sid[wNumber].status=ievent;
		}
	}
	else
	{
		YF_TRACE wNumber,"SetSidEvent","sid  overflow");
		
	}
	
}

/********************************************************************
�������ƣ�BCD_TO_ASCIIString
��������: BCD�봮תASCII�봮
��������: lParam����Ϣ��
���ض���: ��
����ʱ��: 2008-1-27 21:28
��������: ������
ע������: ��	
*********************************************************************/
void BCD_TO_ASCIIString(WORD sid,unsigned char *cSource, unsigned int iSourceLen)
{
	FILE *fp;
	char mmmmm[3200];
	char cResult[3200];
	unsigned int i=0;
    int j=0;
	char filename[30];
	SYSTEMTIME     Clock;


	GetLocalTime(&Clock);

 

	memset(mmmmm, 0, 1600);
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
	cResult[j++]='\n';

	sprintf(filename,"C:\\YFCOMM\\LOG\\pXSSM.log");
	fp = fopen(filename,"a");
	if(fp == NULL)
	{
//		fclose(fp);
		return ;
	}
	fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d  ",
		Clock.wYear,Clock.wMonth, Clock.wDay,
		Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds);
	fwrite(cResult,strlen(cResult),1,fp);
//	fwrite('\n',1,1,fp);
	
	fclose(fp);
	fp=NULL;

	YF_TRACE sid,"","pXSSM->ACC:send data,len=%d,TX: %s", iSourceLen, cResult);
} 
/********************************************************************
�������ƣ�DoTimer
��������:evt:�¼��ţ�sid:�Ự��
��������: ��
���ض���: ��
����ʱ��: 2008-2-20 10:57
��������: ������
ע������: ��	
*********************************************************************/
void DoTimer(DWORD evt,DWORD sid)
{
	stPXSSM_ReleaseCall nReleaseCall;
	stPXSSM_PlayAnnouncement pPlayTone;
	WORD timeId;
	if(sid>=MESSAGE_MAX)
		return ;
//	return ;
	nReleaseCall.sid=sid;
	nReleaseCall.reason = NoResponse;
	pPlayTone.sid=sid;
	pPlayTone.tone.variable=0;
	pPlayTone.tone.vchoiceVariable.toneID=14;
	pPlayTone.canInterupt=FALSE;
	pPlayTone.duration=0;
	pPlayTone.interval=0;
	pPlayTone.repeatTime=0;
	switch(evt)
	{
	case evTimer1://���ֳ�ʱ
	case evTimer2://�м䳬ʱ
		{	
			YF_TRACE sid,"DoTimer","receive dtmf timeout,evt=%d",evt);
				
			
				
//			if(!bPlatForm)//ƽ̨
//			{
////				Sleep(300);
//				XSSM_RecvPlayAnnouncement(&pPlayTone);
//			}
			XSSM_ErrorReport(sid,4);
			timeId = SET_TIMER(1*10,evTimer3,MID_ACC,(WORD)sid,FALSE);
			g_Sid[sid].timerId = timeId;
		}
		break;
	case evTimer3:
		{
			YF_TRACE sid,"DoTimer","release call evTimer3=%d",evTimer3);
			XSSM_RecvReleaseCall(&nReleaseCall);
//			memset(&g_Sid[sid],0,sizeof(XSSM_SID));
//			g_Sid[sid].sid = MESSAGE_MAX;
		}
		break;
	default:
		break;
	}
}
/********************************************************************
�������ƣ�DoKillTimer
��������:evt:�¼��ţ�sid:�Ự��
��������: ��
���ض���: ��
����ʱ��: 2008-2-20 10:57
��������: ������
ע������: ��	
*********************************************************************/
void DoKillTimer(DWORD evt,DWORD sid)
{
	if(sid>=MESSAGE_MAX)
		return;
	KILL_TIMER(g_Sid[sid].timerId);
}
/********************************************************************
�������ƣ�Init_XSSM
��������: 
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 9:55
��������: ������
ע������: ��	
*********************************************************************/
BOOL Init_XSSM()
{
	int i;
	for (i=0;i<MESSAGE_MAX;i++) 
	{
		g_Sid[i].sid = MESSAGE_MAX;
		g_Sid[i].serviceKey=0;
		memset(g_Sid[i].CalledNumber,0,sizeof(g_Sid[i].CalledNumber));
		memset(g_Sid[i].CallingNumber,0,sizeof(g_Sid[i].CallingNumber));
		g_Sid[i].status=0;
		g_Sid[i].attrd.attr = 0;
		memset(&g_Sid[i].pCollectInfor,0,sizeof(COLLECTINFORMATION));
	}
	for (i=0;i<MESSAGE_MAX;i++) 
	{
		memset(&g_Slave_Message[i],0,sizeof(MESSAGE));
	}
	
	ReadConfig();
	return TRUE;
}
/********************************************************************
�������ƣ�ReadConfig
��������: ��ȡ������Ϣ
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 10:00
��������: ������
ע������: ��	
*********************************************************************/
BOOL ReadConfig()
{
	SECTION *sec=NULL;
	INI ini;
	BYTE i,j;
	char ToneNmae[128];
	char tmpSection[LENGTH_SECTION];
	sprintf(ToneNmae,"C:\\yfcomm\\ini\\%s",FILENAME);
	loadINI(&ini,ToneNmae);
	/*�õ�[COMM]*/
	sec=getSection(&ini,INI_XSSM_SERKEY);
	if(sec==NULL)
	{
		YF_LOG_SMM "read pxssm.ini failed");
		return FALSE;
	}
	ServiceKey_Total=atoi(GetKeyList(sec,0));
	bPlatForm = (BOOL)(atoi(GetKeyList(sec,1))&1);
	IsPSTN = (BOOL)(atoi(GetKeyList(sec,2))&1);
	if(!IsPSTN)//����Ҫ����
	{
		freeINI(&ini);
		return FALSE;
	}
	pServiceKey = (SERVICE_KEY *)malloc(sizeof(SERVICE_KEY) * ServiceKey_Total);
	
	/*�õ�[KeyNumber]*/
	for(i=0;i<ServiceKey_Total;i++)
	{
		memset(&pServiceKey[i],0,sizeof(SERVICE_KEY));
		sprintf(tmpSection,"%s%d",INI_XSSM_KEYNUMBER,i);
		sec=getSection(&ini,tmpSection);
		if(sec==NULL)
		{
			YF_LOG_SMM "config file %s errno",ToneNmae);
			return FALSE;
		}
		pServiceKey[i].SkeyType=atoi(GetKeyList(sec,0));
		if(pServiceKey[i].SkeyType==0)
		{
			memset(pServiceKey[i].initialNum,0,sizeof(pServiceKey[i].initialNum));
			pServiceKey[i].serviceKey=atoi(GetKeyList(sec,1));		
			pServiceKey[i].initialtype =(BYTE)atoi(GetKeyList(sec,2));
			strcpy(pServiceKey[i].initialNum,GetKeyList(sec,3));
			pServiceKey[i].mask = (BYTE)atoi(GetKeyList(sec,4));
			pServiceKey[i].nodeTotal = (BYTE)atoi(GetKeyList(sec,5));
			pServiceKey[i].pNodeInfor = (NODE_INFOR *)malloc(sizeof(NODE_INFOR) * pServiceKey[i].nodeTotal);
			if(pServiceKey[i].nodeTotal>0)
			{
				for (j=0;j<pServiceKey[i].nodeTotal;j++)
				{
					pServiceKey[i].pNodeInfor[j].nodenum = (BYTE)(atoi(GetKeyList(sec,(BYTE)(j+6))));
					pServiceKey[i].pNodeInfor[j].serviceTotal = 0;
				}
			}
			strcpy(pServiceKey[i].description,GetKeyList(sec,(BYTE)(pServiceKey[i].nodeTotal+6)));
		}
	}
	
	sprintf(tmpSection,"%s",INI_XSSM_HDL);
	sec=getSection(&ini,tmpSection);
	if(sec==NULL)
	{
		freeINI(&ini);
		return FALSE;
	}
	strcpy(Hdl_Attr.ServerIP,GetKeyList(sec,0));
	Hdl_Attr.port=(WORD)atoi(GetKeyList(sec,1));
	strcpy(Hdl_Attr.username,GetKeyList(sec,2));
	strcpy(Hdl_Attr.passwd,GetKeyList(sec,3));
	Hdl_Attr.UintId = atoi(GetKeyList(sec,4));
	Hdl_Attr.attrd.debug = (WORD)(atoi(GetKeyList(sec,5))&0x01);
	sXssmTime.hdlTimeout=TERN_TIME;
	sXssmTime.sysTimeout=TERN_TIME;
	
	sprintf(tmpSection,"%s",INI_XSSM_TIME);
	sec=getSection(&ini,tmpSection);
	if(sec!=NULL)
	{
		sXssmTime.sysTimeout=atoi(GetKeyList(sec,0))*10;
		sXssmTime.hdlTimeout=atoi(GetKeyList(sec,1))*10;
		
	}

	/*end*/
	freeINI(&ini);
	for(i=0;i<ServiceKey_Total;i++)
	{
		ReadKeyTone(pServiceKey[i].serviceKey);
	}
	return TRUE;

}
/********************************************************************
�������ƣ�ReadKeyTone
��������: ��ȡ����ҵ�����������
��������: keyNum:ҵ�����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-26 11:36
��������: ������
ע������: ��	
*********************************************************************/
BOOL ReadKeyTone(int keyNum)
{
	SECTION *sec=NULL;
	INI ini;
	BYTE i,j;
	char ToneNmae[32];
	int nkeyNum = -1;
	int num;
	int tonenum;
	char tmpSection[LENGTH_SECTION];
	char *pchar;
	
	for(i=0;i<ServiceKey_Total;i++)
	{
		if(keyNum==pServiceKey[i].serviceKey)
		{
			nkeyNum = i;
			break;
		}
	}
	if(nkeyNum == -1)
		return FALSE;
	sprintf(ToneNmae,"C:\\yfcomm\\Tone\\%dTone.ini",keyNum);
	loadINI(&ini,ToneNmae);
	/*�õ�[COMM]*/
	sec=getSection(&ini,INI_XSSM_SERVICETONEKEY);
	if(sec==NULL)
	{
		YF_LOG_SMM "read %s failed",ToneNmae);
		return FALSE;
	}
	
	if(keyNum!=atoi(GetKeyList(sec,0)))
	{
		YF_LOG_SMM "service key erron,keyNum=%d,getkeyNum=%d",keyNum,atoi(GetKeyList(sec,0)));
		return FALSE;
	}
	num = atoi(GetKeyList(sec,1));
	pServiceKey[nkeyNum].pTone.pElement= (SERVICE_KEY_ELEMENT *)malloc(sizeof(SERVICE_KEY_ELEMENT) * num);
	pServiceKey[nkeyNum].pTone.ToneNum = num;

	for (i=0;i<num;i++)
	{
		sprintf(tmpSection,"TONE%d",i+1);
		sec=getSection(&ini,tmpSection);
		if(sec!=NULL)
		{
			tonenum = atoi(GetKeyList(sec,0));
			pServiceKey[nkeyNum].pTone.pElement[i].pToneContent = (SERVICE_TONE *)malloc(sizeof(SERVICE_TONE) * tonenum);
			pServiceKey[nkeyNum].pTone.pElement[i].elementNum = tonenum;
			for(j=0;j<tonenum;j++)
			{
				sprintf(tmpSection,"TONE%d-%d",i+1,j+1);
				sec=getSection(&ini,tmpSection);
				if(sec!=NULL)
				{
					pServiceKey[nkeyNum].pTone.pElement[i].pToneContent[j].serviceKey = keyNum;
					pServiceKey[nkeyNum].pTone.pElement[i].pToneContent[j].toneIndex = i;
					pServiceKey[nkeyNum].pTone.pElement[i].pToneContent[j].argIndes = atoi(GetKeyList(sec,0));
					pServiceKey[nkeyNum].pTone.pElement[i].pToneContent[j].language = atoi(GetKeyList(sec,1));

					pServiceKey[nkeyNum].pTone.pElement[i].pToneContent[j].argType = (BYTE)atoi(GetKeyList(sec,2));
					pchar = GetKeyList(sec,3);
					if(pchar != NULL)
					//	pServiceKey[nkeyNum].pTone.pElement[i].pToneContent[j].reference="";
					//else
						strcpy(pServiceKey[nkeyNum].pTone.pElement[i].pToneContent[j].reference,pchar);
				}
			}
		}
	}
	/*end*/
	freeINI(&ini);

	return TRUE;
}
/********************************************************************
�������ƣ�SwtichToneFile
��������: ��ʽ�������ṹת��
��������: sid:�Ự�ţ�pTone��������ʽ����
���ض���: ����������ʽָ�룬ʧ�ܷ���NULL
����ʱ��: 2008-2-26 17:12
��������: ������
ע������: ��	
*********************************************************************/
PVOICE *SwtichToneFile(DWORD sid,stPXSSM_Tone *pTone)
{
	int i;
	int num = -1;
	int ToneNum;//������Ŀ��
	int j=0;
	
	char buffer[20];
	PVOICE *pVoice=NULL;
	
	if(sid>=MESSAGE_MAX)
		return NULL;
	if(pTone->variable == 0)//fix 
	{
		ToneNum = pTone->vchoiceVariable.toneID;
	}
	else
	{
		ToneNum = pTone->vchoiceVariable.variableTone.toneID;
	}
	for(i=0;i<ServiceKey_Total;i++)
	{
		if(pServiceKey[i].serviceKey == g_Sid[sid].serviceKey)
		{
			num = i;
			break;
		}
	}
	if(num == -1)
	{
		
		return NULL;
	}
	pVoice = (PVOICE *)malloc(sizeof(PVOICE));
	if(pVoice == NULL)
		return NULL;
	pVoice->pVoiceHead.nKey = sid;
	if(pServiceKey[num].pTone.ToneNum<=ToneNum)
	{
		return NULL;
	}
	pVoice->pVoiceHead.nNo = pServiceKey[num].pTone.pElement[ToneNum].elementNum;
	pVoice->pVoice_Content = (PVOICE_CONTENT *)malloc(sizeof(PVOICE_CONTENT) * pServiceKey[num].pTone.pElement[ToneNum].elementNum);
	for(i=0;i<pVoice->pVoiceHead.nNo;i++)
	{
		pVoice->pVoice_Content[i].pType = pServiceKey[num].pTone.pElement[ToneNum].pToneContent[i].argType;
		pVoice->pVoice_Content[i].language = pServiceKey[num].pTone.pElement[ToneNum].pToneContent[i].language;

		if(pVoice->pVoice_Content[i].pType>=2)//�ɱ�
		{
//////////////////////////////////////////////////////////////////////////
			switch(pTone->vchoiceVariable.variableTone.parameter[j].choiceType)
			{
			case PLAY_FILE_TONE_ELMENT://��Ԫ
			case PLAY_FILE_DIGIT://����
				_itoa(pTone->vchoiceVariable.variableTone.parameter[j].value.value,buffer,10);
				strcpy(pVoice->pVoice_Content[i].content,buffer);
				break;
			case PLAY_FILE_CHAR://�ַ���
				strcpy(pVoice->pVoice_Content[i].content,pTone->vchoiceVariable.variableTone.parameter[j].value.number);
				break;
			case PLAY_FILE_CURRENCY://����
				_itoa(pTone->vchoiceVariable.variableTone.parameter[j].value.price,buffer,10);
				strcpy(pVoice->pVoice_Content[i].content,buffer);
				break;
			case PLAY_FILE_DATE://����
				strcpy(pVoice->pVoice_Content[i].content,pTone->vchoiceVariable.variableTone.parameter[j].value.date);
				break;
			case PLAY_FILE_TIME://ʱ��
				strcpy(pVoice->pVoice_Content[i].content,pTone->vchoiceVariable.variableTone.parameter[j].value.time);
				break;
			case PLAY_FILE_FILE://�ļ���
				break;
			default:
				break;
			}
			
			j++;
		}
		else
		{
			strcpy(pVoice->pVoice_Content[i].content,pServiceKey[num].pTone.pElement[ToneNum].pToneContent[i].reference);
		}
		
	}
	return pVoice; 
}
/********************************************************************
�������ƣ�SetRecallFuction
��������: ���ûص�����
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 10:04
��������: ������
ע������: ��	
*********************************************************************/
BOOL SetRecallFuction()
{
	Hdl_SetRecall(&EvtHandler);
	return FALSE;

}
void ReInitKeygoe()
{
	Hdl_Exit();//�˳���DSP������
	Init_XSSM();
	Hdl_Init(Hdl_Attr);
	SetRecallFuction();
	YF_LOG_SMM "Keygoe�����������");
}
/********************************************************************
�������ƣ�EvtHandler
��������: ��������pXSSM��̬����Ϣ����
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 10:04
��������: ������
ע������: ��	
*********************************************************************/
void EvtHandler(DWORD esrParam)
{
	ACS_Hdl_EVENT *pEvent = (ACS_Hdl_EVENT *)esrParam;
	stPXSSM_ResourceReport resourceReport;
	if(pEvent == NULL)
	{
		YF_LOG_SMM "pEvent is null");
		return ;
	}
	switch(pEvent->event)
	{
	case evtXSSM_LinkStatus://��Ӳ���豸��������
		{
			bHdlConnectStatus = TRUE;
		}
		break;
	case evtXSSM_FailStatus://��Ӳ���豸�����쳣
		{
			bHdlConnectStatus = FALSE;
			YF_LOG_SMM "pXSSM: ��DSP�����쳣");
			ReInitKeygoe();//���³�ʼ��KEYGOEϵͳ

		}
		break;
	case evtXSSM_CallIn://����
		{
			
			XSSM_DoCallIn((void *)pEvent);
		}
		break;
	case evtXSSM_CallOut://����
		{
			XSSM_DoCallOut((void* )pEvent);
		}
		break;
	case evtXSSM_AlertCall:
		break;
	case evtXSSM_AnswerCall://
		break;
	case evtXSSM_LinkDevice://�����豸����
		break;
	case evtXSSM_UnLinkDevice://�������豸����
		break;
	case evtXSSM_ClearCall://�һ�
		{
			XSSM_DoClearCall((void *)pEvent);
//			Hdl_clearsid(pEvent->sid);
		}
		break;
	case evtXSSM_JoinConf://�������
		break;
	case evtXSSM_LeaveFromConf://�뿪����
		break;
	case evtXSSM_ClearConf://����������
		break;
	case evtXSSM_Play://�������
		//XSSM_DoAll((void *)pEvent);
		{
			resourceReport.sid = pEvent->sid;
			resourceReport.reason = R_PlayAnnouncement;
			XSSM_ResourceReport(&resourceReport);
			if(!bPlatForm)//
				SetSidEvent((WORD)pEvent->sid,evPXSSM_SendFSKCollectFSK);
		}
		break;
	case evtXSSM_ControlPlay://���Ʒ����������
		break;
	case evtXSSM_Record://¼�����
		//XSSM_DoAll((void *)pEvent);
		{
			resourceReport.sid = pEvent->sid;
			resourceReport.reason = R_InitiateRecord;
			XSSM_ResourceReport(&resourceReport);
		}
		break;
	case evtXSSM_ControlRecord://����¼���������
		{
			resourceReport.sid = pEvent->sid;
			resourceReport.reason = R_InitiateRecord;
			XSSM_ResourceReport(&resourceReport);
		}
		break;
	case evtXSSM_SendFax:
		{
			XSSM_DoAll((void *)pEvent);
		}
		break;
	case evtXSSM_RecvFax:
		{
			XSSM_DoAll((void *)pEvent);
		}
		break;
	case evtXSSM_SendIoData://��������
		//XSSM_DoAll((void *)pEvent);
		{
			YF_TRACE pEvent->sid,"","pXSSM: send io data success");
			resourceReport.sid = pEvent->sid;
			resourceReport.reason = R_SendFsk;
			XSSM_ResourceReport(&resourceReport);
		}
		break;
	case evtXSSM_RecvIoData://���յ�����
		{
			YF_TRACE pEvent->sid,"","pXSSM: recv io data");
			XSSM_DoRecvIoData((void *)pEvent);
		}
		break;
	default:
		break;

	}
	
	free(pEvent);
}
/********************************************************************
�������ƣ�FindKeyMode
��������: ����ҵ�����Ӧ�±�
��������: CalledNum��������룬CallingNum:��������
���ض���: �ɹ�����ҵ�����Ӧ��ģ���,ʧ�ܷ���-1
����ʱ��: 2008-2-15 15:47
��������: ������
ע������: ��	
*********************************************************************/
int FindKeyMode(char CalledNum[32],char CallingNum[32])
{
	int i;

	if(pServiceKey==NULL)
		return -1;
	for(i=0;i<ServiceKey_Total;i++)
	{
		switch(pServiceKey[i].initialtype)
		{
		case 1://���к���ʶ��ҵ��
			{
				if(strcmp(CallingNum,pServiceKey[i].initialNum)==0)
				{
					return i;
				}
			}
			break;
		case 2://���к���ʶ��ҵ��
			{
				if(strcmp(CalledNum,pServiceKey[i].initialNum)==0)
				{
					return i;
				}
			}
			break;
		default:
			break;
		}
	}
	return -1;
}
/********************************************************************
�������ƣ�FindKeyNode
��������: ����ҵ������͵�ģ���
��������: CalledNum��������룬CallingNum:��������
���ض���: �ɹ�����ҵ�����Ӧ��ģ���,ʧ�ܷ���-1
����ʱ��: 2008-2-15 15:47
��������: ������
ע������: ��	
*********************************************************************/
int FindKeyNode(char CalledNum[32],char CallingNum[32])
{
	int i;
	char pchar[32];
	int len,nLen;
	if(pServiceKey==NULL)
		return -1;
	memset(pchar,0,sizeof(pchar));
	for(i=0;i<ServiceKey_Total;i++)
	{
		if(pServiceKey[i].SkeyType!=0)
			continue;
		len =strlen(pServiceKey[i].initialNum);
		switch(pServiceKey[i].initialtype)
		{
		case 1://���к���ʶ��ҵ��
			{
				nLen=strlen(CallingNum);
				if(nLen<len)
				{
					break;
				
				}
				else
				{
					memcpy(pchar,CallingNum,len);
					
				}
				if(strcmp(pchar,pServiceKey[i].initialNum)==0)
				{
					if((pServiceKey[i].bUseNode+1)==pServiceKey[i].nodeTotal)// �ӵ�һ���ڵ㿪ʼ
					{
						pServiceKey[i].bUseNode = 0;
						
					}
					else
					{
						pServiceKey[i].bUseNode ++;
					}

					pServiceKey[i].pNodeInfor[pServiceKey[i].bUseNode].serviceTotal++;
					return pServiceKey[i].pNodeInfor[pServiceKey[i].bUseNode].nodenum;
				}
			}
			break;
		case 2://���к���ʶ��ҵ��
			{
				
				nLen=strlen(CalledNum);
				if(nLen<len)
				{
					break;
				
				}
				else
				{
					memcpy(pchar,CalledNum,len);
					
				}
				if(strcmp(pchar,pServiceKey[i].initialNum)==0)
				{
					if((pServiceKey[i].bUseNode+1)==pServiceKey[i].nodeTotal)// �ӵ�һ���ڵ㿪ʼ
					{
						pServiceKey[i].bUseNode = 0;
						
					}
					else
					{
						pServiceKey[i].bUseNode ++;
					}

					pServiceKey[i].pNodeInfor[pServiceKey[i].bUseNode].serviceTotal++;
					return pServiceKey[i].pNodeInfor[pServiceKey[i].bUseNode].nodenum;
				}
			}
			break;
		default:
			break;
		}
	}
	return -1;
}
/********************************************************************
�������ƣ�FindServiceKey
��������: ����ҵ���
��������: CalledNum��������룬CallingNum:��������
���ض���: �ɹ�����ҵ�����Ӧ��ģ���,ʧ�ܷ���-1
����ʱ��: 2008-2-15 15:47
��������: ������
ע������: ��	
*********************************************************************/
int FindServiceKey(char CalledNum[32],char CallingNum[32])
{
	int i;
	if(pServiceKey==NULL)
		return -1;
	for(i=0;i<ServiceKey_Total;i++)
	{
		switch(pServiceKey[i].initialtype)
		{
		case 1://���к���ʶ��ҵ��
			{
				if(strcmp(CallingNum,pServiceKey[i].initialNum)==0)
				{
					return pServiceKey[i].serviceKey;
				}
			}
			break;
		case 2://���к���ʶ��ҵ��
			{
				if(strcmp(CalledNum,pServiceKey[i].initialNum)==0)
				{
					
					return pServiceKey[i].serviceKey;
				}
			}
			break;
		default:
			break;
		}
	}
	return -1;
}
/********************************************************************
�������ƣ�XSSM_SENDMESSAGE
��������: ������Ϣ
��������: sid:�Ự�ţ�pInfor����Ϣ���ݣ�len:��Ϣ���ȣ�
          bNew:�Ƿ����µ���Ϣ,devent:�¼���
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-22 9:47
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_SENDMESSAGE(DWORD sid,void *pInfor,int len,WORD devent,BOOL bNew)
{
	TID pTid ;
	UINT receivenumber;
	MESSAGE mpMessage;
	BOOL bFind =FALSE;
	BOOL bSend = FALSE;
	int nlen=sizeof(mpMessage.data);
	WORD timerId;
	if(sid>=MESSAGE_MAX)
	{
//		YF_TRACE sid,"","pxssm sid>%d",sid);
		return FALSE;
	}
	DoKillTimer(evTimer1,sid);
	memset(&mpMessage,0,sizeof(MESSAGE));
//////////////////////////////////////////
//	bFind = Read_Message((WORD)sid,&mpMessage);
//	if(!bFind)
//	{
////		YF_TRACE sid,"","pxssm readmessage failed");
//		return FALSE;
//	}
	if(!bNew)
	{
		if(MID_ACC!=mpMessage.head.sender.module)
		{
			memset(&pTid,0,sizeof(TID));
			if(GetTid(MID_SLP,&pTid))
			{
				mpMessage.head.receiver = pTid;
			}
			memset(&pTid,0,sizeof(TID));
			if(GetTid(MID_ACC,&pTid))
			{
				mpMessage.head.sender = pTid;
			}
//			pTid = GetTID(MID_SLP);
//
//			if(pTid != NULL)
//			{
//				
//				mpMessage.head.receiver.node = pTid->node;
//		#ifdef _WIN32
//				{
//					mpMessage.head.receiver.hWnd= pTid->hWnd;
//				}
//		#else
//				{
//					mpMessage.head.receiver.procece=pTid->procece;
//				}
//		#endif
//				mpMessage.head.receiver.module=pTid->module;
//				mpMessage.head.receiver.mId=pTid->mId;
//			}
//			pTid = GetTID(MID_ACC);
//
//			if(pTid != NULL)
//			{
//				mpMessage.head.sender.node = pTid->node;
//		#ifdef _WIN32
//				{
//					mpMessage.head.sender.hWnd= pTid->hWnd;
//				}
//		#else
//				{
//					mpMessage.head.sender.procece=pTid->procece;
//				}
//		#endif
//				mpMessage.head.sender.module=pTid->module;
//				mpMessage.head.sender.mId=pTid->mId;
//				
//			}
		}
			
	}
	else
	{
		memset(&pTid,0,sizeof(TID));
		if(GetTid(MID_SLP,&pTid))
		{
			mpMessage.head.receiver = pTid;
		}
		memset(&pTid,0,sizeof(TID));
		if(GetTid(MID_ACC,&pTid))
		{
			mpMessage.head.sender = pTid;
		}
//		pTid = GetTID(MID_SLP);
////		if(bPlatForm)//ƽ̨
////			pTid = GetTID(MID_SLP);
////		else
////			pTid = GetTID(MID_COM);
//		if(pTid != NULL)
//		{
//			
//			mpMessage.head.receiver.node = pTid->node;
//	#ifdef _WIN32
//			{
//				mpMessage.head.receiver.hWnd= pTid->hWnd;
//			}
//	#else
//			{
//				mpMessage.head.receiver.procece=pTid->procece;
//			}
//	#endif
//			mpMessage.head.receiver.module=pTid->module;
//			mpMessage.head.receiver.mId=pTid->mId;
//		}
	}

// 	pTid = NULL;
		
	mpMessage.head.len = len;
	mpMessage.head.event = devent;
	memset(mpMessage.data,0,sizeof(mpMessage.data));

	memcpy(mpMessage.data,pInfor,len);
	receivenumber=(BYTE)MID_SLP+g_Sid[sid].bnodenum*256;
	WriteLog(receivenumber);
	bSend=SEND_MSG(evIPCMessage,receivenumber,(void *)&mpMessage,(WORD)sizeof(MESSAGE),(WORD)sid,FALSE,NULL,0);
	if((devent!=evPXSSM_ReleaseCall)&&(evPXSSM_ErrorReport!=devent))
	{
		timerId = SET_TIMER(sXssmTime.sysTimeout,evTimer1,MID_ACC,(WORD)sid,FALSE);
		g_Sid[sid].timerId = timerId;
	}
	YF_TRACE sid,"","pXSSM: SendMessage,sender=%d,receiver=%d,source=%d,event=%d,bnew=%d,bPlat=%d,receivenumber=%d"
		,mpMessage.head.sender.module,mpMessage.head.receiver.module,mpMessage.head.Source.module
		,mpMessage.head.event,bNew,bPlatForm,receivenumber);
	WriteLog(bSend);
	return TRUE;

}
/********************************************************************
�������ƣ�XSSM_DoCallIn
��������: ����
��������: pInfor:��������ָ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-1 16:16
��������: ������
ע������: ��	
*********************************************************************/

BOOL XSSM_DoCallIn(void * pInfor)
{
	MESSAGE pMessage;
	TID *pTid = NULL;
	int wNumber = -1;
	int moudlenumber;
	ACS_Hdl_EVENT *pEvent = (ACS_Hdl_EVENT *)pInfor;
	stPXSSM_InitiateDP pInitiateDP;
	stPXSSM_InitiateRecord pRecord;
	WORD timerId=0;
	PVOICE *pPvoice = NULL;
	if(pEvent==NULL)
		return FALSE;
	WriteLog(TRUE);
	moudlenumber=FindKeyNode(pEvent->CalledNum,pEvent->CallingNum);
	
	
	if(moudlenumber<0)
	{
		YF_LOG_SMM "ҵ�������,calledNum=%s,callingNum=%s",pEvent->CalledNum,pEvent->CallingNum);
		Hdl_ReleaseCall(0,pEvent->DeviceNumber,FALSE);
		WriteLog_char(pEvent->CalledNum);
		WriteLog_char("ҵ�������");

		return FALSE;
	}

	pTid = GetTID(MID_ACC);
	
	memset(&pMessage,0,sizeof(MESSAGE));
	if(pTid != NULL)
	{
		
		pMessage.head.sender.node = pTid->node;
		
		pMessage.head.Source.node = pTid->node;
#ifdef _WIN32
		{
			pMessage.head.sender.hWnd= pTid->hWnd;
			pMessage.head.Source.hWnd= pTid->hWnd;
			
		}
#else
		{
			pMessage.head.sender.procece=pTid->procece;
			pMessage.head.Source.procece=pTid->procece;
		}
#endif
		pMessage.head.sender.module=pTid->module;
		pMessage.head.Source.module=pTid->module;
		pMessage.head.Source.mId=pTid->mId;
	}

	pMessage.head.event = evXSSM_Base;

	
	pMessage.head.ptrAck=0;
	pMessage.head.ackLen=0;
	pMessage.head.reserve=0;
	pMessage.head.sync=FALSE;
	pMessage.head.event = evtXSSM_CallIn;
	wNumber=GetSenssionID();/*����Ự��*/
	Update_Message((WORD)wNumber,&pMessage,FALSE);
	pInitiateDP.sid = wNumber;
	pInitiateDP.serviceKey = FindServiceKey(pEvent->CalledNum,pEvent->CallingNum);
	strcpy(pInitiateDP.calledNumber,pEvent->CalledNum);
	strcpy(pInitiateDP.callingNumber,pEvent->CallingNum);
	strcpy(pInitiateDP.dialedNumber,pEvent->CalledNum);
	if((wNumber >= 0)&&(wNumber<MESSAGE_MAX))
	{
		pEvent->sid = wNumber;
		g_Sid[wNumber].status = evtXSSM_CallIn;
		strcpy(g_Sid[wNumber].CalledNumber,pEvent->CalledNum);
		strcpy(g_Sid[wNumber].CallingNumber,pEvent->CallingNum);
		g_Sid[wNumber].sid = wNumber;
		g_Sid[wNumber].bnodenum = moudlenumber;
		g_Sid[wNumber].serviceKey = pInitiateDP.serviceKey;		
	}
	
	else//�ͷź���
	{
		
		YF_LOG_SMM "��������,release calling");
	
		Hdl_ReleaseCall(0,pEvent->DeviceNumber,FALSE);
		WriteLog_char("��������");
		return FALSE;
	}
	WriteLog_char(pEvent->CallingNum);
	//����ҵ�񽡷���Ự��Ȼ������Ӧ��ҵ�����̷�����Ϣ

	//��PXSSM.DLL���ûỰ��
	
	Hdl_setsid((DWORD)wNumber,pEvent->DeviceNumber,pEvent->CalledNum,pEvent->CallingNum);
	Hdl_StartDPAck(pEvent->DeviceNumber,TRUE);
	Hdl_ConnectToResource((DWORD)wNumber);

	XSSM_SENDMESSAGE((DWORD)wNumber,&pInitiateDP/*pEvent*/,sizeof(stPXSSM_InitiateDP),evPXSSM_InitiateDP,TRUE);
	
	pRecord.sid = wNumber;
	pRecord.fileIndex = 1;
	pRecord.duration = 0;
	pRecord.canInterupt = 0 ; //�û���������Ƿ���Դ��
    pRecord.replaceExistedFile = 0 ; //�Ƿ񸲸Ǵ��ڵ��ļ�
//	Hdl_InitateRecord(pRecord);
//	timerId = SET_TIMER(sXssmTime.sysTimeout,evTimer1,MID_ACC,(WORD)wNumber,FALSE);

//	g_Sid[wNumber].timerId = timerId;
	WriteLog(TRUE);
	YF_TRACE wNumber,"","session begin,calling=%s,called=%s,timeId = %d",pEvent->CallingNum,pEvent->CalledNum,timerId);
	return TRUE;
}
/********************************************************************
�������ƣ�SetWaiteTimer
��������: ����������ö�ʱ��
��������: ��
���ض���: �ɹ�����TRUE,ʰܷ���FALSE
����ʱ��: 2008-2-20 11:31
��������: ������
ע������: ��	
*********************************************************************/
void SetWaiteTimer(DWORD sid)
{
	WORD timerId;
	if(sid>MESSAGE_MAX)
		return ;
	if(g_Sid[sid].attrd.bPlayFile==0)
		return ; 
	g_Sid[sid].attrd.bPlayFileEnd = 1;
	g_Sid[sid].attrd.bPlayFile = 0;
	timerId = SET_TIMER(g_Sid[sid].pCollectInfor.firstDigitTimeOut,evTimer1,MID_ACC,(WORD)sid,FALSE);
	g_Sid[sid].timerId = timerId;
}
/********************************************************************
�������ƣ�XSSM_DoAll
��������: �������Զ�̬�����Ϣ��������Ϣ���ݸ�ҵ��ģ��
��������: ��
���ض���: �ɹ�����TRUE,ʧ��FALSE
����ʱ��: 2008-2-16 11:16
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_DoAll(void * pInfor)
{
	
//	int moudlenumber;
	ACS_Hdl_EVENT *pEvent = (ACS_Hdl_EVENT *)pInfor;
	XSSM_SENDMESSAGE(pEvent->sid,pEvent,sizeof(ACS_Hdl_EVENT),evXSSM_Base,FALSE);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_DoCallOut
��������: ��������
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-15 9:35
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_DoCallOut(void * pInfor)
{
	ACS_Hdl_EVENT *pEvent = (ACS_Hdl_EVENT *)pInfor;
	XSSM_SENDMESSAGE(pEvent->sid,&pEvent->sid,sizeof(DWORD),evPXSSM_Connect,FALSE);
	return TRUE;
		 
}
/********************************************************************
�������ƣ�XSSM_DoClearCall
��������: ����һ��¼�
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-16 10:48
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_DoClearCall(void *pInfor)
{
	ACS_Hdl_EVENT *pEvent = (ACS_Hdl_EVENT *)pInfor;
	stPXSSM_ReleaseCall pReleaseCall;


	XSSM_RecvDisConnectForwardConnection(&pEvent->sid);
//	Sleep(1000);
	Sleep(10);
	pReleaseCall.sid = pEvent->sid;
	pReleaseCall.reason = TRelease;
	XSSM_SENDMESSAGE(pEvent->sid,&pReleaseCall,sizeof(stPXSSM_ReleaseCall),evPXSSM_ReleaseCall,FALSE);
	Hdl_clearsid(pEvent->sid);
	memset(&g_Sid[pEvent->sid],0,sizeof(XSSM_SID));
	g_Sid[pEvent->sid].sid = MESSAGE_MAX;
	YF_TRACE pEvent->sid,"XSSM_DoClearCall","release call");
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_DoRecvIoData
��������: ���յ����ݴ���
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-19 17:15
��������: ������
ע������: ��	
*********************************************************************/

BOOL XSSM_DoRecvIoData(void *pInfor)
{
	DWORD sid ;
	stPXSSM_CollectedInformation pCollectedInformation;
	stPXSSM_CollectedFSKInformation pCollectedFSKInfor;
	BYTE data[512];
	WORD timerId;
	ACS_Hdl_EVENT *pEvent = (ACS_Hdl_EVENT *)pInfor;
	sid = pEvent->sid;

	WriteLog(11);	
	if(sid>=MESSAGE_MAX)
	{
		YF_LOG_SMM "XSSM_DoRecvIoData,sid=%d overflow",sid);
		return FALSE;
	}
//	memcpy(pData,pEvent->ioData,pEvent->datalen);

	if((!g_Sid[sid].attrd.bInterrupt)&&(g_Sid[sid].attrd.bPlayFile))//���ܴ���һ��ڷ�������
	{
		YF_TRACE sid,"","���ܴ���һ��ڷ�������");
	//	return FALSE;
	}
	if(g_Sid[sid].attrd.bPlayFile)
	{
		YF_TRACE sid,"","����ֹͣ����");
		//����ֹͣ����
		Hdl_StopFilePlay(sid);
	}
	if (pEvent->datalen==0) 
	{
		YF_LOG_SMM "XSSM_DoRecvIoData:recvice data is 0");
		return FALSE;
	}
	memset(data,0,sizeof(data));
	memcpy(data,pEvent->ioData,pEvent->datalen);
	if(g_Trace)
	{
		BCD_TO_ASCIIString((WORD)sid,data,pEvent->datalen);
	}
//	data =(BYTE *) pEvent->ioData;
    YF_TRACE sid,"","Received io data,current status = %d",g_Sid[sid].status);
	if(g_Sid[sid].status==evPXSSM_SendFSKCollectFSK)
	{
//send fsk to xvapl
		DoKillTimer(evTimer1,sid);
//		YF_TRACE sid,"XSSM_DoRecvIoData","DoKillTimer");
		pCollectedFSKInfor.sid = sid;
		pCollectedFSKInfor.fsk.length = pEvent->datalen;
		memcpy(pCollectedFSKInfor.fsk.message,pEvent->ioData,pEvent->datalen);
		YF_TRACE sid,"","�����յ���FSK");
		XSSM_CollectedFSKInformation(&pCollectedFSKInfor);
		return TRUE;
	}
	if((g_Sid[sid].status != evPXSSM_PromptCollectInformation)
		&&(g_Sid[sid].status != evPXSSM_SendFSKCollectInformation)
		&&(g_Sid[sid].status != evPXSSM_PromptCollectInformationAndSetFormat))
	{
		YF_LOG_SMM "��״̬��֧�ֵĲ�����������");
		return FALSE;
	}
	DoKillTimer(evTimer1,sid);
//	YF_TRACE sid,"XSSM_DoRecvIoData","DoKillTimer");
	if((g_Sid[sid].pCollectInfor.collectedDigits!=0)&&(data[0]==g_Sid[sid].pCollectInfor.cancelDigits))//ȡ������
	{
		memset(g_Sid[sid].pCollectInfor.collectedDigits,0,sizeof(g_Sid[sid].pCollectInfor.collectedDigits));
		g_Sid[sid].pCollectInfor.bConnnectNumber=0;
		timerId = SET_TIMER(g_Sid[sid].pCollectInfor.firstDigitTimeOut*10,evTimer1,MID_ACC,(WORD)sid,FALSE);
		g_Sid[sid].timerId = timerId;
		return FALSE;
	}
	if(g_Sid[sid].pCollectInfor.bConnnectNumber==0)//�տ�ʼ����������Ϣ
	{
		if(g_Sid[sid].pCollectInfor.startDigit!=0)//��ʼ�ַ�������0
		{
			if(g_Sid[sid].pCollectInfor.startDigit!=data[0])
				return FALSE;
			else
			{
				
				memcpy(g_Sid[sid].pCollectInfor.collectedDigits,pEvent->ioData,pEvent->datalen);
				g_Sid[sid].pCollectInfor.bConnnectNumber += pEvent->datalen;
				
			}
		}
		else
		{
			memcpy(g_Sid[sid].pCollectInfor.collectedDigits,pEvent->ioData,pEvent->datalen);
			g_Sid[sid].pCollectInfor.bConnnectNumber += pEvent->datalen;
		}
	}
	else//�м����������Ϣ 
	{
		// DoKillTimer(evTimer1,sid);
		memcpy(g_Sid[sid].pCollectInfor.collectedDigits+g_Sid[sid].pCollectInfor.bConnnectNumber
			,pEvent->ioData,pEvent->datalen);
		g_Sid[sid].pCollectInfor.bConnnectNumber += pEvent->datalen;

	}
	if(g_Sid[sid].pCollectInfor.minDigits==g_Sid[sid].pCollectInfor.maxDigits)//min==max,�����ַ�����
	{
		if(g_Sid[sid].pCollectInfor.bConnnectNumber>=g_Sid[sid].pCollectInfor.maxDigits)
		{
//			pTmid.event=evTimer1;
//			pTmid.dwTime=g_Sid[sid].pCollectInfor.interDigitTimeOut;
//			pTmid.mId = MID_ACC;
//			pTmid.isAbs = FALSE;
//			pTmid.wNum = (WORD)sid;
//			pTmid.ref = 0;
//			KILL_TIMER(pTmid);

			g_Sid[sid].status = pEvent->event;
			pCollectedInformation.sid =sid;
			memcpy(pCollectedInformation.collectedDigits,g_Sid[sid].pCollectInfor.collectedDigits,sizeof(pCollectedInformation.collectedDigits));
			XSSM_CollectedInformation(pCollectedInformation);
			return TRUE;
		}
	}
	else if(g_Sid[sid].pCollectInfor.endDigit==g_Sid[sid].pCollectInfor.collectedDigits[g_Sid[sid].pCollectInfor.bConnnectNumber-1])//�յ������ַ�
	{
//		pTmid.event=evTimer1;
//		pTmid.dwTime=g_Sid[sid].pCollectInfor.interDigitTimeOut;
//		pTmid.mId = MID_ACC;
//		pTmid.isAbs = FALSE;
//		pTmid.wNum = (WORD)sid;
//		pTmid.ref = 0;
//		KILL_TIMER(pTmid);
		g_Sid[sid].status = pEvent->event;
		pCollectedInformation.sid =sid;
		memcpy(pCollectedInformation.collectedDigits,g_Sid[sid].pCollectInfor.collectedDigits,sizeof(pCollectedInformation.collectedDigits));
		XSSM_CollectedInformation(pCollectedInformation);
		return TRUE;
	}
	else if(g_Sid[sid].pCollectInfor.bConnnectNumber>=g_Sid[sid].pCollectInfor.maxDigits)//
	{
//		pTmid.event=evTimer1;
//		pTmid.dwTime=g_Sid[sid].pCollectInfor.interDigitTimeOut;
//		pTmid.mId = MID_ACC;
//		pTmid.isAbs = FALSE;
//		pTmid.wNum = (WORD)sid;
//		pTmid.ref = 0;
//		KILL_TIMER(pTmid);
		g_Sid[sid].status = pEvent->event;
		pCollectedInformation.sid =sid;
		memcpy(pCollectedInformation.collectedDigits,g_Sid[sid].pCollectInfor.collectedDigits,sizeof(pCollectedInformation.collectedDigits));
		XSSM_CollectedInformation(pCollectedInformation);
		return TRUE;
	}

	timerId = SET_TIMER(g_Sid[sid].pCollectInfor.interDigitTimeOut*10,evTimer1,MID_ACC,(WORD)sid,FALSE);
	g_Sid[sid].timerId = timerId;
	pEvent = NULL;
	return  TRUE;
}
/********************************************************************
�������ƣ�XSSM_ActiveTestResponse
��������: ���������Ӧ
��������: lParam:��Ϣ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 9:24
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_ActiveTestResponse(WORD lParam)
{
	XSSM_SENDMESSAGE((DWORD)lParam,&lParam,sizeof(DWORD),evPXSSM_ActivityTestResponse,FALSE);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_ErrorReport
��������: ���󱨸�
��������: sid:�Ự�ţ�ErrorType����������
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 9:29
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_ErrorReport(DWORD sid,BYTE ErrorType)
					  
{
	stPXSSM_ErrorReport pErrorReport;
	pErrorReport.sid = sid;
	pErrorReport.error = ErrorType;
	XSSM_SENDMESSAGE(sid,&pErrorReport,sizeof(stPXSSM_ErrorReport),evPXSSM_ErrorReport,FALSE);
	YF_TRACE sid,"XSSM_ErrorReport","ErrorType=%d",ErrorType);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_StartDP
��������: ��������
��������: pInitiateDP��������Ϣ��������
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 9:30
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_StartDP(stPXSSM_InitiateDP pInitiateDP)
{
	DWORD sid = pInitiateDP.sid;
	XSSM_SENDMESSAGE(sid,&pInitiateDP,sizeof(stPXSSM_InitiateDP),evPXSSM_InitiateDP,FALSE);
	YF_TRACE sid,"XSSM_StartDP","start service");
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RealeaseCall
��������: �ͷź���
��������: pReleaseCall:�ͷź���ԭ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 9:34
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RealeaseCall(stPXSSM_ReleaseCall pReleaseCall)
{
	DWORD sid = pReleaseCall.sid;
	while(g_Sid[sid].attrd.bPlayFile)
	{
		Sleep(100);
	}
	XSSM_SENDMESSAGE(sid,&pReleaseCall,sizeof(stPXSSM_ReleaseCall),evPXSSM_ReleaseCall,FALSE);
	YF_TRACE sid,"XSSM_RealeaseCall","user release");
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_CollectedInformation
��������: �Ѽ��û���Ϣ
��������: pCollectedInformation���û���Ϣ��DTMF��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 9:41
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_CollectedInformation(stPXSSM_CollectedInformation pCollectedInformation)
{
	DWORD sid = pCollectedInformation.sid;
	YF_TRACE (WORD)sid,"","send dtmf information");
	g_Sid[sid].status = 0;
	XSSM_SENDMESSAGE(sid,&pCollectedInformation,sizeof(stPXSSM_CollectedInformation),evPXSSM_CollectedInformation,FALSE);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_CollectedFSKInformation
��������: �Ѽ���FSK��Ϣ
��������: pCollectedFSKInformation��FSK��Ϣ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 9:41
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_CollectedFSKInformation(stPXSSM_CollectedFSKInformation *pCollectedFSKInformation)
{
	DWORD sid ;
	int len;

	YF_TRACE -2,"XSSM_CollectedFSKInformation","service recv fsk data");

	if(NULL == pCollectedFSKInformation)
	{
		YF_TRACE -2,"XSSM_CollectedFSKInformation","NULL == pCollectedFSKInformation");
		return FALSE;
	}
	sid = pCollectedFSKInformation->sid;
	if(MESSAGE_MAX<=sid)
	{
	    YF_TRACE sid,"XSSM_CollectedFSKInformation","MESSAGE_MAX<=sid");
		return FALSE;
	}

	g_Sid[sid].status = 0;
	len = sizeof(stPXSSM_CollectedFSKInformation)-sizeof(pCollectedFSKInformation->fsk.message)+pCollectedFSKInformation->fsk.length;
	XSSM_SENDMESSAGE(sid,pCollectedFSKInformation,len,evPXSSM_CollectedFSKInformation,FALSE);
	
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_ResourceReport
��������: ��Դ���棬
��������: sid:�Ự��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 9:49
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_ResourceReport(/*DWORD sid*/stPXSSM_ResourceReport *pResourceReport)
{
	if(pResourceReport == NULL)
		return FALSE;
	XSSM_SENDMESSAGE(pResourceReport->sid,pResourceReport,sizeof(stPXSSM_ResourceReport),evPXSSM_ResourceReport,FALSE);
	YF_TRACE pResourceReport->sid,"","XSSM_ResourceReport");
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RecvConnectToResouce
��������: ����������
��������: pCollectToResource��������Ϣ
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:54
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvConnectToResouce(void *infor/*stPXSSM_ConnectToResource pCollectToResource*/)
{
	stPXSSM_ConnectToResource *pCollectToResource = NULL;
	pCollectToResource = (stPXSSM_ConnectToResource *)infor;
	if(pCollectToResource == NULL)
	{
		return FALSE;
	}
	DoKillTimer(evTimer1,pCollectToResource->sid);
//	YF_TRACE pCollectToResource->sid,"XSSM_RecvConnectToResouce","DoKillTimer");
	if(Hdl_ConnectToResource(pCollectToResource->sid))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/********************************************************************
�������ƣ�XSSM_RecvDisConnectForwardConnection
��������: �ж�ǰ��������Դ���ӣ��󶨣�
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:54
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvDisConnectForwardConnection(void *infor/*DWORD sid*/)
{
	DWORD sid=0;
	BYTE *data = (BYTE *)infor;
	if(data==NULL)
		return FALSE;
	DoKillTimer(evTimer1,sid);
//	YF_TRACE sid,"XSSM_RecvDisConnectForwardConnection","DoKillTimer");
	memcpy(&sid,data,sizeof(DWORD));
	Hdl_DisconnectForwardResource(sid);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RecvPlayAnnouncement
��������: ����֪ͨ
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:54
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvPlayAnnouncement(void *infor/*stPXSSM_PlayAnnouncement pPlayAnnouncement*/)
{
	DWORD sid;
	PVOICE *pVoice = NULL;
	stPXSSM_PlayAnnouncement *pPlayAnnouncement = (stPXSSM_PlayAnnouncement *)infor;
	sid = pPlayAnnouncement->sid;
	DoKillTimer(evTimer1,sid);
//	YF_TRACE sid,"XSSM_RecvPlayAnnouncement","DoKillTimer");

	pVoice = SwtichToneFile(sid,&pPlayAnnouncement->tone);
	if(pVoice == NULL)
		return FALSE;
	Hdl_StopFilePlay(sid);
	YF_TRACE sid,"","XSSM_RecvPlayAnnouncement,repeatTime=%d,duration=%d,interval=%d,canInterupt=%d",pPlayAnnouncement->repeatTime,pPlayAnnouncement->duration,pPlayAnnouncement->interval,pPlayAnnouncement->canInterupt);
	Hdl_FilePlay(sid,(void *)pVoice);
	g_Sid[sid].attrd.bInterrupt = pPlayAnnouncement->canInterupt;//�ɷ���
	g_Sid[sid].attrd.bPlayFile = 1;
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RecvPromptCollectInformation
��������: ������ʾ���ռ��û���Ϣ
��������: pPromptCollectInformation����Ϣ�ṹ
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:55
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvPromptCollectInformation(void *infor/*stPXSSM_PromptCollectInformation pPromptCollectInformation*/)
{
	DWORD sid;
	PVOICE *pVoice=NULL;
	stPXSSM_PromptCollectInformation *pPromptCollectInformation = (stPXSSM_PromptCollectInformation *)infor;
	
	if(pPromptCollectInformation == NULL)
	{
		YF_LOG_SMM "XSSM_RecvPromptCollectInformation is NULL");
		return FALSE;
	}
	sid = pPromptCollectInformation->playTone.sid;
	DoKillTimer(evTimer1,sid);
//	YF_TRACE sid,"XSSM_RecvPromptCollectInformation","DoKillTimer");
	if(sid>=MESSAGE_MAX)
	{
		YF_LOG_SMM "XSSM_RecvPromptCollectInformation ,sid=%d,is overflow",sid);
		return FALSE;
	}
	YF_TRACE (WORD)sid,"","evPXSSM_PromptCollectInformation");
	pVoice = SwtichToneFile(sid,&pPromptCollectInformation->playTone.tone);
	if(pVoice == NULL)
		return FALSE;
	WriteLog(10);
	Hdl_StopFilePlay(sid);
	Hdl_FilePlay(sid,(void *)pVoice);
	
	g_Sid[sid].pCollectInfor.minDigits = pPromptCollectInformation->minDigits;
	g_Sid[sid].pCollectInfor.maxDigits = pPromptCollectInformation->maxDigits;
	g_Sid[sid].pCollectInfor.endDigit = pPromptCollectInformation->endDigit;
	g_Sid[sid].pCollectInfor.cancelDigits = pPromptCollectInformation->cancelDigits;
	g_Sid[sid].pCollectInfor.startDigit = pPromptCollectInformation->startDigit;
	g_Sid[sid].pCollectInfor.firstDigitTimeOut = pPromptCollectInformation->firstDigitTimeOut;
	g_Sid[sid].pCollectInfor.interDigitTimeOut = pPromptCollectInformation->interDigitTimeOut;
	g_Sid[sid].attrd.bPlayFile = TRUE;
	g_Sid[sid].attrd.bInterrupt = pPromptCollectInformation->playTone.canInterupt;
	SetWaiteTimer(sid);
	YF_TRACE sid,"XSSM_RecvPromptCollectInformation","min=%d,max=%d,end=%d,cance=%d,start=%d,fsTO=%d,intTO=%d,repeatTime=%d,duration=%d,interval=%d,canInterupt=%d",
	pPromptCollectInformation->minDigits,
	pPromptCollectInformation->maxDigits,
	 pPromptCollectInformation->endDigit,
	 pPromptCollectInformation->cancelDigits,
	pPromptCollectInformation->startDigit,
	 pPromptCollectInformation->firstDigitTimeOut,
	pPromptCollectInformation->interDigitTimeOut,
	pPromptCollectInformation->playTone.repeatTime,
	pPromptCollectInformation->playTone.duration,
	pPromptCollectInformation->playTone.interval,
	pPromptCollectInformation->playTone.canInterupt);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_PromptCollectInformationAndSetFormat
��������: ���ò��ռ��û���Ϣ
��������: pPromptCollectInformation����Ϣ�ṹ
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:55
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_PromptCollectInformationAndSetFormat(void *infor)
{

	DWORD sid;
	stPXSSM_PromptCollectInformationAndSetFormat *pPromptCollectInformation = (stPXSSM_PromptCollectInformationAndSetFormat *)infor;
	if(pPromptCollectInformation == NULL)
	{
		YF_LOG_SMM "stPXSSM_PromptCollectInformationAndSetFormat is null");
		return FALSE;
	}
	sid = pPromptCollectInformation->sid;
	DoKillTimer(evTimer1,sid);
//	YF_TRACE sid,"XSSM_PromptCollectInformationAndSetFormat","DoKillTimer");
	if(sid>MESSAGE_MAX)
	{
		YF_TRACE (WORD)sid,"","stPXSSM_PromptCollectInformationAndSetFormat's sid is overflow");
		return FALSE;
	}
	g_Sid[sid].pCollectInfor.minDigits = pPromptCollectInformation->minDigits;
	g_Sid[sid].pCollectInfor.maxDigits = pPromptCollectInformation->maxDigits;
	g_Sid[sid].pCollectInfor.endDigit = pPromptCollectInformation->endDigit;
	g_Sid[sid].pCollectInfor.cancelDigits = pPromptCollectInformation->cancelDigits;
	g_Sid[sid].pCollectInfor.startDigit = pPromptCollectInformation->startDigit;
	g_Sid[sid].pCollectInfor.firstDigitTimeOut = pPromptCollectInformation->firstDigitTimeOut;
	g_Sid[sid].pCollectInfor.interDigitTimeOut = pPromptCollectInformation->interDigitTimeOut;
	g_Sid[sid].attrd.bPlayFile = FALSE;
	g_Sid[sid].attrd.bInterrupt = FALSE;
	SetWaiteTimer(sid);
	YF_TRACE sid,"XSSM_PromptCollectInformationAndSetFormat","min=%d,max=%d,end=%d,cance=%d,start=%d,fsTO=%d,intTO=%d",
	pPromptCollectInformation->minDigits,
	pPromptCollectInformation->maxDigits,
	 pPromptCollectInformation->endDigit,
	 pPromptCollectInformation->cancelDigits,
	pPromptCollectInformation->startDigit,
	 pPromptCollectInformation->firstDigitTimeOut,
	pPromptCollectInformation->interDigitTimeOut);
	return TRUE;

}
/********************************************************************
�������ƣ�XSSM_RecvSendFSK
��������: ����
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:55
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvSendFSK(void *infor/*stPXSSM_SendFSK pSendFSK*/)
{
	
	stPXSSM_SendFSK *pSendFSK = (stPXSSM_SendFSK *)infor;
	DWORD wNumber;
	if(pSendFSK == NULL)
		return FALSE;
	WriteLog(6);
	DoKillTimer(evTimer1,pSendFSK->sid);
//	YF_TRACE pSendFSK->sid,"XSSM_RecvSendFSK","DoKillTimer");
	if(g_Trace)
	{
		BCD_TO_ASCIIString((WORD)pSendFSK->sid,pSendFSK->fsk.message,pSendFSK->fsk.length);
	}
	wNumber = pSendFSK->sid;
	if(wNumber>=MESSAGE_MAX)
	{
		return FALSE;
	}
	if(g_Sid[wNumber].sid != wNumber)
	{
		return FALSE;
	}
	//if(Hdl_SendFSK(pSendFSK->sid,pSendFSK->fsk.message,pSendFSK->fsk.length))
	if(Hdl_SendFSK(pSendFSK->sid,&(pSendFSK->fsk)))
	{
		g_Sid[pSendFSK->sid].attrd.bSendDataNow=1;
		g_Sid[pSendFSK->sid].pCollectInfor.firstDigitTimeOut = sXssmTime.hdlTimeout;
		g_Sid[pSendFSK->sid].attrd.bPlayFile = TRUE;
		SetWaiteTimer(pSendFSK->sid);
		return  TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL XSSM_RecvPlayAnnouncementAndSetFSK(void *infor)
{
	DWORD sid;
	PVOICE *pVoice=NULL;
////
	stPXSSM_InitiateRecord pRecord;
///
	stPXSSM_PromptCollectFSK *pPromptCollectFsk = (stPXSSM_PromptCollectFSK *)infor;
	if(pPromptCollectFsk == NULL)
		return FALSE;
	sid = pPromptCollectFsk->playTone.sid;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	DoKillTimer(evTimer1,sid);
//	YF_TRACE sid,"XSSM_RecvPlayAnnouncementAndSetFSK","DoKillTimer");
////////////
	pRecord.sid = sid;
	pRecord.fileIndex = 1;
	pRecord.duration = 0;
	pRecord.canInterupt = 0 ; //�û���������Ƿ���Դ��
    pRecord.replaceExistedFile = 0 ; //�Ƿ񸲸Ǵ��ڵ��ļ�
//	Hdl_InitateRecord(pRecord);
////////////

	
	if(pPromptCollectFsk->Fskenable)
	{
		SetSidEvent((WORD)sid,evPXSSM_SendFSKCollectFSK);
	}
	pVoice = SwtichToneFile(sid,&pPromptCollectFsk->playTone.tone);
	if(pVoice == NULL)
	{
		YF_LOG_SMM "Play tone not exist,tone id = %d",pPromptCollectFsk->playTone.tone);
		return FALSE;
	}
	Hdl_StopFilePlay(sid);
	Hdl_FilePlay(sid,(void *)pVoice);
	YF_TRACE sid,"XSSM_RecvPlayAnnouncementAndSetFSK","repeatTime=%d,duration=%d,interval=%d,canInterupt=%d",pPromptCollectFsk->playTone.repeatTime,pPromptCollectFsk->playTone.duration,pPromptCollectFsk->playTone.interval,pPromptCollectFsk->playTone.canInterupt);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RecvSendFSKCollectInformation
��������: ������FSK���Ѽ�DTMF
��������: pSendFSKCollectInformation��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:56
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvSendFSKCollectInformation(void *infor/*stPXSSM_SendFSKCollectInformation pSendFSKCollectInformation*/)
{
	stPXSSM_SendFSKCollectInformation *pSendFSKCollectInformation = (stPXSSM_SendFSKCollectInformation *)infor;
	DWORD wNumber;
	if(pSendFSKCollectInformation == NULL)
		return FALSE;
	WriteLog(7);
	if(g_Trace)
	{
		BCD_TO_ASCIIString((WORD)pSendFSKCollectInformation->playFSK.sid,pSendFSKCollectInformation->playFSK.fsk.message,pSendFSKCollectInformation->playFSK.fsk.length);
	}
	wNumber = pSendFSKCollectInformation->playFSK.sid;
	if(wNumber>=MESSAGE_MAX)
	{
		return FALSE;
	}
	if(g_Sid[wNumber].sid != wNumber)
	{
		return FALSE;
	}
	if(Hdl_SendFSK(pSendFSKCollectInformation->playFSK.sid
		,&(pSendFSKCollectInformation->playFSK.fsk)))
//		,pSendFSKCollectInformation->playFSK.fsk.message,pSendFSKCollectInformation->playFSK.fsk.length))
	{
		g_Sid[pSendFSKCollectInformation->playFSK.sid].attrd.bSendDataNow=1;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/********************************************************************
�������ƣ�ControlTrace
��������: ����״̬����
��������: infor:��������
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2009-4-1 10:39
��������: ������
ע������: ��	
*********************************************************************/
BOOL ControlTrace(void *infor)
{
	BYTE *data = (BYTE *)infor;
	if(data == NULL)
		return FALSE;
	EnableTrace(data[0]);
	Hdl_SetTrace(g_Trace);
	Comm_SetTraceValue(g_Trace);

	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RecvSendFSKCollectFSK
��������: ������FSK��Ϣ�����ռ�FSK��Ϣ
��������: pSendFSKCollectFSK��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:56
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvSendFSKCollectFSK(void *infor/*stPXSSM_SendFSKCollectFSK pSendFSKCollectFSK*/)
{
	stPXSSM_SendFSKCollectFSK *pSendFSKCollectFSK = (stPXSSM_SendFSKCollectFSK *)infor;
	DWORD wNumber;
	
	if(pSendFSKCollectFSK == NULL)
		return FALSE;
	WriteLog(8);
	if(g_Trace)
	{
		BCD_TO_ASCIIString((WORD)pSendFSKCollectFSK->playFSK.sid,pSendFSKCollectFSK->playFSK.fsk.message,pSendFSKCollectFSK->playFSK.fsk.length);
	}
	wNumber = pSendFSKCollectFSK->playFSK.sid;
	if(wNumber>=MESSAGE_MAX)
	{
		return FALSE;
	}
	DoKillTimer(evTimer1,wNumber);
	WriteLog(pSendFSKCollectFSK->timeOut);
//	YF_TRACE wNumber,"XSSM_RecvSendFSKCollectFSK","DoKillTimer");
	if(g_Sid[wNumber].sid != wNumber)
	{
		g_Sid[wNumber].pCollectInfor.firstDigitTimeOut = pSendFSKCollectFSK->timeOut;
		g_Sid[wNumber].attrd.bPlayFile = 1;
		SetWaiteTimer(wNumber);
		return FALSE;
	}
	if(pSendFSKCollectFSK->playFSK.fsk.length==0)
	{
		g_Sid[wNumber].pCollectInfor.firstDigitTimeOut = pSendFSKCollectFSK->timeOut;
		g_Sid[wNumber].attrd.bPlayFile = 1;
		SetWaiteTimer(wNumber);
		return FALSE;
	}

	if(Hdl_SendFSK(wNumber
		,&(pSendFSKCollectFSK->playFSK.fsk)))
//		,pSendFSKCollectFSK->playFSK.fsk.message,pSendFSKCollectFSK->playFSK.fsk.length))
	{
			WriteLog(pSendFSKCollectFSK->playFSK.fsk.length);
		g_Sid[wNumber].pCollectInfor.firstDigitTimeOut = pSendFSKCollectFSK->timeOut;
		g_Sid[wNumber].attrd.bSendDataNow=1;
		g_Sid[wNumber].attrd.bPlayFile = 1;
		SetWaiteTimer(wNumber);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
	

}
/********************************************************************
�������ƣ�XSSM_RecvInitiateRecord
��������: ��������¼��
��������: ¼��������Ϣ
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:56
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvInitiateRecord(void *infor/*stPXSSM_InitiateRecord pInitiateRecord*/)
{
	stPXSSM_InitiateRecord *pInitiateRecord = (stPXSSM_InitiateRecord *)infor;
	stPXSSM_InitiateRecord mInitiateRecord;
	if(pInitiateRecord == NULL)
		return FALSE;
	memcpy(&mInitiateRecord,pInitiateRecord,sizeof(stPXSSM_InitiateRecord));
	Hdl_InitateRecord(mInitiateRecord);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RecvStopRecord
��������: ����ֹͣ¼��
��������: sid���Ự��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-28 15:56
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvStopRecord(void *infor/*DWORD sid*/)
{
	DWORD sid;
	BYTE *data = (BYTE *)infor;
	memcpy(&sid,data,sizeof(DWORD));
	Hdl_StopRecord(sid);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RecvReleaseCall
��������:
��������: ��
���ض���: �ɹ�����TRUE,ʰܷ���FALSE
����ʱ��: 2008-1-31 16:02
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvReleaseCall(void *infor)
{
	
	DWORD sid;
	stPXSSM_ReleaseCall *pReleaseCall=(stPXSSM_ReleaseCall *)infor;
	if(pReleaseCall==NULL)
		return FALSE;
	
	sid = pReleaseCall->sid;
	if(g_Sid[sid].sid != sid)
	{
		return FALSE;
	}
	DoKillTimer(evTimer1,sid);
	//	YF_TRACE sid,"XSSM_RecvReleaseCall","DoKillTimer");
	//	memcpy(&sid,data,sizeof(DWORD));
	XSSM_RecvDisConnectForwardConnection(&sid);
	//	Sleep(1000);
	Hdl_ReleaseCall(sid,0,TRUE);
	Update_Message((WORD)sid,NULL,TRUE);
	ReleaseSenssionID((WORD)sid);
	memset(&g_Sid[sid],0,sizeof(XSSM_SID));
	g_Sid[sid].sid = MESSAGE_MAX;
	YF_TRACE sid,"Hdl_DisconnectForwardResource","unlink device,sid=%d",sid);
	return TRUE;
}
/********************************************************************
�������ƣ�XSSM_RecvConect
��������: ���������Ϣ
��������: infor:��Ϣ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-31 16:25
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvConect(WORD sid,void *infor)
{
	stPXSSM_Connect *pConnect = (stPXSSM_Connect *)infor;
	stPXSSM_Connect m_Connect;
	ACS_Hdl_EVENT pHdl_event;
	DoKillTimer(evTimer1,sid);
//	YF_TRACE sid,"XSSM_RecvConect","DoKillTimer");
	memcpy(&m_Connect,pConnect,sizeof(stPXSSM_Connect));
	g_Sid[sid].sid = sid;
	g_Sid[sid].status = evPXSSM_Connect;
	g_Sid[sid].attrd.bCallOut = 1;
	g_Sid[sid].attrd.bneedNullCDR = pConnect->needNullCDR;
	strcpy(g_Sid[sid].CalledNumber,pConnect->routeNumber);
	strcpy(g_Sid[sid].CallingNumber,pConnect->callingNumber);
	g_Sid[sid].pCollectInfor.firstDigitTimeOut = pConnect->timeOut;
	
	if(!Hdl_Connect(m_Connect))//�Ҳ������е������м̻��������豸
	{
		pHdl_event.sid=sid;
		pHdl_event.DeviceNumber=0xffffffff;
		strcpy(pHdl_event.CalledNum,pConnect->routeNumber);
		strcpy(pHdl_event.CallingNum,pConnect->callingNumber);
		pHdl_event.event=evtXSSM_CallOut;
		pHdl_event.datalen=1;
		pHdl_event.ioData[0]=3;
		XSSM_DoCallOut(&pHdl_event);

	}
	else
	{
		SetWaiteTimer(sid);
	}
	return FALSE;
}
/********************************************************************
�������ƣ�XSSM_RecvTTSConvert
��������: ����TTS�ļ�ת��
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-18 18:03
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvTTSConvert(void *infor)
{
	stPXSSM_TTSConvert *pTTSConvert = (stPXSSM_TTSConvert *)infor;
	stPXSSM_TTSConvert m_TTSConvert;
	memcpy(&m_TTSConvert,pTTSConvert,sizeof(stPXSSM_TTSConvert));
	if(Hdl_TTSConvert(m_TTSConvert))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
/********************************************************************
�������ƣ�XSSM_RecvTTSPlay
��������: ���󲥷�TTS�ļ�
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-18 18:02
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvTTSPlay(void *infor)
{
	BOOL bFinished;
	stPXSSM_TTSPlayAnnouncement m_TTSPlay;
	stPXSSM_TTSPlayAnnouncement *pTTSPlay = (stPXSSM_TTSPlayAnnouncement *)infor;
	stPXSSM_ResourceReport resourceReport;
	memcpy(&m_TTSPlay,pTTSPlay,sizeof(stPXSSM_TTSPlayAnnouncement));
	if(Hdl_TTSPlay(pTTSPlay->sid,m_TTSPlay))
	{
		bFinished = TRUE;
	}
	else
	{
		bFinished = FALSE;
	}
	resourceReport.sid = pTTSPlay->sid;
	resourceReport.reason = R_TTSPlay;
	XSSM_ResourceReport(&resourceReport);//��Դ����
	return bFinished;
}
/********************************************************************
�������ƣ�XSSM_RecvRequestReportBCSMEvent
��������: XVAPL��pXSSMע���¼�
��������: infor:��Ϣ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-18 17:27
��������: ������
ע������: ��	
*********************************************************************/
BOOL XSSM_RecvRequestReportBCSMEvent(void *infor)
{
	WORD sid;
	int i;
	stPXSSM_RequestReportBCSMEvent *pBcsmEvent = (stPXSSM_RequestReportBCSMEvent *)infor;
	sid = (WORD)pBcsmEvent->sid;
	if(sid>MESSAGE_MAX)
		return FALSE;
	g_Sid[sid].attrd.bcsm = TRUE;
	g_Sid[sid].eventNumber = pBcsmEvent->eventNumber;
	for(i=0;i<pBcsmEvent->eventNumber;i++)
	{
		g_Sid[sid].events[i] = pBcsmEvent->events[i];
	}
	return TRUE;
}
/********************************************************************
�������ƣ�FreeMerroy
��������: �ͷ��ڴ����Դ
��������: ��
���ض���: ��
����ʱ��: 2008-2-18 18:02
��������: ������
ע������: ��	
*********************************************************************/
void FreeMerroy()
{
	if(pServiceKey)
	{
		free(pServiceKey);
		pServiceKey = NULL;
	}
}
/********************************************************************
�������ƣ�Update_pXSSM_Message
��������: ���¼�¼����Ϣ����
��������: senssionID:�Ự�ţ�pMsg����Ϣ���ݣ�bDelete���Ƿ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-5-6 14:29
��������: ������
ע������: ��	
*********************************************************************/
//
//BOOL Update_Message(WORD senssionID,MESSAGE *pMsg,BOOL bDelete)
//{
//	if(senssionID>=MESSAGE_MAX)
//		return  FALSE;
//	if(bDelete)
//	{
//		memset(&g_pXSSM_Message[senssionID],0,sizeof(MESSAGE));
//	}
//	else
//	{
//		memcpy(&g_pXSSM_Message[senssionID],pMsg,sizeof(MESSAGE));
//	}
//	return TRUE;
//}
/********************************************************************
�������ƣ�Read_pXSSM_Message
��������: ��ȡ���̴�ȡ��Ϣ
��������: senssionID:�Ự�ţ�pMsg����ȡ��Ϣ�Ľṹ
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-5-6 14:41
��������: ������
ע������: ��	
*********************************************************************/
//BOOL Read_Message(WORD senssionID,MESSAGE *pMsg)
//{
//	if(senssionID>=MESSAGE_MAX)
//		return  FALSE;
//	memcpy( pMsg,&g_pXSSM_Message[senssionID],sizeof(MESSAGE));
//	return TRUE;
//}
void SetRegisterHotKey(HWND hwnd)
{
	UnregisterHotKey(hwnd,ALT_F9);
	UnregisterHotKey(hwnd,ALT_F10);
}
void FreeHotKey(HWND hwnd)
{
	UnregisterHotKey(hwnd,ALT_F9);
}
/********************************************************************
�������ƣ�monitor
��������: ���ӳ���
��������: event���¼���,lParam:���ھ��
���ض���: ��
����ʱ��: 2008-4-13 15:57
��������: ������
ע������: ��	
*********************************************************************/
void Monitor(int event, LPARAM lParam)
{
	HWND   hMonitorWnd = NULL;
	if(event == MSG_WATCHDOG_CHECK)//MONITOR��������
	{
		hMonitorWnd = (HWND) lParam;
		if (hMonitorWnd) 
		{
			PostMessage(hMonitorWnd, MSG_WATCHDOG_ACK, 0, (LPARAM)pwnd);
		}
	}
	if(event == MSG_SPP_ACK)
	{
		if(!bFindSpp)
		{
			YF_LOG_SMM "have connected with spp.exe ");
			bFindSpp = TRUE;
			StartAllWork();
		}
		if (gProgramTime>= MAX_SEND_CHECK_COUNT)
		{
			YF_LOG_SMM "again connect with spp.exe");
		}
		gProgramTime = 0;
		KillTimer(pwnd, 0);
		SetTimer(pwnd, 0, TIMER_LEN, NULL);
	}
	if(event == WM_TIMER)
	{
		if (gProgramTime>= MAX_SEND_CHECK_COUNT)   
		{
			if(bFindSpp)
			{
				Hdl_Exit();
				bFindSpp = FALSE;
				YF_LOG_SMM "disconnect with keygoe sys!");
			}
			YF_LOG_SMM "disconnect with spp.exe");
			hSppWnd = NULL;
		}
       if (hSppWnd)     // δ�ﵽ����������������
			PostMessage(hSppWnd, MSG_SPP, 0,(LPARAM)pwnd);
	   else
	   {
		   hSppWnd = FindWindow(SPP_NAME,SPP_NAME);
		   if(hSppWnd != NULL)
		   {
				PostMessage(hSppWnd, MSG_SPP, 0,(LPARAM)pwnd);
		   }
	   }
	   gProgramTime++;
	}

}
void InitTimer()
{
	gProgramTime = 0;
	hSppWnd = FindWindow(SPP_NAME,SPP_NAME);
	if( !hSppWnd ) 
	{
    	YF_LOG_SMM "Can not find %s.exe",SPP_NAME);
	}
	else
	{
		PostMessage(hSppWnd, MSG_SPP, 0, (LPARAM)pwnd);
	}
	SetTimer(pwnd,0,TIMER_LEN,NULL);		
}
void GetSppStatus()
{
	hSppWnd = FindWindow(SPP_NAME,SPP_NAME);
	if( hSppWnd != NULL ) 
	{
		bFindSpp = TRUE;
	}
	else
	{
		bFindSpp = FALSE;
	}

}
