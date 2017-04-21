#ifndef _PXSSM_GLOBAL_H_
#define _PXSSM_GLOBAL_H_

/*  pXSSM  �¼�����     */
/************************************************************************/
/* XVAPL<->pXSSM.EXE֮���¼�����                                        */
/************************************************************************/
#define evPXSSM_BASE							4000   //����ֵ��XVAPLͳһָ����
#define	evPXSSM_ActivityTest	      			evPXSSM_BASE +	1//����״̬����
#define	evPXSSM_ActivityTestResponse			evPXSSM_BASE +	2//����״̬����ȷ��
#define	evPXSSM_ErrorReport					    evPXSSM_BASE +	3//���󱨸�
#define	evPXSSM_InitiateDP						evPXSSM_BASE +	10/*�����Ự*/
#define	evPXSSM_ConnectToResource			    evPXSSM_BASE +	11/*��������Դ*/
#define	evPXSSM_DisconnectForwardConnection		evPXSSM_BASE +	12/*ȡ��������Դ��*/
#define	evPXSSM_ReleaseCall					    evPXSSM_BASE +	13//�ͷŻỰ
#define	evPXSSM_PlayAnnouncement				evPXSSM_BASE +	20//����
#define	evPXSSM_PromptCollectInformation		evPXSSM_BASE +	21//���������ý���DTMF��ʽ
#define evPXSSM_PromptCollectInformationAndSetFormat evPXSSM_BASE +	22//���ý���DTMF��ָ��
#define	evPXSSM_CollectedInformation			evPXSSM_BASE +	23//�ռ���DTMF�û���Ϣ
#define	evPXSSM_ResourceReport					evPXSSM_BASE +	24//��Դ����
#define evPXSSM_PromptCollectFSK        		evPXSSM_BASE +	25//���������ý���FSK
#define	evPXSSM_SendFSK						    evPXSSM_BASE +	30//����FSK
#define	evPXSSM_SendFSKCollectInformation		evPXSSM_BASE +	31//����FSK�����ý���DTMF
#define	evPXSSM_SendFSKCollectFSK			    evPXSSM_BASE +	32//����FSK�����ý���FSK
#define	evPXSSM_CollectedFSKInformation			evPXSSM_BASE +	33//�ռ���FSK��Ϣ
#define	evPXSSM_InitiateRecord					evPXSSM_BASE +	40//Ҫ��¼��
#define	evPXSSM_StopRecord					    evPXSSM_BASE +	41//ֹͣ¼��
#define	evPXSSM_RequestReportBCSMEvent			evPXSSM_BASE +	50//����BCSM�¼�
#define	evPXSSM_EventReportBCSM				    evPXSSM_BASE +	51//BCSM�¼�ȷ��
#define	evPXSSM_Connect						    evPXSSM_BASE +	52//�������
#define	evPXSSM_InitiateCallAttempt				evPXSSM_BASE +	53//�������
#define	evPXSSM_Continue						evPXSSM_BASE +	54
#define	evPXSSM_TTSConvert					    evPXSSM_BASE +	60//TTS�ļ���ʽת��
#define	evPXSSM_TTSPlay						    evPXSSM_BASE +	61//TTS�ļ�����
/************************************************************************/
/* pXSSM.EXE��pXSSM.DLL֮���¼�����                                     */
/************************************************************************/
#define evXSSM_Base				5000
#define evtXSSM_LinkStatus		evXSSM_Base+1//��Ӳ���豸��������
#define evtXSSM_FailStatus		evXSSM_Base+2//��Ӳ���豸�����쳣
#define evtXSSM_CallOut			evXSSM_Base+3//����
#define evtXSSM_CallIn			evXSSM_Base+4//����
#define evtXSSM_AlertCall		evXSSM_Base+5
#define evtXSSM_AnswerCall		evXSSM_Base+6
#define evtXSSM_LinkDevice		evXSSM_Base+7//�����豸����
#define evtXSSM_UnLinkDevice	evXSSM_Base+8//�������豸����
#define evtXSSM_ClearCall		evXSSM_Base+9//�һ�
#define evtXSSM_JoinConf		evXSSM_Base+10//�������
#define evtXSSM_LeaveFromConf	evXSSM_Base+11//�뿪����
#define evtXSSM_ClearConf		evXSSM_Base+12//����������
#define evtXSSM_Play			evXSSM_Base+13//�������
#define evtXSSM_ControlPlay		evXSSM_Base+14//���Ʒ����������
#define evtXSSM_Record			evXSSM_Base+15//����¼���������
#define evtXSSM_ControlRecord	evXSSM_Base+16//����¼���������
#define evtXSSM_SendFax			evXSSM_Base+17//���ʹ���
#define evtXSSM_RecvFax			evXSSM_Base+18//���մ���
#define evtXSSM_SendIoData		evXSSM_Base+19//��������
#define evtXSSM_RecvIoData		evXSSM_Base+20//��������


//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* tACC��xvapl֮���¼�����                                              */
/************************************************************************/
#define evTcp_Base								6000
#define evTcp_InitTCP							evTcp_Base+1///*TCP�����Ự*/
#define evTcp_ReceiveData						evTcp_Base+2//���յ�������
#define evTcp_SendDataAndReceiveData			evTcp_Base+3//�������ݲ���������




//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* checkcase��xvapl֮���¼�����                                              */
/************************************************************************/
#define evTest_Base          6500
#define evTest_BeginControl  evTest_Base+1//��������
#define evTest_BeginControlAck  evTest_Base+2 //��������ȷ��

#define evTest_GetTestSenssion  evTest_Base +3//��ȡ��ǰ���ԵĻỰ��
#define evTest_GetTestSenssionAck  evTest_Base +4//���ص�ǰ���ԵĻỰ��


#define evTest_EndControl  evTest_Base+5//���Խ���
#define evTest_ReadValue  evTest_Base+6 //������ֵ������ֵ
#define evTest_ReadValueAck  evTest_Base+7 //������ֵ������ֵ
#define evTest_SetValue   evTest_Base+8//���ó���������ֵ
#define evTest_SetValueAck   evTest_Base+9//���ó���������ֵȷ��
#define evTest_CurrentSibNo   evTest_Base+10//��ȡ��ǰִ�е�SIB�������
#define evTest_CurrentSibNoAck  evTest_Base+11 //��ȡ��ǰִ�е�SIB���ȷ��
#define evTest_SibControl  evTest_Base+12 //����SIB����


#pragma  pack(1)
typedef  char    PSTNNumber[32];       

typedef char       DateTime[10];
/*    ���󱨸�     */
typedef enum
{
    Cancelled=1,              // ��ȡ��
    CancelFailed,           //ȡ��ʧ�� 
    ImproperCallerResponse, //������û���Ӧ
    TimeOut,                // ��ʱ
    MissingSession,         // �Ҳ�����Ӧ�ĻỰ
	MissingParameter,        //��ʧ���� 
	ParameterOutOfRange,   //����������Χ
    SystemFailure,          //ϵͳ����,
    TaskRefused,           //���񱻾ܾ�������״̬����ȷ,
    UnavailableResource,    //��Դ������,
    UnexpectedParameter,    // ��������ȷ,
	UnknownResource,       // ����֪����Դ
    OtherError               // ��������
}  emPXSSM_ErrorType;

typedef struct  
{	
		DWORD               sid;           //  �Ự��
		enum emPXSSM_ErrorType   error;         //  �������
		BYTE                level;         //  ���󼶱�
		DWORD               rev;            //  ��������
} stPXSSM_ErrorReport;

typedef struct
{
		DWORD  sid		           ;  //  �Ự��
		WORD   serviceKey        ;  //ҵ���
		CHAR   IP[16];//IP��ַ
        CHAR   MAC[20];        //Mac��ַ
} stTCP_InitTcp;//TCP����
typedef struct
{   
	WORD      timeOut;    //�ȴ�ʱ��
    WORD      datalen;      //����������ݳ���
}  stTCP_RecvData_Ask;//TCP�����������

typedef struct
{   
    DWORD    sid;   //�Ự��
    WORD     datalen;      //���ݳ���
 	BYTE      data[CHAR_NUMBER-6];   //��������
}  stTCP_RecvData;//TCP��������
typedef struct
{   
    DWORD    sid;   //�Ự��
	WORD     timeOut;    //�ȴ�ʱ��
    WORD     senddatalen;      //send���ݳ���
	WORD     recvdatalen;   //�������ݳ���
 	BYTE     data[CHAR_NUMBER-10];   //��������
	
}  stTCP_SendDataRecvData;//TCP��������
typedef struct
{
		WORD   id		           ;  //  ���
		WORD   servicetype        ;  //ҵ������
		WORD   serviceKey        ;  //ҵ���
        BYTE   content[1000];        //����
} stODI_InitODI;//ODI����
typedef struct
{
		WORD   id		           ;  //  ���
		BYTE   state        ;  //״̬
		WORD   sid        ;  //�Ự��
} stODI_InitODI_Ack;//ODI����ȷ��


typedef struct
{
		DWORD  sid		           ;  //  �Ự��
		WORD   serviceKey        ;  //ҵ���
		PSTNNumber   dialedNumber  ;  //��������,
		PSTNNumber   calledNumber  ;  //ԭʼ�����û����룬���ں���ת�Ƶ�ҵ���¼��ʼ�ĺ��룬������dialedNumber��ͬ
		PSTNNumber   callingNumber ;  //�����û�����
}   stPXSSM_InitiateDP;//PSTN����

typedef struct
{
		DWORD   sid        ; //�Ự��
		DWORD   resouceID  ; //��Դ�ţ���ʱ����
}   stPXSSM_ConnectToResource;

typedef enum 
{	
  BCSM_RouteFail  = 0  ,  //  ·��ѡ�����
  BCSM_ORelease   = 1  ,  //  O-����
  BCSM_OAbandon   = 2  ,  //  O-����
  BCSM_Busy       = 3  ,  //  ����æ
  BCSM_NoResponse = 4  ,  //  ��Ӧ��
  BCSM_Answer     = 5  ,  //  Ӧ��
  BCSM_TRelease   = 6  ,  //  T-����
  BCSM_TAbandon   = 7     //  T-����
} emPXSSM_BCSM;

typedef enum
{
  RouteFail  = 0  ,  //  ·�ɴ���
  ORelease   = 1  ,  //  O-����
  OAbandon   = 2  ,  //  O-����
  Busy       = 3  ,  //  ����æ
  NoResponse = 4  ,  //  ��Ӧ��
  Answer     = 5  ,  //  Ӧ��
  TRelease   = 6  ,  //  T-����
//	enum emPXSSM_BCSM pbasic;
 TAbandon   = 7  ,  //  T-����
  FatalFail  = 10 ,  //  ��������
  DeviceLoss = 11 ,  //  �豸���Ӷ�ʧ���豸������
  LogicalEnd = 12 ,  //  �û�Ҫ���������߼�����
  UnknowReason =20   //  δ֪
}emPXSSM_ReasonType;


typedef struct
{
	DWORD                     sid	    ;  //�Ự��
	enum emPXSSM_ReasonType        reason  ;  //�ͷ�ԭ��
} stPXSSM_ReleaseCall;
union choiceValue
{
     DWORD       value;      //��ֵ
     char        number[16]; //���ִ�
     int         price;      //����λΪ�ֵ�����������Ϊ��
     char        date[11];   //���ڣ���ʽΪYYYY-MM-DD
     char        time[9];    //�¼�����ʽΪHH:MM:SS
};
typedef struct
{   
	union choiceValue value;
    BYTE    choiceType;          //2����ֵ 3�����ִ�  4�����5������  6��ʱ��
}stPXSSM_ToneParameter;   // ��������

typedef struct
{  
    DWORD                     toneID; //������Ŀ��
    
    BYTE                     number;  //��������
	stPXSSM_ToneParameter    parameter[4];
}stPXSSM_VariableTone;   // �������Ŀɱ���
union choiceVariable
{
    DWORD                 toneID;  //������Ŀ��
    stPXSSM_VariableTone  variableTone;  
} ;
typedef struct
{   
    union choiceVariable    vchoiceVariable;//��������
    BYTE                     variable;   // 0 �̶�����1���ɱ���
}stPXSSM_Tone;    // ����


typedef struct
{   
    DWORD            sid;   //�Ự��
    stPXSSM_Tone     tone;  //����
    BYTE             repeatTime; //�ظ�����
    WORD             duration;  //������ʱ�䣬0��ʾ������
    WORD             interval; // �ظ�֮��ļ��ʱ�䣬����repeatTime>0��Ч
    BOOL             canInterupt  ; //�û���������Ƿ���Դ��
} stPXSSM_PlayAnnouncement;

typedef struct
{ 
    BYTE    minDigits;   /* ��С�ռ��������� */
    BYTE    maxDigits;   /* ����Ѽ�������  */
    BYTE    endDigit;    /* �����ַ� */
    BYTE    cancelDigits;       /* ȡ���ַ� */
    BYTE    startDigit;         /* ��ʼ�ַ� */
    BYTE    firstDigitTimeOut;  /* ��λ��ʱ  */
    BYTE    interDigitTimeOut;  /* λ�䳬ʱ  */
	stPXSSM_PlayAnnouncement  playTone;  // ���ŵ�����
}   stPXSSM_PromptCollectInformation;
typedef struct
{ 
    DWORD            sid;   //�Ự��
    BYTE    minDigits;   /* ��С�ռ��������� */
    BYTE    maxDigits;   /* ����Ѽ�������  */
    BYTE    endDigit;    /* �����ַ� */
    BYTE    cancelDigits;       /* ȡ���ַ� */
    BYTE    startDigit;         /* ��ʼ�ַ� */
    BYTE    firstDigitTimeOut;  /* ��λ��ʱ  */
    BYTE    interDigitTimeOut;  /* λ�䳬ʱ  */
}   stPXSSM_PromptCollectInformationAndSetFormat;//���ý���DTMF��ʽ

typedef struct
{ 
    
    BOOL Fskenable;//�Ƿ����FSK
	stPXSSM_PlayAnnouncement  playTone;  // ���ŵ�����
}stPXSSM_PromptCollectFSK;//������Ͻ���FSK

typedef struct
{ 
    DWORD            sid;        //�Ự��
    char    collectedDigits[64]; //�ռ��������ִ� */
}   stPXSSM_CollectedInformation;

typedef struct
{
    WORD     length;        //��Ϣ����
	BYTE     message[512];  //�����Ƶ�FSK����
}   stFSK;

typedef struct
{   
    DWORD    sid;   //�Ự��
    WORD     duration;      //������ʱ�䣬0��ʾ������
 	stFSK    fsk;   //fsk��Ϣ
}  stPXSSM_SendFSK;

typedef struct
{ 
    
    BYTE    minDigits;   /* ��С�ռ��������� */
    BYTE    maxDigits;   /* ����Ѽ�������  */
    BYTE    endDigit;    /* �����ַ� */
    BYTE    cancelDigits;       /* ȡ���ַ� */
    BYTE    startDigit;         /* ��ʼ�ַ� */
    BYTE    firstDigitTimeOut;  /* ��λ��ʱ  */
    BYTE    interDigitTimeOut;  /* λ�䳬ʱ  */
	stPXSSM_SendFSK  playFSK;  // ���ŵ�FSK
}   stPXSSM_SendFSKCollectInformation;


typedef struct
{ 
    BYTE    timeOut;  /* ��ʱ  */
	stPXSSM_SendFSK  playFSK;  // ���ŵ�FSK
    
}   stPXSSM_SendFSKCollectFSK;

typedef struct
{   
    DWORD    sid;   //�Ự��
    stFSK    fsk;   //�յ���fsk��Ϣ
}  stPXSSM_CollectedFSKInformation;


/************************************************************************/
/*                 ��Դ����                                             */
/************************************************************************/
typedef enum
{
  R_PlayAnnouncement  = 1  ,  // �������
  R_SendFsk   = 2  ,  //  ��������
  R_InitiateRecord   = 3  ,  //  ¼�����߿��������
  R_TTSPlay       = 4,    //  TTSת�����
  R_TcpSend       =5  //TCP��������
}emPXSSM_ResourceReportReason;
typedef struct
{   
    DWORD    sid;   //�Ự��
    enum emPXSSM_ResourceReportReason     reason;   //��Դ��������
}  stPXSSM_ResourceReport;//��Դ����


typedef struct
{   
    DWORD   sid;           //�Ự��
    DWORD   fileIndex;     //¼���ļ����
    WORD    duration;      //�¼��ʱ�䣬0��ʾ������
    BOOL    canInterupt  ; //�û���������Ƿ���Դ��
    BOOL    replaceExistedFile ; //�Ƿ񸲸Ǵ��ڵ��ļ�
}  stPXSSM_InitiateRecord;


/*   BCSM�¼�����                     */

typedef struct
{   
    DWORD         sid;           //�Ự��
    WORD          eventNumber;   //ע����¼�������С�ڵ���8
    enum emPXSSM_BCSM  events[8];     //ע����¼�
}  stPXSSM_RequestReportBCSMEvent;

typedef struct
{   
    DWORD           sid;      //�Ự��
    enum emPXSSM_BCSM    event;    //BCSM�¼� 
}  stPXSSM_EventReportBCSM;


typedef struct
{
	DWORD           sid;      //�Ự��
	PSTNNumber   routeNumber   ;  //·������
	PSTNNumber   callingNumber ;  //�����û�����
	BOOL         needNullCDR; /*�Ƿ��ϱ�0����*/
	WORD         timeOut;//��ʱ�ȴ�ʱ��
}stPXSSM_Connect,stPXSSM_InitiateCallAttempt;

//typedef  struct
//{
//	stPXSSM_Connect  
//} stPXSSM_InitiateCallAttempt;

typedef struct
{
	DWORD		 sid;  //�Ự��
	PSTNNumber   callingNumber;  //�����û�����
	PSTNNumber   calledNumber;  //���к���
	DateTime     connectTime;      //��ʼ����ʱ��
	DateTime     answerTime;       //Ӧ��ʱ��
	DWORD        timeOut;         //����ʱ��
}stPXSSM_CDRReport;

typedef struct
{
	DWORD   sid		 ;  //�Ự��
	char    text[256];  //�ı�
    DWORD   fileIndex;     //¼���ļ����
    BOOL    replaceExistedFile; //�Ƿ񸲸Ǵ��ڵ��ļ�
}   stPXSSM_TTSConvert;

typedef struct
{   
    DWORD            sid;        //�Ự��
    char             text[256];  //����
    BYTE             repeatTime; //�ظ�����
    WORD             duration;   //������ʱ�䣬0��ʾ������
    WORD             interval;   // �ظ�֮��ļ��ʱ�䣬����repeatTime>0��Ч
    BOOL             canInterupt  ; //�û���������Ƿ���Դ��
}  stPXSSM_TTSPlayAnnouncement;

typedef struct 
{
	DWORD sid;//�Ự��
	DWORD DeviceNumber;//�������
	char CallingNum[32];
	char CalledNum[32];
	int event;//�¼�
	int datalen;//���ݳ���
	BYTE ioData[512];//��������
//	void *ioData; //��������
}ACS_Hdl_EVENT;
union WATTR{
		WORD attr;
		struct
		{
			WORD debug : 1;
			WORD channel : 15;
		};
		
};
typedef struct 
{
	char ServerIP[32];
	WORD port;
	char passwd[32];
	char username[32];
	int UintId;
	union WATTR attrd;
}ACS_ATTR;//�ӿڻ�������
typedef struct 
{
	int sysTimeout;
	int hdlTimeout;
}XSSM_TIME;//��ʱĬ������

/************************************************************************/
/* ����ת����ʽ����                                                     */
/************************************************************************/
enum PLAY_FILE_TYPE{
	PLAY_FILE_TONE_ELMENT,//������
	PLAY_FILE_FILE,//�ļ�������·����
	PLAY_FILE_DIGIT,//����
	PLAY_FILE_CHAR,//��
	PLAY_FILE_CURRENCY,//����
	PLAY_FILE_DATE,//����
	PLAY_FILE_TIME,//ʱ��
	 
};//�������ݸ�ʽ
typedef struct 
{
	int nKey;//���,�Ự��
	BYTE nNo;//������Ŀ��
	
}PVOICE_HEAD;//��������ͷ
typedef struct{
	enum PLAY_FILE_FILE pType;//������������
	BYTE language;//����
	char content[32];//��������
}PVOICE_CONTENT;//��������

typedef struct
{
	PVOICE_HEAD pVoiceHead;//��������ͷ
	PVOICE_CONTENT *pVoice_Content;//��������ָ��
}PVOICE;//�����ļ�



enum CALL_OUT_TYPE{
CALLOUT_SUCCESS=1,//����ɹ�
CALLOUT_FAILED,//���ʧ��
FIND_VOICE_FAILED,//�Ҳ������е������м�
FIND_E_FAILED,//�Ҳ������е������м�
};//������ؽ��
typedef struct{
	WORD sid;
	enum CALL_OUT_TYPE pType;
}stPXSSM_CallOutInfor;//�����Ϣȷ��


#pragma  pack()

#endif