#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h> 
#include <fcntl.h>
#include <shellapi.h>
#include <sys\stat.h> 
#include <Tlhelp32.h>
#include "TimerDll.h"
#include "comm.h"
#include "commdll.h"
#include "init_viriable.h"
#include "event.h"
#include "NodeComm.h"
#include "xvapl_monitor.h"
#include "xvapl_slp.h"
#include "log.h"
#include "cdr.h"
#include "DBAP.h"


//#include "../public/DBD.h"
#include "ReadConfig.h"
#include "WatchDefine.h"
#include "Xvapl_TraceThread.h"
#include "yftrace.h"
#include "pMsg.h"

#define WM_OPENWND WM_USER+2220
#define FILENAME "SPP.ini"
#define MONITOR_NAME "YFWATCHDOG"
#define FILE_NAME "yfdog.exe"
HANDLE hPublicMapping;
HANDLE hMessageMapping;
HANDLE hNodeMapping;
HANDLE hModuleMapping;
HANDLE hReBufferMapping;
HANDLE hSenssionMapping;

HANDLE hSidMapping;
HANDLE hOffsetMapping;
HWND    hMonitorWnd=NULL;//�����Ӷ˵Ĵ��ھ��MONITOR
int		gProgramTime=0;//��������û���ϴ���
#define TIMER_LEN  5000             // ������Ϣ��ⷢ��ʱ����
#define MAX_SEND_CHECK_COUNT 3      // �����ӵĳ���Ӧ��������Ϣ�ﵽ��ֵ���������б����ӳ���
MESSAGE g_Slave_Message[MESSAGE_MAX];


LRESULT APIENTRY WndProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL InitialAll();
BOOL Init_Merry();
BOOL DetachMerry();
BOOL ReadSppIni();
BOOL StartAllThread();
BOOL NetCommInit();
void SetRegisterHotKey(HWND hwnd);
void FreeHotKey(HWND hwnd);
int GetKeyValue(SECTION *sec,int bKey);
void Monitor(int event, LPARAM lParam);
void StartTraceThread();
BOOL StartMonitor();
void ReStartProgram();
void Spp_Deal_Message(WPARAM wParam,LPARAM lParam);
void Spp_Send_NodeCom(WPARAM wParam,LPARAM lParam);
void WriteLog();

#define SYSTRAY_ICON_BASE 2222

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					
					PSTR szCmdLine, int iCmdShow)
					
{
	
	static TCHAR szAppName[] = TEXT (MAIN_WINDOW_TITLE) ;
	HWND                    hwnd ;
	MSG                     msg ;	
	WNDCLASS                wndclass ;
	BOOL bDll=FALSE;
	HWND hwnd1 = FindWindow(szAppName,szAppName);//������ڣ���������
// 	g_Trace=EnableTrace(TRUE);//����Ҫ��־��¼�ļ�

	if(hwnd1!=NULL)
	{
		YF_LOG_SPP "SPP�Ѿ���������ͼ��������ʧ��");
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
		return 0; 
	} 
	InitialAll();
	if(!Init_Merry())
	{
		return 0;

	}
	ReadSppIni();	
	hwnd = CreateWindow ( szAppName, TEXT (MAIN_WINDOW_TITLE),
        
		WS_OVERLAPPEDWINDOW,
        
		CW_USEDEFAULT, CW_USEDEFAULT,
        
		CW_USEDEFAULT, CW_USEDEFAULT,
        
		NULL, NULL, hInstance, NULL) ;
	
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;
	g_hWnd = hwnd;
	g_Public->g_hWnd =hwnd;
	if(!RegisterIPC(MID_COM,hwnd))
	{
		YF_LOG_SPP "E00000002:ģ��ע��ʧ��");
	}
//	if(g_Node[0].isSPP)//SPP server is 1,then start all thread
	{
		INIT_TIMER();
		StartAllThread();
	}
//	INIT_TIMER();
// 	StartAllThread();
	NetCommInit();//start ne,t thread
	YF_LOG_SPP "SPP started");
	WriteLog();
	while (GetMessage (&msg, NULL, 0, 0))   
	{    
		TranslateMessage (&msg);      
		DispatchMessage (&msg);   
	}
	return msg.wParam ;	
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
//  tnid.dwState=NIS_SHAREDICON;
//  tnid.uVersion=NOTIFYICON_VERSION;
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
	MESSAGE pMsg;

	switch (message)
	{
		
	case evIPCMessage:

		if(COM_Read_Message((WORD)wParam,&pMsg))
		{
			SEND_MSG(evThreadMessage,pMsg.head.receiver.mId,(void *)&pMsg,sizeof(MESSAGE),(WORD)lParam,FALSE,NULL,0);
		}
		break;

	case evSocketMessage:

		Spp_Send_NodeCom(wParam,lParam);
		break;

	case evThreadMessage:
	
		Spp_Deal_Message(wParam,lParam);

		break;

    case WM_CREATE:
		break;
	case WM_TIMER :
		{
			Monitor(WM_TIMER,lParam);
		}
		break;
	case   WM_SIZE:
		if (wParam==SIZE_MINIMIZED)
		{
			systray_add(hwnd,1,LoadIcon(NULL,IDI_APPLICATION),"SPPͨѶģ��V1.0.0");
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
	case MSG_TRACE_ON:
		{
			
				g_Trace = EnableTrace(TRUE);
//				SetTraceValue(g_Trace);
				YF_TRACE_SPP -1,"","SPP��������");			
		}
		break;
	case MSG_TRACE_OFF:
		{
			YF_TRACE_SPP -1,"","SPPֹͣ����");
			g_Trace = EnableTrace(FALSE);
//			SetTraceValue(g_Trace);
		}
		break;

	case MSG_WATCHDOG_CHECK://�������Լ��ӳ�������֣�������
		{
				WriteLog();
	
			Monitor(MSG_WATCHDOG_CHECK,lParam);
		}
		break;
	case MSG_SPP://��������PXSSM������
		{
			Monitor(MSG_SPP,lParam);
		}
		break;
	case  WM_DESTROY:
		{
			DetachMerry();
			FreeHotKey(hwnd);
			YF_LOG_SPP "SPP�������˳�");
			EnableTrace(FALSE);//�ر���Ҫ��־��¼�ļ����
			PostQuitMessage (0) ;
			return 0 ;
		}
		break;
	default:
		break;
        
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
	
}
void Spp_Deal_Message(WPARAM wParam,LPARAM lParam)
{
	BOOL bFind = FALSE;
	MESSAGE pMsg;
	
	if(lParam>=MESSAGE_MAX)
		return;
	bFind = COM_Read_Message((WORD)wParam,&pMsg);
	if(!bFind)
		return;
		
	switch(pMsg.head.event)
	{
	case evInitial://��ʼ��
		bInital = TRUE;
		break;
	case evTraceStatus://���ٿ���
		{
			EnableTrace_Spp(pMsg.data[0]);
			Comm_SetTraceValue(g_Trace_Spp);
		}
		break;
	
	default:
		break;
	}
	return;
}
void Spp_Send_NodeCom(WPARAM wParam,LPARAM lParam)
{
	TID tId;
//	if(lParam>=MESSAGE_MAX)
//		return;
	if(GetTid(MODULE_MAX,&tId))
	{
		 PostThreadMessage(tId.module,evSocketMessage,wParam,lParam);
	}
	return;
}
/********************************************************************
�������ƣ�GetKeyValue
��������: ��ȡINI�ļ�ĳһ��
��������: sec���ڵ㣬bKey����
���ض���: ��
����ʱ��: 2008-4-13 11:58
��������: ������
ע������: ��	
*********************************************************************/

int GetKeyValue(SECTION *sec,int bKey)
{
	char *getvalue=NULL;
	getvalue=GetKeyList(sec,(BYTE)bKey);
	if(getvalue == NULL)
	{
		return 0;
	}
	else
	{
		return atoi(getvalue);
	}
}
void WriteLog()
{
	SYSTEMTIME     Clock;
	FILE	    *fp=NULL;
	fp = fopen ("C:\\yfcomm\\log\\test.log","w");

	if ( fp == NULL )
		return ;

	GetLocalTime(&Clock);
	fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d have message\n",
			Clock.wYear,Clock.wMonth, Clock.wDay,
		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds);
	fclose (fp);
}
/********************************************************************
�������ƣ�InitialAll
��������: ��ʼ����������
��������: 
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-11-5 22:12
��������: ������
ע������: ��	
*********************************************************************/

BOOL InitialAll()
{
	int i;
	bRun = TRUE;
	bWritConfig = FALSE;
	bLoadService = FALSE;
	g_Public = NULL;
	g_hWnd = 0;
	g_Message = NULL;
	g_Module = NULL;
	g_Node = NULL;
	g_dwNode = 0;
	for(i=0;i<MESSAGE_MAX;i++)
	{
		memset(&g_Slave_Message[i],0,sizeof(MESSAGE));
	}
	return TRUE;
}
/********************************************************************
�������ƣ�ReadSppIni
��������: �������ļ�
��������: ��
���ض���: succed:true,else false
����ʱ��: 2007-12-7 18:09
��������: ������
ע������: ��	
*********************************************************************/

BOOL ReadSppIni()
{
	
	SECTION *sec;
	INI ini;
	BYTE i,j;
	char pChar[32];
//	SPP_CDR cdr;
	BYTE bModule;
	sprintf(pChar,"C:\\yfcomm\\ini\\%s",FILENAME);
	loadINI(&ini,pChar);
	/*�õ�[COMM]*/
	sec=getSection(&ini,INI_SPP_COMM);
	if(sec==NULL)
	{
		YF_LOG_SPP "E00000000:Read %s fail",FILENAME);
		return FALSE;
	}
	{
		s_basic.bNodeNum=GetKeyValue(sec,0);
		s_basic.wAttri.att.isCdr=(WORD)GetKeyValue(sec,1);
		s_basic.wAttri.att.isCdrToSql=(WORD)GetKeyValue(sec,2);
	}
	/*�õ�[Node]*/
	for(i=0;i<s_basic.bNodeNum;i++)
	{
		sprintf(tmpSection,"%s%d",INI_SPP_NODE,i);
		sec=getSection(&ini,tmpSection);
		if(sec==NULL)
		{
			YF_WARN "E00000000:Read %s fail",FILENAME);

			return FALSE;
		}
		{	
			g_Node[i].node=inet_addr(GetKeyList(sec,0));
			if(i==0)
			{
				g_dwNode = g_Node[i].node;
				g_Public->g_dwNode = g_Node[i].node;
			}
			g_Node[i].isSPP =(BOOL)GetKeyValue(sec,1);
			g_Node[i].isServer = (BOOL)GetKeyValue(sec,2);
			g_Node[i].bModuleNum = (BYTE)GetKeyValue(sec,3);
			for(j=0;j<g_Node[i].bModuleNum;j++)
			{
				bModule = (BYTE)GetKeyValue(sec,4+j);
				if(SYSMODULE<=bModule<MODULE_MAX)//ȷ���Ƿ�ϵͳģ��ҲҪд������
				{
					g_Module[bModule].mId = bModule;
					g_Module[bModule].isSysModule = FALSE;
				}
			}
			
		}
	}
	/*�õ�[DATABASE]*/
	sec=getSection(&ini,INI_SPP_SQL);
	if(sec==NULL)
	{
		YF_WARN "W00000000:Read %s fail",FILENAME);

		return FALSE;
	}
	s_database.ComputerIp=inet_addr(GetKeyList(sec,0));
	strcpy(s_database.pUserName,GetKeyList(sec,1));
	strcpy(s_database.pPassWd,GetKeyList(sec,2));
	/*�õ�[IPP]*/
	sec=getSection(&ini,INI_SPP_IPP);
	if(sec==NULL)
	{
		YF_WARN "E00000000:Read %s fail",FILENAME);

		return FALSE;
	}
	s_basic.bIppNum=(BYTE)GetKeyValue(sec,0);
	g_Ipp = (SPP_IPP *)malloc(sizeof(SPP_IPP) * s_basic.bIppNum);
	/*�õ�[Ipp]*/
	for(i=0;i<s_basic.bIppNum;i++)
	{
		sprintf(tmpSection,"%s%d",INI_SPP_IPPX,i);
		sec=getSection(&ini,tmpSection);
		if(sec==NULL)
		{
		YF_WARN "E00000000:Read %s fail",FILENAME);

			return FALSE;
		}
		g_Ipp[i].id = GetKeyValue(sec,0);
		strcpy(g_Ipp[i].pName,GetKeyList(sec,1));						
		g_Ipp[i].isServer = (BOOL)GetKeyValue(sec,2);
		g_Ipp[i].dIp=inet_addr(GetKeyList(sec,3));
		g_Ipp[i].wPort = (WORD)GetKeyValue(sec,4);
	}
	/*�õ�[Version]*/
	sec=getSection(&ini,INI_SPP_VERSION);
	if(sec==NULL)
	{

		YF_WARN "E00000000:Read %s fail",FILENAME);
		return FALSE;
	}
	strcpy(s_basic.version,GetKeyList(sec,0));
	/*end*/
	freeINI(&ini);
	return TRUE;
	
}
/********************************************************************
�������ƣ�StartAllThread
��������: �������������߳�
��������: ��
���ض���: �ɹ���TRUE��ʧ�ܣ�FALSE
����ʱ��: 2007-11-21 11:14
��������: ������
ע������: ��	
*********************************************************************/

BOOL StartAllThread()
{
	//start slp thread
//	StartTraceThread();
	StartMonitor();//���������߳�

	if(g_Node[0].isSPP)//SPP server is 1,then start all thread
	{
		_beginthreadex(NULL,0,DBAPThreadProc,NULL,0,NULL);
		_beginthread(XSCM_ThreadProc,0,NULL);
		_beginthread(logProc,0,NULL);
		_beginthreadex(NULL,0,Cdr_proc,NULL,0,NULL);
	}
	
	return TRUE;
}
/********************************************************************
�������ƣ�NetCommInit
��������: ��������ͨѶ�߳�
��������: ��
���ض���: ��
����ʱ��: 2007-12-7 11:16
��������: ������
ע������: ��	
*********************************************************************/
BOOL NetCommInit()
{
	if(s_basic.bNodeNum>1)//����ڵ�>1
	{
		_beginthread(OnInitNetWork,0,NULL);
		return TRUE;
	}
	return FALSE;

}
/********************************************************************
�������ƣ�StartTraceThread
��������: ����������־�߳�
��������: ��
���ض���: wu
����ʱ��: 2009-3-2 14:15
��������: ������
ע������: ��	
*********************************************************************/
void StartTraceThread()
{
	_beginthread(Trace_ThreadProc,0,NULL);
}
/********************************************************************
�������ƣ�StartMonitor
��������: �������ӳ���
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-12-29 9:58
��������: ������
ע������: ��	
*********************************************************************/

BOOL StartMonitor()
{
	_beginthread(xvapl_monitor_run,0,NULL);
	return TRUE;
}
/********************************************************************
�������ƣ�Init_Merry
��������: ��ʼ�������ڴ�
��������: ��
���ض���: �ɹ���TRUE��ʧ�ܣ�FALSE
����ʱ��: 2007-11-15 15:32
��������: ������
ע������: ��	
*********************************************************************/
BOOL Init_Merry()
{
	DWORD dw=MESSAGE_MAX*sizeof(MESSAGE);
	int i;
	//////////////////////////////////////////////////////////////////////////
	/*��¼���ںͽڵ���Ϣ*/
	//////////////////////////////////////////////////////////////////////////
	hPublicMapping = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1*sizeof(PUBLIC),SPP_PUBLIC);
	if(hPublicMapping==NULL)
	{
		YF_LOG_SPP "E00000001:Init_Merryʧ��");
		return FALSE;
	}
	g_Public=(PUBLIC *)MapViewOfFile(hPublicMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	g_Public->g_hWnd = NULL;
	g_Public->g_dwNode =0;
	//////////////////////////////////////////////////////////////////////////
	//��Ϣ�ڴ�����
	//////////////////////////////////////////////////////////////////////////
	
	hMessageMapping=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,MESSAGE_MAX*sizeof(MESSAGE),SPP_MESSAGE);
	if(hMessageMapping==NULL)
	{
		YF_LOG_SPP "E00000001:Init_Merryʧ��");

		return FALSE;
	}
	g_Message=(MESSAGE *)MapViewOfFile(hMessageMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	for(i=0;i<MESSAGE_MAX;i++)
	{
		memset(g_Message+i,0,sizeof(MESSAGE));
	}

	
	//////////////////////////////////////////////////////////////////////////
	//�ڵ��ڴ�����
	//////////////////////////////////////////////////////////////////////////
	hNodeMapping=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,NODE_MAX*sizeof(NID),SPP_NODE);
	if(hNodeMapping==NULL)
	{
		YF_LOG_SPP "E00000001:Init_Merryʧ��");

		return FALSE;
	}
	g_Node=(NID *)MapViewOfFile(hNodeMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	for(i=0;i<NODE_MAX;i++)
	{
		memset(g_Node+i,0,sizeof(NID));
	}
	//////////////////////////////////////////////////////////////////////////
	//ģ���ڴ�����
	//////////////////////////////////////////////////////////////////////////
	hModuleMapping=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,(MODULE_MAX+1)*sizeof(MID),SPP_MODULE);
	if(hModuleMapping==NULL)
	{
		YF_LOG_SPP "E00000001:Init_Merryʧ��");

		return FALSE;
	}
	g_Module=(MID *)MapViewOfFile(hModuleMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	for(i=0;i<MODULE_MAX+1;i++)
	{
		g_Module[i].mId=i;
		memset(g_Module+i,0,sizeof(MID));
	}
	
	//////////////////////////////////////////////////////////////////////////
	//��¼��Ϣ�հ��ڴ�����
	//////////////////////////////////////////////////////////////////////////


	hOffsetMapping = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1*sizeof(SID_POSITION),SPP_OFFSET);
	if(hOffsetMapping==NULL)
	{
		YF_LOG_SPP "E00000001:Init_Merryʧ��");
		return FALSE;
	}
	g_Offset_Position=(SID_POSITION *)MapViewOfFile(hOffsetMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	g_Offset_Position->offset = 0;
	for(i=0;i<MESSAGE_MAX;i++)
	{
		g_Offset_Position->bSign[i] = 0;
	}

	hSidMapping = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1*sizeof(SID_POSITION),SPP_SID);
	if(hSidMapping==NULL)
	{
		return FALSE;
	}
	g_Sid_Position=(SID_POSITION *)MapViewOfFile(hSidMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	g_Sid_Position->offset = 0;
	for(i=0;i<MESSAGE_MAX;i++)
	{
		g_Sid_Position->bSign[i] = 0;
	}
	return TRUE;
}

/********************************************************************
�������ƣ�DetachMerry
��������: �ͷŹ����ڴ�������ڴ�
��������: ��
���ض���: ��
����ʱ��: 2007-11-15 15:33
��������: ������
ע������: ��	
*********************************************************************/
BOOL DetachMerry()
{
	
	UnmapViewOfFile(g_Message);
	UnmapViewOfFile(g_Node);
	UnmapViewOfFile(g_Module);
	UnmapViewOfFile(g_Sid_Position);
	UnmapViewOfFile(g_Offset_Position);
	UnmapViewOfFile(g_Public);

	g_Public = NULL;
	g_Message = NULL;
	g_Node = NULL;
	g_Module = NULL;
	g_Sid_Position = NULL;
	g_Offset_Position = NULL;
	
	CloseHandle(hPublicMapping);
	CloseHandle(hMessageMapping);
	CloseHandle(hNodeMapping);
	CloseHandle(hModuleMapping);
	
	CloseHandle(hSidMapping);
	CloseHandle(hOffsetMapping);
	
	return TRUE;
}
/********************************************************************
�������ƣ�SetRegisterHotKey
��������: ע���ȼ�
��������: ��
���ض���: ��
����ʱ��: 2008-4-13 15:53
��������: ������
ע������: ��	
*********************************************************************/

void SetRegisterHotKey(HWND hwnd)
{
	DWORD dwPID;
	HWND hWND2;
	char bb[256];
	UnregisterHotKey(hwnd,ALT_F7);
	UnregisterHotKey(hwnd,ALT_F8);
	GetWindowThreadProcessId(hwnd,&dwPID);
	hWND2 = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID);
	if(RegisterHotKey(hwnd,ALT_F7,MOD_ALT,VK_F7))
	{
		YF_LOG_SPP "SetRegisterHotKey,Set ALT+F7 success");
	}
	else
	{
		sprintf(bb,"SetRegisterHotKey,Set ALT+F7 fail,%d",GetLastError());
		YF_LOG_SPP bb);
	}
}
/********************************************************************
�������ƣ�FreeHotKey
��������: �ȼ�ע��
��������: ��
���ض���: ��
����ʱ��: 2008-4-13 15:53
��������: ������
ע������: ��	
*********************************************************************/
void FreeHotKey(HWND hwnd)
{
	UnregisterHotKey(hwnd,ALT_F7);
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
			PostMessage(hMonitorWnd, MSG_WATCHDOG_ACK, 0, (LPARAM)g_hWnd);
		}
	}

	if(event == MSG_SPP)
	{
		hMonitorWnd = (HWND) lParam;
		if (hMonitorWnd) 
		{
			PostMessage(hMonitorWnd, MSG_SPP_ACK, 0, (LPARAM)g_hWnd);
		}
	}
}


