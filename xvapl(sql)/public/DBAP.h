#ifndef _YF_DBD_H
#define _YF_DBD_H

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Commdll.h"
#include "define.h"
#include "event.h"
#include "init_viriable.h"
#include "public_define.h"
#include "DBI.h"

#define DBQUERY_THREAD_NUM  10
extern char g_sDBUser[50],g_sDBPwd[50];
extern BYTE g_DBtns[150];
extern BOOL g_bDbapTrace;
#pragma pack(1)



typedef struct
{
	UINT iConnectionNum;     //���ݿ����ӵ��ܸ���
	BYTE sConnInfo[DBQUERY_THREAD_NUM]; //�����ӵ���Ϣ,ÿ��BYTE�洢һ�����ӵ���Ϣ;
							//0:���ӶϿ�,1:��������;2:������Ч;... 
}DBCONNINFO;      //�������ݿ�������Ч�Լ�����Ϣ��

typedef struct
{
	UINT mId;   //ģ���
	BYTE iConnState;//���ݿ�״̬: 0:������; 1:����;....
	BOOL bIsBusy;//��ģ�����е����ݿ������Ƿ�æµ
}DBCONNSTATE;

#pragma pack()

BOOL DBInitial(MESSAGE*,WORD);
BOOL DBConnect(MESSAGE*,WORD);
BOOL DBAvailCheck(MESSAGE*,WORD);
BOOL DBDistrQuery(MESSAGE*,WORD);
void DBInfoMag();
void DBWriteLogFile(char* str);
void DBStateReport(MESSAGE*);
void DBTraceStatus(MESSAGE*);
//void DBAPThreadProc(LPVOID);
unsigned  __stdcall /*void*/ DBAPThreadProc(LPVOID);
unsigned  __stdcall /*void*/ DBIProc(LPVOID);
unsigned  __stdcall /*void*/ DBMonitorProc(LPVOID);

void InitThread();//��ʼ�������߳�

#endif