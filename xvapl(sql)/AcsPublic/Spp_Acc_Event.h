/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2007-12-13   15:33
�ļ�����: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\AcsPublic\Spp_Acc_Event.h
�ļ�·��: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\AcsPublic
file base:Spp_Acc_Event
file ext: h
author:	  ������

purpose:	ACC�ṹ����
*********************************************************************/
#ifndef _XMS_DEMO_EVENT_H_
#define _XMS_DEMO_EVENT_H_
#include <windows.h>
#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsTUPDef.h"
#include "DJAcsISUPDef.h"
//#include "DJTest.h"
#define MAX_DSP_MODULE_NUMBER_OF_XMS 256


enum TONE_ELEMENT{
	TONE_ZERO,//0
	TONE_ONE,//1
	TONE_TWO,//2
	TONE_THREE,//3
	TONE_FOUR,//4
	TONE_FIVE,//5
	TONE_SIX,//6
	TONE_SEVEN,//7
	TONE_EIGHT,//8
	TONE_NINE,//9
	TONE_TEN,//10
	TONE_HUNDRED,//��
	TONE_THOUSAND,//ǧ
	TONE_TENTHOUSAND,//��
	TONE_HUNFREDMILLION,//��
	TONE_K,//k
	TONE_M,//M
	TONE_G,//G
	TONE_DOT,//��
	TONE_NEGATIVE,//��
	TONE_YUAN,//Ԫ
	TONE_JIAO,//��
	TONE_FEN,//��
	TONE_YEAR,//��
	TONE_MONTH,//��
	TONE_DAY,//��
	TONE_HOUR,//ʱ
	TONE_MINUTE,//��
	TONE_SECOND,//��
	TONE_YAO,//��
};

enum TRUNK_STATE{
	TRK_WAITOPEN,//�ȴ���
	TRK_FREE,	//����
	TRK_USED,//ռ��
	TRK_CALLIN, //�Ự����
	TRK_CALLOUT,//����
// 	TRK_WELCOME,
//	TRK_ACCOUNT,
//	TRK_PASSWORD,
//	TRK_SELECT,		
//	TRK_PLAYRESULT,
//	TRK_RECORDFILE,
//	TRK_PLAYRECORD,	
	TRK_HANGON,//�һ�

	// new add for XMS
	TRK_WAIT_ANSWERCALL,//
	TRK_WAIT_LINKOK,
	TRK_EXDATA,//���ݽ���
	TRK_BUSY_SIGNAL,//æ��
	TRK_WAIT_REMOVE,
};
enum USER_STATE {
	USER_WAITOPEN,
	USER_FREE,
	USER_OFFHOOK,
	USER_CALLOUT,
	USER_LINK,
	USER_WAITHANGUP,

	USER_WAIT_REMOVE,
};
typedef struct
{
	// ----------------
//	DeviceID_t	deviceID;
//	int			iSeqID;
//	int			iModSeqID;
//	int			iLineState;
//
//	DeviceID_t	VocDevID;
//	DJ_U8		u8PlayTag;
//	enum TRUNK_STATE	State;
//
//	int		DtmfCount;
//	char	DtmfBuf[32];
//
//	char CallerCode[20];
//	char CalleeCode[20];
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;
	DJ_U8		u8PlayTag;
	DeviceID_t	LinkDevID;

	// -----------------
	enum TRUNK_STATE	State;

	// -----------------
	enum USER_STATE	UserState;
	int			iUserSeqID;

	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerCode[20];
	char CalleeCode[20];

} TRUNK_STRUCT;//�м�ͨ��

enum VOICE_STATE {
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED,
	VOC_WAIT_REMOVE,
} ;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;

	DeviceID_t	UsedDevID;

	// ----------------
	enum VOICE_STATE	State;


} VOICE_STRUCT;//����ͨ��

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	BOOL		bOpenFlag;		// OpenDevice�ɹ��ı�־

	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;//PCMͨ��

enum	REMOVE_STATE
{
	DSP_REMOVE_STATE_NONE	=	0,		// û��ɾ��DSPӲ��
	DSP_REMOVE_STATE_START	=	1,		// ׼��ɾ��DSPӲ�����ȴ������豸��Դ�ͷ�
	DSP_REMOVE_STATE_READY	=	2,		// ������Դ�Ѿ��ͷţ�����ɾ��DSPӲ����
};
// --------------------------------------------------------------------------------
// ����ṹ������DSP�����ṩ���豸��Դ
typedef	struct
{
	BYTE	lFlag;				/*��DSP�Ƿ����, 0�������ڣ�1������*/

	DeviceID_t	deviceID;		/*��DSPģ���deviceID*/
	int			iSeqID;			/*��DSPģ���˳���*/
	BOOL		bOpenFlag;		/*OpenDevice�ɹ��ı�־*/
	BOOL		bErrFlag;		/*������CloseDevice�¼��ı�־*/
	enum REMOVE_STATE	RemoveState;	/*ֹͣDSPӲ���ı�־*/

	WORD	lVocNum;			/*��DSP�ϵ�ACS_DEVMAIN_VOICE����*/
	WORD	lVocOpened;			/*��DSP��OpenDevice�ɹ���VOICE����*/
	WORD	lVocFreeNum;		/*��DSP�Ͽ��õ�VOICE����*/
	VOICE_STRUCT	*pVoice;	/*������Ҫ��������Ӧ�Ľṹ*/

	WORD	lPcmNum;			/*��DSP�ϵ�ACS_DEVMAIN_DIGITAL_PORT����*/
	WORD	lPcmOpened;			/*��DSP��OpenDevice�ɹ���Pcm����*/
	PCM_STRUCT		*pPcm;		/*������Ҫ��������Ӧ�Ľṹ*/

	WORD	lTrunkNum;			/*��DSP�ϵ�ACS_DEVMAIN_INTERFACE_CH����*/
	WORD	lTrunkOpened;		/*��DSP��OpenDevice�ɹ���Trunk����*/
	TRUNK_STRUCT	*pTrunk;			/*������Ҫ��������Ӧ�Ľṹ*/

} TYPE_XMS_DSP_DEVICE_RES_DEMO;

// ����ṹ����ͨ���ţ����Է���ز��ҵ�ģ���ͨ��
//           ��Ҫ���Ĵ˽ṹ��Ҳ��Ҫ��״̬���߱�������˽ṹ��
typedef struct
{
    ModuleID_t      m_s8ModuleID;    /*device module type*/
    ChannelID_t     m_s16ChannelID;  /*device chan id*/
} TYPE_CHANNEL_MAP_TABLE;
//��¼���ڵ�DSP�ṹ,���ڼ���
typedef struct{
	BYTE g_iTotalModule;//DSP����
	BYTE pMapTable[MAX_DSP_MODULE_NUMBER_OF_XMS];//ÿ��DSP����Ϣ
} TYPE_XMS_DSP_EXIST;
typedef struct  
{
	WORD total;//����
	TYPE_CHANNEL_MAP_TABLE pTable[8*MAX_DSP_MODULE_NUMBER_OF_XMS];//ÿ��ͨ��������
}TYPE_CHANNEL_EXIST;



#define		M_OneVoice(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVoice[(DevID).m_s16ChannelID]
#define		M_OnePcm(DevID)			AllDeviceRes[(DevID).m_s8ModuleID].pPcm[(DevID).m_s16ChannelID]
#define		M_OneTrunk(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pTrunk[(DevID).m_s16ChannelID]
#endif