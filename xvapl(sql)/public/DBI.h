#ifndef _YF_DBI_H
#define _YF_DBI_H

#define DBNTWIN32

#include <windows.h>
#include <stdio.h>
#include <sqlfront.h>
#include <sqldb.h>
#include "event.h"
#include "init_viriable.h"
#include "Commdll.h"
#include "DBAP.h"

#define DBUSER "yfxvapluser"
#define DBPWD "yfxvaplpwd"
#define DBQ_OPEN_TIMEOUT    5           //�½�һ�����ݿ����ӵĳ�ʱ�ȴ�ʱ��(s)
#define DBQ_SQL_TIMEOUT		5			//SQL���Ĳ�ѯ�ȴ���ʱʱ��(s)
#define DBINIFILEPATH "C:\\yfcomm\\ini\\SPP.ini"
#define DBINISQL_SQLSERVER "SELECT [user],[password] FROM [master].[dbo].[yfcommini]"
#define DBCONN_TEST_SQL "select 1"
//#define DBCONN_TEST_SQL "select * from a"

#pragma pack(1)

typedef struct
{
	UINT mId;   //ģ���
	PDBPROCESS	pDBConn;//���ݿ������� 
}DBI_CONN;

#pragma pack()

int errid;
BOOL dbi_InitialDB();//��ʼ�����ݿ�;

BOOL dbi_InitialConn(UINT mId);//��ʼ�����ݿ�����

BOOL dbi_CreateConn(UINT mId);//��������(��������)

BOOL dbi_ReleaseConn(UINT mid);//�ͷ�����(��������)

BOOL dbi_ConnAvailCheck(UINT mId);//��������Ƿ����

BOOL dbi_ExecSql(UINT mId, MESSAGE* msg,WORD);//sqlִ����(����/ģ���,SQL���) 

void dbi_DBExit();

void dbi_WriteLogFile(char* str);

BOOL TransformIP(char *addr,DWORD *dwIP,BOOL flag);
int err_handler(PDBPROCESS, INT, INT, INT, LPCSTR, LPCSTR);
int msg_handler(PDBPROCESS, DBINT, INT, INT, LPCSTR, LPCSTR,LPCSTR, DBUSMALLINT);
BOOL dbi_TestExeSql(UINT mId);

void dbi_MsgAck_NoConn(UINT mId, MESSAGE* pyfMsg,WORD wNum);//���ݿ�������
void dbi_MsgAck_ExecFailed(UINT mId, MESSAGE* pyfMsg,WORD wNum);//���ݿ�ִ��ʧ��
void dbi_MsgAck_MemLack(UINT mId, MESSAGE* pyfMsg,WORD wNum);//�ڴ�����ʱʧ��
void dbi_MsgAck_LenLack(UINT mId, MESSAGE* pyfMsg,WORD wNum);//������ȳ����洢������
void dbi_MsgAck_NoneTableRet(UINT mId, MESSAGE* pyfMsg,WORD wNum);//�޽����¼���ɷ���
void dbi_MsgAck_NeednotRet(UINT mId, MESSAGE* pyfMsg,WORD wNum);
void dbi_MsgAck_UnFindDstRow(UINT mId, MESSAGE* pyfMsg,WORD wNum,UINT TotalRow);//δ�ҵ�ָ��Ҫ���ص���
void dbi_MsgAck_FindDstRow(UINT mId, MESSAGE* pyfMsg,WORD wNum,char *sBuf,UINT DstRow,UINT TotalRow);//�ҵ�ָ����,��������.
void dbi_TestConn(UINT);

void dbi_GetDBIni(char * DstUser,char * DstPwd);
void 	dbi_ReleaseMemory(char**,INT);
#endif