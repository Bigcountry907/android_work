/********************************************************************
公司名称：江苏怡丰通讯有限公司
创建时间: 2007-12-23   16:16
文件名称: F:\江苏怡丰\源代码\HdlDll\HdlDll.c
文件路径: F:\江苏怡丰\源代码\HdlDll
file base:HdlDll
file ext: c
author:	  刘定文

  purpose:	HdlDLL动态库
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commdll.h"
#include "../AcsPublic/Spp_Acc_Event.h"
#include "SppAccDll.h"
#include "yftrace.h"
#include "AccDll.h"
void WriteLog(BOOL status);
enum DEVICE_CALL_STATE
{
	DEVICE_COMMON_STATE_START   = 0x0000, //
		DCS_FREE                    = 0x0001, //
		DCS_LOCAL_BLOCK             = 0x0002, //
		DCS_REMOTE_BLOCK            = 0x0003, //
		DCS_BLOCK                   = 0x0004, //
		DCS_UNAVAILABLE             = 0x0005, //
		
		DCS_CALLOUT                 = 0x0006, //
		DCS_CALLIN                  = 0x0007, //
		DCS_ALERTING                = 0x0008, //
		
		DCS_CONNECT                 = 0x0009, //
		
		DCS_DISCONNECT              = 0x000A, //
		DCS_WAIT_FREE               = 0x000B, //
		
		DEVICE_ESPECIAL_STATE_START = 0x0100,
		DES_ISUP_STATE_START        = (XMS_DEVSUB_E1_SS7_ISUP << 8), //
		DES_ISUP_LOCAL_SUSPEND      = (DES_ISUP_STATE_START | 0x01), //
		DES_ISUP_REMOTE_SUSPEND     = (DES_ISUP_STATE_START | 0x02), //
		DES_ISUP_LOCAL_RESUME       = (DES_ISUP_STATE_START | 0x03), //
		DES_ISUP_REMOTE_RESUME      = (DES_ISUP_STATE_START | 0x04), //
		DES_ISUP_RECV_INR           = (DES_ISUP_STATE_START | 0x05), //
		DES_ISUP_RECV_INF           = (DES_ISUP_STATE_START | 0x06), //
		DES_ISUP_RECV_CQR           = (DES_ISUP_STATE_START | 0x07), //
		DES_ISUP_RECV_USR           = (DES_ISUP_STATE_START | 0x08), //
		DES_ISUP_RECV_CPG           = (DES_ISUP_STATE_START | 0x09), //
		
		DES_TUP_STATE_START         = (XMS_DEVSUB_E1_SS7_TUP << 8),  // 
		DES_TUP_RECV_CCL            = (DES_TUP_STATE_START | 0x01),  // 
		DES_TUP_RECV_RAN            = (DES_TUP_STATE_START | 0x02),  // 
		DES_TUP_RECV_GSM            = (DES_TUP_STATE_START | 0x03),  // 
		
		DES_DSS1_STATE_START        = (XMS_DEVSUB_E1_DSS1 << 8),      // 
		DES_DSS1_RESTRAT            = (DES_DSS1_STATE_START | 0x01),  // 
		DES_DSS1_WAIT_CONNECT_ACK   = (DES_DSS1_STATE_START | 0x02),  // 
		DES_DSS1_CALLER_RECV_ALERT  = (DES_DSS1_STATE_START | 0x03),  //
		DES_DSS1_CALLER_RECV_CALLPROCESS = (DES_DSS1_STATE_START | 0x04),  //
		DES_DSS1_CALLER_RECV_SETUP_ACK	 = (DES_DSS1_STATE_START | 0x05),  //
		DES_DSS1_CALLER_WAIT_SETUP_ACK	 = (DES_DSS1_STATE_START | 0x06),  //
		
		DES_H323_STATE_START        = 0x00010100,
		DES_H323_PROCEEDING         = (DES_H323_STATE_START + 0x01), // proceeding
		DES_H323_TRANSFERING        = (DES_H323_STATE_START + 0x02), // transfering
		DES_H323_ACF                = (DES_H323_STATE_START + 0x03), // admission confirm (reserved)
		DES_H323_ARJ                = (DES_H323_STATE_START + 0x04), // admission reject (reserved)
		DES_H323_INCOMPADDR         = (DES_H323_STATE_START + 0x05), // incomplete address (reserved)
		DES_H323_WAITACK            = (DES_H323_STATE_START + 0x06), // wait for address ack (reserved)
		
		DES_SIP_STATE_START         = 0x00010200,
		DES_SIP_REDIRECTED          = (DES_SIP_STATE_START + 0x01),  // call redirected
		DES_SIP_UNAUTH              = (DES_SIP_STATE_START + 0x02),  // unauthenticated
		DES_SIP_ACCEPTED            = (DES_SIP_STATE_START + 0x03),  // accepted (200 ok received)
		DES_SIP_REMOTEACCEPT        = (DES_SIP_STATE_START + 0x04),  // remote party accepted (reserved)
		DES_SIP_DISCONNECTING       = (DES_SIP_STATE_START + 0x05),  // BYE sent
		DES_SIP_TERMINATED          = (DES_SIP_STATE_START + 0x06),  // call finished
		DES_SIP_CANCELLED           = (DES_SIP_STATE_START + 0x07),  // call cancelled
		DES_SIP_CANCELLING          = (DES_SIP_STATE_START + 0x08),  // CANCEL sent
		DES_SIP_TIMEOUT             = (DES_SIP_STATE_START + 0x09),  // timeout after 1xx response received (reserved)
		DES_SIP_MSGSENDFAIL         = (DES_SIP_STATE_START + 0x0A),  // message send failed
		
		DES_ANALOG_TRUNK_STATE_START     = (XMS_DEVSUB_ANALOG_TRUNK << 8),         // 
		DES_ANALOG_TRUNK_OFFHOOK         = (DES_ANALOG_TRUNK_STATE_START | 0x01),  // 
		DES_ANALOG_TRUNK_CALLOUT_FAILURE = (DES_ANALOG_TRUNK_STATE_START | 0x02),  // 
		
		DES_ANALOG_USER_STATE_START      = (XMS_DEVSUB_ANALOG_USER << 8),         // 
		DES_ANALOG_USER_RING             = (DES_ANALOG_USER_STATE_START | 0x01),  // 
		DES_ANALOG_USER_OFFHOOK          = (DES_ANALOG_USER_STATE_START | 0x02),  // 
		
		DES_FAX_START                      = (XMS_DEVMAIN_FAX<<16),  // 
		DES_FAX_OPEN                       = (DES_FAX_START ) ,      //  
		DES_FAX_CLOSE                      = (DES_FAX_START | 0x01), //  
		DES_FAX_RESET                      = (DES_FAX_START | 0x02), //
		
		DES_FAX_SENDFAX_RECVCMD            = (DES_FAX_START | 0x10), //
		DES_FAX_SENDFAX_START              = (DES_FAX_START | 0x11), //
		DES_FAX_SENDFAX_SETLOCAL           = (DES_FAX_START | 0x12), //
		DES_FAX_SENDFAX_RESOLVE_FISNIESH   = (DES_FAX_START | 0x13), //
		DES_FAX_SENDFAX_LOCAL_FINISH       = (DES_FAX_START | 0x14), //
		DES_FAX_SENDFAX_SHAKEHAND_END      = (DES_FAX_START | 0x15), //
		DES_FAX_SENDFAX_SENDDATA           = (DES_FAX_START | 0x16), //
		DES_FAX_SENDFAX_SENDFINISH         = (DES_FAX_START | 0x17), //
		DES_FAX_SENDFAX_USERSTOP           = (DES_FAX_START | 0x18), //
		DES_FAX_SENDFAX_TIFFFILE_NOTEXIST  = (DES_FAX_START | 0x19), //
		DES_FAX_SENDFAX_SENDEND            = (DES_FAX_START | 0x20), //
		DES_FAX_SENDFAX_SENDSTOP           = (DES_FAX_START | 0x21), //
		
		DES_FAX_RECVFAX_RECVCMD            = (DES_FAX_START | 0x30), //
		DES_FAX_RECVFAX_WAIT_HANDSHAKE     = (DES_FAX_START | 0x31), //
		DES_FAX_RECVFAX_HANDSHAKE_OK       = (DES_FAX_START | 0x32), //
		DES_FAX_RECVFAX_RECVDATA_START     = (DES_FAX_START | 0x33), //
		DES_FAX_RECVFAX_BACKPAGESTATE_SUCC = (DES_FAX_START | 0x34), //
		DES_FAX_RECVFAX_RE_HANDSHAKE       = (DES_FAX_START | 0x35), //
		DES_FAX_RECVFAX_RECVDATA_END       = (DES_FAX_START | 0x36), //
		DES_FAX_RECVFAX_USERSTOP           = (DES_FAX_START | 0x37), //
		DES_FAX_RECVFAX_COMPOSE_START      = (DES_FAX_START | 0x38), //
		DES_FAX_RECVFAX_COMPOSE_END        = (DES_FAX_START | 0x39), //
		DES_FAX_RECVFAX_BACKPAGESTATE_FAIL = (DES_FAX_START | 0x40), //
		DES_FAX_RECVFAX_RECVEND            = (DES_FAX_START | 0x41), //
};
/********************************************************************
函数名称：DllMain
函数功能: 入口函数
参数定义: 无
返回定义: 无
创建时间: 2007-12-23 16:57
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
	
    switch (reason)
    {
	case DLL_PROCESS_ATTACH:
        break;
		
	case DLL_PROCESS_DETACH:
        break;
		
	case DLL_THREAD_ATTACH:
        break;
		
	case DLL_THREAD_DETACH:
        break;
    }
	
    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}
/********************************************************************
函数名称：Hdl_Init
函数功能: Hdl初始化接口
参数定义: 无
返回定义: 无
创建时间: 2007-12-23 16:58
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT void Hdl_Init(ACS_ATTR attr)
{
	pDspMap.g_iTotalModule=0;
	memset(pDspMap.pMapTable,0,sizeof(pDspMap.pMapTable));
	ReadToneIndexFile();
	InitAcsSys(attr);	
	g_Trace=TRUE;
}
/********************************************************************
函数名称：Hdl_SendMessage
函数功能: 向ACS系统发送消息
参数定义: 无
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2007-12-23 16:58
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_SendMessage(DWORD sid,DWORD esrParam )
{
	Acs_Evt_t *			    pAcsEvt = NULL;
	Acs_Dev_List_Head_t * pAcsDevList = NULL;
	Acs_IO_Data				*pIOData = NULL;
	ACS_Hdl_EVENT *pHdlEvent = (ACS_Hdl_EVENT *)esrParam;
	DeviceID_t FreeVocDeviceID;
	WORD DspId;
	WORD TrunkId;
	if(sid>=MESSAGE_MAX)
		return FALSE;
    DspId = (WORD)(g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256));
    TrunkId = (WORD)(g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256));
	memset(&FreeVocDeviceID,0,sizeof(FreeVocDeviceID));
	pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
	
	pAcsEvt = (Acs_Evt_t *) esrParam;
	if(MAX_DSP_MODULE_NUMBER_OF_XMS<=DspId)
		return FALSE;
	if(AllDeviceRes[DspId].lTrunkNum<=TrunkId)
		return FALSE;
	
	
	return TRUE;
}
DLLIMPORT void Hdl_SetTrace(BOOL value)
{
	g_Trace = value;
}
/********************************************************************
函数名称：Hdl_SetRecall
函数功能: 设置回掉函数
参数定义: esr：回掉函数指针
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2007-12-23 16:59
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_SetRecall(void *esr)
{
	pEvtHandle= esr ;
	return  TRUE;
}

/********************************************************************
函数名称：BCD_TO_ASCIIString
函数功能: BCD码串转ASCII码串
参数定义: lParam：消息号
返回定义: 无
创建时间: 2008-1-27 21:28
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void BCD_TO_ASCIIString(WORD sid,unsigned char *cSource, unsigned int iSourceLen)
{
	FILE *fp;
	char mmmmm[3200];
	char cResult[3200];
	unsigned int i=0;
    int j=0;
	char filename[30];
	SYSTEMTIME     Clock;


	GetLocalTime(&Clock);

 

	memset(mmmmm, 0, 1600);
	memset(cResult,0,sizeof(cResult));
    for(i=0;i<iSourceLen;i++) 
    {
        cResult[j+2]=32;
        cResult[j+1] = cSource[i]%16 + '0';
        if(cResult[j+1] > 9+'0')
        {
            cResult[j+1] = cResult[j+1]+7;
        }
        cResult[j] = cSource[i]/16+'0';
        if(cResult[j] > 9+'0')
        {
            cResult[j] = cResult[j]+7;
        }
        j+=3;
    }
    cResult[j] = '\0';
	cResult[j++]='\n';

	sprintf(filename,"C:\\YFCOMM\\LOG\\pAccData.log");
	fp = fopen(filename,"a");
	if(fp == NULL)
	{
//		fclose(fp);
		return ;
	}
	fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d  ",
		Clock.wYear,Clock.wMonth, Clock.wDay,
		Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds);
	fwrite(cResult,strlen(cResult),1,fp);
//	fwrite('\n',1,1,fp);
	
	fclose(fp);
	fp=NULL;

} 
/********************************************************************
函数名称：Hdl_SendFSK
函数功能: 发送FSK
参数定义: sid:会话号，data:数据内容，len:数据长度
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-1-31 14:50
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_SendFSK(DWORD sid,/*BYTE *data,WORD len*/void *infor)
{
	
	WORD DspId;
	WORD TrunkId;
	stFSK *fsk=(stFSK *)infor;
	DeviceID_t pDevice;
	RetCode_t r;
	if(fsk == NULL)
		return FALSE;
	
	if(sid>MESSAGE_MAX)
	{
		
		//		YF_LOG_SMM ,"ACC->Keygoe:sid is overflow,send fsk failed");
		
		return FALSE;
	}
	if(!g_Sid_Xssm_Dll_Infor[sid].bBusy)
	{
		//		YF_LOG_SMM "Send FSK resource busy");
		return FALSE;
	}
	DspId = (WORD)(g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256));
	TrunkId = (WORD)(g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256));
	if(DspId >= MAX_DSP_MODULE_NUMBER_OF_XMS)
		return FALSE;
	if(&AllDeviceRes[DspId]==NULL)
	{
		
		YF_LOG_SMM  "ACC->Keygoe : AllDeviceRes[%d] = NULL,send fsk failed",DspId);
		
		return FALSE;
	}
	if(AllDeviceRes[DspId].lTrunkNum<=TrunkId)
		return FALSE;
	if(&AllDeviceRes[DspId].pTrunk[TrunkId]==NULL)
	{
		
		YF_LOG_SMM  "ACC->Keygoe : AllDeviceRes[%d].pTrunk[%d]==NULL,send fsk failed",DspId,TrunkId);
		
		return FALSE;
	}
	//pDevice = AllDeviceRes[DspId].pTrunk[TrunkId].deviceID;
	if(&AllDeviceRes[DspId].pTrunk[TrunkId].VocDevID==NULL)
	{
		
		YF_LOG_SMM  "ACC->Keygoe : AllDeviceRes[%d].pTrunk[%d].VocDevID==NULL,send fsk failed",DspId,TrunkId);
		
		
		return FALSE;
	}
	pDevice = AllDeviceRes[DspId].pTrunk[TrunkId].VocDevID;
	r=XMS_ctsSendIOData(g_acsHandle,&pDevice,XMS_IO_TYPE_FSK,fsk->length,fsk->message);
	if(r<0)
	{
		YF_LOG_SMM  "ACC->Keygoe : send fsk failed,return = %d",r);
		
		return FALSE;
	}
	else
	{
//		BCD_TO_ASCIIString(sid,fsk->message,fsk->length);
		WriteLog(fsk->length);
		return TRUE;
	}
}
/********************************************************************
函数名称：Hdl_Exit
函数功能: Hdl退出，系统自动关闭与ACS系统的连接，释放分配的内存
参数定义: 无
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2007-12-23 17:00
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

DLLIMPORT BOOL Hdl_Exit()
{
	return(ExitSystem());
}
/********************************************************************
函数名称：Hdl_PlayFile
函数功能: 放音
参数定义: 无
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2007-12-23 17:01
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
//DLLIMPORT BOOL Hdl_PlayFile(BYTE bType,BYTE bNo,BOOL bPlay)
DLLIMPORT void Hdl_PlayFile(int nDsp,int nTrunkNo,LPARAM lParam)
{
	DeviceID_t pDevice;
	PVOICE *pVoice =(PVOICE *)lParam;
	int i;
	if(MAX_DSP_MODULE_NUMBER_OF_XMS<=nDsp)
		return ;
	if(AllDeviceRes[nDsp].lTrunkNum<=nTrunkNo)
		return ;
	if(pVoice == NULL)
		return;
	memcpy(&pDevice,&AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID,sizeof(DeviceID_t));
	//pDevice = AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID;
	AllDeviceRes[nDsp].pTrunk[nTrunkNo].u8PlayTag=0;
	
	for(i=0;i<pVoice->pVoiceHead.nNo;i++)
	{
		VoiceFileHandle(&pDevice,pVoice,i);
	}
	
	
}
/********************************************************************
函数名称：DualLinkDevice
函数功能: 设备双向连接
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 11:06
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void DualLinkDevice(DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	if(XMS_ctsLinkDevice ( g_acsHandle, pDev1, pDev2, NULL )<0) 
		WriteLog(FALSE);
	else
		WriteLog(TRUE);
	if(XMS_ctsLinkDevice ( g_acsHandle, pDev2, pDev1, NULL )<0)
		WriteLog(FALSE);
	else
		WriteLog(TRUE);
}
/********************************************************************
函数名称：DualUnLinkDevice
函数功能: 解除设备双向连接
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 11:08
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void DualUnLinkDevice(DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
//	SetGtD(pDev1);//设置语音参数
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
}

/********************************************************************
函数名称：CloseDeviceOK
函数功能: 关闭设备
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 10:01
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void CloseDeviceOK(DeviceID_t *pDevice )
{
	TRUNK_STRUCT	*pOneTrunk;
	if(pDevice == NULL)
		return;
	AllDeviceRes[pDevice->m_s8ModuleID].bErrFlag = TRUE;
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = FALSE;
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(*pDevice);
		
		Change_State ( pOneTrunk, TRK_WAITOPEN );
		
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened --;
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		// init this Device: Voice
		Change_Voc_State ( &M_OneVoice(*pDevice), VOC_WAITOPEN);
		
		
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened --;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum --;
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		M_OnePcm(*pDevice).bOpenFlag = FALSE;
		
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened --;
	}
}

void VoiceFileHandle(DeviceID_t *pDevId,PVOICE *pVoice,int offset)
{
	char pChar[32];
	char *pMax,*pMin,*pYear ,*pMonth ,*pDay ,*pHour ,*pMinu ,*pSec ,*pMisec ; 
	BYTE bMax,bMin;
	int i;
	char value;
	BOOL bZerno = FALSE;
	int len=0;
	int number;
	int index,index1;
	BYTE u8PlayTag;
	if(pDevId == NULL)
		return;
	if(pVoice == NULL)
		return;
	if(MAX_DSP_MODULE_NUMBER_OF_XMS<=pDevId->m_s8ModuleID)
		return ;
	if(AllDeviceRes[pDevId->m_s8ModuleID].lTrunkNum<=pDevId->m_s16ChannelID)
		return ;
	////ceshi/////////////////////////////////////////////////////////////////////////
	
	u8PlayTag = (AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag & 255);
	//		u8PlayTag = pDevId->m_s16ChannelID;
	//////////////////////////////////////////////////////////////////////////
	
	strcpy(pChar,pVoice->pVoice_Content[offset].content);
	switch(pVoice->pVoice_Content[offset].pType)
	{
	case PLAY_FILE_TONE_ELMENT:
		{
			number =atoi(pChar);
			PlayIndex(pDevId,(WORD)number,u8PlayTag,FALSE);
			//////////////////////////////////////////////////////////////////////////
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
		}
		
		///ceshi///////////////////////////////////////////////////////////////////////
		
		break;
	case PLAY_FILE_DIGIT://数字
	case PLAY_FILE_CURRENCY://货币
		{
			pMax=strtok(pChar,".");
			pMin=strtok(NULL,".");
			
			if(pMax==NULL)
				bMax = 0;
			else
				bMax=strlen(pMax);//整数位数
			if(pMin==NULL)
				bMin = 0;
			else
				bMin=strlen(pMin);//小数位数
			for(i=0;i<bMax;i++)//整数部分
			{
				value = pMax[i];
				index=atoi(&value);
				if(value==0)//值为0且bzern=TRUE
				{
					if(bZerno)
					{
						continue;
					}
					else
					{
						bZerno = TRUE;
					}
				}
				else
				{
					bZerno = FALSE;
				}
				PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
				if((i!=(bMax-1))&&(value != 0))
				{
					index1=bMax-1-i+10;
					PlayIndex(pDevId,(WORD)index1,u8PlayTag,TRUE);
					if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
					else
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
					if(u8PlayTag==255)
						u8PlayTag= 0;
					else
						u8PlayTag++;
				}
				
			}
			if(bMin<=0)
				break;
			PlayIndex(pDevId,18,u8PlayTag,TRUE);//小数点
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
			for(i=0;i<bMin;i++)//小数部分
			{
				value = pMin[i];
				index=atoi(&value);
				PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
			}
			if(PLAY_FILE_CURRENCY==pVoice->pVoice_Content[offset].pType)
			{
				PlayIndex(pDevId,TONE_YUAN,u8PlayTag,TRUE);//yuan
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
			}
		}
		break;
	case PLAY_FILE_CHAR://数字串
		for (i=0;i<(int)strlen(pChar);i++)
		{
			value = pChar[i];
			index=atoi(&value);
			PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
		}
		break;
	case PLAY_FILE_TIME://time
		{
			pHour=strtok(pChar,":");
			pMinu=strtok(NULL,":");
			pSec= strtok(NULL,":");
			pMisec=strtok(NULL,":");
			if(pHour == NULL)
				len =0;
			else
				len = (int)strlen(pHour);
			for(i=0;i<len;i++)
			{
				value = pHour[i];
				index=atoi(&value);
				PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
				index1=len-1-i+10;
				PlayIndex(pDevId,(WORD)index1,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
			}
			PlayIndex(pDevId,TONE_HOUR,u8PlayTag,TRUE);//hour
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
			len = (int)strlen(pMinu);
			for(i=0;i<len;i++)
			{
				value = pMinu[i];
				index = atoi(&value);
				PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
				index1=len-1-i+10;
				PlayIndex(pDevId,(WORD)index1,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
			}
			PlayIndex(pDevId,TONE_MINUTE,u8PlayTag,TRUE);//minute
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
			len = (int)strlen(pSec);
			for(i=0;i<len;i++)
			{
				value = pSec[i];
				index = atoi(&value);
				PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
				index1=len-1-i+10;
				PlayIndex(pDevId,(WORD)index1,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
			}
			PlayIndex(pDevId,TONE_SECOND,u8PlayTag,TRUE);//second
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
			len = strlen(pMisec);
			for(i=0;i<len;i++)
			{
				value = pMisec[i];
				index = (atoi(&value));
				PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
				index1=len-1-i+10;
				PlayIndex(pDevId,(WORD)index1,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
			}
			PlayIndex(pDevId,TONE_YAO,u8PlayTag,TRUE);//Millisecond 
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
		}
		break;
	case PLAY_FILE_DATE://日期
		{
			pYear =strtok(pChar,"-");
			pMonth =strtok(NULL,"-");
			pDay = strtok(NULL,"-");
			if(pYear == NULL)
				len = 0;
			else
				len = strlen(pYear);
			for(i=0;i<len;i++)
			{
				value = pYear[i];
				index = atoi(&value);
				PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
				if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
				else
					AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
				if(u8PlayTag==255)
					u8PlayTag= 0;
				else
					u8PlayTag++;
				
			}
			PlayIndex(pDevId,TONE_YEAR,u8PlayTag,TRUE);//year
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
			len = strlen(pMonth);
			for(i=0;i<len;i++)
			{
				value = pMonth[i];
				index = atoi(&value);
				if((i==0)&&(len>1))
				{
					if(index!=1)
						continue;
				}
				else
				{
					PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
					if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
					else
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
					if(u8PlayTag==255)
						u8PlayTag= 0;
					else
						u8PlayTag++;
				}
				if(i!=(len-1))
				{
					index1=len-1-i+10;
					PlayIndex(pDevId,(WORD)index1,u8PlayTag/*1*/,TRUE);
					if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
					else
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
					if(u8PlayTag==255)
						u8PlayTag= 0;
					else
						u8PlayTag++;
				}
			}
			PlayIndex(pDevId,TONE_MONTH,u8PlayTag,TRUE);//month
			((AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++) & 256);
			u8PlayTag++;
			len = strlen(pDay);
			for(i=0;i<len;i++)
			{
				value = pDay[i];
				index = atoi(&value);
				if((i==0)&&(len>1))
				{
					if(index!=1)
						continue;
				}
				else
				{
					PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
					if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
					else
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
					if(u8PlayTag==255)
						u8PlayTag= 0;
					else
						u8PlayTag++;
				}
				if(i!=(len-1))
				{
					index1=len-1-i+10;
					PlayIndex(pDevId,(WORD)index1,u8PlayTag/*1*/,TRUE);
					if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
					else
						AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
					if(u8PlayTag==255)
						u8PlayTag= 0;
					else
						u8PlayTag++;
					
				}
			}
			PlayIndex(pDevId,TONE_DAY,u8PlayTag,TRUE);//day
			if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
			else
				AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
			if(u8PlayTag==255)
				u8PlayTag= 0;
			else
				u8PlayTag++;
		}
		break;
	case PLAY_FILE_FILE://文件名
		index = atoi(pChar);
		PlayIndex(pDevId,(WORD)index,u8PlayTag/*1*/,TRUE);
		if(AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag==255)
			AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag=0;
		else
			AllDeviceRes[pDevId->m_s8ModuleID].pTrunk[pDevId->m_s16ChannelID].u8PlayTag++;
		if(u8PlayTag==255)
			u8PlayTag= 0;
		else
			u8PlayTag++;
		break;
	}
}


/********************************************************************
函数名称：Hdl_StartDPAck
函数功能: 呼叫确认
参数定义: 无
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-2-1 15:53
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_StartDPAck(DWORD DeviceNumber,BOOL bFree)
{
	int nDsp,nTrunkNo;
	nDsp=DeviceNumber/(256*256);
	nTrunkNo = DeviceNumber%(256*256);
	if(nDsp >= MAX_DSP_MODULE_NUMBER_OF_XMS)
		return FALSE;
	
	if(&AllDeviceRes[nDsp]==NULL)
		return FALSE;
	if(AllDeviceRes[nDsp].lTrunkNum<=nTrunkNo)
		return FALSE;
	if(&(AllDeviceRes[nDsp].pTrunk[nTrunkNo])==NULL)
		return FALSE;
	if(&AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID==NULL)
		return FALSE;
	if(AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
	{
		if(bFree)//空闲
		{
			
			return TRUE;
		}
		else//挂机 
		{
			XMS_ctsClearCall ( g_acsHandle, &AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID, 0, NULL );
			Change_UserState(&(AllDeviceRes[nDsp].pTrunk[nTrunkNo]),USER_FREE);
		}
	}
	else
	{
		if(bFree)//空闲
		{
			
			XMS_ctsAlertCall (g_acsHandle, &AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID, NULL );
			XMS_ctsAnswerCallIn (g_acsHandle, &AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID, NULL );
		}
		else//挂机 
		{
			XMS_ctsClearCall ( g_acsHandle, &AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID, 0, NULL );
			Change_State(&(AllDeviceRes[nDsp].pTrunk[nTrunkNo]),TRK_FREE);
		}
	}
	
	return TRUE;
}
/********************************************************************
函数名称：Hdl_setsid
函数功能: 设置会话号
参数定义: 
sid：会话号
calledNum：被叫号码
callingnum:主叫号码
返回定义: 成功：TRUE，失败：FALSE
创建时间: 2008-1-22 11:54
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_setsid(DWORD sid,DWORD DeviceNumber,char calledNum[32],char callingNum[32])
{
	int number;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	number = sid;
	g_Sid_Xssm_Dll_Infor[number].sid = (WORD)sid;
	g_Sid_Xssm_Dll_Infor[number].DeviceNumber = DeviceNumber;
	strcpy(g_Sid_Xssm_Dll_Infor[number].CalleeCode,callingNum);
	strcpy(g_Sid_Xssm_Dll_Infor[number].CallerCode,calledNum);
	g_Sid_Xssm_Dll_Infor[number].bBusy=TRUE;
	return FALSE;
}
/********************************************************************
函数名称：Hdl_clearsid
函数功能: 注销会话号
参数定义: sid:会话号
返回定义: 成功：TRUE，失败：FALSE
创建时间: 2008-1-22 13:38
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_clearsid(DWORD sid)
{
	if(sid>=MESSAGE_MAX)
	{
		return FALSE;
	}
	else
	{
		ClearSid(sid);
	}
	return TRUE;
}
/********************************************************************
函数名称：Hdl_FilePlay
函数功能: 文件放音
参数定义: sid:会话号，pVoiceContent：放音内容
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2008-1-22 14:33
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

DLLIMPORT BOOL Hdl_FilePlay(DWORD sid, void * VoiceAddr/*PVOICE *pVoiceContent*/)
{
	DeviceID_t pDevice;
	PVOICE *pVoice =(PVOICE *)VoiceAddr;
	int i;
	int nDsp,nTrunkNo;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
	if(MAX_DSP_MODULE_NUMBER_OF_XMS<=nDsp)
		return FALSE;
	if(AllDeviceRes[nDsp].lTrunkNum<=nTrunkNo)
		return FALSE;
	memcpy(&pDevice,&AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID,sizeof(DeviceID_t));
	//	pDevice = AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID;
	if(pVoice==NULL)
		return FALSE;
	
	for(i=0;i<pVoice->pVoiceHead.nNo;i++)
	{
		VoiceFileHandle(&pDevice,pVoice,i);
	}
	return TRUE;
}
/********************************************************************
函数名称：Hdl_StopFilePlay
函数功能: 停止放音接口
参数定义: sid: 会话号
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2008-1-22 14:39
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_StopFilePlay(DWORD sid)
{
	ControlPlay_t	controlPlay;
	int nDsp,nTrunkNo;
	DeviceID_t pDevice;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
	if(nDsp>=MAX_DSP_MODULE_NUMBER_OF_XMS)
		return FALSE;
	if(nTrunkNo >= AllDeviceRes[nDsp].lTrunkNum)
		return FALSE;
	memcpy(&pDevice,&AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID,sizeof(DeviceID_t));
	//	pDevice = AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID;
	controlPlay.m_u16ControlType = XMS_STOP_PLAY;
	XMS_ctsControlPlay ( g_acsHandle,&pDevice,&controlPlay, NULL );
	
	YF_TRACE sid,"l_StopFilePlay","ACC->Keygoe:Stop play file");
	
	return TRUE;
}
void SetGtD_AnalogTrunk(DeviceID_t* pDevId)
{
	//========Set GTG Begin========
	DJ_U16 u16ParamType = VOC_PARAM_UNIPARAM;
	
	void* p ;
	CmdParamData_Voice_t cmdVoc;
	DJ_U16 u16ParamSize = sizeof(cmdVoc);
	memset(&cmdVoc,0,sizeof(cmdVoc));
	cmdVoc.m_u8GtdCtrlValid = 1 ;						//Enable GTD
	cmdVoc.m_VocGtdControl.m_u8ChannelEnable = 1;		//Enable Gtd channel
	cmdVoc.m_VocGtdControl.m_u8DTMFEnable = 1;			// Detect DTMF
	cmdVoc.m_VocGtdControl.m_u8GTDEnable = 1;			// Detect GTD 
	cmdVoc.m_VocGtdControl.m_u8FSKEnable = 1;			// Detect FSK for receive CallerID
	
	cmdVoc.m_VocGtdControl.m_u8EXTEnable = 0x2;		// Enable PVD Detect
	//cmdVoc.m_VocGtdControl.m_u8EXTEnable = 0x1;		// Enable PVD Detect
	
	strcpy((char*)&cmdVoc.m_VocGtdControl.m_u8GTDID[0],"GHIJK");	// Detect Busy Tone
	
	
	p = (void*) &cmdVoc;
	
	XMS_ctsSetParam( g_acsHandle,pDevId,u16ParamType,u16ParamSize,(void*)p);
	
	
	//========Set GTG End  ========
}
/********************************************************************
函数名称：Hdl_ConnectToResource
函数功能: 将呼叫绑定到语音资源
参数定义: sid: 会话号
返回定义: 无
创建时间: 2008-1-22 14:42
函数作者: 刘定文
注意事项: 成功返回TRUE，失败返回FALSE	
*********************************************************************/
DLLIMPORT BOOL Hdl_ConnectToResource(DWORD sid)
{
	int nDsp,nTrunkNo;
	TRUNK_STRUCT	*pOneTrunk;
	DeviceID_t	FreeVocDeviceID;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
	if(nDsp >= MAX_DSP_MODULE_NUMBER_OF_XMS)
		return FALSE;
	if(AllDeviceRes[nDsp].lTrunkNum<=nTrunkNo)
		return FALSE;
	pOneTrunk = &(AllDeviceRes[nDsp].pTrunk[nTrunkNo]);
	if(SearchOneFreeVoice(pOneTrunk,&FreeVocDeviceID)<0)
	{
		return FALSE;
	}
	pOneTrunk->u8PlayTag = 16;
	pOneTrunk->VocDevID = FreeVocDeviceID;
	M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
	DualLinkDevice(&(pOneTrunk->deviceID),&FreeVocDeviceID);
	if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
	{
		SetGtD_AnalogTrunk(&pOneTrunk->VocDevID);		// prepare for get Busy Tone
		return TRUE;
	}
	if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
	{
		SetGtD_AnalogTrunk(&pOneTrunk->VocDevID);
		return TRUE;
	}
	SetGtD(&pOneTrunk->VocDevID);
	return TRUE;
}
/********************************************************************
函数名称：Hdl_DisconnectForwardResource
函数功能: 切断前向语音资源连接
参数定义: sid: 会话号
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2008-1-22 14:44
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_DisconnectForwardResource(DWORD sid)
{
	int nDsp,nTrunkNo;
	TRUNK_STRUCT	*pOneTrunk;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
	if(nDsp>=MAX_DSP_MODULE_NUMBER_OF_XMS)
		return FALSE;
	if(AllDeviceRes[nDsp].lTrunkNum<=nTrunkNo)
		return FALSE;
	pOneTrunk = &(AllDeviceRes[nDsp].pTrunk[nTrunkNo]);
	if(pOneTrunk == NULL)
		return FALSE;
	if ( pOneTrunk->VocDevID.m_s16DeviceMain == 0)
		return FALSE;
	StopPlayFile ( &pOneTrunk->VocDevID );
	DualUnLinkDevice(&pOneTrunk->deviceID,&pOneTrunk->VocDevID);
	FreeOneFreeVoice (&pOneTrunk->VocDevID);
	if(AllDeviceRes[(pOneTrunk->VocDevID).m_s8ModuleID].pVoice!=NULL)
		memset(&M_OneVoice(pOneTrunk->VocDevID).UsedDevID,0,sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
	memset(&pOneTrunk->VocDevID,0,sizeof(DeviceID_t));		// 0表示还没有分配 Device
	return TRUE;
}
/********************************************************************
函数名称：Hdl_ReleaseCall
函数功能: 释放呼叫
参数定义: sid: 会话号,DeviceNumber:设备号,bUseSid:sid是否有效，TRUE有效的
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2008-1-22 14:56
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_ReleaseCall(DWORD sid,DWORD DeviceNumber,BOOL bUseSid)
//DLLIMPORT BOOL Hdl_ReleaseCall(DWORD sid)
{
	int nDsp,nTrunkNo;
	DeviceID_t pDevice;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	if(!g_Sid_Xssm_Dll_Infor[sid].bBusy)
	{
		return FALSE;
	}
	if(!bUseSid)
	{
		nDsp = DeviceNumber/(256*256);
		nTrunkNo = DeviceNumber%(256*256);	
	}
	else
	{
		nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
		nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
		ClearSid(sid);
	}
	if(nDsp>=MAX_DSP_MODULE_NUMBER_OF_XMS)
		return FALSE;
	if(AllDeviceRes[nDsp].lTrunkNum<=nTrunkNo)
		return FALSE;
	if((&AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID)==NULL)
		return FALSE;
	memcpy(&pDevice,&AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID,sizeof(DeviceID_t));
	//	pDevice = AllDeviceRes[nDsp].pTrunk[nTrunkNo].deviceID;
	XMS_ctsClearCall ( g_acsHandle, &pDevice, 0, NULL );
	//	ResetTrunk(&(AllDeviceRes[nDsp].pTrunk[nTrunkNo]));
	Change_State(&(AllDeviceRes[nDsp].pTrunk[nTrunkNo]),TRK_HANGON);
	return TRUE;
}
/********************************************************************
函数名称：Hdl_InitateRecord
函数功能: 启动录音
参数定义: pInitateRecord:录音文件具体格式
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2008-1-22 14:57
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_InitateRecord(stPXSSM_InitiateRecord pInitateRecord)
{
	DWORD sid;
	WORD nKey;
	sid = pInitateRecord.sid;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nKey = g_Sid_Xssm_Dll_Infor[sid].serviceKey;
	
	if(RecordFile(nKey,pInitateRecord)<0)
	{
		return FALSE;
	}
	return TRUE;
}
int RecordFile(WORD nKey,stPXSSM_InitiateRecord pInitateRecord)
{
	RecordProperty_t	recordProperty;
	RetCode_t			r;
	DeviceID_t	*pVocDevID;
	char		s8FileName[256];
	int nDsp,nTrunkNo;
	DWORD sid =pInitateRecord.sid;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
	pVocDevID = &(AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID);
	
	memset(&recordProperty,0,sizeof(RecordProperty_t));		
	
	sprintf(s8FileName,"c:\\yfcomm\\ini\\%d.wav",nKey,pInitateRecord.fileIndex);
	//查找文件是否存在，存在需要触发事件
	recordProperty.m_u32MaxSize = 0;
	recordProperty.m_u32MaxTime = pInitateRecord.duration;
	if ( pInitateRecord.replaceExistedFile)
		recordProperty.m_s8IsAppend = XMS_REC_FILE_APPEND;
	else
		recordProperty.m_s8IsAppend = XMS_REC_FILE_TRUNC;
	recordProperty.m_u8SRCMode = XMS_SRC_8K;
	if(pInitateRecord.canInterupt)//收到任一个DTMF码或Tone音检测码时停止录音
	{
		recordProperty.m_u8StopMode = XMS_Any_Code;
	}
	recordProperty.m_u8EncodeType=XMS_Alaw_type;
	
	strcpy ( recordProperty.m_s8FileName, s8FileName);


	r = XMS_ctsRecord ( g_acsHandle, pVocDevID, &recordProperty, NULL );

	return r;

	/*
	RecordProperty_t	recordProperty;
	RetCode_t			r;
	DeviceID_t	*pVocDevID,pVocDevFree;
	char		s8FileName[256];
	int nDsp,nTrunkNo;
	DWORD sid =pInitateRecord.sid;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
	if(SearchOneFreeVoice(&(AllDeviceRes[nDsp].pTrunk[nTrunkNo]),&pVocDevFree)<0)
//	{
//		return FALSE;
//	}
//	pVocDevID = &(AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID);
//	XMS_ctsLinkDevice ( g_acsHandle, pVocDevID, &pVocDevFree, NULL );
//
//
//	memset(&recordProperty,0,sizeof(RecordProperty_t));		
	

	pVocDevID = &(AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID);
	
	memset(&recordProperty,0,sizeof(RecordProperty_t));	
	sprintf(s8FileName,"c:\\yfcomm\\ini\\%d.wav",nKey,pInitateRecord.fileIndex);
	//查找文件是否存在，存在需要触发事件
	recordProperty.m_u32MaxSize = 0;
	recordProperty.m_u32MaxTime = pInitateRecord.duration;
	if ( pInitateRecord.replaceExistedFile)
		recordProperty.m_s8IsAppend = XMS_REC_FILE_APPEND;
	else
		recordProperty.m_s8IsAppend = XMS_REC_FILE_TRUNC;
	recordProperty.m_u8SRCMode = XMS_SRC_8K;
	if(pInitateRecord.canInterupt)//收到任一个DTMF码或Tone音检测码时停止录音
	{
		recordProperty.m_u8StopMode = XMS_Any_Code;
	}
	recordProperty.m_u8EncodeType=XMS_Alaw_type;
	recordProperty.m_s8IsMixEnable = 1;
	recordProperty.m_MixerControl.m_u8SRC1_Ctrl=XMS_MIXER_FROM_INPUT;
	recordProperty.m_MixerControl.m_u16SRC_ChID1=pVocDevID->m_s16ChannelID;
//	recordProperty.m_MixerControl.m_u8SRC2_Ctrl=;
//	recordProperty.m_MixerControl.m_u16SRC_ChID2=;
	
	strcpy ( recordProperty.m_s8FileName, s8FileName);
	
	
//	r = XMS_ctsRecord ( g_acsHandle, &pVocDevFree, &recordProperty, NULL );
	r = XMS_ctsRecord ( g_acsHandle, pVocDevID, &recordProperty, NULL );
	
	return r;*/
}
/********************************************************************
函数名称：Hdl_Connect
函数功能: 呼叫连接到一个目的地址，完成呼叫功能
参数定义: pConnect：呼叫内容
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2008-1-22 15:02
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_Connect(stPXSSM_Connect pConnect)
{
	
	int nFindTrunk;
	DeviceID_t FreeTrkDeviceID,	FreeVocDeviceID;
	int nDsp,nTrunkNo;
	//	DJ_S8	s8ModID;
	//	DJ_S16	s16ChID;
	DWORD sid =pConnect.sid;
	TRUNK_STRUCT *pOneUser = NULL;
	DWORD Devicenumber;
	nFindTrunk = SearchOneFreeTrunk(&FreeTrkDeviceID);//查找一空的中继
	if(nFindTrunk >=0)
	{
		pOneUser = &M_OneTrunk(FreeTrkDeviceID);
		if ( SearchOneFreeVoice ( pOneUser,  &FreeVocDeviceID ) >= 0 )//查找空的语音资源
		{
			pOneUser->VocDevID = FreeVocDeviceID;
			M_OneVoice(FreeVocDeviceID).UsedDevID = pOneUser->deviceID; 
			DualLinkDevice(&pOneUser->deviceID,&FreeVocDeviceID);//link 设备
		}
		if(XMS_ctsMakeCallOut ( g_acsHandle, &FreeTrkDeviceID, pConnect.callingNumber, pConnect.routeNumber, NULL )>0)
		{//呼出成功
			Change_State ( pOneUser, TRK_CALLOUT );
			nDsp=FreeTrkDeviceID.m_s8MachineID;
			nTrunkNo = FreeTrkDeviceID.m_s16ChannelID;
			Devicenumber =(DWORD)(nDsp*256*256+nTrunkNo);
			Hdl_setsid(sid,Devicenumber,pConnect.routeNumber,pConnect.callingNumber);
			return TRUE;
		}
		
	}
	return FALSE;
}
/********************************************************************
函数名称：IsTrunk
函数功能: 中继判断
参数定义: s16DevSub：设备类型
返回定义: 成功返回TRUE,拾芊祷谾ALSE
创建时间: 2008-1-24 13:36
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

BOOL IsTrunk(int s16DevSub)
{
	
	if ( (s16DevSub == XMS_DEVSUB_E1_PCM)
		|| (s16DevSub == XMS_DEVSUB_E1_CAS)
		|| (s16DevSub == XMS_DEVSUB_E1_DSS1)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_TUP)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_ISUP) 
		|| (s16DevSub == XMS_DEVSUB_ANALOG_TRUNK) 
		)
	{
		return TRUE;
	}
	
	return FALSE;
}
/********************************************************************
函数名称：IsDigitTrunk
函数功能: 数字中继判断
参数定义: s16DevSub：中继类型
返回定义: 成功返回TRUE,拾芊祷谾ALSE
创建时间: 2008-1-24 13:36
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

BOOL IsDigitTrunk(int s16DevSub)
{
	
	if ( (s16DevSub == XMS_DEVSUB_E1_PCM)
		|| (s16DevSub == XMS_DEVSUB_E1_CAS)
		|| (s16DevSub == XMS_DEVSUB_E1_DSS1)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_TUP)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_ISUP) 
		)
	{
		return TRUE;
	}
	
	return FALSE;
}
/********************************************************************
函数名称：SearchOneFreeTrunk
函数功能: 查找一个空闲的中继(按照顺序方式查找)
参数定义: 无
返回定义: 成功返回TRUE,拾芊祷谾ALSE
创建时间: 2008-1-24 10:19
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
int	SearchOneFreeTrunk (DeviceID_t *pFreeTrkDeviceID)
{
	int		i, iTmp;
	DJ_S8	s8ModID;
	DJ_S16	s16ChID;
	static	int	iLoopStart = 0;
	
	for ( i = 0; i < pTrunkMap.total; i ++ )// 顺序方式	
	{
		s8ModID = pTrunkMap.pTable[iLoopStart].m_s8ModuleID;
		s16ChID = pTrunkMap.pTable[iLoopStart].m_s16ChannelID;
		
		if ( ( AllDeviceRes[s8ModID].pTrunk[s16ChID].iLineState ==1 /*DCS_FREE*/ )
			//&& IsDigitTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )
			&& IsTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )
		{
			*pFreeTrkDeviceID = AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID;
			iTmp = iLoopStart;
			iLoopStart = (iLoopStart + 1 ) % pTrunkMap.total;
			return iTmp;
		}
		iLoopStart = (iLoopStart + 1 ) % pTrunkMap.total;
	}
	
	return -1;
}
/********************************************************************
函数名称：Hdl_IniateCallAttempt
函数功能: 启动试呼
参数定义: pCallAttempt:呼叫内容
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2008-1-22 15:15
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_IniateCallAttempt(stPXSSM_InitiateCallAttempt pCallAttempt)
{
	Hdl_Connect(pCallAttempt);
	return TRUE;
}
/********************************************************************
函数名称：Hdl_StopRecord
函数功能: 结束正在的录音
参数定义: sid:会话号
返回定义: 成功返回TRUE,拾芊祷谾ALSE
创建时间: 2008-1-22 15:17
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_StopRecord(DWORD sid)
{
	ControlRecord_t	controlRecord;
	int nDsp,nTrunkNo;
	DeviceID_t pVocDevID;
	if(sid>=MESSAGE_MAX)
		return FALSE;
	if(!g_Sid_Xssm_Dll_Infor[sid].bBusy)
		return FALSE;
	nDsp=g_Sid_Xssm_Dll_Infor[sid].DeviceNumber/(256*256);
	nTrunkNo = g_Sid_Xssm_Dll_Infor[sid].DeviceNumber%(256*256);
	if(nDsp>=MAX_DSP_MODULE_NUMBER_OF_XMS)
		return FALSE;
	if(nTrunkNo >= AllDeviceRes[nDsp].lTrunkNum)
		return FALSE;
	memcpy(&pVocDevID,&AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID,sizeof(DeviceID_t));
	//	pVocDevID = AllDeviceRes[nDsp].pTrunk[nTrunkNo].VocDevID;
	controlRecord.m_u32ControlType = XMS_STOP_RECORD;
	XMS_ctsControlRecord(g_acsHandle, &pVocDevID,&controlRecord,NULL);
	return TRUE;
}
/********************************************************************
函数名称：Hdl_TTSConvert
函数功能:将文本转化为语音文件
参数定义: pTTSConvert：转换内容及要求
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-1-22 15:26
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

DLLIMPORT BOOL Hdl_TTSConvert(stPXSSM_TTSConvert pTTSConvert)
{
	return TRUE;
}
/********************************************************************
函数名称：Hdl_TTSPlay
函数功能: TTS放音
参数定义: sid:会话号，pTTSFile：放音内容
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-1-22 15:28
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_TTSPlay(DWORD sid,stPXSSM_TTSPlayAnnouncement pTTSFile)
{
	stPXSSM_PlayAnnouncement pp;
	DWORD psid = sid;
	memcpy(&pp,&pTTSFile,sizeof(stPXSSM_PlayAnnouncement));
	//	pp=pTTSFile;
	return TRUE;
}
/********************************************************************
函数名称：Hdl_StopTTSPlay
函数功能: 停止TTS放音
参数定义: sid:会话号
返回定义: 成功返回TRUE,拾芊祷谾ALSE
创建时间: 2008-1-22 15:31
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL Hdl_StopTTSPlay(DWORD sid)
{
	return TRUE;
}
/********************************************************************
函数名称：InitAcsSys
函数功能: 初始化ACS系统
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 9:57
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL InitAcsSys(ACS_ATTR pAcs_attr)
{
	//read config
	int r;
	YF_LOG_SMM "[InitAcsSys] %s","begin init keygoe system");
	InitAllDevice();//clear device content
	InitAllDllInfor();
	g_acsHandle = -1;
	memcpy(cfg_ServerID.m_s8ServerIp,pAcs_attr.ServerIP,sizeof(pAcs_attr.ServerIP));
	cfg_ServerID.m_u32ServerPort=pAcs_attr.port;
	memcpy(cfg_ServerID.m_s8UserPwd,pAcs_attr.passwd,sizeof(pAcs_attr.passwd));
	memcpy(cfg_ServerID.m_s8UserName,pAcs_attr.username,sizeof(pAcs_attr.username));
	
	cfg_iDispChnl = 32;
	cfg_iVoiceRule = 0;
	cfg_iPartWork = 0;
	cfg_iPartWorkModuleID = 2;
	cfg_s32DebugOn = 0;
	g_u8UnitID = 2;
	cfg_s32DebugOn =pAcs_attr.attrd.debug;
	r = XMS_acsOpenStream ( &g_acsHandle, 
		&cfg_ServerID,
		g_u8UnitID,		// App Unit ID 
		128,				// sendQSize, in K Bytes
		128,				// recvQSize, in K Bytes
		cfg_s32DebugOn,	// Debug On/Off
		NULL);//打开和Keygoe的连接
	
	if ( r < 0 )
	{
		YF_LOG_SMM "[InitAcsSys] %s","XMS_acsOpenStream FALSE");
		return FALSE;
	}
	YF_LOG_SMM "[InitAcsSys] %s","XMS_acsOpenStream ok");
	
	r = XMS_acsSetESR (g_acsHandle,(EsrFunc)EvtHandler,0,1);
	if ( r < 0 )
	{
		YF_LOG_SMM "[InitAcsSys] %s","XMS_acsSetESR fail");
		return FALSE;
	}
	YF_LOG_SMM "[InitAcsSys] %s","XMS_acsSetESR ok");
	XMS_acsGetDeviceList ( g_acsHandle, NULL );
	YF_LOG_SMM "[InitAcsSys] %s","XMS_acsGetDeviceList ok");
	return TRUE;
}
/********************************************************************
函数名称：ClearSid
函数功能: 清空会话表内容
参数定义:sid：会话号
返回定义: 无
创建时间: 2008-1-22 12:05
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void ClearSid(DWORD sid)
{
	if(sid>=MESSAGE_MAX)
		return;
	g_Sid_Xssm_Dll_Infor[sid].sid=0;
	memset(g_Sid_Xssm_Dll_Infor[sid].CallerCode,0,sizeof(g_Sid_Xssm_Dll_Infor[sid].CallerCode));
	memset(g_Sid_Xssm_Dll_Infor[sid].CalleeCode,0,sizeof(g_Sid_Xssm_Dll_Infor[sid].CalleeCode));
	g_Sid_Xssm_Dll_Infor[sid].DeviceNumber=0;
	g_Sid_Xssm_Dll_Infor[sid].bBusy=FALSE;
}
/********************************************************************
函数名称：InitAllDllInfor
函数功能: 初始化所有记录会话的信息
参数定义: 无
返回定义: 无
创建时间: 2008-1-22 11:14
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

BOOL InitAllDllInfor()
{
	int i;
	for (i=0;i<MESSAGE_MAX;i++)
	{
		ClearSid((WORD)i);
	}
	return TRUE;
}
/********************************************************************
函数名称：RefreshDspMap
函数功能: 更新记录DSP的索引表
参数定义: 无
返回定义: 无
创建时间: 2008-1-11 13:12
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void RefreshDspMap(void)
{
	int i,j;
	int iModSeqID = 0;
	pDspMap.g_iTotalModule=0;
	pVoiceMap.total = 0;
	pPcmMap.total = 0;
	pUserMap.total = 0;
	pTrunkMap.total = 0;
	for (i = 0;i< MAX_DSP_MODULE_NUMBER_OF_XMS;i++ )
	{
		iModSeqID = 0;
		if ( AllDeviceRes[i].lFlag == 1 )
		{
			// DSP 模块
			AllDeviceRes[i].iSeqID = pDspMap.g_iTotalModule;
			pDspMap.pMapTable[pDspMap.g_iTotalModule]=i;
			pDspMap.g_iTotalModule++;
			// 语音通道
			for ( j = 0; j < AllDeviceRes[i].lVocNum; j ++ )
			{
				AllDeviceRes[i].pVoice[j].iSeqID = pVoiceMap.total;
				pVoiceMap.pTable[pVoiceMap.total].m_s8ModuleID = AllDeviceRes[i].pVoice[j].deviceID.m_s8ModuleID;
				pVoiceMap.pTable[pVoiceMap.total].m_s16ChannelID = AllDeviceRes[i].pVoice[j].deviceID.m_s16ChannelID;
				pVoiceMap.total ++;
			}
			
			// PCM通道
			for ( j = 0; j < AllDeviceRes[i].lPcmNum; j ++ )
			{
				AllDeviceRes[i].pPcm[j].iSeqID = pPcmMap.total;
				pPcmMap.pTable[pPcmMap.total].m_s8ModuleID = AllDeviceRes[i].pPcm[j].deviceID.m_s8ModuleID;
				pPcmMap.pTable[pPcmMap.total].m_s16ChannelID = AllDeviceRes[i].pPcm[j].deviceID.m_s16ChannelID;
				pPcmMap.total ++;
			}
			
			// 接口通道
			for ( j = 0; j < AllDeviceRes[i].lTrunkNum; j ++ )
			{
				AllDeviceRes[i].pTrunk[j].iSeqID = pTrunkMap.total;
				pTrunkMap.pTable[pTrunkMap.total].m_s8ModuleID = AllDeviceRes[i].pTrunk[j].deviceID.m_s8ModuleID;
				pTrunkMap.pTable[pTrunkMap.total].m_s16ChannelID = AllDeviceRes[i].pTrunk[j].deviceID.m_s16ChannelID;
				pTrunkMap.total ++;
				
				if ( AllDeviceRes[i].pTrunk[j].deviceID.m_s16DeviceSub != XMS_DEVSUB_UNUSABLE )
				{
					// 可用的通道
					AllDeviceRes[i].pTrunk[j].iModSeqID = iModSeqID;
					iModSeqID ++;
				}
				
				if (AllDeviceRes[i].pTrunk[j].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER ) 
				{
					AllDeviceRes[i].pTrunk[j].iUserSeqID = pUserMap.total;
					pUserMap.pTable[pUserMap.total].m_s8ModuleID = AllDeviceRes[i].pTrunk[j].deviceID.m_s8ModuleID;
					pUserMap.pTable[pUserMap.total].m_s16ChannelID = AllDeviceRes[i].pTrunk[j].deviceID.m_s16ChannelID;
					pUserMap.total ++;
				}
			}
		}
	}
}

/********************************************************************
函数名称：EvtHandler
函数功能: 回掉函数
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 9:58
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void EvtHandler(DJ_U32 esrParam)
{
	
	Acs_Dev_List_Head_t * pAcsDevList = NULL;
	Acs_Evt_t *			    pAcsEvt = NULL;
	//	Acs_CallControl_Data *pCallControl = NULL;
	TRUNK_STRUCT *pOneTrunk = NULL;
	DeviceID_t	*pDevID = NULL;
	pAcsEvt = (Acs_Evt_t *) esrParam;
	
	if(pAcsEvt == NULL)
		return;
	switch(pAcsEvt->m_s32EventType)
	{
		
	case XMS_EVT_QUERY_DEVICE:
		if ( ( cfg_iPartWork == 0 ) || (pAcsEvt->m_DeviceID.m_s8ModuleID == cfg_iPartWorkModuleID) )
		{
			pAcsDevList = ( Acs_Dev_List_Head_t *) FetchEventData(pAcsEvt);
			
			//收到设备变化的事件，增加/删除 该设备资源
			AddDeviceRes ( pAcsDevList );
		}
		break; 
	case XMS_EVT_QUERY_ONE_DSP_END:
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:
		if ( ( cfg_iPartWork == 0 ) || (pAcsEvt->m_DeviceID.m_s8ModuleID == cfg_iPartWorkModuleID) )
		{
			if ( pAcsEvt->m_s32EventType == XMS_EVT_QUERY_ONE_DSP_END )
			{
				AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 1;		// 该DSP可以用了
				OpenAllDevice_Dsp ( pAcsEvt->m_DeviceID.m_s8ModuleID );
			}
			else 
			{
				AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].lFlag = 0;		// 该DSP已经Remove完毕
			}
			RefreshDspMap();
			
		}
		break;
		
	case XMS_EVT_QUERY_DEVICE_END:	// 获取设备列表结束
		break;
		
	case XMS_EVT_OPEN_DEVICE:
		OpenDeviceOK ( &pAcsEvt->m_DeviceID );
		break;
		
	case XMS_EVT_CLOSE_DEVICE:		// 在删除设备之前，先发给应用程序CloseDevice事件；调用函数XMS_ctsCloseDevicey也会产生本事件
		CloseDeviceOK ( &pAcsEvt->m_DeviceID );
		break;
		
	case XMS_EVT_DEVICESTATE:
		HandleDevState ( pAcsEvt );
		break;
		
		//		case XMS_EVT_UNIFAILURE:
		//			// must handle this event in your real System
		//			break;
		//		case XMS_EVT_PLAY:
		//			break;
		//		case XMS_EVT_LINKDEVICE:
		//			break;
		//		case XMS_EVT_RECVIODATA:
		//			break;
	default:
		{
			if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH  )
			{
				pOneTrunk =  &M_OneTrunk(pAcsEvt->m_DeviceID);
			}
			else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE )
			{
				
				pDevID = &M_OneVoice(pAcsEvt->m_DeviceID).UsedDevID;
				if(pDevID==NULL)
					break;
				if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
				{
					pOneTrunk = &M_OneTrunk(*pDevID);
				}
				
			}
			if(pOneTrunk==NULL)
				break;
			if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
				UserHandle(esrParam);
			else
				TrunkHandle(esrParam);
			
		}
		break;
	}
}
/********************************************************************
函数名称：InitAllDevice
函数功能: 初始清空结构
参数定义: 无
返回定义: 无
创建时间: 2007-12-13 16:25
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

BOOL InitAllDevice()
{
	int i;
	for(i=0;i<MAX_DSP_MODULE_NUMBER_OF_XMS;i++)
	{
		memset ( &AllDeviceRes[i], 0, sizeof(TYPE_XMS_DSP_DEVICE_RES_DEMO) );
	}
	return TRUE;
}

/********************************************************************
函数名称：ExitSystem
函数功能: 退出与ACS系统的连接 
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 9:58
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL ExitSystem()
{
	int r;
	r = XMS_acsCloseStream ( g_acsHandle, NULL );
	YF_LOG_SMM "[ExitSystem],ACC->Keygoe:disconnect to KeyGoe system");
	FreeAllDeviceRes();
	return TRUE;
}

/********************************************************************
函数名称：AddDeviceRes_Pcm
函数功能: 添加PCM通道设备
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 10:00
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void AddDeviceRes_Pcm(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	DeviceID_t	 *pDev;
	s32Num = pAcsDevList->m_s32DeviceNum;
	
	if ( (AllDeviceRes[s8DspModID].lPcmNum == 0) && (s32Num > 0) )		// 新增加的资源
	{
		// 
		AllDeviceRes[s8DspModID].pPcm = (PCM_STRUCT*)malloc(sizeof(PCM_STRUCT)*s32Num);
		if( !AllDeviceRes[s8DspModID].pPcm )
		{
			AllDeviceRes[s8DspModID].lPcmNum = 0;
			AllDeviceRes[s8DspModID].lPcmOpened = 0;
			
			
		}
		else
		{
			memset ( AllDeviceRes[s8DspModID].pPcm, 0, sizeof(PCM_STRUCT)*s32Num );
			AllDeviceRes[s8DspModID].lPcmNum =(WORD)s32Num;
			AllDeviceRes[s8DspModID].lPcmOpened = 0;
			
			
			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
			for ( i = 0; i < s32Num; i ++ )
			{
				AllDeviceRes[s8DspModID].pPcm[i].deviceID = pDev[i];
				AllDeviceRes[s8DspModID].pPcm[i].bOpenFlag = FALSE;
			}
		}
	}
	else if ( (AllDeviceRes[s8DspModID].lPcmNum > 0) && (s32Num == 0) )		// 删除该资源
	{
		// 如果有设备没有关闭，则等同于关闭事件
		for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i ++ )
		{
			if ( AllDeviceRes[s8DspModID].pPcm[i].bOpenFlag != FALSE )
				CloseDeviceOK ( &AllDeviceRes[s8DspModID].pPcm[i].deviceID );
		}
		
		AllDeviceRes[s8DspModID].lPcmNum = 0;
		AllDeviceRes[s8DspModID].lPcmOpened = 0;
		
		free(AllDeviceRes[s8DspModID].pPcm);
		AllDeviceRes[s8DspModID].pPcm = NULL;
	}
	
}

void CloseTrunkDevice(TRUNK_STRUCT *pOneTrunk )
{
	pOneTrunk->State= TRK_WAITOPEN;
	XMS_ctsCloseDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
	
}

void CloseVoiceDevice(VOICE_STRUCT *pOneVoice )
{
	XMS_ctsCloseDevice(g_acsHandle, &pOneVoice->deviceID, NULL );
	
}

void ClosePcmDevice(PCM_STRUCT *pOnePcm )
{
	XMS_ctsCloseDevice(g_acsHandle, &pOnePcm->deviceID, NULL );
}

void CloseBoardDevice(DeviceID_t *pBoardDevID )
{
	XMS_ctsCloseDevice(g_acsHandle, pBoardDevID, NULL );
}

void OpenAllDevice_Dsp(DJ_S8 s8DspModID )
{
	int			 i;
	
	AllDeviceRes[s8DspModID].bErrFlag = FALSE;
	AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_NONE;
	
	// Open Board
	OpenBoardDevice ( s8DspModID );
	
	// pVoice
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		OpenVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
	}
	
	// pPcm
	for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
	{
		OpenPcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
	}
	
	// pTrunk
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		OpenTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}
}

// --------------------------------------------------------------------------------
void OpenTrunkDevice(TRUNK_STRUCT *pOneTrunk )
{
	
	if ( pOneTrunk->State == TRK_WAITOPEN )		// 还没有Open
	{
		XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
		
	}
}

void OpenVoiceDevice(VOICE_STRUCT *pOneVoice )
{
	
	if ( pOneVoice->State == VOC_WAITOPEN )		// 还没有Open
	{
		XMS_ctsOpenDevice ( g_acsHandle, &pOneVoice->deviceID, NULL );
		
	}
}

void OpenPcmDevice(PCM_STRUCT *pOnePcm )
{
	if ( pOnePcm->bOpenFlag == FALSE )		// 还没有Open
	{
		XMS_ctsOpenDevice ( g_acsHandle, &pOnePcm->deviceID, NULL );
		
	}
}

void OpenBoardDevice(DJ_S8 s8DspModID )
{
	
	if ( AllDeviceRes[s8DspModID].bOpenFlag == FALSE )	// 还没有Open
	{
		XMS_ctsOpenDevice ( g_acsHandle, &AllDeviceRes[s8DspModID].deviceID, NULL );
		
	}
}
void	PrepareForCallerID ( TRUNK_STRUCT *pOneTrunk )
{
	DeviceID_t FreeVocDeviceID;
	
	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_TRUNK )
		return;
	
	if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
	{
		pOneTrunk->VocDevID = FreeVocDeviceID;
		
		M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
		
		DualLinkDevice( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );
		SetGtD_AnalogTrunk(&pOneTrunk->VocDevID);
	}
}

void OpenDeviceOK(DeviceID_t *pDevice )
{
	TRUNK_STRUCT	*pOneTrunk;
	void* p = NULL;
//	DeviceID_t FreeVocDeviceID;
	CmdParamData_AnalogTrunk_t cmdAnalogTrunk;
	DJ_U16 u16ParamType = ANALOGTRUNK_PARAM_UNIPARAM ;
	DJ_U16 u16ParamSize = sizeof(CmdParamData_AnalogTrunk_t);
	RetCode_t r ;
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )//DSP
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = TRUE;
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(*pDevice);
		
		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		
		// init this Device: Trunk
		InitTrunkChannel ( pOneTrunk );
		
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		
		// modify the count
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened ++;
		
		if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
		{
			
			// Set AnalogTrunk
			
			
			
			//			memset(&cmdAnalogTrunk,0,sizeof(cmdAnalogTrunk));
			//	cmdAnalogTrunk.m_u8CallerIdOption = XMS_ANALOG_TRUNK_CALLERID_FSK;;
			memset(&cmdAnalogTrunk,0,sizeof(cmdAnalogTrunk));
			cmdAnalogTrunk.m_u16CallInRingCount = 3;			// 3 times RingBack
			cmdAnalogTrunk.m_u16CallInRingTimeOut = 6000;		// 60 seconds
			//			cmdAnalogTrunk.m_u8DialtoneDetect = XMS_ANALOG_TRUNK_DIALTONE_DETECT;
			//			cmdAnalogTrunk.m_u8CallOutMethod = XMS_ANALOG_TRUNK_CALLOUT_NORMAL_PROGRESS; //XMS_ANALOG_TRUNK_CALLOUT_POLARITY_REVERSE;
			p = (void*)&cmdAnalogTrunk;
			r = XMS_ctsSetParam( g_acsHandle, & pOneTrunk->deviceID, u16ParamType, u16ParamSize, (DJ_Void *)p );
			PrepareForCallerID ( pOneTrunk );
//			if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
//			{
//				//		cmdAnalogTrunk.m_VocDevID = FreeVocDeviceID;
//				
//				//指定绑定关系
//				pOneTrunk->VocDevID = FreeVocDeviceID;		
//				M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID;	
//				r = XMS_ctsSetParam( g_acsHandle, & pOneTrunk->deviceID, u16ParamType, u16ParamSize, (DJ_Void *)p );
//				DualLinkDevice( &FreeVocDeviceID, &pOneTrunk->deviceID );
//				SetGtD_AnalogTrunk(&FreeVocDeviceID);
//			}
		}
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		M_OneVoice(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		
		// init this Device: Voice
		Change_Voc_State ( &M_OneVoice(*pDevice), VOC_FREE);
		
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		
		// Build Index
		if ( AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened == 0 )
		{
			BuildVoiceIndex ( pDevice );
		}
		
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		M_OnePcm(*pDevice).deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		M_OnePcm(*pDevice).bOpenFlag = TRUE;
		
		// init the Device: Pcm
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened ++;
		
	}
}
/********************************************************************
函数名称：BuildVoiceIndex
函数功能: 创建语音索引
参数定义: pVocDevID：语音设备
返回定义: 无
创建时间: 2008-1-3 9:58
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void BuildVoiceIndex(DeviceID_t	*pVocDevID )
{
	RetCode_t	r;
	char		FileName[256];
	int			i;
	PlayProperty_t  playProperty;
	r = XMS_ctsInitPlayIndex ( g_acsHandle, pVocDevID, NULL );
	if ( r < 0 )
	{
		return ;
	}
	memset(&playProperty,0,sizeof(playProperty));
	playProperty.m_u16PlayType = XMS_BUILD_INDEX_RAM;
	playProperty.m_u8SrcMode=XMS_SRC_8K;
	playProperty.m_u8DecodeType=XMS_Ulaw_type;
	playProperty.m_u32StopMode = XMS_Normal_Stop;
	for(i=0;i<pToneIndex.g_total;i++)
	{
		
		sprintf(FileName,"C:\\yfcomm\\Voc\\%s",pToneIndex.pIndexFIle[i].filename);
		strcpy ( playProperty.m_s8PlayContent,FileName /*pToneIndex.pIndexFIle[i].filename*/ );
		r=XMS_ctsBuildPlayIndex(g_acsHandle,pVocDevID,&playProperty,NULL);
	}
}

/********************************************************************
函数名称：SearchOneFreeVoice
函数功能:查找一个空闲的语音设备，尽量采用最多空闲VOICE设备优先原则，
查找中继设备和语音设备编号相等时绑定
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 10:26
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

int	SearchOneFreeVoice(TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID )
{
	long			lNowMostFreeNum;
	DJ_S8			s8ModID;
	DJ_S16			s16ChID;
	int				i;
	//	static	int		iLoopStart = 0;
	VOICE_STRUCT	*pOneVoice;
	DJ_S8			s8SearchModID;
	
	if(pOneTrunk == NULL)
		return -1;
	s8ModID = pOneTrunk->deviceID.m_s8ModuleID;
	
	if ( cfg_iVoiceRule == 0 )			// Fix relationship between Trunk & Voice
	{
		s16ChID = pOneTrunk->deviceID.m_s16ChannelID;
		
		i = pOneTrunk->iModSeqID;
		
		if ( i < AllDeviceRes[s8ModID].lVocNum )
		{
			pOneVoice = &AllDeviceRes[s8ModID].pVoice[i];
			if ( pOneVoice->State != VOC_WAITOPEN )
			{
				*pFreeVocDeviceID = pOneVoice->deviceID;
				
				// 占用该voice device 
				Change_Voc_State ( pOneVoice, VOC_USED);
				AllDeviceRes[s8ModID].lVocFreeNum--;
				return i;
			}
		}
		
		return -1;
	}
	
	if ( cfg_iVoiceRule == 1 )			// Search in Same Module(dsp),
	{
		s8SearchModID = s8ModID;
	}
	else		// Search in Most free resource module(dsp)
	{
		s8SearchModID = -1;
		lNowMostFreeNum = -1;
		for ( i = 0; i < pDspMap.g_iTotalModule; i ++ )
		{
			if ( AllDeviceRes[pDspMap.pMapTable[i]].lVocFreeNum > lNowMostFreeNum )
			{
				s8SearchModID = pDspMap.pMapTable[i];
				lNowMostFreeNum = AllDeviceRes[pDspMap.pMapTable[i]].lVocFreeNum;
			}
		}
	}
	
	for ( i = 0; i < AllDeviceRes[s8SearchModID].lVocNum; i ++ )
	{
		pOneVoice = &AllDeviceRes[s8SearchModID].pVoice[i];
		if ( pOneVoice->State == VOC_FREE )
		{
			*pFreeVocDeviceID = pOneVoice->deviceID;
			
			// 占用该voice device 
			Change_Voc_State ( pOneVoice, VOC_USED);
			AllDeviceRes[s8SearchModID].lVocFreeNum--;
			return i;
		}
	}
	
	return -1;
	
	
}

/********************************************************************
函数名称：FreeOneFreeVoice
函数功能: 将语音设备的状态设为空闲
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 11:20
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

int	FreeOneFreeVoice(DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8	s8ModID;
	if(pFreeVocDeviceID == NULL)
		return -1;
	s8ModID = pFreeVocDeviceID->m_s8ModuleID;
	if ( AllDeviceRes[s8ModID].lFlag == 1 )
	{
		Change_Voc_State ( &M_OneVoice(*pFreeVocDeviceID), VOC_FREE);
		
		AllDeviceRes[s8ModID].lVocFreeNum++;
		return	0;		// OK
	}
	
	return -1;			// invalid VocDeviceID
}
/********************************************************************
函数名称：Change_UserState
函数功能: 用户中继状态设置
参数定义: 无
返回定义: 
创建时间: 2008-1-24 15:33
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void Change_UserState (TRUNK_STRUCT *pOneTrunk, enum USER_STATE NewState)
{
	if(pOneTrunk == NULL)
		return;
	pOneTrunk->UserState = NewState;
	// 检查是否准备开始删除DSP单元
	if ( (AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == USER_FREE) )
	{
		pOneTrunk->UserState = USER_WAIT_REMOVE;
	}
}
/********************************************************************
函数名称：Change_Voc_State
函数功能: 语音设备状态设置
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 10:32
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void Change_Voc_State(VOICE_STRUCT *pOneVoice, enum VOICE_STATE NewState )
{
	if(pOneVoice == NULL)
		return;
	pOneVoice->State = NewState;
	
	// 检查是否准备开始删除DSP单元
	if ( (AllDeviceRes[pOneVoice->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == VOC_FREE) )
	{
		pOneVoice->State = VOC_WAIT_REMOVE;
	}
}
/********************************************************************
函数名称：Change_State
函数功能: 中继设备状态设置
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 10:40
函数作者: 刘定文
注意事项: 无	
*********************************************************************/


void Change_State(TRUNK_STRUCT *pOneTrunk, enum TRUNK_STATE NewState )
{
	if(pOneTrunk == NULL)
		return;
	pOneTrunk->State = NewState;
	
	
	// 检查是否准备开始删除DSP单元
	if ( (AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START)
		&& (NewState == TRK_FREE) )
	{
		pOneTrunk->State = TRK_WAIT_REMOVE;
	}
}
void	InitUserChannel ( TRUNK_STRUCT *pOneUser )
{
	Change_UserState ( pOneUser, USER_FREE );
	
	InitDtmfBuf ( pOneUser );
	
	memset ( &pOneUser->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配Voc Device
	memset ( &pOneUser->LinkDevID, 0, sizeof(DeviceID_t) );	// 0表示还没有分配LinkDevID
	
	
}
/********************************************************************
函数名称：InitTrunkChannel
函数功能: 初始化中继通道
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 11:22
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void InitTrunkChannel(TRUNK_STRUCT *pOneTrunk )
{
	if(pOneTrunk == NULL)
		return;
	Change_State ( pOneTrunk, TRK_FREE );
	
	pOneTrunk->CallerCode[0]=0;
	pOneTrunk->CalleeCode[0]=0;
	
	InitDtmfBuf ( pOneTrunk );
	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// -1表示还没有分配Voc Device
}
/********************************************************************
函数名称：InitDtmfBuf
函数功能: 初始化中继设备的DTMF长度及内容
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 10:55
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void InitDtmfBuf(TRUNK_STRUCT *pOneTrunk)
{
	if(pOneTrunk == NULL)
		return;
	pOneTrunk->DtmfCount = 0;
	pOneTrunk->DtmfBuf[0] = 0; 
}

/********************************************************************
函数名称：ResetTrunk
函数功能: 更新中继设备
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 11:24
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void ResetTrunk(TRUNK_STRUCT *pOneTrunk )
{
	if(pOneTrunk == NULL)
		return;
	StopPlayFile ( &pOneTrunk->VocDevID );
	// free the used Voice Resource
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		DualUnLinkDevice (&pOneTrunk->VocDevID, &pOneTrunk->deviceID );
		
		FreeOneFreeVoice (&pOneTrunk->VocDevID );
		Change_Voc_State(&M_OneVoice(pOneTrunk->VocDevID),VOC_FREE);
		if(AllDeviceRes[(pOneTrunk->VocDevID).m_s8ModuleID].pVoice!=NULL)
			memset (&M_OneVoice(pOneTrunk->VocDevID).UsedDevID,0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
		memset (&pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
	}
	InitTrunkChannel( pOneTrunk );
	PrepareForCallerID ( pOneTrunk );
}
/********************************************************************
函数名称：StopPlayFile
函数功能: 停止语音播放
参数定义: 无
返回定义: 无
创建时间: 2008-1-3 11:26
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

int StopPlayFile(DeviceID_t *pVocDevID )
{
	RetCode_t		r;
	ControlPlay_t	controlPlay;
	
	controlPlay.m_u16ControlType = XMS_STOP_PLAY;
	
	r = XMS_ctsControlPlay ( g_acsHandle, pVocDevID, &controlPlay, NULL );
	
	return r;
}
BOOL GetCharString(char *pchar,BYTE *pData,int len)
{
	int i;
	char cc=' ';
	for(i=0;i<len;i++)
	{
		
		strncat(pchar+2*i,pData+i,1);
		strncat(pchar+2*i+1,&cc,1);
	}
	return FALSE;
}

void BCD_TO_ASCII1(unsigned char *cSource, unsigned char *cResult, unsigned int iSourceLen, unsigned int iResultLen)
{
    unsigned int i=0;
    int j=0;
    for(i=0;i<iSourceLen;i++) 
    {
        cResult[j+2]=32;
        cResult[j+1] = cSource[i]%16 + '0';
        if(cResult[j+1] > 9+'0')
        {
            cResult[j+1] = cResult[j+1]+7;
        }
        cResult[j] = cSource[i]/16+'0';
        if(cResult[j] > 9+'0')
        {
            cResult[j] = cResult[j]+7;
        }
        j+=3;
    }
    cResult[iResultLen] = '\0';
	/* printf(":%s\n", cResult);*/
}  
void ResetUser ( TRUNK_STRUCT *pOneUser)
{
	TRUNK_STRUCT		*pLinkTrunk = NULL;
	
	if ( pOneUser->VocDevID.m_s16DeviceMain != 0 )
	{
		DualUnLinkDevice ( &pOneUser->VocDevID, &pOneUser->deviceID );
		
		FreeOneFreeVoice (  &pOneUser->VocDevID );
		
	}
	
	if ( pOneUser->LinkDevID.m_s16DeviceMain != 0 )
	{
		DualUnLinkDevice ( &pOneUser->LinkDevID, &pOneUser->deviceID );
		
		pLinkTrunk = &M_OneTrunk(pOneUser->LinkDevID);
		
		memset ( &pLinkTrunk->LinkDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配LinkDevID
		XMS_ctsClearCall ( g_acsHandle, &pLinkTrunk->deviceID, 0, NULL );
	}
	
	InitUserChannel ( pOneUser );
}
/********************************************************************
created:	2011/01/04   14:51
filename: 	E:\苏州卡易付系统\卡易付最新\xvapl源代码\AccDll\AccDll.c
file path:	E:\苏州卡易付系统\卡易付最新\xvapl源代码\AccDll
file base:	AccDll
file ext:	c
author:		ldw

  purpose:坐席事件响应 	
*********************************************************************/

void UserHandle(DJ_U32 esrParam /*TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt*/ )
{
	
	ACS_Hdl_EVENT pHdlEvent;
	TRUNK_STRUCT *pOneTrunk = NULL;
	Acs_Evt_t *pAcsEvt = NULL;
	Acs_IO_Data	*pIOData = NULL;
	char		*p;
	WORD  EventType;
	Acs_CallControl_Data *pCallControl = NULL;
	DeviceID_t	*pDevID = NULL;
	Acs_UniFailure_Data * pAcsUniFailure = NULL;
	Acs_MediaProc_Data *pMediaProc_Data = NULL;
	Acs_IO_Data *pIo_Data = NULL;
	TID * tId;
	COPYDATASTRUCT cds;
	
	
	
	int len;
	pAcsEvt = (Acs_Evt_t *)esrParam;
	if(pAcsEvt == NULL)
		return ;
	
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH  )
	{
		pOneTrunk =  &M_OneTrunk(pAcsEvt->m_DeviceID);
	}
	else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		
		pDevID = &M_OneVoice(pAcsEvt->m_DeviceID).UsedDevID;
		
		if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
		{
			pOneTrunk = &M_OneTrunk(*pDevID);
		}
	}
	
	switch(pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_OPEN_STREAM://硬件连接成功
		{
			EventType = evtXSSM_LinkStatus;
		}
		break;
	case XMS_EVT_UNIFAILURE://硬件连接失败
		{
			EventType = evtXSSM_FailStatus;
			pAcsUniFailure = (Acs_UniFailure_Data *) FetchEventData(pAcsEvt);
			if(pAcsUniFailure->m_s32AcsEvtErrCode!=ACSERR_LINKBROKEN)
				return;
		}
		break;
	case XMS_EVT_CALLOUT ://呼出	
		{
			EventType = evtXSSM_CallOut;
			
		}
		break;
	case XMS_EVT_CALLIN ://呼入
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			if(pCallControl != NULL)
			{
				{
					len = strlen(pCallControl->m_s8CallingNum);
					if((strlen(pCallControl->m_s8CallingNum))>0)
					{
						{
							if(len<=8)
							{
								strcpy(pHdlEvent.CallingNum,pCallControl->m_s8CallingNum);
							}
							else
							{
								strcpy(pHdlEvent.CallingNum,pCallControl->m_s8CallingNum+len-8);
							}
						}
					}
					else
					{
						strcpy(pHdlEvent.CallingNum,"888888");
					}
					len = strlen(pCallControl->m_s8CalledNum);
					if(len>0)
					{
						if(len<=8)
						{
							strcpy(pHdlEvent.CalledNum,pCallControl->m_s8CalledNum);
						}
						else
						{
							strcpy(pHdlEvent.CalledNum,pCallControl->m_s8CalledNum+len-8);
						}
					}
					else
					{
						strcpy(pHdlEvent.CalledNum,"888888");
					}
				}
				
			}
			if(pOneTrunk!=NULL)
			{
				YF_TRACE -1, "XMS_EVT_CALLIN","Keygoe->ACC: 呼入,modID=%d,channID=%d,calledNum=%s,callingNum=%s",pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID,pHdlEvent.CalledNum,pHdlEvent.CallingNum);
			}
			else
			{
				YF_TRACE -1, "XMS_EVT_CALLIN","Keygoe->ACC: 呼入,pOneTrunk=null,calledNum=%s,callingNum=%s",pHdlEvent.CalledNum,pHdlEvent.CallingNum);
			}
			EventType = evtXSSM_CallIn;
		}
		break;
		
	case XMS_EVT_CLEARCALL ://挂机
		ResetUser(pOneTrunk);
		EventType = evtXSSM_ClearCall;
		break;
	case XMS_EVT_JOINTOCONF ://参加会议
		EventType = evtXSSM_JoinConf;
		break;
	case XMS_EVT_LEAVEFROMCONF ://离开会议
		EventType = evtXSSM_LeaveFromConf;
		break;
	case XMS_EVT_CLEARCONF ://清除会议
		EventType = evtXSSM_ClearConf;
		break;
	case XMS_EVT_PLAY ://放音完成
		EventType = evtXSSM_Play;
		pAcsUniFailure = (Acs_UniFailure_Data *) FetchEventData(pAcsEvt);
		if(pAcsUniFailure->m_s32AcsEvtErrCode>2)
			return;
		break;
		//	case XMS_EVT_INITINDEX :
		//		break;
		//	case XMS_EVT_BUILDINDEX :
		//		break;
	case XMS_EVT_CONTROLPLAY ://控制放音完成
		EventType = evtXSSM_ControlPlay;
		break;
	case XMS_EVT_RECORD ://录音完成
		EventType = evtXSSM_Record;
		break;
	case XMS_EVT_CONTROLRECORD ://控制录音命令完成
		EventType = evtXSSM_ControlRecord;
		break;
	case XMS_EVT_SENDFAX ://发送传真
		EventType = evtXSSM_SendFax;
		break;
	case XMS_EVT_RECVFAX ://接收传真
		EventType = evtXSSM_RecvFax;
		break;
	case XMS_EVT_SENDIODATA ://发送数据
		EventType = evtXSSM_SendIoData;
		{
			if(pIOData!=NULL)
				YF_TRACE  -1,"","send data len=%d",pIOData->m_u16IoDataLen);
		}
		break;
	case XMS_EVT_RECVIODATA ://接收数据
		pIo_Data = (Acs_IO_Data *)FetchEventData(pAcsEvt); 
		p = (char *)FetchIOData(pAcsEvt);
		if((pOneTrunk!=NULL)&&(pIo_Data != NULL))
		{
			YF_TRACE -10, "XMS_EVT_RECVIODATA","接收到数据,modID=%d,channelID=%d,len=%d,type=%d",
				pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID, pIo_Data->m_u16IoDataLen, pIo_Data->m_u16IoType);
		}
		else if(pOneTrunk!=NULL)
		{
			YF_TRACE -10, "XMS_EVT_RECVIODATA","接收到数据,modID=%d,channelID=%d,io_data=NULL",
				pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);
		}
		else if(pIo_Data!=NULL)
		{
			YF_TRACE -10, "XMS_EVT_RECVIODATA","接收到数据,pOneTrunk=NULL,len=%d,type=%d",
				pIo_Data->m_u16IoDataLen, pIo_Data->m_u16IoType);
		}
		else 
		{
			YF_TRACE -10, "XMS_EVT_RECVIODATA","接收到数据,pOneTrunk=NULL,io_data=NULL");
		}
		
		//////////////////////////////////////////////////////////////////////////
		
		if ( ( pIo_Data->m_u16IoType == XMS_IO_TYPE_GTG ) //
			&& ( pIo_Data->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);
			if(*p == 'b')//爆破音检测
				return;
			if ( *p == 'I' || *p == 'J' || *p == 'K' )//忙音
			{
				XMS_ctsResetDevice(g_acsHandle,&pOneTrunk->deviceID,NULL);
				XMS_ctsClearCall(g_acsHandle,&pOneTrunk->deviceID,0,NULL);
				ResetUser(pOneTrunk);
				return;
			}
			
		}
		//////////////////////////////////////////////////////////////////////////
		EventType = evtXSSM_RecvIoData;
		break;
	case XMS_EVT_BUILDINDEX:
		return ;
		break;
	default:
		return;
		break;
	}
	if(pOneTrunk== NULL)
	{
		pHdlEvent.sid = MESSAGE_MAX + 1;
		pHdlEvent.DeviceNumber = 0;
		YF_TRACE  -1,"","pOneTrunk== NULL，sid overflow");
	}
	else
	{
		pHdlEvent.sid = SearchSid(pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);
		if((pHdlEvent.sid>=MESSAGE_MAX)  && (EventType != evtXSSM_CallIn))
		{
			YF_TRACE -1,"","searchsid fail");
		}
		pHdlEvent.DeviceNumber = pOneTrunk->deviceID.m_s8ModuleID*256*256+pOneTrunk->deviceID.m_s16ChannelID;
	}
	
	pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
	p = (char *)FetchIOData(pAcsEvt);
	pHdlEvent.event = EventType;
	if (pIOData)
	{
		pHdlEvent.datalen = pIOData->m_u16IoDataLen;
	}
	else
	{
		pHdlEvent.datalen = 0;
	}
	if(pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA)
	{
		memcpy(pHdlEvent.ioData,p,pHdlEvent.datalen);
		//		pHdlEvent.ioData = p;
	}
	tId = GetTID(MID_ACC);
	if(tId!=NULL)
	{
		cds.dwData = 0;
		cds.cbData = sizeof(ACS_Hdl_EVENT);
		cds.lpData = &pHdlEvent;
		SendMessage(tId->hWnd,WM_COPYDATA,0,(LPARAM)&cds);
	}
	else
		YF_TRACE pHdlEvent.sid,"", "TID寻址失败，消息丢弃");
	
	
}
char My_GetGtdCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	char		*p;

	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Event*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);

		if ( ( pIOData->m_u16IoType== XMS_IO_TYPE_PVD)//XMS_IO_TYPE_GTG ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);
			return *p;
		}
	}

	return -1;	// not a good GTD
}
unsigned char *My_GetFskCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	unsigned char		*p;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Evemt*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
		
		if ( ( pIOData->m_u16IoType== XMS_IO_TYPE_FSK ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (unsigned char *)FetchIOData(pAcsEvt);
			
			return p;
		}
	}
	
	return NULL;	// not a good FSK
}
void HandleMultiple ( char far *IDStr )
{
	int i;
	char TmpIDStr[80];
	char far *p;
	char c;
	
	p = IDStr;
	
	i = 0;
	while ( *p != 0 ) 
	{
		c = *p;
		if ( (c==1) || (c==2) || (c==4) || (c==7) || (c==8) ) 
		{
			p += 2;
		}
		else 
		{
			TmpIDStr[i] = *p;
			i ++;
			p ++;
		}
	}
	TmpIDStr[i] = 0;
	strcpy ( IDStr, TmpIDStr );
}
// end of multiple

int	ConvertRawFskToCallerID ( unsigned char *RawFSKStr, char *IDStr, int IDStrBufLen )
{
	WORD			count;
	WORD			i,j;
	WORD			pos,len;
	unsigned char	ChkSum;
	
	count	= strlen ( (char *)RawFSKStr );
	strcpy ( IDStr, "" );
	
	if ( count == 0 )
	{
		return -1;		// not a good Caller ID
	}
	
	// find the Format
	for ( i = 0; i < count; i ++ ) 
	{
		if ( RawFSKStr[i] == 0x04 )		// single format
		{
			break;
		}
		
		if ( RawFSKStr[i] == 0x80 ) {		// multiple format
			break;
		}
	}
	
	if ( (i == count) || (i==count-1) )
	{
		return -1;		// not a good Caller ID
	}
	
	// start fetch the Caller ID
	pos = i;
	len = RawFSKStr[pos+1];
	
	// "count-pos-2"    is the length of now received
	for ( i = pos+2,j = 0; (i<(count-1)) && (j<len); i ++,j++ )
	{
		IDStr[j] = RawFSKStr[i] & 0x7F;
		if ( j == IDStrBufLen - 1 )				// it will be overflow
			break;
	}
	IDStr[j] = 0;
	
	if ( (count-pos-3) < len  )
	{
		return -1;		// not a good Caller ID
	}
	
	ChkSum = 0;
	for ( i = pos; i < pos+len+3; i ++ )
		ChkSum += RawFSKStr[i];
	
	// multiple format
	if ( RawFSKStr[pos] == 0x80 ) 
	{
		HandleMultiple ( IDStr );
	}
	// end of multiple
	
	if ( ChkSum == 0 )
		return	0;		// OK
	else
		return -1;		// Fail
}
int	ConvertCallerIDToRawFsk (  char *IDStr, unsigned char *RawFSKStr, int FSKStrBufLen )
{
	int				pos, len;
	int				i;
	unsigned char	ChkSum;
	char			TmpDateTimeStr[16];
	
	// get the Tmp Data Time
	SYSTEMTIME		ST;
	GetLocalTime ( &ST );
	sprintf ( TmpDateTimeStr, "%02d%02d%02d%02d", ST.wMonth, ST.wDay, ST.wHour, ST.wMinute );
	
	pos = 0;
	len = strlen(TmpDateTimeStr) + strlen(IDStr);
	
	RawFSKStr[pos++] = 0x04;			// we use single format
	RawFSKStr[pos++] = len;	
	
	for ( i = 0; i < (int)strlen(TmpDateTimeStr); i ++ )
	{
		RawFSKStr[pos++] = TmpDateTimeStr[i];
		if ( pos >= (FSKStrBufLen-1) )		// overflow
			break;
	}
	
	for ( i = 0; i < (int)strlen(IDStr); i ++ )
	{
		RawFSKStr[pos++] = IDStr[i];
		if ( pos >= (FSKStrBufLen-1) )		// overflow
			break;
	}
	
	// calculate the CheckSum
	ChkSum = 0;
	for ( i = 0; i < pos; i ++ )
		ChkSum += RawFSKStr[i];
	
	ChkSum = 0 - ChkSum;
	
	RawFSKStr[pos++] = ChkSum;
	
	return pos;		// OK
}
void WriteLog(BOOL status)
{
	SYSTEMTIME     Clock;
	FILE	    *fp=NULL;
	return;
	fp = fopen ("C:\\yfcomm\\log\\acc.log","a");

	if ( fp == NULL )
		return ;

	GetLocalTime(&Clock);

 
	fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d acc have call in message,staus=%d\n",
			Clock.wYear,Clock.wMonth, Clock.wDay,
		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,status);
	fclose (fp);
}
void AccWriteLog_Trace(char *pChar)
{
	SYSTEMTIME     Clock;
	FILE	    *fp=NULL;
	char pFile[36];
	GetLocalTime(&Clock);
	sprintf(pFile,"C:\\yfcomm\\log\\acc%04d%02d%02d.log",Clock.wYear,Clock.wMonth, Clock.wDay);
	fp = fopen (pFile,"a");

	if ( fp == NULL )
		return ;



 
	fprintf (fp, "%04d-%02d-%02d %02d:%02d:%02d:%03d:%s\n",
			Clock.wYear,Clock.wMonth, Clock.wDay,
		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,pChar);
	fclose (fp);
}
/********************************************************************
函数名称：TrunkHandle
函数功能: 消息处理
参数定义: esrParam：指针地址
返回定义: 无
创建时间: 2008-1-25 10:44
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void TrunkHandle(DJ_U32 esrParam /*TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt*/ )
{
	
	ACS_Hdl_EVENT pHdlEvent;
	//	char vvvvv[2000];
	//	ACS_Hdl_EVENT *ppHdlEvent =NULL;
	TRUNK_STRUCT *pOneTrunk = NULL;
	Acs_Evt_t *pAcsEvt = NULL;
	Acs_IO_Data	*pIOData = NULL;
	char		*p;
	WORD  EventType;
	char pChar[100];
	Acs_CallControl_Data *pCallControl = NULL;
	DeviceID_t	*pDevID = NULL;
	Acs_UniFailure_Data * pAcsUniFailure = NULL;
	Acs_MediaProc_Data *pMediaProc_Data = NULL;
	Acs_IO_Data *pIo_Data = NULL;
	TID * tId;
	COPYDATASTRUCT cds;
	char					TmpGtd;
	
	
	int len;
	pAcsEvt = (Acs_Evt_t *)esrParam;
	if(pAcsEvt == NULL)
		return ;
	
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH  )
	{
		pOneTrunk =  &M_OneTrunk(pAcsEvt->m_DeviceID);
	}
	else if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		
		pDevID = &M_OneVoice(pAcsEvt->m_DeviceID).UsedDevID;
		
		if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
		{
			pOneTrunk = &M_OneTrunk(*pDevID);
		}
	}
	if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
	{
		TmpGtd = My_GetGtdCode ( pAcsEvt );
		if ( TmpGtd  != -1 ) 
		{
			if(TmpGtd == 'b')//爆破音检测
				return;
			if ( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' )	// Busy Tone
			{
				XMS_ctsResetDevice(g_acsHandle,&pOneTrunk->deviceID,NULL);
				XMS_ctsClearCall(g_acsHandle,&pOneTrunk->deviceID,0,NULL);
				ResetTrunk(pOneTrunk);
				return;
			}
		}
	}
	switch(pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_OPEN_STREAM://硬件连接成功
		{
			EventType = evtXSSM_LinkStatus;
		}
		break;
	case XMS_EVT_UNIFAILURE://硬件连接失败
		{
			EventType = evtXSSM_FailStatus;
			pAcsUniFailure = (Acs_UniFailure_Data *) FetchEventData(pAcsEvt);
			if(pAcsUniFailure->m_s32AcsEvtErrCode!=ACSERR_LINKBROKEN)
				return;
		}
		break;
	case XMS_EVT_CALLOUT ://呼出	
		{
			EventType = evtXSSM_CallOut;
			
		}
		break;
	case XMS_EVT_CALLIN ://呼入
		{
			if ( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
			{
				
				len = strlen(pOneTrunk->CallerCode);
				if(len>0)
				{
					
					if(len<=8)
					{
						strcpy(pHdlEvent.CallingNum,pOneTrunk->CallerCode);
					}
					else
					{
						strcpy(pHdlEvent.CallingNum,pOneTrunk->CallerCode+len-8);
					}
					
				}
				else
				{
					strcpy(pHdlEvent.CallingNum,"888888");
				}
				strcpy(pHdlEvent.CalledNum,"888888");
				DualUnLinkDevice( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );
				
				FreeOneFreeVoice (  &pOneTrunk->VocDevID );
				
				memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
				memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Device
				
			}

			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			if(pCallControl != NULL)
			{
				if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_TRUNK )
				{
					
					
					len = strlen(pCallControl->m_s8CallingNum);
					if((strlen(pCallControl->m_s8CallingNum))>0)
					{
						{
							if(len<=8)
							{
								strcpy(pHdlEvent.CallingNum,pCallControl->m_s8CallingNum);
							}
							else
							{
								strcpy(pHdlEvent.CallingNum,pCallControl->m_s8CallingNum+len-8);
							}
						}
					}
					else
					{
						strcpy(pHdlEvent.CallingNum,"888888");
					}
					
					
					len = strlen(pCallControl->m_s8CalledNum);
					if(len>0)
					{
						if(len<=8)
						{
							strcpy(pHdlEvent.CalledNum,pCallControl->m_s8CalledNum);
						}
						else
						{
							strcpy(pHdlEvent.CalledNum,pCallControl->m_s8CalledNum+len-8);
						}
					}
					else
					{
						strcpy(pHdlEvent.CalledNum,"888888");
					}
				}
				
			}
			if(pOneTrunk!=NULL)
			{
				Change_State(pOneTrunk,TRK_USED);
				sprintf(pChar,"Keygoe->ACC: 呼入,modID=%d,channID=%d,calledNum=%s,callingNum=%s",pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID,pHdlEvent.CalledNum,pHdlEvent.CallingNum);
				YF_TRACE -1, "XMS_EVT_CALLIN","Keygoe->ACC: 呼入,modID=%d,channID=%d,calledNum=%s,callingNum=%s",pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID,pHdlEvent.CalledNum,pHdlEvent.CallingNum);
			}
			else
			{
				sprintf(pChar,"Keygoe->ACC: 呼入,pOneTrunk=null,calledNum=%s,callingNum=%s",pHdlEvent.CalledNum,pHdlEvent.CallingNum);
				
				YF_TRACE -1, "XMS_EVT_CALLIN","Keygoe->ACC: 呼入,pOneTrunk=null,calledNum=%s,callingNum=%s",pHdlEvent.CalledNum,pHdlEvent.CallingNum);
			}
			AccWriteLog_Trace(pChar);
			EventType = evtXSSM_CallIn;
		}
		break;
	case XMS_EVT_ALERTCALL :
		EventType = evtXSSM_AlertCall;
		break;
	case XMS_EVT_ANSWERCALL://
		EventType = evtXSSM_AnswerCall;
		break;
		//	case XMS_EVT_LINKDEVICE ://单向连接
		//		EventType = evtXSSM_LinkDevice;
		//		break;
		//	case XMS_EVT_UNLINKDEVICE ://单向解除连接
		//		EventType = evtXSSM_UnLinkDevice;
		//		break;
	case XMS_EVT_CLEARCALL ://挂机
		ResetTrunk(pOneTrunk);
		EventType = evtXSSM_ClearCall;
		break;
	case XMS_EVT_JOINTOCONF ://参加会议
		EventType = evtXSSM_JoinConf;
		break;
	case XMS_EVT_LEAVEFROMCONF ://离开会议
		EventType = evtXSSM_LeaveFromConf;
		break;
	case XMS_EVT_CLEARCONF ://清除会议
		EventType = evtXSSM_ClearConf;
		break;
	case XMS_EVT_PLAY ://放音完成
		EventType = evtXSSM_Play;
		pAcsUniFailure = (Acs_UniFailure_Data *) FetchEventData(pAcsEvt);
		if(pAcsUniFailure->m_s32AcsEvtErrCode>2)
			return;
		break;
		//	case XMS_EVT_INITINDEX :
		//		break;
		//	case XMS_EVT_BUILDINDEX :
		//		break;
	case XMS_EVT_CONTROLPLAY ://控制放音完成
		EventType = evtXSSM_ControlPlay;
		break;
	case XMS_EVT_RECORD ://录音完成
		EventType = evtXSSM_Record;
		break;
	case XMS_EVT_CONTROLRECORD ://控制录音命令完成
		EventType = evtXSSM_ControlRecord;
		break;
	case XMS_EVT_SENDFAX ://发送传真
		EventType = evtXSSM_SendFax;
		break;
	case XMS_EVT_RECVFAX ://接收传真
		EventType = evtXSSM_RecvFax;
		break;
	case XMS_EVT_SENDIODATA ://发送数据
		EventType = evtXSSM_SendIoData;
		{
			if(pIOData!=NULL)
				YF_TRACE  -1,"","send data len=%d",pIOData->m_u16IoDataLen);
		}
		break;
	case XMS_EVT_RECVIODATA ://接收数据
		if((pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)&&(pOneTrunk->State==TRK_FREE))
		{
			
			
			p = My_GetFskCode ( pAcsEvt );
			if ( p != NULL )
			{
				pOneTrunk->CallerCode[0]=0;
				ConvertRawFskToCallerID ( p, pOneTrunk->CallerCode, 20 );
			}
			return;
		}

		pIo_Data = (Acs_IO_Data *)FetchEventData(pAcsEvt); 
		p = (char *)FetchIOData(pAcsEvt);
		if((pOneTrunk!=NULL)&&(pIo_Data != NULL))
		{
			YF_TRACE -10, "XMS_EVT_RECVIODATA","接收到数据,modID=%d,channelID=%d,len=%d,type=%d",
				pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID, pIo_Data->m_u16IoDataLen, pIo_Data->m_u16IoType);
		}
		else if(pOneTrunk!=NULL)
		{
			YF_TRACE -10, "XMS_EVT_RECVIODATA","接收到数据,modID=%d,channelID=%d,io_data=NULL",
				pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);
		}
		else if(pIo_Data!=NULL)
		{
			YF_TRACE -10, "XMS_EVT_RECVIODATA","接收到数据,pOneTrunk=NULL,len=%d,type=%d",
				pIo_Data->m_u16IoDataLen, pIo_Data->m_u16IoType);
		}
		else 
		{
			YF_TRACE -10, "XMS_EVT_RECVIODATA","接收到数据,pOneTrunk=NULL,io_data=NULL");
		}
		
		//////////////////////////////////////////////////////////////////////////
		
//		if ( ( pIo_Data->m_u16IoType == XMS_IO_TYPE_GTG ) //
//			&& ( pIo_Data->m_u16IoDataLen > 0 ) )
//		{
////			p = (char *)FetchIOData(pAcsEvt);
//			if(*p == 'b')//爆破音检测
//				return;
//			if ( *p == 'I' || *p == 'J' || *p == 'K' )//忙音
//			{
//				XMS_ctsResetDevice(g_acsHandle,&pOneTrunk->deviceID,NULL);
//				XMS_ctsClearCall(g_acsHandle,&pOneTrunk->deviceID,0,NULL);
//				ResetTrunk(pOneTrunk);
//				return;
//			}
//			
//		}
		//////////////////////////////////////////////////////////////////////////
		EventType = evtXSSM_RecvIoData;
		
		break;
	case XMS_EVT_BUILDINDEX:
		return ;
		break;
	default:
		return;
		break;
	}
	if(pOneTrunk== NULL)
	{
		pHdlEvent.sid = MESSAGE_MAX + 1;
		pHdlEvent.DeviceNumber = 0;
		YF_TRACE  -1,"","pOneTrunk== NULL，sid overflow");
	}
	else
	{
		pHdlEvent.sid = SearchSid(pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);
		if((pHdlEvent.sid>=MESSAGE_MAX)  && (EventType != evtXSSM_CallIn))
		{
			YF_TRACE -1,"","searchsid fail");
		}
		pHdlEvent.DeviceNumber = pOneTrunk->deviceID.m_s8ModuleID*256*256+pOneTrunk->deviceID.m_s16ChannelID;
	}
	
	pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
	p = (char *)FetchIOData(pAcsEvt);
	pHdlEvent.event = EventType;
	if (pIOData)
	{
		pHdlEvent.datalen = pIOData->m_u16IoDataLen;
	}
	else
	{
		pHdlEvent.datalen = 0;
	}
	if(pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA)
	{
		memcpy(pHdlEvent.ioData,p,pHdlEvent.datalen);
		//		pHdlEvent.ioData = p;
	}
	tId = GetTID(MID_ACC);
	if(tId!=NULL)
	{
		cds.dwData = 0;
		cds.cbData = sizeof(ACS_Hdl_EVENT);
		cds.lpData = &pHdlEvent;
		SendMessage(tId->hWnd,WM_COPYDATA,0,(LPARAM)&cds);
	}
	else
		YF_TRACE pHdlEvent.sid,"", "TID寻址失败，消息丢弃");
	
}
/********************************************************************
函数名称：查找会话号SID
函数功能:
参数定义: 无
返回定义: 返回会话号
创建时间: 2008-1-25 11:14
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DWORD SearchSid(WORD wDsp,WORD wTrunk)
{
	int i;
	for (i=0;i<MESSAGE_MAX;i++)
	{
		if((g_Sid_Xssm_Dll_Infor[i].DeviceNumber==(DWORD)(wDsp*256*256+wTrunk))&& (g_Sid_Xssm_Dll_Infor[i].bBusy))
		{
			return i;
		}
	}
	return MESSAGE_MAX+1;
}
/********************************************************************
函数名称：PlayFile
函数功能:放音处理(file)
参数定义: 无
返回定义: 成功返回TRUE,拾芊祷谾ALSE
创建时间: 2008-1-25 10:59
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
int	 PlayFile (DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, BOOL bIsQueue )
{	
	DJ_U32           i = 0;	
	PlayProperty_t  playProperty;
	RetCode_t		 r;
	
	memset(&playProperty,0,sizeof(playProperty));		
	
	if ( bIsQueue )
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE_QUEUE;	
	else
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE;	
	
	playProperty.m_u8TaskID = u8PlayTag;
	
	strcpy ( playProperty.m_s8PlayContent, s8FileName );
	
	r = XMS_ctsPlay ( g_acsHandle, pVocDevID, &playProperty, NULL );
	
	return r;
}
/********************************************************************
函数名称：PlayIndex
函数功能: 放音处理(index)
参数定义: 无
返回定义: 
创建时间: 2008-1-25 10:59
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

int	 PlayIndex(DeviceID_t	*pVocDevID, DJ_U16 u16Index, DJ_U8 u8PlayTag, BOOL bIsQueue )
{	
	DJ_U32           i = 0;	
	PlayProperty_t  playProperty;
	RetCode_t		 r;
	char bb[256];
	
	memset(&playProperty,0,sizeof(playProperty));		
	
	if ( bIsQueue )
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_INDEX_QUEUE;	
	else
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_INDEX;	
	
	playProperty.m_u8TaskID = u8PlayTag;
	
	playProperty.m_u16PlayIndex = u16Index;
	playProperty.m_u32StopMode = 0x01;//XMS_Normal_Stop;
	playProperty.m_u32StopCode = 0;
	r=XMS_ctsPlay(g_acsHandle,pVocDevID,&playProperty,NULL);
	sprintf(bb,"ACC:play file,r=%d,u16Index=%d,u8PlayTag=%d",r,u16Index,u8PlayTag);
	OutputDebugString(bb);
	return r;
}

BOOL CheckPlayEnd(TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_MediaProc_Data	*pMediaData = NULL;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_PLAY )	
	{
		pMediaData = (Acs_MediaProc_Data *) FetchEventData(pAcsEvt);
		if ( pMediaData->m_u8TaskID == pOneTrunk->u8PlayTag )
			return TRUE;
	}
	return FALSE;
}

void SetGtD(DeviceID_t* pDevId)
{
	//========Set GTG Begin========
	void* p	;	
	DJ_U16 u16ParamType;
	DJ_U16 u16ParamSize;
	CmdParamData_Voice_t cmdVoc;
	memset(&cmdVoc,0,sizeof(cmdVoc));
	cmdVoc.m_u8GtdCtrlValid = 1 ; //Enable GTD
	cmdVoc.m_VocGtdControl.m_u8ChannelEnable = 1;//Enable Gtd channel
	cmdVoc.m_VocGtdControl.m_u8DTMFEnable = 1;
	cmdVoc.m_VocGtdControl.m_u8MR2FEnable = 0;
	cmdVoc.m_VocGtdControl.m_u8MR2BEnable = 0;
	cmdVoc.m_VocGtdControl.m_u8GTDEnable = 1;
	cmdVoc.m_VocGtdControl.m_u8FSKEnable = 1;
	strcpy((char*)&cmdVoc.m_VocGtdControl.m_u8GTDID[0],"GHIJK");
	u16ParamType = VOC_PARAM_UNIPARAM;
	u16ParamSize = sizeof(cmdVoc);
	p= (void*) &cmdVoc;
	XMS_ctsSetParam( g_acsHandle,pDevId,u16ParamType,u16ParamSize,(void*)p);
	//========Set GTG End  ========
	
	
}

void FreeOneDeviceRes(int ID )
{
	if ( AllDeviceRes[ID].pVoice != NULL )
		free(AllDeviceRes[ID].pVoice);
	
	if ( AllDeviceRes[ID].pPcm != NULL )
		free(AllDeviceRes[ID].pPcm);
	
	if ( AllDeviceRes[ID].pTrunk != NULL )
		free(AllDeviceRes[ID].pTrunk);
	
	memset ( &AllDeviceRes[ID], 0, sizeof (TYPE_XMS_DSP_DEVICE_RES_DEMO) );
}

void FreeAllDeviceRes(void)
{
	int		i;
	
	for ( i = 0; i < MAX_DSP_MODULE_NUMBER_OF_XMS; i ++ )
	{
		FreeOneDeviceRes ( i );
	}
}
void	GetString_LineState ( char *StateStr, int iLineState )
{
	switch ( iLineState )
	{
	case DCS_FREE:							strcpy ( StateStr, "S_FREE");			break;
	case DCS_LOCAL_BLOCK:					strcpy ( StateStr, "S_LOCAL_BLOCK");	break;
	case DCS_REMOTE_BLOCK:					strcpy ( StateStr, "S_REMOTE_BLOCK");	break;
	case DCS_BLOCK:							strcpy ( StateStr, "S_BLOCK");			break;
	case DCS_UNAVAILABLE:					strcpy ( StateStr, "S_UNAVAILABLE");	break;
		
	case DCS_CALLOUT:						strcpy ( StateStr, "S_CALLOUT");		break;
	case DCS_CALLIN:						strcpy ( StateStr, "S_CALLIN");			break;
	case DCS_ALERTING:						strcpy ( StateStr, "S_ALERTING");		break;
		
	case DCS_CONNECT:						strcpy ( StateStr, "S_CONNECT");		break;
	case DCS_DISCONNECT:					strcpy ( StateStr, "S_DISCONNECT");		break;
	case DCS_WAIT_FREE:						strcpy ( StateStr, "S_WAIT_FREE");		break;
	default:
		sprintf ( StateStr, "0x%X", iLineState );
		break;
	}
}
void WriteTrunkState(TRUNK_STRUCT	*pOneTrunk)
{
	FILE *fp;
	char buffer[512];
	SYSTEMTIME     Clock;
	char data[100];
	if(pOneTrunk==NULL)
		return ;
	
	fp=fopen("C:\\yfcomm\\log\\AccState.log","a");
	GetLocalTime(&Clock);
	memset(data,0,sizeof(data));
	GetString_LineState(data,pOneTrunk->iLineState);
	sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d:%03d [m_s8ModuleID=%d],[m_s16ChannelID=%d],state_int=%d,state_str=%s\n",Clock.wYear,Clock.wMonth, Clock.wDay,
		Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID,pOneTrunk->iLineState,data);
	fwrite(buffer,strlen(buffer),1,fp);
	
	fclose(fp);
}
void HandleDevState(Acs_Evt_t *pAcsEvt )
{
	TRUNK_STRUCT	*pOneTrunk;
	PCM_STRUCT		*pOnePcm;
	Acs_GeneralProc_Data *pGeneralData = NULL;
	
	pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(pAcsEvt->m_DeviceID);
		
		pOneTrunk->iLineState = pGeneralData->m_s32DeviceState;
		//		WriteTrunkState(pOneTrunk);
	}
	
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &M_OnePcm(pAcsEvt->m_DeviceID);
		
		pOnePcm->u8E1Type = (pGeneralData->m_s32DeviceState >> 16) & 0xFF;
		pOnePcm->s32AlarmVal = (pGeneralData->m_s32DeviceState & 0xFFFF);
	}
	
}
/********************************************************************
函数名称：AddDeviceRes_Board
函数功能: 添加主板设备信息
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 10:00
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void AddDeviceRes_Board(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	DeviceID_t	 *pDev;
	s32Num = pAcsDevList->m_s32DeviceNum;
	
	if ( (AllDeviceRes[s8DspModID].lFlag == 0) && (s32Num > 0) )		// 新增加的资源
	{
		
		pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
		
		AllDeviceRes[s8DspModID].deviceID = pDev[0];
		AllDeviceRes[s8DspModID].bOpenFlag = FALSE;
		AllDeviceRes[s8DspModID].bErrFlag = FALSE;
		AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_NONE;
		
	}
	else if ( (AllDeviceRes[s8DspModID].lFlag == 1) && (s32Num == 0) )	// 删除该资源
	{
		// 如果有设备没有关闭，则等同于关闭事件
		if ( AllDeviceRes[s8DspModID].bOpenFlag != FALSE )
			CloseDeviceOK ( &AllDeviceRes[s8DspModID].deviceID );
		
		memset ( &AllDeviceRes[s8DspModID].deviceID, 0, sizeof(DeviceID_t) );
	}
	
}
/********************************************************************
函数名称：AddDeviceRes_Trunk
函数功能: 添加接口通道设备
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 9:59
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void AddDeviceRes_Trunk(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S32	s32Num;
	int		i;
	DeviceID_t	 *pDev;
	s32Num = pAcsDevList->m_s32DeviceNum;
	
	if ( (AllDeviceRes[s8DspModID].lTrunkNum == 0) && (s32Num > 0) )		// 新增加的资源
	{
		// 
		AllDeviceRes[s8DspModID].pTrunk =(TRUNK_STRUCT *)malloc(sizeof(TRUNK_STRUCT)*s32Num); 
		if( !AllDeviceRes[s8DspModID].pTrunk )
		{
			AllDeviceRes[s8DspModID].lTrunkNum = 0;
			AllDeviceRes[s8DspModID].lTrunkOpened = 0;
		}
		else
		{
			AllDeviceRes[s8DspModID].lTrunkNum = (WORD)s32Num;
			AllDeviceRes[s8DspModID].lTrunkOpened = 0;
			memset ( AllDeviceRes[s8DspModID].pTrunk, 0, sizeof(TRUNK_STRUCT)*s32Num );
			
			
			
			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
			
			for ( i = 0; i < s32Num; i ++ )
			{
				AllDeviceRes[s8DspModID].pTrunk[i].deviceID = pDev[i];
				AllDeviceRes[s8DspModID].pTrunk[i].State = TRK_WAITOPEN;
			}
			
		}
	}
	else if ( (AllDeviceRes[s8DspModID].lTrunkNum > 0) && (s32Num == 0) )		// 删除该资源
	{
		// 如果有设备没有关闭，则等同于关闭事件
		for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i ++ )
		{
			if ( AllDeviceRes[s8DspModID].pTrunk[i].State != TRK_WAITOPEN )
				CloseDeviceOK ( &AllDeviceRes[s8DspModID].pTrunk[i].deviceID );
		}
		
		AllDeviceRes[s8DspModID].lTrunkNum = 0;
		AllDeviceRes[s8DspModID].lTrunkOpened = 0;
		
		free(AllDeviceRes[s8DspModID].pTrunk);
		AllDeviceRes[s8DspModID].pTrunk = NULL;
	}
	
}
/********************************************************************
函数名称：AddDeviceRes
函数功能: 添加设备
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 9:59
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Type, s32Num;
	DJ_S8	s8DspModID;
	
	s32Type = pAcsDevList->m_s32DeviceMain;
	s32Num = pAcsDevList->m_s32DeviceNum;
	
	s8DspModID = (DJ_S8) pAcsDevList->m_s32ModuleID;
	if ( (s8DspModID < 0) || (s8DspModID >= MAX_DSP_MODULE_NUMBER_OF_XMS) )
		return;				// invalid ModuleID
	
	switch ( s32Type )
	{
	case XMS_DEVMAIN_VOICE:	
		AddDeviceRes_Voice(s8DspModID, pAcsDevList );
		break;
		
	case XMS_DEVMAIN_FAX:				
		break;
		
	case XMS_DEVMAIN_DIGITAL_PORT:
		AddDeviceRes_Pcm(s8DspModID, pAcsDevList );
		break;
		
	case XMS_DEVMAIN_INTERFACE_CH:
		AddDeviceRes_Trunk(s8DspModID, pAcsDevList );
		break;
		
	case XMS_DEVMAIN_DSS1_LINK:		
		break;
	case XMS_DEVMAIN_SS7_LINK:			
		break;
		
	case XMS_DEVMAIN_BOARD:
		AddDeviceRes_Board(s8DspModID, pAcsDevList );
		break;
		
	case XMS_DEVMAIN_CTBUS_TS:		
		break;
	case XMS_DEVMAIN_VOIP:			
		break;
	case XMS_DEVMAIN_CONFERENCE:	
		break;
		
	case XMS_DEVMAIN_VIDEO:			
		break;
	default:
		break;
	}
}

/********************************************************************
函数名称：AddDeviceRes_Voice
函数功能: 添加声音通道设备
参数定义: 无
返回定义: 无
创建时间: 2007-12-24 9:59
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

void AddDeviceRes_Voice(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
	DJ_S32	s32Num;
	int		i;
	DeviceID_t	 *pDev;
	s32Num = pAcsDevList->m_s32DeviceNum;
	
	if ( (AllDeviceRes[s8DspModID].lVocNum == 0) && (s32Num > 0) )		// 新增加的资源
	{
		AllDeviceRes[s8DspModID].pVoice =(VOICE_STRUCT*)malloc(sizeof(VOICE_STRUCT)*s32Num);
		if( !AllDeviceRes[s8DspModID].pVoice )
		{
			AllDeviceRes[s8DspModID].lVocNum = 0;
			AllDeviceRes[s8DspModID].lVocOpened = 0;
			AllDeviceRes[s8DspModID].lVocFreeNum = 0;
		}
		else
		{
			AllDeviceRes[s8DspModID].lVocNum = (WORD)s32Num;
			AllDeviceRes[s8DspModID].lVocOpened = 0;
			AllDeviceRes[s8DspModID].lVocFreeNum = 0;
			memset ( AllDeviceRes[s8DspModID].pVoice, 0, sizeof(VOICE_STRUCT)*s32Num );
			
			
			pDev = (DeviceID_t *)((BYTE *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
			
			for ( i = 0; i < s32Num; i ++ )
			{
				AllDeviceRes[s8DspModID].pVoice[i].deviceID = pDev[i];
				AllDeviceRes[s8DspModID].pVoice[i].State = VOC_WAITOPEN;
			}
		}
	}
	else if ( (AllDeviceRes[s8DspModID].lVocNum > 0) && (s32Num == 0) )		// 删除该资源
	{
		// 如果有设备没有关闭，则等同于关闭事件
		for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i ++ )
		{
			if ( AllDeviceRes[s8DspModID].pVoice[i].State != VOC_WAITOPEN )
				CloseDeviceOK ( &AllDeviceRes[s8DspModID].pVoice[i].deviceID );
		}
		
		AllDeviceRes[s8DspModID].lVocNum = 0;
		AllDeviceRes[s8DspModID].lVocOpened = 0;
		AllDeviceRes[s8DspModID].lVocFreeNum = 0;
		
		free(AllDeviceRes[s8DspModID].pVoice);
		AllDeviceRes[s8DspModID].pVoice = NULL;
	}
	
}



/********************************************************************
函数名称：ReadToneIndexFile
函数功能: 读取配置信息
参数定义: 无
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-1-28 10:00
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL ReadToneIndexFile()
{
	SECTION *sec;
	INI ini;
	BYTE i;
	char ToneName[32];
	char tmpSection[LENGTH_SECTION];
	sprintf(ToneName,"C:\\yfcomm\\Tone\\ToneIndex.ini");
	loadINI(&ini,ToneName);
	/*得到[TONEINDEX]*/
	sprintf(ToneName,"TONEINDEX");
	sec=getSection(&ini,ToneName);
	if(sec==NULL)
	{
		YF_LOG_SMM "ReadToneIndexFile,read ToneIndex.ini failed");
		return FALSE;
	}
	YF_LOG_SMM "ReadToneIndexFile,read ToneIndex.ini ok");
	{
		pToneIndex.g_total=(WORD)atoi(GetKeyList(sec,0));
	}
	pToneIndex.pIndexFIle = (TONE_INDEX_FILE *)malloc(sizeof(TONE_INDEX_FILE) * pToneIndex.g_total);
	/*得到[KeyNumber]*/
	for(i=0;i<pToneIndex.g_total;i++)
	{
		sprintf(tmpSection,"INDEX%d",i);
		sec=getSection(&ini,tmpSection);
		if(sec==NULL)
		{
			return FALSE;
		}
		{
			pToneIndex.pIndexFIle[i].index=atoi(GetKeyList(sec,0));		
			strcpy(pToneIndex.pIndexFIle[i].filename,GetKeyList(sec,1));
			
		}
	}
	/*end*/
	freeINI(&ini);
	return TRUE;
	
}






