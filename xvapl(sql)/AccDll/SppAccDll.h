/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2007-12-23   17:29
�ļ�����: F:\��������\Դ����\AccDll\SppAccDll.h
�ļ�·��: F:\��������\Դ����\AccDll
file base:SppAccDll
file ext: h
author:	  ������

purpose:	ACCDLL�ڲ���������������	
*********************************************************************/
#ifndef _SPP_ACC_DLL_H_
#define _SPP_ACC_DLL_H_
#include "../public/comm.h"
#include "AccDll.h"
//#include "../public/pXSSM.h"
#pragma  pack(1)
//enum PLAY_FILE_TYPE{
//	PLAY_FILE_NONE=1,//������
//	PLAY_FILE_DIGIT,//����
//	PLAY_FILE_CHAR,//��
//	PLAY_FILE_CURRENCY,//����
//	PLAY_FILE_DATE,//����
//	PLAY_FILE_TIME,//ʱ��
//	PLAY_FILE_FILE,//�ļ�������·���� 
//};//�������ݸ�ʽ
//typedef struct 
//{
//	int nKey;//���,�Ự��
//	BYTE nNo;//������Ŀ��
//}PVOICE_HEAD;//��������ͷ
//typedef struct{
//	enum PLAY_FILE_FILE pType;//������������
//	char content[32];//��������
//}PVOICE_CONTENT;//��������
//
//typedef struct
//{
//	PVOICE_HEAD pVoiceHead;//��������ͷ
//	PVOICE_CONTENT *pVoice_Content;//��������ָ��
//}PVOICE;//�����ļ�

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

typedef	 void (*pEvtdefine)(DJ_U32 esrParam);


ServerID_t		cfg_ServerID;
char			cfg_VocPath[128];
int				cfg_iDispChnl;
int				cfg_iVoiceRule;

int				cfg_iPartWork;
int				cfg_iPartWorkModuleID;

int				cfg_s32DebugOn;

// var about work
DWORD		g_acsHandle ;
BYTE		g_u8UnitID ;
pEvtdefine    pEvtHandle;
TONE_INDEX pToneIndex;//�����ļ�����
TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];//DSP���Լ�¼
SID_XSSM_DLL_INFOR g_Sid_Xssm_Dll_Infor[MESSAGE_MAX];//�Ự��¼��������
TYPE_XMS_DSP_EXIST pDspMap;//��¼���ڵ�DSP��Ϣ��������
TYPE_CHANNEL_EXIST pTrunkMap;//�м�
TYPE_CHANNEL_EXIST pUserMap;//ģ���м�
TYPE_CHANNEL_EXIST pVoiceMap;//�����м�
TYPE_CHANNEL_EXIST pPcmMap;//E��
void VoiceFileHandle(DeviceID_t *pDevId,PVOICE *pVoice,int offset);
void HandleDevState(Acs_Evt_t *pAcsEvt );
BOOL InitAcsSys(ACS_ATTR pAcs_attr);
BOOL ExitSystem();
void EvtHandler(DJ_U32 esrParam);
BOOL InitAllDevice();
void ClearSid(DWORD sid);
BOOL InitAllDllInfor();
void AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList);
void AddDeviceRes_Trunk(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void AddDeviceRes_Voice(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void AddDeviceRes_Board(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void AddDeviceRes_Pcm(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void CloseBoardDevice(DeviceID_t *pBoardDevID );
void ClosePcmDevice(PCM_STRUCT *pOnePcm );
void CloseVoiceDevice(VOICE_STRUCT *pOneVoice );
void CloseTrunkDevice(TRUNK_STRUCT *pOneTrunk );
void CloseDeviceOK(DeviceID_t *pDevice );
void OpenAllDevice_Dsp(DJ_S8 s8DspModID );
void OpenTrunkDevice(TRUNK_STRUCT *pOneTrunk );
void OpenVoiceDevice(VOICE_STRUCT *pOneVoice );
void OpenPcmDevice(PCM_STRUCT *pOnePcm );
void OpenBoardDevice(DJ_S8 s8DspModID );
void ResetUser ( TRUNK_STRUCT *pOneUser );
void OpenDeviceOK(DeviceID_t *pDevice);
int	SearchOneFreeVoice(TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID );
void ResetTrunk(TRUNK_STRUCT *pOneTrunk);
void InitTrunkChannel(TRUNK_STRUCT *pOneTrunk );
void Change_State(TRUNK_STRUCT *pOneTrunk, enum TRUNK_STATE NewState );
void Change_UserState (TRUNK_STRUCT *pOneTrunk, enum USER_STATE NewState);
void Change_Voc_State(VOICE_STRUCT *pOneVoice, enum VOICE_STATE NewState );
int	 FreeOneFreeVoice(DeviceID_t *pFreeVocDeviceID );
void BuildVoiceIndex(DeviceID_t	*pVocDevID );
void InitDtmfBuf(TRUNK_STRUCT *pOneTrunk);
int StopPlayFile(DeviceID_t *pVocDevID);
void DualLinkDevice(DeviceID_t *pDev1, DeviceID_t *pDev2 );
void DualUnLinkDevice(DeviceID_t *pDev1, DeviceID_t *pDev2 );
void TrunkHandle(DJ_U32 esrParam/*TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt*/ );
void UserHandle(DJ_U32 esrParam/*TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt*/ );
int	 PlayFile(DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, BOOL bIsQueue );
int	 PlayIndex(DeviceID_t	*pVocDevID, DJ_U16 u16Index, DJ_U8 u8PlayTag, BOOL bIsQueue );
BOOL CheckPlayEnd(TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void SetGtD(DeviceID_t* pDevId);
void FreeOneDeviceRes(int ID );
void FreeAllDeviceRes(void);
int RecordFile(WORD nKey,stPXSSM_InitiateRecord pInitateRecord);
int	 SearchOneFreeTrunk (DeviceID_t *pFreeTrkDeviceID);
BOOL IsTrunk(int s16DevSub);
BOOL IsDigitTrunk(int s16DevSub);
DWORD SearchSid(WORD wDsp,WORD wTrunk);

int TestPrintf(int i,int j,const char *format,...);
void RefreshDspMap(void);
BOOL ReadToneIndexFile();
#endif