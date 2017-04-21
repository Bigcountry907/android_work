#ifndef _YFSPP_EVENT_H
#define _YFSPP_EVENT_H

/****************     ����¼�     **********************/
#define evInitial               WM_USER
#define evLog                   (WORD) (evInitial + 1)
#define evWarring               (WORD) (evInitial + 2)
#define evError1                (WORD) (evInitial + 3)
#define evError2                (WORD) (evInitial + 4)
#define evError3                (WORD) (evInitial + 5)


#define evStateReport           (WORD) (evInitial + 6)
#define evStateReportAck        (WORD) (evInitial + 7)
#define evTraceStatus           (WORD) (evInitial + 8)

#define evReset                 (WORD) (evInitial + 10)
#define evState                 (WORD) (evInitial + 11)


/****************     ��ʱ���¼�     **********************/
#define evTimer0                (WORD)(evInitial + 100)//���û�����ʱ
#define evTimer1                (WORD)(evTimer0 + 1)//���ó�ʱ
#define evTimer2                (WORD)(evTimer0 + 2)//���õ�������ʱ
#define evTimer3                (WORD)(evTimer0 + 3)//���ó�ʱ�һ�


/****************  ���ݿ�����¼�,DBAP  **********************/
#define evDataBase              (WORD)(evInitial  + 150)

#define evDBDInital				(WORD)(evDataBase + 1)/*:���ݿ��ʼ��DBD*/
#define evDBQInital				(WORD)(evDataBase + 2)/*:���ݿ��ʼ��DBQ*/

#define evDBDConnection			(WORD)(evDataBase + 3)	/*�������ݿ�����DBD*/
#define evDBQConnection			(WORD)(evDataBase + 4)	/*�������ݿ�����DBQ*/

#define evDBDConnAvailCheck		(WORD)(evDataBase + 5)	/*���ݿ�������Ч�Լ��*/
#define evDBDConnAvailCheckAck	(WORD)(evDataBase + 6)	/*���ݿ�������Ч�Լ�鷵��*/

#define evDBDAsk				(WORD)(evDataBase + 7)/*:���ݿ��ѯ����DBD*/
#define evDBQuery				(WORD)(evDataBase + 8) /*:��DBQ�������ݿ��ѯ����*/






/***************SLP*********************************************************/
#define evSLPTN                        (WORD)(evInitial + 270)
#define evDBQueryAck				   (WORD)(evSLPTN+2)/*���ݿ��ѯ�������*/

#define evDBQueryNak				   (WORD)(evSLPTN+8)/*���ݿ��ѯ����쳣����*/
#define evFSKQueryAck				   (WORD)(evSLPTN+65)/*��������ѯ�������*/
///////SLP->DBD///////////////////////////////////////////////////////////
#define evDBD						  (WORD)(evInitial + 300)            
#define evAckDBD                      (WORD)(evDBD+7) //���ݿ�����ʧ��ֱ�ӷ�����Ϣ��SLP������DBD->SLP


/////////SLP<->IPP//////////////////////////////////////////////////////////
#define evIPP                         (WORD)(evInitial+330)

///////////////SPP<->BILL///////////////////////////////////////////////////////////
#define      evBILL             (WORD)(evInitial+350)
#define      evASK_BILL         (WORD)(evBILL+1)       //BILL(����)ģ����Ӧ��Ϣ�¼���
#define      evBILL_ACK         (WORD)(evBILL+2)       //������Ϣ�¼���


///////////////BILL<->CDR///////////////////////////////////////////////////////////

#define      evCDR              (WORD)(evInitial+380)
#define      evASK_CDR          (WORD)(evCDR+1)     //CDRģ����Ӧ��Ϣ�¼���


/////////////////////LOG��־�¼�/////////////////////////////////////////////////////
#define evLogAsk                                evInitial+500//LOG��־�¼�
//////////////////////////////////////////////////////////////////////////
#define evTraceAsk                              evInitial+551//��¼TRACE�����¼�
/************************************************************************/
/*  purpose:SPP��ServiceMan�ӿ��¼�����	                                */
/************************************************************************/

#define evSerMan_Base                           evInitial+976//����ֵ��XVAPLͳһָ��
#define	evSerMan_ActivityTest	      			evSerMan_Base +	1//���Ӳ���
#define	evSerMan_ActivityTestAck   			    evSerMan_Base +	2//����ȷ��
#define	evSerMan_ErrorReport					evSerMan_Base +	3//���󱨸�
#define	evSerMan_SetStatus						evSerMan_Base +	10//����ҵ��״̬
#define	evSerMan_SetStatusAck           	    evSerMan_Base +	11//����ҵ��״̬ȷ��

#define evSerMan_SetConstValue					evSerMan_Base +	12//���ó���ֵ
#define	evSerMan_SetConstValueAck           	evSerMan_Base +	13//���ó���ֵȷ��

#define evSerMan_GetConstValue					evSerMan_Base +	14//��ȡ����ֵ
#define	evSerMan_GetConstValueAck           	evSerMan_Base +	15//��ȡ����ֵȷ��

#define evSerMan_GetConstAttr					evSerMan_Base +16//��ȡ��������
#define	evSerMan_GetConstAttrAck				evSerMan_Base +17//��ȡ��������ȷ��

#define evSerMan_GetAllConstAttr				evSerMan_Base +18//��ȡ����������������
#define	evSerMan_GetAllConstAttrAck				evSerMan_Base +19//������������ȷ��



/************************************************************************/
/* ODIģ����Ϣ��                                                        */
/************************************************************************/
#define evODI_Message                           evInitial +1200 //	������Ϣ��ODI 
#define evODI_Message_Ack                       evInitial +1201 //ODI Ӧ����Ϣ
#define evODI_Access                            evInitial +1202 //ODI������Ϣ��SLP
#define evODI_Access_Ack                        evInitial +1203 //SLP������Ϣ��ODIȷ��

#define evODI_JS_Ask                            evInitial +1204 //������ODI����������Ϣ
#define evODI_JS_Ack                            evInitial +1205 //ODC����㷢��ȷ����Ϣ




#endif


