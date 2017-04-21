#ifdef     XVAPL_ODI_123456789
#define    XVAPL_ODI_123456789 

#else

#include <windows.h>
#include <stdio.h>
#include "comm.h"
#include "Commdll.h"
#include "define.h"
//#include "WatchDefine.h"
#include "yftrace.h"
//#include "public_define.h"
#include "init_viriable.h"
#include "event.h"
//#include "log.h"

#define             szWindowClass     "dongyongzhi"  
#define             TIMER_EVENT          1                             
#define             SET_TIMER_LEN      10*1000             
#define             szODIMESSAGE       0x1234    
      
#pragma  pack(1)

typedef struct                         
{
   byte  type;
   char  HostDate[9];
   char  HostTime[7];
   char  Deptno[5];
   char  Termno[13];
   char  Operno[5];
   char  Asn[21];
   char  Rndumber2[9];
   char  CardTradeNo[5];
   DWORD CardDolDbal;
   DWORD TrdeMoney;
   char  TradeType[3];
   char  KeyverSion[3];
   char  ArithIndex[3];
   char  Mac1[9];
   char  TradeId[17];
   char  BankCardNo[20];
   char  CardNo[17];
   char  Cardtype[3];
   char  rand1[17];
   char  rand2[17];

}Att_Fmac;

typedef struct                         
{
   BYTE err;
   char info[256];
   char mac[9]; 

}OUtMAC;

typedef struct               
{
	BYTE     label; 
	union    CONTENT           
	{   
		BYTE  value;
		char  buf[500];     
	}cmsg;
	
}ODIMSG_ACK;

typedef struct                         
{
   BYTE label;
   BYTE buf[CHAR_NUMBER-1];

}ODIMSG;

typedef struct           //ģ�����ṹ
{
  char  userNum[32];     // �û�����
  char  acessNum[32];    // �������
  char  reversed[64];    // ����������
  WORD  bkey;            // �������Ϣ��ҵ��� 

}strODIACCESS;

#pragma  pack()

//Att_Fmac  szODI[MESSAGE_MAX];         //����һ�������Ϣת��������.  
int       id_mac1,id_supp,id_trade;   //�ֽ��ֵ�� �����߳�ID

    
WORD     MyRegisterClass(HINSTANCE hInstance);
BOOL     CreateWin(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL     ReceiveMessageFromXVAPL(HWND hWnd,WORD offset,DWORD sid);
BOOL     SendMessageToXVAPL(WORD sid, MESSAGE msg,int ack_len,UINT mid,WORD evt);
void     Monitor(int event, LPARAM lParam,HWND hwnd );
BOOL     LinkExternalSendACKTOXVAPL(HWND hWnd, DWORD sid, MESSAGE*pmsg,int  omsg_len);
int      CardCommitex(char*buf,Att_Fmac mac,OUtMAC *OutMac);
int      CardVerify(char*buf,int* AddMoney,OUtMAC *OutMac);
int      SZ_TRADELOAD(Att_Fmac mac,OUtMAC *OutMac);
int      SZ_SupplyCommit (Att_Fmac mac,OUtMAC *OutMac);
int      AttestattionMac(Att_Fmac mac,OUtMAC* OutMac);
BOOL     TransformMessage(ODIMSG *pmsg ,Att_Fmac* mac);
void     WritelogFile(BYTE type,WORD sid,WORD line,char* format,...);

unsigned int __stdcall  AttMac1(void);              // MAC1��֤�߳�
unsigned int __stdcall  SupplyCommit(void);         // ��ֵ�ӿ��ύ�߳�
unsigned int __stdcall  TradeLoad(void);            // д�������߳� 


#endif XVAPL_ODI_123456789