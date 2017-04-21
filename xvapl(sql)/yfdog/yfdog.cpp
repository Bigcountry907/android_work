/**********************************************************************
*                                                                     *
* �ļ����ƣ� yfdog.cpp                                                *
*                                                                     *
* ����˵���� ����ϵͳ�ļ�������������״̬����������ͨ�������ļ�����   *
*            �����ļ���   yfdog.ini                                   *
*                                                                     *
* ���л�����windows                                                   *
**********************************************************************/
#include "stdafx.h"
#include <windows.h>
#include <winuser.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <winsock.h>
#include <process.h>
#include "yfdog.h"
#pragma   comment(lib,"wsock32")

#define MAX_WATCH_PROGRAM    7      // �����Լ��ӵĳ������ 

#define MAX_RESTART_COUNT    20     // ���𱻼��ӵĳ���ﵽһ����������������

#define MAX_SEND_CHECK_COUNT 5      // �����ӵĳ���Ӧ��������Ϣ�ﵽ��ֵ���������б����ӳ���

#define TIMER_LEN  5000             // ������Ϣ��ⷢ��ʱ����


#define IP_MAX 20  //IP�ֽ�
#define YFDOG_LOG  "c:\\tms\\log\\yfdog.log"

#define YF_DOGLOG  WriteFileLog(YFDOG_LOG, __FILE__, __LINE__,
#define YF_ERROR   WriteErrorInf(YFDOG_LOG, __FILE__, __LINE__,

int    gProgramNum = 0;                   // �����ӵĳ�������
int    gCurProgram = 0;                   // ��ǰ���ֵĳ�����,0��gProgramNum-1
int    gProgramTime[MAX_WATCH_PROGRAM];   // ����û��Ӧ��������Ϣ����
char   gName[MAX_WATCH_PROGRAM][256];     // Ӧ�ó���
char   gTitle[MAX_WATCH_PROGRAM][256];    // ��������������
char   gProgram[MAX_WATCH_PROGRAM][256];  // ���������ڱ���
char   g_IP[IP_MAX];   //����ͨѶ��IP��ַ


HWND   gHWND[MAX_WATCH_PROGRAM];          // ���������ھ��

HWND   hWnd  = NULL;           // ���ӳ��������ھ��

void ReStartProgram();
void ReceiveFromTrace(int fd);
void TcpCustom_Run(void *arg);
//////////////////////////////////////////////////////////////////////////   
  //************************************************************************   
  //ģ�����֣�AdjustProcessPrivileges(LPSTR)   
  //ģ�鹦�ܣ��޸ĵ��ý��̵�Ȩ��   
  //������ֵ���ɹ�����TRUE��ʧ�ܷ���FALSE   
  //ע�������   
  //************************************************************************   
  //����˵����������                   |       ����/���     |                 ����˵��   
  //               szPrivilegesName     |         IN                 |      Ҫ������Ȩ����   
  //************************************************************************   
  //��������Ϊ����ֵ��     
  //   #define   SE_BACKUP_NAME                 TEXT("SeBackupPrivilege")   
  //   #define   SE_RESTORE_NAME               TEXT("SeRestorePrivilege")   
  //   #define   SE_SHUTDOWN_NAME             TEXT("SeShutdownPrivilege")   
  //   #define   SE_DEBUG_NAME                   TEXT("SeDebugPrivilege")   
  //////////////////////////////////////////////////////////////////////////   
    
  BOOL   AdjustProcessPrivileges(LPCSTR   szPrivilegesName)   
  {   
	  HANDLE   hToken;     
	  TOKEN_PRIVILEGES   tkp;   
	    
	  if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))   
		  return   FALSE;   
	    
	  if(!LookupPrivilegeValue(NULL,szPrivilegesName,&tkp.Privileges[0].Luid))   
	  {   
		  CloseHandle(hToken);   
		  return   FALSE;   
	  }   
	    
	  tkp.PrivilegeCount   =   1;   
	  tkp.Privileges[0].Attributes   =   SE_PRIVILEGE_ENABLED;   
	    
	  if(!AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),NULL,NULL))   
	  {   
		  CloseHandle(hToken);   
		  return   0;   
	  }   
	    
	  CloseHandle(hToken);   
	  return   TRUE;   
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
	va_list	args;	
	SYSTEMTIME     Clock;
	FILE	    *fp=NULL;
	char	 	fileName[255],fileName1[255];
	struct stat sbuf;
	char	    buf[1024];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

 	memset(buf,0,sizeof(buf));

	va_start(args,fmt);
	_vsnprintf(buf,sizeof(buf)-1,fmt,args);
	va_end(args);

	if (!logname) 
		return FALSE;
	else 
		strcpy(fileName, logname);

	if (stat(fileName,&sbuf) == 0)
	{
		if  (sbuf.st_size>= MAXFILELEN)	
		{
			sprintf(fileName1,"%s.old", fileName);
			remove(fileName1);
			rename(fileName,fileName1);
		}
	}
	fp = fopen (fileName,"a");

	if ( fp == NULL )
		return FALSE;

	GetLocalTime(&Clock);

    _splitpath( cfile, drive,dir, fname, ext ); 

	fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d F[%8s%4s] L[%4d] %s\n",
			Clock.wYear,Clock.wMonth, Clock.wDay,
		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,
			fname,ext,line, buf );
	fclose (fp);
	return TRUE;
}

void  WriteErrorInf(const char * logname, const char *cfile, const int line,const char * fun)
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

    WriteFileLog(logname, cfile, line,"%s Failed with error %d: %s",fun,dw, lpMsgBuf);
    LocalFree(lpMsgBuf);
}


DWORD GetProcessIdFromName(LPCTSTR name)
{

	 PROCESSENTRY32 pe;
	 DWORD id = 0;

	 HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	 pe.dwSize = sizeof(PROCESSENTRY32);
	 if( !Process32First(hSnapshot,&pe) )
	 {
	     YF_ERROR "GetPID");
		 return 0;
	 }
     do
	 {
		  pe.dwSize = sizeof(PROCESSENTRY32);
		  if(stricmp(pe.szExeFile,name) == 0)
		  {
			   id = pe.th32ProcessID;
			   break;
		  }

	 } while(Process32Next(hSnapshot,&pe));

	 CloseHandle(hSnapshot);

	 return id;
}

void KillAllProgram()
{
	 PROCESSENTRY32 pe;
	 if (!AdjustProcessPrivileges(SE_DEBUG_NAME ))
		YF_ERROR "AdjustProcessPrivileges");
	 HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	 pe.dwSize = sizeof(PROCESSENTRY32);
	 if( !Process32First(hSnapshot,&pe) )
	 {
		YF_ERROR "GetPID1");
	    return ;
	 }
	 do
	 {
		  pe.dwSize = sizeof(PROCESSENTRY32);
		  for (int j=0;j<gProgramNum;j++)
		  {
			  if(stricmp(pe.szExeFile,gProgram[j]) == 0)
			  {
				  HANDLE hdl=OpenProcess(PROCESS_TERMINATE ,FALSE,pe.th32ProcessID) ;
				  if (hdl)
				  {
					if (TerminateProcess(hdl,0))
						YF_DOGLOG "Kill program :%s",gProgram[j]);
					else
					{
						YF_ERROR "TerminateProcess");
						YF_DOGLOG "Cannot kill program :%s",gProgram[j]);
					}

				  }
				  else
				  {
					YF_ERROR "OpenProcess");
					YF_DOGLOG "Cannot get handle :%s",gProgram[j]);
				  }
			  }
		  }

	 } while(Process32Next(hSnapshot,&pe));

	 CloseHandle(hSnapshot);
}

/*******************************************************************************************
*                                                                                          *
*  ��ʼ�������Ͷ�ȡ�����ӵĳ���                                                            *
*  �����ļ�  yfdog.ini                                                                     *
*  [general]                                                                               *
*  ProgramCount=2       �����ӵĳ���������С�ڵ���5                                        *
*                                                                                          *
*  program0=spp.exe     ��һ�������ӳ����������ͬһ��Ŀ¼����ָ����ϸ·��               *
*  name0=SPP Name       ����������                                                         *
*  title0=SPP Title     �����ڱ�����                                                       *
*                                                                                          *
*  program1=pxSSM.exe                                                                      *
*  name1=pxSSM Name                                                                        *
*  title1=pxSSM Title                                                                      *
*                                                                                          *
********************************************************************************************/

void   ReadConfig()
{
	char str[20],dir[256];
	int i;
	memset(gName,0,256*MAX_WATCH_PROGRAM);
	memset(gTitle,0,256*MAX_WATCH_PROGRAM);
	memset(gProgram,0,256*MAX_WATCH_PROGRAM);
	for (i=0;i<	MAX_WATCH_PROGRAM;i++)  
	{
	   gProgramTime[i]=0; 
	   gHWND[i] = NULL;
	}
//	GetCurrentDirectory(200,(LPTSTR)dir);
	strcpy(dir,"C:\\tms\\ini\\yfdog.ini");
    gProgramNum = GetPrivateProfileInt("general", "ProgramCount", 0, dir);
    if (gProgramNum>=MAX_WATCH_PROGRAM)  gProgramNum = MAX_WATCH_PROGRAM;
    for (i=0;i< gProgramNum ;i++)
	{
 		sprintf(str, "program%d", i);
	    GetPrivateProfileString("general", str, "",gProgram[i],256, dir);
 		sprintf(str, "name%d", i);
	    GetPrivateProfileString("general", str, "",gName[i],256, dir);
 		sprintf(str, "title%d", i);
	    GetPrivateProfileString("general", str, "",gTitle[i],256, dir);
	}  
  	GetPrivateProfileString("general", "ip", "",g_IP,IP_MAX, dir);
    		
}



/*******************************************************************************************
*                                                                                          *
*  �����澯                                                                                *
*  ��� �� beep�Ĵ���                                                                      *
*                                                                                          *
********************************************************************************************/
void SoundAlarm(int count)
{
	int i;
	for(i=0;i<count;i++) {
		MessageBeep(0xFFFFFFFF);
		Sleep(200);
	}
}

/*******************************************************************************************
*                                                                                          *
*  ��������                                                                              *
*                                                                                          *
********************************************************************************************/

void Reset()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp;   
    
	if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
	{
		YF_ERROR "OpenProcessToken");   
	}
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);  
	tkp.PrivilegeCount = 1;     
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
	AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if( !ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0) )
	{
		YF_ERROR "ExitWindowsEx");
	}
	ExitProcess(0);
}

/*******************************************************************************************
*                                                                                          *
*  �������б����ӵ�Ӧ��                                                                    *
*  ÿ�ν�kill���еı����ӽ��̣�Ȼ���������еı�����Ӧ��                                    *
*                                                                                          *
********************************************************************************************/

void ReStartProgram()
{
	static unsigned int g_RestartCount=0;
	DWORD dwPID;
	HWND  hWND;
	HANDLE hWND2;
	int   i;
	YF_DOGLOG "Retart Programs %d count.",g_RestartCount);

	if(++g_RestartCount >= MAX_RESTART_COUNT)	{
		SoundAlarm(20);
		Reset();
	}
	for(i=0;i<gProgramNum;i++)
	{
		hWND = gHWND[i];
		if( hWND ) 
		{
			GetWindowThreadProcessId(hWND,&dwPID);
			hWND2 = OpenProcess(PROCESS_TERMINATE,false,dwPID);
			if( hWND2 )
			{
				TerminateProcess(hWND2,0); 
				CloseHandle(hWND2);
				gHWND[i]=NULL;
				gProgramTime[i] = 0;
			}
		}
	}

	for(i=0;i<gProgramNum;i++)
	{
		WinExec(gProgram[i],SW_HIDE);
	//		hWND = FindWindow(gName[i],gTitle[i]);
		hWND = FindWindow(NULL,gTitle[i]);
		if( !hWND ) 
		{
    		YF_DOGLOG "Can not start %s",gProgram[i]);
    		SoundAlarm(5);
		}
		else
		{
		     gHWND[i]=hWND;
		     gProgramTime[i]=0;
		}
		Sleep(2000);
	}
	FILE *fp=fopen("c:\\tms\\ini\\yfdog.dat","wb");
	if (fp)
	{
		fwrite(&gProgramNum,sizeof(gProgramNum),1,fp);
		fwrite(gHWND,sizeof(HWND)*gProgramNum,1,fp);
		fclose(fp);
	}
	gCurProgram = 0;
}

/*******************************************************************************************
*                                                                                          *
*  �����¼�����Ĵ���                                                                      *
*  ��Σ��¼���һ��Ϊ�յ�ACK������һ��Ϊ��ʱû���յ�ACK                                    *
*                                                                                          *
********************************************************************************************/

void monitor(int event)
{
	int i;
	if (event == MSG_WATCHDOG_ACK)      // �յ�Ӧ��,������������������һ��
	{
		gProgramTime[gCurProgram] = 0;
		KillTimer(hWnd, 0);
		gCurProgram++;
		if (gCurProgram >= gProgramNum) 
				gCurProgram = 0;
		SetTimer(hWnd, 0, TIMER_LEN, NULL);
	}

	if (event == WM_TIMER)          // ��ʱ,����������������������Ӽ��ӻ�����
	{
		if (gProgramTime[gCurProgram] >= MAX_SEND_CHECK_COUNT)   //  �ﵽ���������������еĳ���
		{
			YF_DOGLOG "%s timer out.",gProgram[gCurProgram]);
			ReStartProgram();
		}
       if (gHWND[gCurProgram])     // δ�ﵽ����������������
			PostMessage(gHWND[gCurProgram], MSG_WATCHDOG_CHECK, 0,(LPARAM)hWnd);
		gProgramTime[gCurProgram]++;
	}
	if(event == MSG_TRACE_ON)
	{
		for(i=0;i<gProgramNum;i++)
		{
			if (gHWND[i])     // send trace order
				PostMessage(gHWND[i], MSG_TRACE_ON, 0,(LPARAM)hWnd);
		}
	}
	if(event == MSG_TRACE_OFF)
	{
		for(i=0;i<gProgramNum;i++)
		{
			if (gHWND[i])     // send trace order
				PostMessage(gHWND[i], MSG_TRACE_OFF, 0,(LPARAM)hWnd);
		}
	}
}


int WINAPI  WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitApplication(HINSTANCE hInstance)
{
    WNDCLASS wc;
    DWORD   id;
	if (id=GetProcessIdFromName("yfdog.exe"))   // ֻ������һ�����ӳ���
    {
		if (GetCurrentProcessId()!=id)
		{
			YF_DOGLOG "An old version already exist,Quit it and retry.");
			return false;
		}
	}

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (struct HBRUSH__ *)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = WATCHDOG_WIN_NAME;
    
	return RegisterClass(&wc);
}

int UnhandledExceptionHappen = 0; 
LONG MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	UnhandledExceptionHappen = 1;
	SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
	return EXCEPTION_EXECUTE_HANDLER;
	ExceptionInfo;
}

bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hWnd = CreateWindow(WATCHDOG_WIN_NAME, WATCHDOG_TITLE_NAME, WS_MINIMIZE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
    
	if (!hWnd) 
		return (false);
	
	ShowWindow(hWnd, SW_HIDE);        //  ��̨����
	UpdateWindow(hWnd);
    
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST);
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)MyUnhandledExceptionFilter); 
	return (TRUE);
}

int WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)   {

	case MSG_WATCHDOG_ACK:           //  �յ�Ӧ��
		monitor(MSG_WATCHDOG_ACK);
		break;

	case WM_TIMER:                   //  ��ʱ
		monitor(WM_TIMER);
		break;
	case MSG_TRACE_ON://�򿪸���
		monitor(MSG_TRACE_ON);
		break;
	case MSG_TRACE_OFF://�رո���
		monitor(MSG_TRACE_OFF);
		break;
	default:
		return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return 0;
}
 


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char * lpCmdLine, int nCmdShow)
{ 
	MSG msg;
	int i=0;
    
	if (!InitApplication(hInstance)) 
		return (false);
    
	if (!InitInstance(hInstance, nCmdShow))
		return (false);

    ReadConfig();
	if (gProgramNum <=0 ) 
	{
		YF_DOGLOG  "No program need monitor,check config.");
		return false;
	}
	KillAllProgram();
	ReStartProgram();
//	_beginthread(TcpCustom_Run,0,NULL);
	SetTimer(hWnd, 0, TIMER_LEN, NULL);
//	SetTimer(hWnd, 2, TIMER_LEN, NULL);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}



/********************************************************************
�������ƣ�TcpCustom_Run
��������: �ͻ���ͨѶ���
��������: ��
���ض���: ��
����ʱ��: 2007-11-9 15:20
��������: ������
ע������: ��	
*********************************************************************/
void TcpCustom_Run(void *arg)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	SOCKET  sock_fd;
	unsigned long ul=1;
	int nRet,bconn;
	static struct sockaddr_in remote_addr;
	static struct sockaddr_in local_addr;
	fd_set rdset;
	struct timeval tv;
	int m_error = -1;
	BOOL m_ret;
	int len = sizeof(int);
	BOOL bRun = TRUE;
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return ;
	}
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		WSACleanup( );
		return ; 
	}
	
	while(bRun)
	{
		sock_fd=socket(AF_INET, SOCK_STREAM, /*IPPROTO_TCP*/0);
		if (sock_fd == INVALID_SOCKET)
		{
			closesocket(sock_fd);
			Sleep(100000);
			continue;
		}
		
		nRet = ioctlsocket(sock_fd,FIONBIO,(unsigned long *) &ul);/*������ģʽ��Ĭ��������ģʽ*/
		if(nRet == SOCKET_ERROR)
		{
			//return FALSE; 
			continue;
			//Failed to put the socket into nonblocking mode
		}		
		memset(&remote_addr,0,sizeof(remote_addr));
		remote_addr.sin_family = AF_INET ;
		remote_addr.sin_port = htons (9999) ; 
		remote_addr.sin_addr.S_un.S_addr = inet_addr(g_IP)/*("192.168.0.19")htonl(INADDR_ANY)*/; ;
		bconn = connect(sock_fd,(SOCKADDR *)&remote_addr,sizeof(remote_addr));
		if( bconn==SOCKET_ERROR)
		{
			FD_ZERO(&rdset);
			FD_SET(sock_fd,&rdset);
			tv.tv_sec=0;
			tv.tv_usec=500*1000;
			if (select(sock_fd + 1, NULL, &rdset, NULL, &tv) > 0)
			{
				getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, (char *)&m_error, &len);
				if (m_error == 0)
					m_ret = TRUE;
				else
					m_ret = FALSE;
			}
			else
				m_ret = FALSE;
		}
		if(m_ret)
		{
			//���ٷ��������̣߳����������߳�
			ReceiveFromTrace(sock_fd);			
		}
		Sleep(1*1000);
	}
	closesocket (sock_fd) ;
    WSACleanup () ;
	_endthread();
    return  ;
	
}

/********************************************************************
�������ƣ�Tcp_RecvThread
��������:��������߳�
��������: arg:����socket���
���ض���: ��
����ʱ��: 2007-11-9 15:22
��������: ������
ע������: ��	
*********************************************************************/
void ReceiveFromTrace(int fd)
{
	
	char buf[255];
	int len=0;
	BOOL bRun = TRUE;
	while(bRun)
	{
		memset(buf,0,sizeof(buf));
        len=recv(fd,buf,255,0);
		if(len<0)
		{
			//�жϴ���
			{
				if (WSAEWOULDBLOCK != (WSAGetLastError ()))
				{
					closesocket (fd) ;
					break;
				}
				
			}
		}
		else if(len>0)
		{
			if(len>=4)
			{
				if(buf[3]==0x16)
				{
					if(buf[2] == 0x02)
					{
						PostMessage(hWnd, MSG_TRACE_ON, 0,(LPARAM)hWnd);
						YF_DOGLOG "open trace");
					}
					else if(buf[2] == 0x03)
					{
						PostMessage(hWnd, MSG_TRACE_OFF, 0,(LPARAM)hWnd);
						YF_DOGLOG "close trace");
					}
				}
			}
		}
		
	}

	
}