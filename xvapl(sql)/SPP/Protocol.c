/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2007-11-6   11:33
�ļ�����: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\SPP\Protocol.c
�ļ�·��: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\SPP
file base:Protocol
file ext: c
author:	  ������

purpose:	Э�����
*********************************************************************/
#include "../public/Protocol.h"
#include "xvapl_slp.h"
/********************************************************************
�������ƣ�GetSum
��������:��У��
��������: data:�������ݣ�len:���ݳ���
���ض���: ����ɹ�����TRUE��ʧ�ܷ���FALSE
����ʱ��: 2007-11-6 18:00
��������: ������
ע������: ��	
*********************************************************************/
BYTE GetSum(BYTE *data,int len)
{
	BYTE pdata=0;
	int i;
	for(i=0;i<len;i++)
	{
		pdata=*(data+i)+pdata;
	}
	pdata=~pdata;
	return (BYTE)(pdata+1);
}
/********************************************************************
�������ƣ�JudgSum
��������: ����У���ж�
��������: senssionID���Ự��
���ض���: �ɹ�����TRUE,ʧ�ܷ���FALSE
����ʱ��: 2008-12-1 10:37
��������: ������
ע������: ��	
*********************************************************************/

BOOL JudgSum(WORD senssionID)
{
	MESSAGE_XSCM_SLAVE pMessage;
	BOOL bFind = FALSE;
	BYTE value;
	if(senssionID >= MESSAGE_MAX)
		return FALSE;
	bFind=ReadXSCMMessage((WORD)senssionID,&pMessage);
	if(!bFind)
		return FALSE;
	value=GetSum(pMessage.messageInfor.data,pMessage.messageInfor.head.len-1);
	if(value == pMessage.messageInfor.data[pMessage.messageInfor.head.len-1])
		return TRUE;
	else
		return FALSE;
}
/********************************************************************
�������ƣ�UpdateFskData
��������: ����FSK����
��������: data:�������ݣ�datalen:���ݳ���
���ض���: �ɹ�����FSK���ݳ���,���󷵻�-1
����ʱ��: 2008-12-1 12:03
��������: ������
ע������: ��	
*********************************************************************/

int UpdateFskData(BYTE *data,int datalen,BYTE step)
{
	BYTE data1[1024];
	if(data == NULL)
		return -1;
	memset(data1,0,sizeof(data1));
	data1[0]=0x84;
	data1[1]=datalen+1;
	data1[2]=step;
	memcpy(data1+3,data,datalen);
	data1[3+datalen]=GetSum(data1,datalen+3);
	return (datalen+4);
}

