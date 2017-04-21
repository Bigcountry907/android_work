#include "pMsg.h"
extern MESSAGE g_Slave_Message[];

BOOL Read_Message(WORD senssionID,MESSAGE *pMsg)
{
	if(senssionID<MESSAGE_MAX)
	{
		memcpy( pMsg,&g_Slave_Message[senssionID],sizeof(MESSAGE));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/********************************************************************
�������ƣ�Update_Message
��������: ��Ϣ���ݸ��£�����ɾ���͸����ж�
��������: pMsg:��Ϣ����,wNumber:��Ϣƫ������
		  bDelete:TRUEΪ�����Ϣ��FALSEΪ������Ϣ
���ض���: �ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2008-7-21 1:44
��������: ������
ע������: ��	
*********************************************************************/
BOOL Update_Message(WORD senssionID,MESSAGE *pMsg,BOOL bDelete)
{
	BOOL bTrue = TRUE;
	if(MESSAGE_MAX<=senssionID)
	{
		return FALSE;
	}
	if(bDelete)//�����Ϣ
	{
		//call clean message
		memset(&g_Slave_Message[senssionID],0,sizeof(MESSAGE));
	}
	else//������Ϣ
	{
		if((pMsg->head.sender.node == 0)&&(pMsg->head.receiver.node == 0))//����TIMER����Ϣ
		{
			g_Slave_Message[senssionID].head.event=pMsg->head.event;
		}
		else
		{
			memcpy(&g_Slave_Message[senssionID],pMsg,sizeof(MESSAGE));	
		}
		
	}
	return  bTrue;
}