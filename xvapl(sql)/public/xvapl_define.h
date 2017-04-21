#ifndef _XVAPL_DEFINE_H_
#define _XVAPL_DEFINE_H_

#include <windows.h>
#include "../public/comm.h"
#include "../public/Commdll.h"
#pragma pack(1)


#define OPERATION_EQUL 0
#define OPERATION_ADD  1
#define OPERATION_SUB  2
#define OPERATION_MUL  3
#define OPERATION_DIV  4
#define OPERATION_RESIDUAL  5
#define OPERATION_AND  6
#define OPERATION_OR   7
#define OPERATION_XOR  8
#define OPERATION_NOT  9
#define OPERATION_STRCAT 10
#define OPERATION_STRLEFT 11
#define OPERATION_STRRIGHT 12
#define OPERATION_STRLTRIM 13
#define OPERATION_STRRTRIM 14
#define OPERATION_STRTRIM  15
#define OPERATION_STRLENGT 16
#define OPERATION_STRPLACE 17
#define OPERATION_STRATOI 18
#define OPERATION_STRITOA 19
#define OPERATION_NOW     20
#define OPERATION_SECONDS 21
#define OPERATION_TIME    22
#define OPERATION_ENCRYPT 23
#define OPERATION_UNENCRYPT 24
#define OPERATION_SID       25
#define OPERATION_ADDRESS   26
#define OPERATION_MEMCPY    27
#define OPERATION_TONEID    28
#define OPERATION_ERRORNO   29
#define OPERATION_CRUURENTSIB 30
#define OPERATION_MEMST       31
#define OPERATION_SETDESBUFFER 32
#define OPERATION_SETSOURCEBUFFER 33
#define OPERATION_BUFFERMEMCPY 34
#define OPERATION_RAND 35
#define OPERATION_BCDToStr 36
#define OPERATION_HIBYTE 37
#define OPERATION_LOBYTE 38
#define OPERATION_XORVALUE 39
#define OPERATION_SUMVALUE 40
#define OPERATION_CHARTOBYTE 41
#define OPERATION_CHARTOLOWER 42
#define OPERATION_CHARTOUPPER 43
#define OPERATION_MAKEWORD  44
#define OPERATION_SETSERCERTKEY 45
#define OPERATION_MACCALCULATE 46
#define OPERATION_STRTOBCD 47
#define OPERATION_BCDTOHEX 48
#define OPERATION_BCDTODECVALUE 49
#define OPERATION_GETSERVICETYPE 51
#define OPERATION_HEXTOSTR 50
//#define OPERATION_REPLACEMEM 50

#define OPERATION_STRTOWORD 52
#define OPERATION_CRC 53
#define OPERATION_ESCAPES 54
#define OPERATION_AUTHR 55
#define OPERATION_QUERYPOS 56
#define OPERATION_ASCHEXII 57
#define OPERATION_READACCID 58

#define TIMER_SQL     100
#define TIMER_DTMF    200
#define TIMER_FSK     50
#define TIMER_VOICE   200
#define TIMER_ODI     300
#define TIMER_TCP     6000
//#define TIMER_TCP     100


typedef enum
{
    Var_BYTE,           //�ֽ���
	Var_BOOL,          //BOOL ��
	Var_WORD,           //����
	Var_DWORD,          //˫����
	Var_CHARSTR,       //�ַ���
	Var_BYTESTR,           //�ֽڴ�        
}  emSIB_VarType;//��������



typedef enum
{
	tsibFun = 0,//����
	tsibIf,//�ж�
	tsibSwitch,	//��֧
	tsibCommpare,//�Ƚ�
	tsibDBQuery,//���ݿ��ѯ	
	tsibLog,//��־
	tsibCdr,//CDR��¼
	tsibTimer,//��ʱ
	tsibMessage,//��Ϣ
	tsibBCSM,//����BCSM	
	tsibPlayAnnouncement,//����
	tsibPromptCollectInformation,//���������ý���DTMF
	tsibPromptCollectFSK,//���������ý���FSK
	tsibPromptCollectInformationAndSetFormat,//���ý���DTMF��ʽ	
	tsibSendFSK,//����FSK
	tsibSendFSKCollectInformation,//����FSK������DTMF(����DTMF��ʽ)
	tsibSendFSKCollectFSK,//����FSK������FSK
	tsibReleaseCall,//�ͷź���
	tsibInitiateRecord,//����¼��
	tsibInitDAP,//�Ự��ʼ
	tsibCallSib,//�ص�SIB
	tsibReturnSib,//����SIB
	tsibAttempCallOutSib,//���SIB
	tsibInitOdiSib,//�Ự��ʼ(ODI)
	tsibTcpLinkSib,//TCP����SIB
	tsibInitTcpSib,//�Ự��ʼ(TCP)
	tsibTcpRecvDataSib,//TCP��������
	tsibTcpSendDataRecvDataSib,//TCP�������ݽ�������
	tsibDataToFixStructSib,//���ݹ̶��ṹ����
}SIB_TYPE;//SIB�����б�
typedef struct 
{
	char name[16];//��������
	WORD type;//��������
	WORD length;//����
	DWORD offset;//�������������ʼ��ƫ����
}XSCM_VARIABLE;//��������

typedef struct 
{
	char name[16];//��������
	WORD type;//��������,0:���ֽڣ�1��BOOL��2��˫�ֽڣ�3�����ֽڵ�������4���ַ�����5���ֽڴ�
	WORD length;//����
	DWORD offset;//����ڱ�����������ʼ��ƫ����
}XSCM_CONSTVARIABLE;//��������

BYTE *pConstVariableValue;//����ֵ

typedef struct 
{
	DWORD len;//����
	DWORD offset;//ƫ��λ��(�������ļ���)
}SIB_STATISTICS;//SIBͳ������


typedef struct 
{
	WORD wTotal;//SIB����
	SIB_STATISTICS *pSibStatistics;//ÿ��SIBͳ������
}SIB_ALL_STATISTICS;//SIB����ͳ������

typedef struct 
{
	WORD identiNo;//��ʶ�ţ����ڸ���
//	enum SIB_TYPE sibType;//SIB����
	BYTE sibType;//SIB����
	WORD nextstep;//��һ��(�±�)
	WORD Errhandle;//��������
	BYTE paramTotal;//��������
	DWORD len;//��SIB����Ĵ�ֵ���ܳ���L
}SIB_BASIC;//SIB��������

typedef enum
{
	Param_INT=0,//����
	Param_CHARSTRING,//�ַ���
	Param_BYTESTRING,//�ֽڴ�
	Param_VARIABLE,//����
	Param_CONSTVARIALE//����		
}PARAM_TYPE;//�������

typedef struct 
{
	BYTE paramNo;//�������
//	enum PARAM_TYPE paramType;//�������
	BYTE paramType;//�������
	DWORD paramValue;/*����ֵ,���������0����ʾһ������ֵ	��1��2������ƫ�Ƶ�ַ
						��3��������ţ��ڱ����������еı�ţ���0��ʼ���α��
						��4��������ţ��ڳ����������еı�ţ���0��ʼ���α��*/
}SIB_PARAM;//SIB��������

typedef struct 
{
	
	SIB_BASIC	sibBasic;//��������
	SIB_PARAM	*pParam;//��������
	BYTE		*pvalue;//��ֵ
}XSCM_SIB;//SIB����
/********************************************************************
ҵ���������
  ��������: ��
���ض���: ��
����ʱ��: 2008-6-10 17:06
��������: ������
ע������: ��	
*********************************************************************/

typedef struct 
{
	char ServiceLogo[16];//�ļ���ʶ
	BYTE languageSelect;//0:ΪWINDOWS��1ΪUNIX
	WORD serviceKey;//ҵ���
	char serviceDescrible[109];//ҵ������
	WORD nVariableTotal;//��������
	WORD nVariableLen;//��������
	WORD nConstVariableTotal;//��������
	WORD nConstVariableLen;//��������
	WORD nTotalSib;//SIB����
	DWORD nSibLen;//SIB����
	BYTE bAttr[66];//��������
	char author[32];//����/�����
	BYTE date[8];//����
	BYTE version[8];
}XSCM_SLP_BASIC;//ҵ���������

typedef struct 
{
	BYTE bServiceNumber;//ҵ����
	int  stopsibno;//ֹͣ���е�SIB���
	char reference[32];
	BOOL bTest;//�Ƿ������˵���
	BOOL bTestNow;//�Ƿ����ڵ���
	WORD senssionID;//�Ự��
	
}TEST_SERVICRSET;//��������

typedef struct 
{
	XSCM_SLP_BASIC	 sBasic;//������Ϣ
//	TEST_SERVICRSET  sServiceTest;//������Ϣ
	XSCM_CONSTVARIABLE *pConstAtte;//��������
	XSCM_VARIABLE    *pVariableAttr;//��������
	BYTE		     *pConstVaribles;//������
	XSCM_SIB		 *pSib;//SIB��
}XSCM_SLP;//ҵ�����̼�¼




#pragma  pack()


//void XVAPL_CDR(WORD senssionID,char ServiceDesc[32],char CallingNum[32],char ServiceCode[32],const char *fmt,.....);
//void XVAPL_TRACE(const char * logname, const char *cfile, const int line,int senssionID,const char *function_name,const char *fmt,...);
//void XVAPL_WARN(const char * logname,  const char *cfile, const int line,int level,const char *fmt,...);
//void XVAPL_LOG(const char * logname,  const char *cfile, const int line,const char *fmt,...);
/********************************************************************
�������ƣ�XVAPL_ServiceMAN
��������: ServiceMan����ֵҵ��ӿ�
��������: wAttr:���ԣ���λ����
15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
									д�����ļ�	ȡ������	��������	ж��ҵ��	����ҵ��	����ҵ��	ͣҵ��ֹ

���ض���: ��
*********************************************************************/
void XVAPL_ServiceMAN(WORD  wAttr);
//���ã�֪ͨSPP��ҵ�����һЩ����������ֹͣҵ�񡢼���ҵ�񡢼���ҵ��ж��ҵ���������١�ȡ�����١�д�����ļ��ȵ�,�����õ�ʱ����Щ�����λ���Զ�Ϊ"0"�����ǲ��ܶ�Ϊ"1"��



/********************************************************************
�������ƣ�XVAPL_SetConstValue
��������: ���ó���ֵ
��������: ServiceKey :ҵ�����offsetAddr��ƫ��λ�ã�len�����ݳ��ȣ�pValue:����ֵ
���ض���: ��
*********************************************************************/

void XVAPL_SetConstValue(int ServiceKey,WORD offsetAddr,BYTE len ,BYTE *pValue);

//���ã��Ծ���ҵ���ҵ�������е�һЩ����ֵ�����޸ġ�




/********************************************************************
�������ƣ�XVAPL_LINKTEST
��������: SPP����״̬����
��������: ��
���ض���: ״̬��������TRUE������������FALSE
*********************************************************************/
BOOL  XVAPL_LINKTEST();
//���ã�����SPP����״̬����״̬����������²��ܽ����������Ϣ�����ã���ServiceMAN��ʾ��SPP״̬��ϢҲͨ���˺����õ���


#endif