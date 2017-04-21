#ifndef _YFSPP_COMM_H_
#define _YFSPP_COMM_H_


#include <windows.h>
#include "define.h"
/* ��������������ж��� _YFSPP_COMM_C_  �� g_dwNode ,g_hWnd��g_dwProcessId*/
#define _YFSPP_COMM_C_
// #define _WIN32
#ifndef _YFSPP_COMM_C_
extern  DWORD g_dwNode;
#ifdef  _WIN32
extern  HWND  g_hWnd;
#else
extern  DWORD g_dwProcessId;
#endif
#endif

#define  MESSAGE_MAX         2000    /*  ��Ϣ�������                  */
#define  TIMER_MAX           10      /*  timer����                     */
#define  CHAR_NUMBER         1024    /*  һ����Ϣ���ֽڸ���            */
#define  RETSQL_INT_LEN	 	 1024	 /*  SQLִ�к󷵻ص��������ݵĳ��� */
#define  RETSQL_STR_LEN      1024	 /*  SQLִ�к󷵻ص��ַ����ĳ���   */

#pragma   pack(1)

typedef struct
{
	DWORD  node;     /* �ڵ�� */
	BOOL   isSPP;    /* �Ƿ�SPP������ */
	BOOL   isServer; /* �Ƿ�ͨ�����ӷ���� */
	BYTE   bModuleNum;/*��ģ����*/
	DWORD  module;   /*�ڵ�ͨѶģ���̺߳�*/
	BOOL   isActive; /* ����״̬�Ƿ���� */
} NID;

typedef struct
{
	DWORD  node;       /* �ڵ��   */
#ifdef _WIN32	
	HWND   hWnd;       /* �����̵������ھ�� */
#else
	DWORD  procece;    /* ���̺ţ���windowϵͳ   */
#endif
	DWORD  module;     /*  �������̣߳�Ϊ�̺߳ţ�����Ϊģ���   */
	UINT   mId;         /*ģ���*/
} TID;

typedef struct  
{
	UINT     mId;       /* ģ��� */
	TID      tId;       /* ģ���̱߳�ʶ */  
	BOOL     isSysModule; /* �Ƿ�ϵͳģ�� */
	UINT     state;      
}MID;
typedef struct
{
	TID     Source;   /* ��Ϣ����Դ */
	TID		sender;   /* ������ */
	TID		receiver;	/* ������ */
	WORD	event;    /* �¼� */
	WORD	len;      /* ��Ϣ���� */
	BOOL	sync;     /* ͬ����־�����ΪTRUE������ȴ���Ϣ���� */
	DWORD	ptrAck;   /* �������ݻ�����ָ�룬����ͬ����Ϣ��Ҫ��������ʱ���ã��첽��ʽ��ΪNULL */
	WORD	ackLen;   /* �������ݻ��������� */
	DWORD	reserve;  /* ���� */
} MSG_HEAD;

typedef struct
{
	MSG_HEAD  head;   /* ��Ϣͷ */
	BYTE     data[CHAR_NUMBER];   /* ��lenָ�����ȵ����� */
} MESSAGE;              /* ��Ϣ�������ɷ�����Ϣ���߳�������ͷ� */
                    /* ������Ϣ���߳��յ���Ϣ�������������һ���ڴ棬����Ϣ���������ݿ����������в���   */


typedef struct 
{
	WORD	event;		/* ��ʱ����ʱ���������¼� */
	//TID		recvTid;	/* ��ʱ�������� */
	UINT     mId;       /* ģ��� */
	WORD    wNum;       /*��Ϣƫ����*/
	BOOL	isAbs;		// ���Զ�ʱ����־ 
						//TRUE:Ϊһ������ʱ��,��ʱ�䵽��ʱ���� 
						//FALSE:һ�����ʱ��
	DWORD	dwTime;	/* ��ʱ������Ϊ��λ��������Զ�ʱ�������ʾʱ�� */
	DWORD	ref;		/* �ο�ֵ�����������Լ����壬����ʱԭֵ���� */
} TMID;

typedef struct 
{
	WORD offset;//���θö���λ��
	BYTE bSign[MESSAGE_MAX];//���ñ�־��0Ϊ���ã�1Ϊ������
} SID_POSITION;//��¼�Ự����Ϣƫ��λ��


/*INI�ļ��ṹ*/
#define LENGTH_KEY     32   /*�ؼ��ֳ���*/
#define LENGTH_SECTION 64   /*�ڳ���*/
#define LENGTH_VALUE   128  /*�������ݳ���*/
#define LENGTH_COMMENT 128  /*ע�ͳ���*/
#define LENGTH_BUFFER  256  /*�ַ������С*/
/*�ؼ��ʴ洢�ṹ*/
typedef struct Key
{
    char key[LENGTH_KEY],value[LENGTH_VALUE],comment[LENGTH_COMMENT];/*�ؼ���\����\ע��*/
    struct Key *next;/*��һ���*/
}KEY,K;
/*�ڴ洢�ṹ*/
typedef struct Section
{
   char section[LENGTH_SECTION],comment[LENGTH_COMMENT];/*������ע��*/
   struct Key headKey;/*��Ӧ�ڹؼ����׽��,�׽�㲻������*/
   struct Section *next;/*��һ���*/
}SECTION,SEC,S;
/*�����ļ��ṹ*/
typedef struct Ini
{
   struct Section headSection;/*���׽��,�׽�㲻������*/
}INI,I;



typedef struct 
{
	BYTE Tel_Type;//���к����жϻ��Ǳ��к����жϣ�1Ϊ���У�2Ϊ����
	BYTE Tel_Match;//1��ʾ��ƥ�䣬2��ʾ��ƥ�䣬3��ʾȫƥ�䣬4��ʾȫ���绰����
	char Telephone[32];//�绰����
	BOOL bGetSysInfor;//���ϵͳ��Ϣ
	BOOL bTrace;//�Ƿ����
}TRACE_CONDITION;//��������




#endif _YFSPP_COMM_H_