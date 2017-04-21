/********************************************************************
	created:	2010/10/27
	created:	27:10:2010   15:17
	filename: 	E:\���ݿ��׸�ϵͳ\���׸�����\xvaplԴ����\Acc_BDll\Acc_BDll.c
	file path:	E:\���ݿ��׸�ϵͳ\���׸�����\xvaplԴ����\Acc_BDll
	file base:	Acc_BDll
	file ext:	c
	author:		ldw
	
	purpose:	
*********************************************************************/
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
#include "Tc08a32.h"
#include "NewSig.h"
#include "commdll.h"
#include "D.h"

#include "Acc_BDll.h"
TONE_INDEX_FILE *pToneIndex;
int nToneTotal;
BOOL InitToneIndex();
BOOL InitAllDllInfor();

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
		
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
�������ƣ�Hdl_Init
��������: Hdl��ʼ���ӿ�
��������: ��
���ض���: ��
����ʱ��: 2007-12-23 16:58
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT void Hdl_Init(ACS_ATTR attr)
{
	TotalChannel=0;
	DriverOpenFlag=-50;
	InitAllDllInfor();
	nToneTotal=0;
	InitToneIndex();
	_beginthreadex(NULL,0,BAcc_ThreadProc,NULL,0,NULL);
	
}

DLLIMPORT void Hdl_SetTrace(BOOL value)
{
	//g_Trace = value;
}
/********************************************************************
�������ƣ�Hdl_SetRecall
��������: ���ûص�����
��������: esr���ص�����ָ��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-12-23 16:59
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_SetRecall(void *esr)
{
//	pEvtHandle= esr ;
	return  TRUE;
}
/********************************************************************
�������ƣ�Hdl_SendFSK
��������: ����FSK
��������: sid:�Ự�ţ�data:�������ݣ�len:���ݳ���
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-1-31 14:50
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_SendFSK(DWORD sid,/*BYTE *data,WORD len*/void *infor)
{


	WORD ChannelId;
	int r;
	stFSK *fsk=(stFSK *)infor;

	if(fsk == NULL)
		return FALSE;
	
	if(sid>MESSAGE_MAX)
	{
		
//		YF_LOG_SMM ,"ACC->Keygoe:sid is overflow,send fsk failed");
		
		return FALSE;
	}
	if(!g_Sid_Xssm_Dll_Infor[sid].bBusy)
	{
//		YF_LOG_SMM "Send FSK resource busy");
		return FALSE;
	}
	ChannelId = (WORD)(g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256));
	if(ChannelId >= TotalChannel)
		return FALSE;
	if((pChannelInfor[ChannelId].State==CH_FREE))
		return FALSE;
//	pChannelInfor[ChannelId].senddatalen=fsk->length;
//	memset(pChannelInfor[ChannelId].senddata,0,sizeof(pChannelInfor[ChannelId].senddata));
//	memcpy(pChannelInfor[ChannelId].senddata,fsk->message,fsk->length);
//	pChannelInfor[ChannelId].State=CH_SENDFSK;
//	return TRUE;
	r=DJFsk_SendFSK(ChannelId,fsk->message,fsk->length,FSK_CH_TYPE_160);
	if(r!=1)
		return FALSE;
	else
	{
		pChannelInfor[ChannelId].State=CH_CHECKFSK;
		return TRUE;
	}
}
/********************************************************************
�������ƣ�Hdl_Exit
��������: Hdl�˳���ϵͳ�Զ��ر���ACSϵͳ�����ӣ��ͷŷ�����ڴ�
��������: ��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-12-23 17:00
��������: ������
ע������: ��	
*********************************************************************/

DLLIMPORT BOOL Hdl_Exit()
{
	DriverOpenFlag=-255;
	InitAllDllInfor();
	free(pToneIndex);
	pToneIndex=NULL;
//	return(ExitSystem());
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_PlayFile
��������: ����
��������: ��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-12-23 17:01
��������: ������
ע������: ��	
*********************************************************************/
//DLLIMPORT BOOL Hdl_PlayFile(BYTE bType,BYTE bNo,BOOL bPlay)
DLLIMPORT void Hdl_PlayFile(int nDsp,int nTrunkNo,LPARAM lParam)
{
	char pChar[32];
	char FileName[200];
	int toneno;
	PVOICE *pVoice =(PVOICE *)lParam;
	

	int i;
	if(nDsp>=TotalChannel)
		return;
	if(pChannelInfor[nDsp].State==CH_FREE)
		return ;
	if(pVoice == NULL)
		return;
	StopPlayFile((WORD)nDsp);
	RsetIndexPlayFile((WORD)nDsp);
	for(i=0;i<pVoice->pVoiceHead.nNo;i++)
	{
		
		memset(pChar,0,sizeof(pChar));
		memset(FileName,0,sizeof(FileName));
		strcpy(pChar,pVoice->pVoice_Content[i].content);
		toneno=atoi(pChar);
		sprintf(FileName,"C:\\yfcomm\\Voc\\%s",pToneIndex[toneno].filename);
		AddIndexPlayFile((WORD)nDsp,FileName);
	}
	StartIndexPlayFile((WORD)nDsp);
	pChannelInfor[nDsp].State=CH_CHECKPLAY;
	pChannelInfor[nDsp].datalen=0;
	memset(pChannelInfor[nDsp].data,0,sizeof(pChannelInfor[nDsp].data));
}
/********************************************************************
�������ƣ�Hdl_FilePlay
��������: �ļ�����
��������: sid:�Ự�ţ�pVoiceContent����������
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 14:33
��������: ������
ע������: ��	
*********************************************************************/

DLLIMPORT BOOL Hdl_FilePlay(DWORD sid, void * VoiceAddr/*PVOICE *pVoiceContent*/)
{
	PVOICE *pVoice =(PVOICE *)VoiceAddr;
	int i;
	int nDsp,nTrunkNo;
	char pChar[32];
	char FileName[200];
	int toneno;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);

	if(nDsp>=TotalChannel)
		return FALSE;
	if(pChannelInfor[nDsp].State==CH_FREE)
		return FALSE;
	if(pVoice == NULL)
		return FALSE;
	StopPlayFile((WORD)nDsp);
	RsetIndexPlayFile((WORD)nDsp);
	for(i=0;i<pVoice->pVoiceHead.nNo;i++)
	{
		
		memset(pChar,0,sizeof(pChar));
		memset(FileName,0,sizeof(FileName));
		strcpy(pChar,pVoice->pVoice_Content[i].content);
		toneno=atoi(pChar);
		sprintf(FileName,"C:\\yfcomm\\Voc\\%s",pToneIndex[toneno].filename);
		AddIndexPlayFile((WORD)nDsp,FileName);
	}
	StartIndexPlayFile((WORD)nDsp);
	pChannelInfor[nDsp].State=CH_CHECKPLAY;
	pChannelInfor[nDsp].datalen=0;
	memset(pChannelInfor[nDsp].data,0,sizeof(pChannelInfor[nDsp].data));
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_setsid
��������: ���ûỰ��
��������: 
		sid���Ự��
		calledNum�����к���
		callingnum:���к���
���ض���: �ɹ���TRUE��ʧ�ܣ�FALSE
����ʱ��: 2010-10-29 11:54
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_setsid(DWORD sid,DWORD DeviceNumber,char calledNum[32],char callingNum[32])
{
	int number;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	number = sid;
	g_Sid_Xssm_Dll_Infor[number].sid = (WORD)sid;
	g_Sid_Xssm_Dll_Infor[number].DeviceNumber = DeviceNumber;
	strcpy(g_Sid_Xssm_Dll_Infor[number].CalleeCode,callingNum);
	strcpy(g_Sid_Xssm_Dll_Infor[number].CallerCode,calledNum);
	g_Sid_Xssm_Dll_Infor[number].bBusy=TRUE;
	return FALSE;
}

/********************************************************************
�������ƣ�ClearSid
��������: ��ջỰ������
��������:sid���Ự��
���ض���: ��
����ʱ��: 2010-10-29 12:05
��������: ������
ע������: ��	
*********************************************************************/
void ClearSid(DWORD sid)
{
	if(sid>=MESSAGE_MAX)
		return;
	g_Sid_Xssm_Dll_Infor[sid].sid=0;
	memset(g_Sid_Xssm_Dll_Infor[sid].CallerCode,0,sizeof(g_Sid_Xssm_Dll_Infor[sid].CallerCode));
	memset(g_Sid_Xssm_Dll_Infor[sid].CalleeCode,0,sizeof(g_Sid_Xssm_Dll_Infor[sid].CalleeCode));
	g_Sid_Xssm_Dll_Infor[sid].DeviceNumber=0;
	g_Sid_Xssm_Dll_Infor[sid].bBusy=FALSE;
}
/********************************************************************
�������ƣ�InitAllDllInfor
��������: ��ʼ�����м�¼�Ự����Ϣ
��������: ��
���ض���: ��
����ʱ��: 2010-10-29 11:14
��������: ������
ע������: ��	
*********************************************************************/

BOOL InitAllDllInfor()
{
	int i;
	for (i=0;i<MESSAGE_MAX;i++)
	{
		ClearSid((WORD)i);
	}
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_clearsid
��������: ע���Ự��
��������: sid:�Ự��
���ض���: �ɹ���TRUE��ʧ�ܣ�FALSE
����ʱ��: 2010-10-29 13:38
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_clearsid(DWORD sid)
{
	if(sid>=MESSAGE_MAX)
	{
		return FALSE;
	}
	else
	{
		ClearSid(sid);
	}
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_InitateRecord
��������: ����¼��
��������: pInitateRecord:¼���ļ������ʽ
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 14:57
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_InitateRecord(stPXSSM_InitiateRecord pInitateRecord)
{

	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_IniateCallAttempt
��������: �����Ժ�
��������: pCallAttempt:��������
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 15:15
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_IniateCallAttempt(stPXSSM_InitiateCallAttempt pCallAttempt)
{

	Hdl_Connect(pCallAttempt);

	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_SendMessage
��������: ��ACSϵͳ������Ϣ
��������: ��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-12-23 16:58
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_SendMessage(DWORD sid,DWORD esrParam )
{

	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_StartDPAck
��������: ����ȷ��
��������: ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-2-1 15:53
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_StartDPAck(DWORD DeviceNumber,BOOL bFree)
{
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_StopFilePlay
��������: ֹͣ�����ӿ�
��������: sid: �Ự��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 14:39
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_StopFilePlay(DWORD sid)
{
	int nDsp;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	if(nDsp>=TotalChannel)
		return FALSE;
	if(pChannelInfor[nDsp].State==CH_FREE)
		return FALSE;
	StopPlayFile((WORD)nDsp);
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_StopRecord
��������: �������ڵ�¼��
��������: sid:�Ự��
���ض���: �ɹ�����TRUE,ʰܷ���FALSE
����ʱ��: 2010-10-29 15:17
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_StopRecord(DWORD sid)
{

	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_TTSConvert
��������:���ı�ת��Ϊ�����ļ�
��������: pTTSConvert��ת�����ݼ�Ҫ��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 15:26
��������: ������
ע������: ��	
*********************************************************************/

DLLIMPORT BOOL Hdl_TTSConvert(stPXSSM_TTSConvert pTTSConvert)
{
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_TTSPlay
��������: TTS����
��������: sid:�Ự�ţ�pTTSFile����������
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 15:28
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_TTSPlay(DWORD sid,stPXSSM_TTSPlayAnnouncement pTTSFile)
{

	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_StopTTSPlay
��������: ֹͣTTS����
��������: sid:�Ự��
���ض���: �ɹ�����TRUE,ʰܷ���FALSE
����ʱ��: 2010-10-29 15:31
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_StopTTSPlay(DWORD sid)
{
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_Connect
��������: �������ӵ�һ��Ŀ�ĵ�ַ����ɺ��й���
��������: pConnect����������
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 15:02
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_Connect(stPXSSM_Connect pConnect)
{


	DWORD sid =pConnect.sid;
	int i;
	int ChNo=-1;
	DWORD Devicenumber;
	for(i=0;i<TotalChannel;i++)
	{
		if((pChannelInfor[i].nType==CHTYPE_TRUNK)&&(pChannelInfor[i].State=CH_FREE))
		{
			ChNo=i;
			break;
		}
	}
	if(ChNo==-1)
		return FALSE;
	Devicenumber =(DWORD)(ChNo*256*256);
	strcpy(pChannelInfor[ChNo].CallerID,pConnect.callingNumber);
	strcpy(pChannelInfor[ChNo].TelNum,pConnect.routeNumber);
	pChannelInfor[ChNo].datalen=0;
	memset(pChannelInfor[ChNo].data,0,sizeof(pChannelInfor[ChNo].data));
	OffHook((WORD)ChNo);
	Sig_StartDial((WORD)ChNo,pChannelInfor[ChNo].TelNum,"", 0);
	pChannelInfor[ChNo].State=CH_DIAL;
	Hdl_setsid(sid,Devicenumber,pConnect.routeNumber,pConnect.callingNumber);
	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_ConnectToResource
��������: �����а󶨵�������Դ
��������: sid: �Ự��
���ض���: ��
����ʱ��: 2010-10-29 14:42
��������: ������
ע������: �ɹ�����TRUE��ʧ�ܷ���FALSE	
*********************************************************************/
DLLIMPORT BOOL Hdl_ConnectToResource(DWORD sid)
{

	return TRUE;
}
/********************************************************************
�������ƣ�Hdl_DisconnectForwardResource
��������: �ж�ǰ��������Դ����
��������: sid: �Ự��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 14:44
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_DisconnectForwardResource(DWORD sid)
{
	return TRUE;
}
void HWriteBLog_char1(char *pChar)
{
	SYSTEMTIME     Clock;
	FILE	    *fp=NULL;
	fp = fopen ("C:\\yfcomm\\log\\HACC_BLock.log","a");

	if ( fp == NULL )
		return ;

	GetLocalTime(&Clock);

	
		fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d have call in message,pChar is %s\n",
			Clock.wYear,Clock.wMonth, Clock.wDay,
		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,pChar);

	fclose (fp);
}
/********************************************************************
�������ƣ�Hdl_ReleaseCall
��������: �ͷź���
��������: sid: �Ự��,DeviceNumber:�豸��,bUseSid:sid�Ƿ���Ч��TRUE��Ч��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2010-10-29 14:56
��������: ������
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Hdl_ReleaseCall(DWORD sid,DWORD DeviceNumber,BOOL bUseSid)
{
	int nDsp,nTrunkNo;
	char pData[255];
	if(sid>=MESSAGE_MAX)
		return FALSE;
	if(!g_Sid_Xssm_Dll_Infor[sid].bBusy)
	{
		return FALSE;
	}
	if(!bUseSid)
	{
		nDsp = DeviceNumber/(256*256);
		nTrunkNo = DeviceNumber%(256*256);	
	}
	else
	{
		nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
		nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
		ClearSid(sid);
	}
	memset(pData,0,sizeof(pData));
	sprintf(pData,"release call channel is %d,ntype=%d,ntotal=%d",nDsp,pChannelInfor[nDsp].nType,TotalChannel);
	
	if(nDsp>=TotalChannel)
		return FALSE;
	if(pChannelInfor==NULL)
		return FALSE;
	HWriteBLog_char1(pData);
	if(pChannelInfor[nDsp].nType==CHTYPE_TRUNK)
	{
		
		HangUp((WORD)nDsp);
		Sig_ResetCheck((WORD)nDsp);
		StartSigCheck((WORD)nDsp);
		
	}
	pChannelInfor[nDsp].Dtmf[0]=0;
	pChannelInfor[nDsp].CallerID[0]=0;
	pChannelInfor[nDsp].State = CH_FREE;
	

	return TRUE;
}
BOOL InitToneIndex()
{
	SECTION *sec;
	INI ini;
	BYTE i;
	char ToneName[32];
	char tmpSection[LENGTH_SECTION];
	sprintf(ToneName,"C:\\yfcomm\\Tone\\ToneIndex.ini");
	loadINI(&ini,ToneName);
	/*�õ�[TONEINDEX]*/
	sprintf(ToneName,"TONEINDEX");
	sec=getSection(&ini,ToneName);
	if(sec==NULL)
	{
		return FALSE;
	}
	{
		nToneTotal=(WORD)atoi(GetKeyList(sec,0));
	}
	pToneIndex = (TONE_INDEX_FILE *)malloc(sizeof(TONE_INDEX_FILE) * nToneTotal);
	/*�õ�[KeyNumber]*/
	for(i=0;i<nToneTotal;i++)
	{
		sprintf(tmpSection,"INDEX%d",i);
		sec=getSection(&ini,tmpSection);
		if(sec==NULL)
		{
			return FALSE;
		}
		{
			pToneIndex[i].index=atoi(GetKeyList(sec,0));		
			strcpy(pToneIndex[i].filename,GetKeyList(sec,1));
			
		}
	}
	/*end*/
	freeINI(&ini);
	return TRUE;
}