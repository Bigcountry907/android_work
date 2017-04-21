#ifndef _COMM_H_
#define _COMM_H_
#include "comm.h"
//#include "init_global.h"

/***********************************comm*******************************/
int node_numer;//�ڵ����
int iHaveNodeNumber;//�Ѿ�ע��Ľڵ����


/************************************************************************
�������ܣ� ���һ����Ϣ				  							
���������� MESSAGE *pMsg	//��Ϣ����							
����ֵ  �� �ɹ�������Ϣ��ƫ����,���򷵻�-1					
ע����� ��													
************************************************************************/
int Add_Message(void *pMsg,int len);
BOOL Clean_Message(WORD wNumber);
BOOL UpdateMessage(WORD wNumber,const MESSAGE *pMsg);

BOOL Update_Message_COM(MESSAGE *pMsg,WORD wNumber,BOOL bDelete);

BOOL SendThreadMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD wNumber);
BOOL SendForkMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD wNumber);
BOOL SendSocketMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD sid);
//BOOL ReadRebuffer(LPDWORD wNumber);
BOOL ReadRebuffer();
BOOL WriteRebuffer(WORD wNumber);
int ReadSenssionBuffer();
BOOL WriteSenssionBuffer(WORD senssionID);

BOOL SetSenssionId(WORD offset);
void SetOffsetId(WORD offset);
int GetSenssionNumber();
int GetOffsetId();
/********************************************************************
�������ƣ�SetWindowsHandle
��������: ���ö�̬�������Ĵ��ھ��
��������: hWnd�����ھ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
ע������: ��	
*********************************************************************/

BOOL SetWindowsHandle(HWND hWnd);
/********************************************************************
�������ƣ�GetWindowsHandle
��������: �õ�����̬�������Ľ��̵Ĵ��ھ��
��������: ��
���ض���: ���ر���̬�������Ľ��̵Ĵ��ھ��
ע������: ��	
*********************************************************************/
HWND GetWindowsHandle();
BOOL Share_Merry();
void Detach_Merry();

HANDLE hPublicMapping;
HANDLE hMessageMapping;
HANDLE hNodeMapping;
HANDLE hModuleMapping;
HANDLE hReBufferMapping;
HANDLE hSenssionMapping;
HANDLE hSidMapping;
HANDLE hOffsetMapping;
/***********************************timer*******************************/
#pragma pack(1)
typedef struct tagTMCtrl
{
	TMID    tmID;
	struct tagTMCtrl  *prev,*next;
} TMCtrl;
#pragma pack()

/*  �����ڴ棬���һ�� TMCtrl�����뵽˫������β��   */
BOOL NewTimer(TMID  tmid);

/*  �������в���һ����ʱ�����ƿ�   */
TMCtrl * FindTimer(TMID tmid);
/*  ���������ҵ��ö�ʱ����ɾ����Ȼ���ͷ��ڴ�  */
BOOL DeleteTimer(TMID  tmid);
/*  ��ʱ������������ʱ�¼���Ȼ��ɾ���ö�ʱ�����ƿ�  */
void TriggerTimer(TMCtrl * pTmctrl);
unsigned __stdcall TimerThread(void * lpP);

/***********************************timer*******************************/

#endif