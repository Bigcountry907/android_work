#ifndef _ACC_BPUBLIC_H
#define _ACC_BPUBLIC_H
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h> 
#include <fcntl.h>
#include "comm.h"
#pragma  pack(1)
enum CHANNEL_STATE {
	CH_FREE,//���� 
	CH_RECEIVEID,//�������к��룬ժ��
	CH_CHECKPLAY,//��� �����Ƿ����
	CH_CHECKFSK,//���FSK�Ƿ������
	CH_RECVFSK,//����FSK
	CH_SENDFSK,//����FSK
	CH_RECVDTMF,//����DTMF
	CH_DOOTHER,//ͳһ����
	CH_WAITSECONDRING,
	CH_WELCOME,
	CH_ACCOUNT,
	CH_ACCOUNT1,
	CH_PASSWORD,
	CH_PASSWORD1,
	CH_SELECT,
	CH_SELECT1,
	CH_PLAYRESULT,
	CH_RECORDFILE,
	CH_PLAYRECORD,
	CH_OFFHOOK,
	CH_WAITUSERONHOOK,
	CH_DIAL,
	CH_CHECKSIG,
	CH_PLAY,
	CH_ONHOOK,
	CH_CONNECT,

	CH_BUSY,
	CH_NOBODY,
	CH_NOSIGNAL,
	CH_NODIALTONE,
	CH_NORESULT
};
typedef struct 
{
	int nType;//����
	int State;//״̬
	char CallerID[32];//���к���
	char TelNum[32];//���к���
	char Dtmf[32];//dtmf����
	int nTimeElapse;//�ȴ�ʱ��
	int senddatalen;//����FSK����
	BYTE senddata[1024];//�������ݳ���
	int datalen;//���ݳ���
	BYTE data[1024];// �������� 
	BOOL FBeginRecv;
	int FPrevLen;
	int FHeadPos;
	DWORD dStart;
}CHANNElINFOR;//�忨ͨ����Ϣ
enum VOICE_TYPE{
		VOICE_NONE,//������
		VOICE_FILE,//�ļ�
		VOICE_INDEX,//���
		VOICE_Variable,//����
};
/************************************************************************/
/* �������Ͳ�ͬ�������ļ���ʽҲ��ͬ����ΪVOICE_FILEʱ,�ļ�����Ϊ�ļ���(����·��),
��ΪVOICE_INDEXʱ,�ļ�����Ϊ��������,��ΪVOICE_Variableʱ, �ļ�����Ϊ��������,
ÿ������������,��ʽ����:
��ֵ%d,���%m,��%s,����%date,ʱ��%t                                            */
/************************************************************************/
typedef struct 
{
	enum VOICE_TYPE pVoiceType;//��������
	char content[256];//����
	int nLen;//���ݳ���
	BOOL bInterrupt;//�Ƿ���Դ��
}PA;//�����ļ�

typedef struct 
{
	PA   pPa;//�����ļ�
	BYTE bFirstOverTime;//��λ��ʱ
	BYTE bPositionOverTime;//λ�䳬ʱ
	BOOL bEndChar;//�Ƿ���Ҫ������
	char pEndChar;//������
	BYTE bMinLen;//������С����
	BYTE bMaxLen;//������󳤶�
}RECEIVE_PA;//���������ļ�
enum DIRECTION
{
	DIR_NONE,
	DIR_FSK,//fsk->fsk
	DIR_DTMF,//fsk->dtmf
};
typedef struct 
{
	BYTE data[256];//data content
	BYTE len; //data length
	BYTE overtime; //overtime set
	enum DIRECTION fsk_dir;//fsk direction
}FSK;

typedef struct
{
	WORD sid;/*�Ự��*/
	WORD serviceKey;/*ҵ���*/
	DWORD DeviceNumber;/*�豸�����ϣ������豸��ͬ����Ҫ��д��ֵҲ��ͬ��Keygoeϵͳ����ΪDSP�ţ�����Ϊ�м��豸���*/
	char CallerCode[20];  /*���к���*/
	char CalleeCode[20]; /*���к���*/
	BOOL bBusy;/*״̬������ʹ�û��ǿ��У�FALSEΪ���У�TRUEΪռ��*/
}SID_XSSM_DLL_INFOR;//�Ự������Ϣ��¼

typedef struct
{
	WORD sid;/*�Ự��*/
	WORD serviceKey;/*ҵ���*/
	SOCKET fd;/*�������Ӿ��*/
	char ip[16];  /*�ͻ���IP*/
	char markadd[20]; /*MARK��ַ*/
	BOOL bState;/*״̬������ʹ�û��ǿ��У�FALSEΪ���У�TRUEΪռ��*/
}SID_TCP_INFOR;//TCP�Ự������Ϣ��¼

typedef struct 
{
	WORD serviceKey;/*ҵ���*/
	char ip[16];  /*�ͻ���IP*/
	WORD port; /*port*/
	SOCKET fd;/*�������Ӿ��*/
	BOOL bState;/*״̬������ʹ�û��ǿ��У�FALSEΪ���У�TRUEΪռ��*/
}TCP_SERVICE_INFOR;//ҵ���������;
typedef struct 
{

	WORD index;//������
	char filename[32];//�ļ���

}TONE_INDEX_FILE;//�����ļ�����
typedef struct 
{
	WORD g_total;//������Ŀ��
	TONE_INDEX_FILE *pIndexFIle;//�����ļ�����
}TONE_INDEX;//�����ļ�������

#pragma  pack()
//typedef unsigned short      WORD;
typedef unsigned long     DJ_U32;
typedef	 void (*pEvtdefine)(DJ_U32 esrParam);
SID_XSSM_DLL_INFOR g_Sid_Xssm_Dll_Infor[MESSAGE_MAX];//�Ự��¼��������
WORD TotalChannel;
pEvtdefine    pEvtHandle;
long DriverOpenFlag;
CHANNElINFOR *pChannelInfor;
char VoicePath[100];
char sTmp[128];

#endif
