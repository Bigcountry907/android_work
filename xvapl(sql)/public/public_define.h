/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2007-12-17   9:47
�ļ�����: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\public\public_define.h
�ļ�·��: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\public
file base:public_define
file ext: h
author:	  ������

purpose:	�����ṹ����
*********************************************************************/
#ifndef _PUBLIC_DEFINE_H_
#define _PUBLIC_DEFINE_H_
#include <windows.h>
#include "comm.h"
#pragma   pack(1)   //   ����1�ֽڷ�ʽ���ж���   

#define MAX_LENGTH CHAR_NUMBER
typedef struct
{
	BYTE bAccNum;/*�������������*/
	BYTE bIppNum;/*������ϵͳ����*/
	char version[4];/*�汾��*/
	char pName[64];/*����*/
	WORD wAttri;/*��������*/
	
} BASIC_INFOR; /*������Ϣ*/


typedef struct
{
	char pComputerName[64];/*�������*/
	char pDataBase[64];/*���ݿ���*/
	char pPassWd[16];/*����*/
} DATABASE_INFOR;/*���ݿ���Ϣ*/

typedef struct
{
	int id;/*���*/
	char pName[64];/*����*/
	WORD wAttri;/*����*/

}ACC_INFOR;/*�����������Ϣ*/

typedef struct
{
	int id;/*���*/
	char pName[64];/*����*/
	DWORD dIp;/*IP��ַ;*/
	WORD  wPort;/*�˿ں�*/
	WORD  wAttri;/*����*/
}IPP_INFOR;/*�������ӿ���Ϣ*/


typedef struct 
{
	UINT iRow;	//��Ҫ���صļ�¼:0:����Ҫ�������ݼ�¼;1:���ص�һ��;2:���صڶ���;....	                     	
	char sSql[MAX_LENGTH - sizeof(UINT)];  //sql���

}SQLSrcMsg;

typedef enum
{
	DBACK_FAILED =				0,
	DBACK_SUCC_NOTRETRECORD =	1,
	DBACK_SUCC_FINDRECORD =		2,
	DBACK_SUCC_NOTFINDRECORD =	3,
	DBACK_SUCC_NORECORDRET =	4,
	DBACK_FAILED_EXECSQL =		5,
	DBACK_FAILED_LENLACK =		6,
	DBACK_FAILED_MEMLACK =		7,
	DBACK_FAILED_NOCONN =		8
}DBAPACK;
typedef struct 
{
	UINT iSqlExecRet;  //SQL���ִ�н��:
						//0:ʧ��; 
						//1:ִ�гɹ�,δҪ�󷵻ؼ�¼��;=
						//2:ִ�гɹ�,�ҵ�ָ����¼;
						//3:ִ�гɹ�,δ�ҵ�ָ�������ݼ�¼
		                //4:ִ�гɹ�,û�м�¼���ɷ���; =
						//5:SQLִ��ʧ��(�﷨����)������Ͽ�; 
						//6:���ؽ�����ȳ����洢������
						//7:ϵͳ�����ѯ��������������ڴ治�ɹ�;
						//8:���ݿ��޷�����;
	UINT iTotalNumsOfRows; //��ִ�н�������ļ�¼����
	UINT iRow;       //����Ƕ�����¼,���ص�������¼���еĵڼ�����¼.
	char sResults[MAX_LENGTH-3*sizeof(UINT)];
}SQLDestMsg;

//typedef struct
//{
//	char userNum[32];//�û�����
//	char accessNum[32];//�������
//	char reversed[64]; //��������ͬҵ����ܲ�ͬ
//	WORD bkey;//�������Ϣ��ҵ���
//}strODIACCESS;//ODI����ҵ��ṹ


#pragma   pack() 

#endif