#ifndef _SPP_COMM_H_
#define _SPP_COMM_H_

#include "comm.h"
#include "pXSSM_global.h"

#pragma   pack(1)

typedef struct 
{
	int timerId;  //���ص�timerId
	BOOL bUsed;    //TIMER�Ƿ�����
}TIMER_COUNT;


typedef struct 
{
	BYTE bVaribleTotal;//��������
	DWORD dVaribleNo[20];//�������
}VARIBLE_SAVE;//��¼������ر����ı�ż���������


typedef struct  
{ 
	WORD sibNo[STACK_SIZE]; //�����ջ������
	int top;//��ջλ��,��ʼ��Ϊ-1
}SIB_STACK; //�����ջ�������ڼ�¼SIB��Ŷ�ջ




typedef struct 
{
	WORD servicetype;//ҵ�����ͣ�0��ʾPSTN��1��ʾTCP,odi�������10
	union INIT
	{
		BYTE init_value[1024];
		stPXSSM_InitiateDP init_pstn;
		stTCP_InitTcp init_tcp;
		stODI_InitODI init_odi;
	}sInit;

}SP_INIT;//�����������

typedef struct 
{
	BYTE           serviceId;//ҵ����
	int		       serviceKey;//ҵ������
	SP_INIT        serInit;//�����������
	BYTE           status;   /*״̬*/
	BYTE           step;     /*����*/
	WORD           lastsibNo; /*�ϴ�ִ�е�SIB*/
	WORD           sibNo;//��Ҫִ�е�SIB
	BOOL           bReissues;/*�Ƿ��Ѿ��ظ�*/    
//	char    CallingNum[32];//���к���
//	char    CalledNum[32];//���к���
	SIB_STACK      sibStack;//��ջSIB���
	TIMER_COUNT    timerevent[TIMER_MAX];//���ö�ʱ�����¼�
	WORD           oldevent;//�ϴ��¼�
	WORD           wlastevent;//�ϴη����¼�
	DWORD          toneID;//�������
	char           ref[32];//

}MSG_HEAD_SLAVE;

typedef struct 
{
	BOOL bCheckCase;//�Ƿ��Ѿ���������
	BOOL bStopRun;//�Ƿ���ͣ����
	int  stopsib;//ֹͣ���е�SIB

}TEST_INIT;//��������

typedef struct 
{
	MESSAGE           messageInfor;   //������Ϣ
	TEST_INIT         sTestinit;      //������Ϣ
	MSG_HEAD_SLAVE    slavehead;      //��Ϣ���ݼ�¼ͷ
	BYTE              accID;          //�쳣�¼���
	WORD              AcceseMID;       //������Ϣ��Դ
	VARIBLE_SAVE      sVaribleSave;   //��¼������ر����ı�ż���������
	WORD              variblelen;     //��������
	BYTE              *pVarileValue;  //����ֵ
	
}MESSAGE_XSCM_SLAVE;//��Ϣ����



/*�ڲ�����,ֻ��SPP��*/
typedef struct 
{
	HWND       g_hWnd;				/*���ھ��*/
	DWORD      g_dwNode;			/*�ڵ㣨IP��ַ��*/ 
}PUBLIC;/*��¼���ںͽڵ���Ϣ*/

typedef struct
{
	BYTE bNodeNum;/*�ڵ����*/
	BYTE bIppNum;/*������ϵͳ����*/
	char version[4];/*�汾��*/
	char pName[64];/*����*/
	union ATTR{
		WORD uwAttri;
		struct 
		{
			WORD isCdr : 1;//
			WORD isCdrToSql : 1;
			WORD res :14;
		}att;
	} wAttri;/*��������*/
	
} SPP_BASIC; /*������Ϣ*/


typedef struct
{
	DWORD ComputerIp;/*�������*/
	char pUserName[64];/*�û���*/
	char pPassWd[16];/*����*/
} SPP_DATABASE;/*���ݿ���Ϣ*/

typedef struct
{
	int id;/*���*/
	char pName[64];/*����*/
	WORD wAttri;/*����*/

}SPP_ACC;/*�����������Ϣ*/

typedef struct
{
	int id;/*���*/
	char pName[64];/*����*/
	BOOL isServer; /*��������*/
	DWORD dIp;/*IP��ַ;*/
	WORD  wPort;/*�˿ں�*/
	
}SPP_IPP;/*�������ӿ���Ϣ*/
typedef struct 
{
	DWORD ip;
	char content[256];
}SPP_CDR;




typedef struct 
{
	BYTE bType;//ҵ������
	BYTE bServiceNumber;//ҵ����
    int stopsibno;//ֹͣ���е�SIB���
	char reference [32];//����ֵ��ҵ������Ϊ0��ʾ����ֵ�ǵ绰���룬Ϊ1����IP��ַ
}TEST_BEGINCONTROL;
typedef struct 
{
	BOOL  bFind;//�����Ƿ�ɹ�������ɹ�������ĻỰ�������壬��֮��
//	DWORD senssionID;//�����õ��ԵĻỰ�� 
}TEST_BEGINCONTROLACK;


typedef struct 
{
	BYTE bType;//ҵ������
    BYTE bServiceNumber;//ҵ����
    char reference [32];//����ֵ��ҵ������Ϊ0��ʾ����ֵ�ǵ绰���룬Ϊ1����IP��ַ
}TEST_GETTESTSENSSION;//��ȡ��ǰ���ԵĻỰ��

typedef struct 
{
	BOOL  bFind;//�����Ƿ�ɹ�������ɹ�������ĻỰ�������壬��֮��
   WORD senssionID;//�Ự��
}TEST_GETTESTSENSSIONACK;//���͵�ǰ���ԵĻỰ��

typedef struct 
{
	BYTE bType;//ҵ������
    BYTE bServiceNumber;//ҵ����
    char reference [32];//����ֵ��ҵ������Ϊ0��ʾ����ֵ�ǵ绰���룬Ϊ1����IP��ַ 
}TEST_ENDCONTROL;

typedef struct 
{
	DWORD senssionID;//�����õ��ԵĻỰ�� 
    BOOL  VaribleType;//���ͣ�0��ʾ������1��ʾ����
    char    variblename[32];// �������������
}TEST_READVALUE;
typedef struct 
{
	BOOL bStatus;//��ȡ�Ƿ�ɹ�
	char    variblename[32];// �������������
    int datalen;// �����ֽڳ���
    BYTE data[CHAR_NUMBER-40];//��������

}TEST_READVALUEACK;
typedef struct 
{
	DWORD senssionID;//�����õ��ԵĻỰ�� 
    int  VaribleType;//���ͣ�0��ʾ������1��ʾ����
    char    variblename[32];// �������������
    int datalen;//���ݳ���
    BYTE data[CHAR_NUMBER-44];//��������
}TEST_SETVALUE;

typedef struct 
{
	BOOL bStatus;//�����Ƿ�ɹ�
}TEST_SETVALUEACK;
typedef struct 
{
	DWORD senssionID;//�����õ��ԵĻỰ�� 
}TEST_CURRENTSIBNO;

typedef struct 
{
	BOOL bStatus;//��ȡ�Ƿ�ɹ�
    int sibno;//��ǰҪִ��SIB���
}TEST_CURRENTSIBNOACK;
typedef struct 
{
    DWORD senssionID;//�����õ��ԵĻỰ��
	BOOL bRun;//TRUE��ʾִ�в�����ִ��ĳ��SIBʱֹͣ������FALSE��ʾ��Ҫ��sibnoִ��ǰֹͣ����
    int sibno;//��ǰҪִ��SIB���
}TEST_SIBCONTROL;

#pragma  pack()
#endif