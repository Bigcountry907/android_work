#ifndef _PXSSM_H_
#define _PXSSM_H_
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "comm.h"
#include "../AccDll/AccDll.h"
#include "WatchDefine.h"

#pragma pack(1)
union BCSM_ATTR{
	WORD attr;
	struct
	{
		WORD bcsm:1;/*�Ƿ�����BCSM*/
		WORD bPlayFile:1;/*�Ƿ����ڷ���*/
		WORD bInterrupt:1;/*�Ƿ���Դ��*/
		WORD bPlayFileEnd:1;/*�Ƿ�������*/
		WORD bfirstDigitTimeOut:1;/*�Ƿ���λ��ʱ*/
		WORD binterDigitTimeOut:1;/*�Ƿ�λ�䳬ʱ*/
		WORD bneedNullCDR:1;/*�Ƿ��ϱ�0����*/
		WORD bCallOut:1;/*�Ƿ����*/
		WORD bSendDataNow:1;/*֮ǰ�Ƿ񷢳�����*/
		WORD res : 7;
	};
	
}; /*BCSM״̬����������*/
typedef struct 
{
	BYTE    minDigits;   /* ��С�ռ��������� */
    BYTE    maxDigits;   /* ����Ѽ�������  */
    BYTE    endDigit;    /* �����ַ� */
    BYTE    cancelDigits;       /* ȡ���ַ� */
    BYTE    startDigit;         /* ��ʼ�ַ� */
    WORD    firstDigitTimeOut;  /* ��λ��ʱ  */
    BYTE    interDigitTimeOut;  /* λ�䳬ʱ  */
	BYTE   bConnnectNumber; /* �ɼ��������ݸ��� */
	BYTE   collectedDigits[64];/*�ռ������ַ���*/
}COLLECTINFORMATION;//�ռ���Ϣ��������
typedef struct
{
	DWORD    sid;     /* �Ự�� */
	char    CalledNumber[32];    /* ���к��� */
	char    CallingNumber[32]; /* ���к��� */
	DWORD   status;//״̬
	union BCSM_ATTR  attrd;   /*BCSM״̬����������*/
	COLLECTINFORMATION pCollectInfor;//�ռ���Ϣ��������
	WORD          eventNumber;   //ע����¼�������С�ڵ���8
    enum emPXSSM_BCSM  events[8];     //ע����¼�
	BYTE bnodenum;//�����Ǹ��ڵ�
	int  serviceKey;     /* ҵ��� */
	WORD timerId;//��ʱ������ֵ
	
}XSSM_SID;//�Ự��¼��������
typedef struct
{
	int    serviceKey;     /* ҵ��� */
	BYTE   toneIndex;    /* ҵ��������� */
	BYTE   language; /* �������� */
	BYTE   argIndes;/*������ţ���Ϊ0ʱ��ʾreferenceֵΪ�������ܸ�������������*/
	BYTE   argType; /*������� 0����Ԫ    1���ļ����� 2����ֵ 3�����ִ�  4����� 5������    6��ʱ��*/
	char reference[32];/*����ֵ*/	
} SERVICE_TONE;//ҵ��������Ԫ����
typedef struct 
{
	BYTE         elementNum;//��Ԫ����
	SERVICE_TONE *pToneContent;//������Ԫ
}SERVICE_KEY_ELEMENT;//�������ݼ�¼
typedef struct 
{
	BYTE ToneNum;//������Ŀ��
	SERVICE_KEY_ELEMENT *pElement;//�������ݼ�¼
}SERVICE_KEY_TONE;//����ҵ��
typedef struct 
{
	BYTE   nodenum; /*�ڵ��*/
	WORD   serviceTotal;    /* ҵ�������� */
}NODE_INFOR;/*�ڵ�ҵ������Ϣ*/
typedef struct
{
	int   SkeyType;//ҵ���������
	int   serviceKey;     /* ҵ��� */
	BYTE  initialtype;     /*1�����к���ʶ��ҵ�� 2�����к���ʶ��ҵ��*/
	char  initialNum[16]; /*����ҵ��ĺ���*/
	BYTE  mask;  /* 0��������ƥ�� 1��������ƥ�� 2��������ȫƥ��*/
	BYTE  nodeTotal; /*�����ҵ���XVAPLƽ̨�ڵ������֧��1-4���ڵ�*/	
	NODE_INFOR *pNodeInfor;/*�ڵ�ҵ������Ϣ*/
	int  bUseNode;//�ϴ�ҵ��ʹ�õĽڵ��
	SERVICE_KEY_TONE pTone;//����ҵ��
	char description[32];//ҵ���Ҫ����
//	int  modulenumber;/*ģ����*/
} SERVICE_KEY;/*ҵ����Ϣ*/

#pragma  pack()
XSSM_SID  g_Sid[MESSAGE_MAX];

BOOL bPlatForm;//�Ƿ�Ϊƽ̨
BOOL bHdlConnectStatus;/*��Ӳ���豸����״̬ */
int  ServiceKey_Total;//ҵ���ܸ���
BOOL IsPSTN;//�Ƿ���PSTN
SERVICE_KEY *pServiceKey;//ҵ������
ACS_ATTR Hdl_Attr;//��¼HDL������Ϣ
XSSM_TIME sXssmTime;//��¼��ʱʱ����Ϣ
HWND pwnd;

BOOL bFindSpp;   /*spp���ӱ�־*/
BOOL ControlTrace(void *infor);
BOOL XSSM_ActiveTestResponse();
BOOL XSSM_ErrorReport(DWORD sid,BYTE ErrorType);
BOOL XSSM_StartDP(stPXSSM_InitiateDP pInitiateDP);
BOOL XSSM_RealeaseCall(stPXSSM_ReleaseCall pReleaseCall);
BOOL XSSM_CollectedInformation(stPXSSM_CollectedInformation pCollectedInformation);
BOOL XSSM_CollectedFSKInformation(stPXSSM_CollectedFSKInformation *pCollectedFSKInformation);
//BOOL XSSM_ResourceReport(DWORD sid);
BOOL XSSM_ResourceReport(/*DWORD sid*/stPXSSM_ResourceReport *pResourceReport);
BOOL XSSM_DoCallIn(void * pInfor);
BOOL XSSM_DoCallOut(void * pInfor);
BOOL XSSM_DoClearCall(void *pInfor);
BOOL XSSM_DoAll(void * pInfor);
BOOL XSSM_DoRecvIoData(void *pInfor);


BOOL XSSM_RecvConnectToResouce(void *infor);
BOOL XSSM_RecvDisConnectForwardConnection(void *infor);
BOOL XSSM_RecvPlayAnnouncement(void *infor);
BOOL XSSM_RecvPromptCollectInformation(void *infor);
BOOL XSSM_PromptCollectInformationAndSetFormat(void *infor);
BOOL XSSM_RecvSendFSK(void *infor);
BOOL XSSM_RecvSendFSKCollectInformation(void *infor);
BOOL XSSM_RecvPlayAnnouncementAndSetFSK(void *infor);
BOOL XSSM_RecvSendFSKCollectFSK(void *infor);
BOOL XSSM_RecvInitiateRecord(void *infor);
BOOL XSSM_RecvStopRecord(void *infor);
BOOL XSSM_RecvReleaseCall(void *infor);
BOOL XSSM_RecvConect(WORD sid,void *infor);
BOOL XSSM_RecvRequestReportBCSMEvent(void *infor);
BOOL XSSM_RecvTTSConvert(void *infor);
BOOL XSSM_RecvTTSPlay(void *infor);

void DealWithMessage(WPARAM wParam,LPARAM lParam);
void DoTimer(DWORD evt,DWORD sid);
void DoKillTimer(DWORD evt,DWORD sid);
void SetWaiteTimer(DWORD sid);
BOOL Init_XSSM();
BOOL Start_All();
BOOL ReadConfig();
BOOL ReadKeyTone(int keyNum);
BOOL SetRecallFuction();
void EvtHandler(DWORD esrParam);
void ReInitKeygoe();


BOOL XSSM_SENDMESSAGE(DWORD sid,void *pInfor,int len,WORD devent,BOOL bNew);
PVOICE *SwtichToneFile(DWORD sid,stPXSSM_Tone *pTone);
int FindKeyModule(char CalledNum[32],char CallingNum[32]);
int FindKeyNode(char CalledNum[32],char CallingNum[32]);
void FreeMerroy();
void SetSidEvent(WORD wNumber,DWORD ievent);

void SetRegisterHotKey(HWND hwnd);
void FreeHotKey(HWND hwnd);
void Monitor(int event, LPARAM lParam);
void InitTimer();
void GetSppStatus();
void StartAllWork();
//BOOL Update_Message(WORD senssionID,MESSAGE *pMsg,BOOL bDelete);
//BOOL Read_Message(WORD senssionID,MESSAGE *pMsg);
#endif