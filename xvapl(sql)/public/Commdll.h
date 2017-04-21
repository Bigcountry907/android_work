#ifndef _COMM_DLL_H_
#define _COMM_DLL_H_
#include "../public/comm.h"
#include "../public/init_viriable.h"
#if COMM_DLL
# define DLLIMPORT __declspec (dllimport)
#else /* Not COMM_DLL */
# define DLLIMPORT __declspec (dllexport)
#endif /* Not COMM_DLL */

/********************************************************************
��������: ������Ϣ
��������: event:�¼��ţ�receiverModule:������Ϣģ���
          wNumber����Ϣƫ����,bSync:�Ƿ�ͬ��
���ض���: �ɹ�����TURE��ʧ�ܷ���FALSE
ע������: ��	
*********************************************************************/
//DLLIMPORT BOOL COM_SEND_MSG(WORD SenssionID,MESSAGE *pMsg,WORD event,UINT receiverModule,BOOL bSync);
DLLIMPORT BOOL SEND_MSG(WORD event,UINT receiverModule,void *data,WORD len,WORD ref,BOOL sync, BYTE far *ack,WORD ackLen);
DLLIMPORT BOOL SEND_SOCKET_MSG(WORD event,DWORD iThreadId,void *data,WORD len,WORD ref,BOOL sync, BYTE far *ack,WORD ackLen);

/********************************************************************
�������ƣ�Update_SEND_MSG
��������: ���²�������Ϣ
��������: wNumber����Ϣƫ����
		  event:�¼���
		  pMsg����Ϣ����
		  receiverModule:������Ϣģ���
          bSync:�Ƿ�ͬ��
		  bDelete:������Ϣ��ʽ��TRUEΪ�����Ϣ���ݣ�FALSEΪ������Ϣ����
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL Update_SEND_MSG(WORD wNumber,WORD event,MESSAGE *pMsg,UINT receiverModule,BOOL bSync,BOOL bDelete);

/********************************************************************
��������: ע��һ���߳�
��������: �߳�ģ���
���ض���: �ɹ�����TURE��ʧ�ܷ���FALSE
ע������: ע��������Ψһ�������߳̿���ͨ�������ƻ��TID	
*********************************************************************/
DLLIMPORT BOOL RegisterThread(UINT mId);


/********************************************************************
��������: �ͷ��Ѿ�ע���һ���߳�
��������: �߳�ģ���
���ض���: �ͷųɹ�����TRUE��ʧ�ܷ���FALSE
ע������: ��
*********************************************************************/
DLLIMPORT BOOL UnRegisterThread(UINT mId);


/********************************************************************
��������: ע��һ������ģ��
��������: mId������ģ��ţ�hwnd�����̵Ĵ��ھ��
���ض���: �ɹ�����TURE��ʧ�ܷ���FALSE
ע������: ע��ģ��ű���Ψһ�������߳̿���ͨ����ģ��ųƻ��TID	
*********************************************************************/
DLLIMPORT BOOL RegisterIPC(UINT mId,HWND hwnd);


/********************************************************************
��������: �ͷ�ע�����һ������ģ��
��������: mId������ģ���
���ض���: �ɹ�����TURE��ʧ�ܷ���FALSE
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL UnRegisterIPC(UINT mId);//release IPC
/********************************************************************
��������: ע������ģ��
��������: mId������ģ��ţ�nodeip���ڵ�IP
���ض���: �ɹ�����TURE��ʧ�ܷ���FALSE
ע������: ע��ģ��ű���Ψһ�������߳̿���ͨ����ģ��ųƻ��TID	
*********************************************************************/
DLLIMPORT BOOL RegisterNodeMod(UINT mId,DWORD nodeip);

/********************************************************************
��������: �ͷ�ע������ģ��
��������: mId������ģ���
���ض���: �ɹ�����TURE��ʧ�ܷ���FALSE
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL UnRegisterNodeMod(UINT mId);
/************************************************************************
�������ܣ���ȡ��Ϣ��������   										
����������WORD wNumber      //��Ϣƫ����							
����ֵ  ��MESSAGE *         //�з�����Ϣ���ݣ�����Ϣ�򷵻ؿ�ָ��	
ע�������													
************************************************************************/
//DLLIMPORT MESSAGE *Read_Message(WORD wNumber);
DLLIMPORT BOOL COM_Read_Message(WORD wNumber,MESSAGE *pMsg);



/***********************************************************************
�������ܣ�����һ���Ự��   					
������������         								
����ֵ  ���ɹ����ػỰ��>=0��ʧ�ܷ���-1							
ע�������													
***********************************************************************/
DLLIMPORT int GetSenssionID();
/***********************************************************************
�������ܣ��ͷŻỰ��   					
����������SenssionID:�Ự��        								
����ֵ  ���ɹ�����TRUE��ʧ�ܷ���FALSE							
ע�������													
***********************************************************************/
DLLIMPORT BOOL ReleaseSenssionID(WORD SenssionID);



/********************************************************************
��������: �ͷŹ����ڴ�
��������: ��
���ض���: ��
ע������: ��	
*********************************************************************/
DLLIMPORT void FreeDll();
DLLIMPORT void Comm_SetTraceValue(BOOL bValue);


/********************************************************************
��������: �¼��쳣��¼
��������: wMid���¼�����ģ�飬*function_name���������ƣ�*reason��ԭ��
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE	
ע������: ��	
*********************************************************************/
DLLIMPORT BOOL EventLog(const WORD wMid,const char *function_name,const char *reason);


/************************************************************************
�������ܣ�����ģ��ŵõ�ע��ID 									
����������mId:ģ���         										
����ֵ  ��Tid *:  //TIDָ��									
ע�������													
************************************************************************/
DLLIMPORT TID  *GetTID(UINT mId); 
DLLIMPORT BOOL GetTid(UINT mId,TID *pTid);
DLLIMPORT BOOL GetMid(UINT mId,MID *pMid);
/* ����INI�ļ� */
DLLIMPORT void loadINI(INI *ini,char *filename);
/*�ͷ�INI�ļ�*/
DLLIMPORT void freeINI(INI *ini);
/*�õ���Ҫ��ֵ*/
DLLIMPORT char * GetKeyList(SECTION *keySEC,BYTE index);
/*�������ƻ�ȡָ����*/
DLLIMPORT SECTION *getSection(INI *ini,char *name);

#endif