#ifndef _TEST_DLL_H_
#define _TEST_DLL_H_
#if BUILDING_DLL
# define DLLIMPORT __declspec (dllimport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllexport)
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h> 
#include <time.h>
#include <../AcsPublic/DJAcsDataDef.h>
#define CALLEDNUM "744991"
#define DEVICENUM 516
#define TESTINIFILE "C:\\yfcomm\\test\\device.ini"

#pragma  pack(1)

typedef struct 
{
	DJ_S8 DspID;
	WORD wE1Num;//DIGITAL_PORT
	WORD wInterfaceNum;//INTERFACE_CH
	WORD wVoiceNum;
	WORD wFaxNum;
	WORD wVoIPNum;
	WORD wConferenceNum;
	WORD wTotalDevNum;
}DspInfo;

typedef struct 
{
	DeviceID_t Device_ID;	//m_s8ModuleID:				�豸���ڵ�DSP��ID
							//m_s16DeviceGroup:			�����豸����--0:E1,1:�ӿ�,2:����,3:����,4:VOIP,5:������.
							//m_s16ChannelID:			�������豸���������е�λ��
							//m_s8MachineID:			�豸���ڵ�DSP��ID
							//m_CallID.m_s32FlowChannel:�������豸�е������±�.
							//m_CallID.m_s32FlowType:	���豸�������±�,������Ϊ-1
							//m_Rfu2[1]:				���豸����,��:-1, ��Դ���豸:-2, Ŀ���豸:-3, ����:-4.					
	INT DeviceState;				//�豸״̬.
	BOOL bHaveSrcBinding;			//�Ƿ���Դ���豸
	DeviceID_t SrcBindingDevID;		//Դ���豸
	BOOL bHaveDstBinding;			//�Ƿ���Ŀ�İ��豸
	DeviceID_t DstBindingDevID;		//Ŀ�İ��豸
	char TerminalNum[32];			//���豸��ʹ�ú����к���
	char CalledNum[32];				//���к���
}DeviceID;

enum DJ_MSG_TYPE
{
	MSG_TYPE_CALLIN		= 0,
	MSG_TYPE_ALERT		= 1,
	MSG_TYPE_ANSWER		= 2,
	MSG_TYPE_PLAY		= 3,
	MSG_TYPE_SENDDATA	= 4,
	MSG_TYPE_CLEARCALL	= 5,
	MSG_TYPE_CONTROLPLAY = 6,
	MSG_TYPE_PAUSE      = 7,
};

typedef struct
{
	BYTE MsgType;		//��Ϣͷ
	DJ_S8 DeviceType;	//�豸����
	WORD wDataLen;//��Ϣ����
	DJ_S8 nDspID;		//DSP��ID
	WORD wDevPos;		//�豸���������λ��
	char TerminalNum[32];
}MsgHead;

typedef struct 
{
	MsgHead msgHead;
	char message[256];
}NetMsg;

typedef struct
{
	Acs_Evt_t Event;
	Acs_GeneralProc_Data Data;
}DJ_OPEN_STREAM;//�򿪶���,���豸,

typedef struct 
{
	Acs_Evt_t Event;
	Acs_Dev_List_Head_t DevListHead;
	DeviceID_t szDev[256];
}DJ_DEV_LIST;

typedef struct 
{
	Acs_Evt_t Event;
	Acs_UniFailure_Data Data;
}DJ_OpenDev_Failed;

typedef struct 
{
	Acs_Evt_t Event;
	Acs_MediaProc_Data Data;
}DJ_Voice_Play;

typedef struct
{
	Acs_Evt_t Event;
	Acs_CallControl_Data Data;
}DJ_Link_Dev;

typedef struct 
{
	Acs_Evt_t Event;
	Acs_IO_Data Data;
	BYTE Content[256];
}DJ_SEND_DATA;
typedef struct
{
	Acs_Evt_t Event;
	Acs_ParamProc_Data Data;
}DJ_Param;

typedef struct
{
	BYTE TYPE; //��Ϣ����
	BYTE len;//��Ϣ����
	BYTE Index;//��Ϣ���к�
	BYTE FSK;//ҵ���ʽ��־��
	//--------------------------------------------------------
	BYTE C5_T;//��ϢԤ�������
	BYTE C5_A;//�澯���
	BYTE C5_L;//��Ϣ����
//	BYTE C5_L2;//��Ϣ����2
	BYTE C5_L3;//��Ϣ����3
	BYTE C5_C4;//�ն˵��ض���Ϣ 
				/*	0X00 Ϊ������������Ʒ�������ն��ض�����Ϣ��������������������
					0X01Ϊ������������Ʒ�������ն���������
					0X02Ϊ������������Ʒ�������ն�������������
					0X03Ϊ������������Ʒ������C4��������Ч��˵��*/			
	BYTE C5_Content[64];//ҵ���ʽ��־��
}DJ_ACCC5;//��¼��ѯ������Ϣ���޸�����Ϣ

#pragma  pack()

typedef	 void (*pEvtdefine)(DJ_U32 );


void Init();


DLLIMPORT RetCode_t  XMS_acsOpenStream(ACSHandle_t * acsHandle,
									   ServerID_t *serverID,
									   DJ_U8 u8AppID,
									   DJ_U32 u32SendQSize,
									   DJ_U32 u32RecvQSize,
									   DJ_S32 s32DebugOn,
									   PrivateData_t * privateData);

DLLIMPORT RetCode_t  XMS_acsSetESR(ACSHandle_t acsHandle,
								   EsrFunc esr,
								   DJ_U32 esrParam,
								   BOOL notifyAll);	


DLLIMPORT RetCode_t  XMS_ctsSendIOData(ACSHandle_t acsHandle,
									   DeviceID_t * deviceID,
									   DJ_U16 u16IoType,
									   DJ_U16 u16IoDataLen,
									   DJ_Void * ioData);
		

DLLIMPORT RetCode_t  XMS_ctsLinkDevice(ACSHandle_t acsHandle,
									   DeviceID_t * srcDeviceID,
									   DeviceID_t * destDeviceID,
									   PrivateData_t * privateData);


DLLIMPORT RetCode_t  XMS_ctsUnlinkDevice(ACSHandle_t acsHandle,
									 DeviceID_t * srcDeviceID,
									 DeviceID_t * destDeviceID,
									 PrivateData_t * privateData);


DLLIMPORT RetCode_t  XMS_acsGetDeviceList(ACSHandle_t acsHandle,
									  PrivateData_t * privateData);


DLLIMPORT RetCode_t XMS_ctsAlertCall(ACSHandle_t acsHandle,
									 DeviceID_t * deviceID,
									 PrivateData_t * privateData);


DLLIMPORT RetCode_t XMS_ctsAnswerCallIn(ACSHandle_t acsHandle,
										DeviceID_t * deviceID,
										PrivateData_t * privateData);


DLLIMPORT RetCode_t XMS_ctsClearCall(ACSHandle_t acsHandle, 
									 DeviceID_t * deviceID,
									 DJ_S32 s32ClearCause,
									 PrivateData_t * privateData);


DLLIMPORT RetCode_t XMS_ctsPlay(ACSHandle_t acsHandle,
							 DeviceID_t * mediaDeviceID,
							 PlayProperty_t * playProperty,
							 PrivateData_t * privateData);


DLLIMPORT RetCode_t XMS_ctsSetParam(ACSHandle_t acsHandle,
									DeviceID_t * deviceID,
									DJ_U16 u16ParamCmdType,
									DJ_U16 u16ParamDataSize,
									DJ_Void * paramData);
	
DLLIMPORT	RetCode_t  XMS_acsCloseStream(ACSHandle_t acsHandle,PrivateData_t * privateData);

DLLIMPORT	RetCode_t  XMS_ctsOpenDevice(ACSHandle_t acsHandle,DeviceID_t * deviceID,PrivateData_t * privateData);

DLLIMPORT	RetCode_t  XMS_ctsCloseDevice(ACSHandle_t acsHandle,DeviceID_t * deviceID,PrivateData_t * privateData);

DLLIMPORT	RetCode_t  XMS_ctsResetDevice(ACSHandle_t acsHandle,DeviceID_t * deviceID,PrivateData_t * privateData);
	
DLLIMPORT	RetCode_t  XMS_ctsGetDevState(ACSHandle_t acsHandle,DeviceID_t * deviceID,PrivateData_t * privateData);
			
DLLIMPORT	RetCode_t  XMS_ctsMakeCallOut(ACSHandle_t acsHandle,DeviceID_t * deviceID,CallNum_t * callingID,
										CallNum_t * calledID,PrivateData_t * privateData);	

DLLIMPORT	RetCode_t  XMS_ctsGetParam(ACSHandle_t acsHandle,DeviceID_t * deviceID,
									   DJ_U16 u16ParamCmdType,DJ_U16 u16ParamDataSize,DJ_Void * paramData);
	
DLLIMPORT	RetCode_t  XMS_ctsInitPlayIndex(ACSHandle_t acsHandle,DeviceID_t * mediaDeviceID,PrivateData_t * privateData);

DLLIMPORT	RetCode_t  XMS_ctsBuildPlayIndex(ACSHandle_t acsHandle,DeviceID_t * mediaDeviceID,
											 PlayProperty_t * playProperty,PrivateData_t * privateData);

DLLIMPORT	RetCode_t  XMS_ctsControlPlay(ACSHandle_t acsHandle,DeviceID_t * mediaDeviceID,
								ControlPlay_t * controlPlay,PrivateData_t * privateData);
	
DLLIMPORT	RetCode_t  XMS_ctsRecord(ACSHandle_t acsHandle,DeviceID_t * mediaDeviceID,
								RecordProperty_t * recordProperty,PrivateData_t * privateData);	
DLLIMPORT	RetCode_t  XMS_ctsControlRecord(ACSHandle_t acsHandle,DeviceID_t * mediaDeviceID,
								ControlRecord_t * controlRecord,PrivateData_t * privateData);
void OpenE1Device(DeviceID_t * deviceID);
void OpenInterfaceDevice(DeviceID_t * deviceID);
void OpenVoiceDevice(DeviceID_t * deviceID);
void OpenDevice(DeviceID_t * deviceID,int nStart,int nEnd);
void PlayThread(void * arg);
//
void FindDevice(DeviceID_t * deviceID,int * nDspNum,int * nDevNum);

WORD GetMsgBufPos();

void DealMsg_SendData(NetMsg* pMsg,int len);

void DealMsg_RecvData(NetMsg* pMsg,int len);

void  DealMsg_Play(NetMsg* pMsg,int len);

void DealMsg_CallIn(NetMsg* pMsg,int len);

void DealMsg_Answer(NetMsg* pMsg,int len);

void DealMsg_Alert(NetMsg* pMsg,int len);

void DealMsg_Connect(NetMsg* pMsg,int len);

void DealMsg_ClearCall(NetMsg* pMsg,int len);

BOOL GetFreeTrunk(int *pDspPos,int *pDevPos);

void openStream(void *arg);

BOOL judgeClearCall(NetMsg* pMsg);

BOOL GetIP();

#endif