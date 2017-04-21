#ifndef _YF_DBD_H
#define _YF_DBD_H

#define DBNTWIN32
#include <windows.h>
#include <process.h>
#include <sqlfront.h>
#include <sqldb.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Commdll.h"
#include "event.h"
#include "init_viriable.h"
#include "Alarmdll.h"

/****************  ���ݿ�����¼�  **********************/
//#define evInitial               WM_USER
//#define evDataBase              (WORD)(evInitial  + 200)
//#define evDBDInital				(WORD)(evDataBase + 1)/*:���ݿ��ʼ��DBD*/
//#define evAckDBConnectionTest   (WORD)(evDataBase + 2)/*: ���ݿ�����״̬���Խ��DBD*/
//#define evDBQueryAsk			(WORD)(evDataBase + 3)/*:���ݿ��ѯ����DBD*/
//
//#define evDBConnectInital		(WORD)(evDataBase + 4)/*:���ݿ����ӳ�ʼ��DBQ*/
//#define evDBConnectionTest		(WORD)(evDataBase + 5)/*�����ݿ�����״̬����DBQ*/
//#define evDBExeAsk				(WORD)(evDataBase + 6)/*:���ݿ��ѯִ��DBQ*/
//#define evDBDisconnection		(WORD)(evDataBase + 7)/*:���ݿ����ӶϿ�DBQ*/

#define DBQ_CON_NUM			10          //���ݿ������߳���
#define TEST_TIME_INTERVAL  1500		//DBQ�̼߳������״̬��ʱ����(ms)
#define DBQ_OPEN_TIMEOUT    5           //�½�һ�����ݿ����ӵĳ�ʱ�ȴ�ʱ��(s)
#define DBQ_SQL_TIMEOUT		2			//SQL���Ĳ�ѯ�ȴ���ʱʱ��(s)
#define DBTEST_PAUSE_TIME	5000		//DBTEST �̵߳�һ������ʱҪ�ȴ���ʱ��(������DBQ������)

#pragma pack(1)
typedef struct 
{
	UINT		mId;               // for DBQ,DBT
	PDBPROCESS	pDBConName;			//���ݿ�������  for DBQ,DBT
	BOOL		bBusy;              //DBQ��ѯ�Ƿ�æµ for DBQ,DBT
	BOOL		bConState;			//���ݿ�״̬
}DBQTHREAD;
#pragma pack()

//void GetIni(void);
/*unsigned  __stdcall*/void DBDThreadProc(LPVOID lpParameter);
BOOL SetMsg(MESSAGE * ,TID *sender,TID * recv,UINT msg);
extern void InitDBQ(DBQTHREAD *);
#endif