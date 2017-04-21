/********************************************************************
公司名称：江苏怡丰通讯有限公司
创建时间: 2007-10-29   17:11
文件名称: E:\商品信息查询分析系统业务逻辑处理项目\源代码\Comm\Comm.c
文件路径: E:\商品信息查询分析系统业务逻辑处理项目\源代码\Comm
file base:Comm
file ext: c
author:	  刘定文

  purpose:	通讯DLL函数
*********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <process.h>
#include "../public/Commdll.h"
#include "../public/SppComm.h"
#include "../public/init_viriable.h"
#include "../public/yftrace.h"
BOOL    bIsInit = FALSE;
DWORD   dwThreadId = 0;
HANDLE  hMutex;
TMCtrl *pHead = NULL, *pCur = NULL,*pTail = NULL;
HWND    huserWnd;
int     createlable=1;
/********************************************************************

函数名称：DllMain
函数功能: 入口函数
参数定义: 无
返回定义: 无
创建时间: 2007-10-29 17:11
函数作者: 刘定文
注意事项: 无
	
*********************************************************************/

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
	int a=0;

	node_numer=0;
	iHaveNodeNumber=0;
    
    if(createlable)
	{
	  hMutex=CreateMutex(NULL,FALSE,"mutex");
	  createlable=0;
	}
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
函数名称：SetWindowsHandle
函数功能: 设置动态库依附的窗口句柄
参数定义: hWnd：窗口句柄号
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-11-27 18:02
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL SetWindowsHandle(HWND hWnd)
{
	if(!Share_Merry())
	{
		return FALSE;
	}
	huserWnd = hWnd;
	return TRUE;
}
/********************************************************************
函数名称：GetWindowsHandle
函数功能: 得到本动态库依附的进程的窗口句柄
参数定义: 无
返回定义: 返回本动态库依附的进程的窗口句柄
创建时间: 2008-11-28 10:36
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
HWND GetWindowsHandle()
{
	return huserWnd ;
}
void WriteSendMessageState(WORD sid,int sen_mod,BYTE recv_mod,char *value)
{
	FILE *fp;
	char buffer[512];
	SYSTEMTIME     Clock;
	char data[100];
	
	fp=fopen("C:\\yfcomm\\log\\Commerr.log","a");
	GetLocalTime(&Clock);
	memset(data,0,sizeof(data));
	if(value!=NULL)
	{
		sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d:%03d [sid=%d] [sen_mod=%d] [recv_mod = %d] ,%s\n",Clock.wYear,Clock.wMonth, Clock.wDay,
			Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,sid,sen_mod,recv_mod,value);
	}
	else
	{
		sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d:%03d [sid=%d] [sen_mod=%d] [recv_mod = %d] \n",Clock.wYear,Clock.wMonth, Clock.wDay,
			Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,sid,sen_mod,recv_mod);
	}
	if(fp!=NULL)
	{
		fwrite(buffer,strlen(buffer),1,fp);
		fclose(fp);
	}
}
void BCD_TO_ASCIIString(unsigned char *cSource, unsigned int iSourceLen,BOOL bTx)
{
	char mmmmm[3000];
	char cResult[3000];
	unsigned int i=0;
	char bSign[10];
    int j=0;
	FILE *fp;
	SYSTEMTIME     Clock;
	char content[255];
	memset(mmmmm, 0, 3000);
	memset(bSign,0,sizeof(bSign));
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
	fp=fopen("C:\\yfcomm\\log\\CommData.log","a");
	GetLocalTime(&Clock);
	if(fp!=NULL)
	{
		memset(content,0,sizeof(content));
		sprintf(content,"\n%04d-%02d-%02d %02d:%02d:%02d:%03d\n",Clock.wYear,Clock.wMonth, Clock.wDay,Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds);
		fwrite(content,strlen(content),1,fp);
		if(bTx)
		{
			sprintf(bSign,"TX:");
		}
		else
		{
			sprintf(bSign,"\nRX:");
		}
		fwrite(bSign,strlen(bSign),1,fp);
		fwrite(cResult,strlen(cResult),1,fp);
		
		fclose(fp);
	}
} 
/********************************************************************
函数名称：SEND_MSG
函数功能:发送消息
参数定义: event:事件号
receiverModule:接收消息模块号
data:消息内容
len: 发送消息长度
ref: 其他参数
bSync:是否同步
ack:返回数据指针
ackLen:返回数据长度
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-10-20 14:38
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL SEND_MSG(WORD event,UINT receiverModule,void *data,WORD len,WORD ref,BOOL sync, BYTE far *ack,WORD ackLen)
{
	
	BYTE bModNo;
	BOOL bSocketComm = FALSE;
	BOOL bSuc = FALSE;
	TID pTid ;
	int sid;
	HWND   hWnd;
	char data12[30];
	MESSAGE *pMessage;
	int sen_mod = -1;

	memset(data12,0,sizeof(data12));
	hWnd=GetTopWindow(NULL);
	if(receiverModule>=MODULE_MAX)
	{
		bSocketComm = TRUE;
	}
	bModNo = receiverModule;
	if((MODULE_MAX<= bModNo)/*||(ref >= MESSAGE_MAX)*/)
		return FALSE;
	if((g_Module==NULL)||(g_Node==NULL))
	{
		if(!Share_Merry())
		{
			YF_LOG_SPP "E00000003:[SEND_MSG] %s","共享内存没找到,发送消息失败");
			return FALSE;
		}
	}
	pMessage = (MESSAGE *)data;
	if(pMessage!=NULL)
		sen_mod=pMessage->head.sender.mId;
	else
		sen_mod = -1;

	sid = Add_Message(data,len);

	if(sid < 0)
	{
		YF_LOG_SPP "E00000004:找不到空闲的消息内存空间，消息发送失败");
		return FALSE;
	}
	if(bSocketComm)
	{
		bSuc=SendSocketMessage(event,bModNo,ref,(WORD)sid);//节点通讯
	}
	else
	{
		if(!GetTid(bModNo,&pTid))
		{
			YF_LOG_SPP "E00000006:mod id=%d,模块号没注册，得不到模块相关信息",bModNo);
			return FALSE;
		}
		if(pTid.hWnd==huserWnd)//接收模块和发送模块在同一个进程一致
		{
			
			if(!g_Node[0].isSPP)//is not spp sever
			{
				bSuc=FALSE;
			}
			else
			{
				bSuc=SendThreadMessage(evThreadMessage,bModNo,ref,(WORD)sid);
			}
		}
		else //非系统模块
		{
			if(g_Module[bModNo].tId.node==g_Public->g_dwNode)
			{
				bSuc=SendForkMessage(evIPCMessage,bModNo,ref,(WORD)sid);//进程通讯
			}
			else
			{
				bSuc=SendSocketMessage(evSocketMessage,bModNo,ref,(WORD)sid);//节点通讯
			}
		}
	}
	if(!bSuc)
	{
		Clean_Message((WORD)sid);
	}
	pMessage = NULL;
	return bSuc;
}
/********************************************************************
函数名称：SEND_SOCKET_MSG
函数功能: 特殊处理往网络线程发送消息
参数定义: iThreadId：线程号
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2009-6-12 16:29
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL SEND_SOCKET_MSG(WORD event,DWORD iThreadId,void *data,WORD len,WORD ref,BOOL sync, BYTE far *ack,WORD ackLen)
{
	BOOL bSuc = FALSE;
	int sid;
	if((g_Module==NULL)||(g_Node==NULL))
	{
		if(!Share_Merry())
		{
			//			YF_LOG "[SEND_MSG] %s","共享内存没找到,发送消息失败");
			return FALSE;
		}
	}
	
	sid = Add_Message(data,len);
	if(sid < 0)
	{
		return FALSE;
	}
	bSuc = PostThreadMessage(iThreadId,event,ref,sid);
	if(!bSuc)
	{
		Clean_Message((WORD)sid);
	}
	return bSuc;
}
/********************************************************************
函数名称：SendThreadMessage
函数功能: 同进程内发送线程消息
参数定义: event:事件号，receiverModule:接收消息模块号
wNumber：消息偏移量
返回定义: 成功返回TURE，错误返回FALSE
创建时间: 2007-11-8 18:54
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL SendThreadMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD sid)
{
	BOOL bSuc=FALSE;
	if(g_Module[receiverModule].state == 1)
	{
		if(g_Module[receiverModule].tId.module == g_Module[receiverModule].tId.mId)//进程发送
			bSuc = PostMessage(g_Module[receiverModule].tId.hWnd,event,sid,SenssionID);
		else
			bSuc = PostThreadMessage(g_Module[receiverModule].tId.module,event,sid,SenssionID);
	}
	return bSuc;
}
/********************************************************************
函数名称：SendForkMessage
函数功能: 进程间发送线程消息
参数定义: event:事件号，receiverModule:接收消息模块号
wNumber：消息偏移量
返回定义: 成功返回TURE，错误返回FALSE
创建时间: 2007-11-8 18:54
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL SendForkMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD sid)
{
	BOOL bSuc = FALSE;
	TID tId;
	tId = g_Module[receiverModule].tId;
	bSuc=PostMessage(tId.hWnd,event,sid,SenssionID);
	return bSuc;
}
/********************************************************************
函数名称：SendSocketMessage
函数功能: 节点之间发送线程消息
参数定义: event:事件号，receiverModule:接收消息模块号
wNumber：消息偏移量
返回定义: 成功返回TURE，错误返回FALSE
创建时间: 2007-11-8 18:54
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL SendSocketMessage(WORD event,UINT receiverModule,WORD SenssionID,WORD sid)
{
	
	TID tId;
	
	DWORD module;
	int i;
	BYTE bSuc = FALSE;
	int NodeNum= -1;
	BYTE MoudleNum;
	if(receiverModule>=MODULE_MAX)
	{
		NodeNum = receiverModule/0xff;
		MoudleNum = receiverModule%0xff; 
		
	}
	if(receiverModule<MODULE_MAX)
	{
		tId = g_Module[receiverModule].tId;
		for(i=0;i<MODULE_MAX;i++)
		{
			if(g_Node[i].node==tId.node)
			{
				module = g_Node[i].module;
				bSuc = TRUE;
				break;
			}
		}
	}
	else
	{
		bSuc = FALSE;
	}
	if(bSuc)
	{
		if(GetTid(MODULE_MAX,&tId))
		{
			if(tId.hWnd==huserWnd)//接收模块和发送模块在同一个进程一致
			{
				bSuc = PostThreadMessage(module,event,sid,SenssionID);
			}
			else
			{
				bSuc=PostMessage(tId.hWnd,event,sid,SenssionID);
			}
		}
		else
			bSuc = FALSE;
	}
	return bSuc;
}
/********************************************************************
函数名称：RegisterIPC
函数功能: 注册一个进程
参数定义: 线程名
返回定义: TID,返回注册成功后的TID信息
创建时间: 2007-10-29 17:13
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL RegisterIPC(UINT mId,HWND hwnd)
{
	TID tId;
	BOOL bSysModule = TRUE;
	if(g_Module==NULL)
	{
		if(!Share_Merry())
		{
			//YF_LOG "[RegisterIPC] %s","共享内存没找到,注册进程失败");
			return FALSE;
		}
	}
	if(mId>=MODULE_MAX)
		return FALSE;
	if(mId>=SYSMODULE)
	{
		bSysModule=FALSE;
	}
	tId.hWnd=hwnd;
	tId.node=g_Public->g_dwNode;
	tId.module=mId;
	tId.mId = mId;
	g_Module[mId].mId=mId;
	g_Module[mId].tId = tId;
	g_Module[mId].isSysModule=bSysModule;
	g_Module[mId].state=1;
	SetWindowsHandle(hwnd);
	return TRUE;	
}

/********************************************************************
函数名称：UnRegisterIPC
函数功能: 取消进程注册
参数定义: mId:模块号
返回定义: 无
创建时间: 2007-12-6 10:29
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL UnRegisterIPC(UINT mId)
{
	return UnRegisterThread(mId);
}

/********************************************************************
函数名称：RegisterThread
函数功能: 注册一个线程
参数定义: 线程名
返回定义: TID,返回注册成功后的TID信息
创建时间: 2007-10-29 17:13
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL RegisterThread(UINT mId)
{
	TID tId;
	BOOL bSysModule = TRUE;
	if(g_Module==NULL)
	{
		if(!Share_Merry())
		{
			//YF_LOG "RegisterThread %s","共享内存没找到,注册线程失败");
			return FALSE;
		}
	}
	if(mId>=(MODULE_MAX+1))
		return FALSE;
	if(mId>=SYSMODULE)
	{
		bSysModule=FALSE;
	}
	
	//	tId.hWnd=g_Public->g_hWnd;
	tId.hWnd=huserWnd;
	tId.node=g_Public->g_dwNode;
	tId.module=GetCurrentThreadId();
	tId.mId = mId;
	g_Module[mId].mId=mId;
	g_Module[mId].tId = tId;
	g_Module[mId].isSysModule=bSysModule;
	g_Module[mId].state=1;
	return TRUE;	
}
DLLIMPORT BOOL RegisterSocketThread(UINT mId)
{
	TID tId;
	BOOL bSysModule = TRUE;
	if(g_Module==NULL)
	{
		if(!Share_Merry())
		{
			return FALSE;
		}
	}
	if(mId!=MODULE_MAX)
		return FALSE;
	
	tId.hWnd=huserWnd;
	tId.node=g_Public->g_dwNode;
	tId.module=GetCurrentThreadId();
	tId.mId = mId;
	g_Module[mId].mId=mId;
	g_Module[mId].tId = tId;
	g_Module[mId].isSysModule=bSysModule;
	g_Module[mId].state=1;
	return TRUE;	
}
DLLIMPORT BOOL UnRegisterThread(UINT mId)
{
	if(mId < MODULE_MAX+1)
	{
		memset(&g_Module[mId].tId,0,sizeof(TID));
		g_Module[mId].state = 0;
	}
	else
	{
		return FALSE;
	}
	return  TRUE;
}
/********************************************************************
函数名称：RegisterNodeTread
函数功能: 注册其他网络节点模块
参数定义: 无
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2009-4-7 9:56
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL RegisterNodeMod(UINT mId,DWORD nodeip)
{
	BOOL bSysModule = FALSE;
	if(g_Module==NULL)
	{
		if(!Share_Merry())
		{
			return FALSE;
		}
	}
	if(mId>=MODULE_MAX)
		return FALSE;
	if(mId>=SYSMODULE)
	{
		bSysModule=FALSE;
	}
	g_Module[mId].tId.node = nodeip;
	g_Module[mId].tId.mId = mId;
	g_Module[mId].mId = mId;
	g_Module[mId].state = 1;
	g_Module[mId].isSysModule = bSysModule;
	return TRUE;
}
DLLIMPORT BOOL UnRegisterNodeMod(UINT mId)
{
	if(mId < MODULE_MAX)
	{
		memset(&g_Module[mId].tId,0,sizeof(TID));
		g_Module[mId].state = 0;
	}
	else
	{
		return FALSE;
	}
	return  TRUE;
}
/********************************************************************
函数名称：Comm_SetTraceValue
函数功能: 设置跟踪开关
参数定义: bValue：是否打开跟踪，TRUE打开跟踪，FALSE关闭跟踪
返回定义: 无
创建时间: 2009-4-10 10:55
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT void Comm_SetTraceValue(BOOL bValue)
{
	EnableTrace(bValue);
 
}
/********************************************************************
函数名称：GetMid
函数功能: 获取模块基本属性
参数定义: mId:模块号,pMid:获取MID的内容
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-12-4 14:54
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL GetMid(UINT mId,MID *pMid)
{
	BOOL bOk = FALSE;
	if(g_Module==NULL)
		return FALSE;
	if(pMid == NULL)
		return FALSE;
	if(mId<MODULE_MAX)
	{
		if(g_Module[mId].state==1)
		{
			memcpy(pMid,&(g_Module[mId]),sizeof(MID));
			bOk = TRUE;
		}
		else
		{
			pMid = NULL;
		}
	}
	else
	{
		pMid = NULL;
	}
	return bOk;
}
/********************************************************************
函数名称：GetTid
函数功能: 根据模块号得到注册ID
参数定义: mId:模块号,pTid:获取TID的内容
返回定义: 成功返回TRUE,失败返回FALSE
创建时间: 2008-10-21 9:45
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL GetTid(UINT mId,TID *pTid)
{
	BOOL bOk = FALSE;
	if(g_Module==NULL)
		return FALSE;
	if(pTid == NULL)
		return FALSE;
	if(mId<(MODULE_MAX+1))
	{
		if(g_Module[mId].state==1)
		{
			memcpy(pTid,&(g_Module[mId].tId),sizeof(TID));
			bOk = TRUE;
		}
		else
		{
			pTid = NULL;
		}
	}
	else
	{
		pTid = NULL;
	}
	return bOk;
}
/********************************************************************
函数名称：GetTID
函数功能: 根据模块号得到注册ID
参数定义: mId:模块号
返回定义: pTid
创建时间: 2007-10-29 17:30
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

DLLIMPORT TID *GetTID(UINT mId)
//DLLIMPORT BOOL GetTID(UINT mId,TID & pTid)
{
	
	if(g_Module==NULL)
		return NULL;
	if(mId<MODULE_MAX)
	{
		if(g_Module[mId].state==1)
		{
			return &(g_Module[mId].tId);
			
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
/********************************************************************
函数名称：Read_Message
函数功能: 读取消息具体内容
参数定义: 消息偏移量
返回定义: 有返回消息内容，无返回空指针
创建时间: 2007-11-8 20:03
函数作者: 刘定文
注意事项: 无	
*********************************************************************/


DLLIMPORT BOOL COM_Read_Message(WORD wNumber,MESSAGE *pMsg)
{
	if(g_Message==NULL)
	{
		if(!Share_Merry())
		{	
			return FALSE;
		}
	}
	if(MESSAGE_MAX>wNumber)
	{	
		memcpy( pMsg,g_Message+wNumber,sizeof(MESSAGE));
		Clean_Message(wNumber);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
/********************************************************************
函数名称：GetSenssionID
函数功能：分配一个会话号   					
函数参数：无         								
返回值  ：成功返回会话号>=0，失败返回-1								
创建时间: 2008-5-5 15:43
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT int GetSenssionID()
{
	int SenssionId = -1;
	
    SenssionId=GetSenssionNumber();
	if(SenssionId < 0)
	{
		return -1;
	}
	return SenssionId;
}
/********************************************************************
函数名称：ReleaseSenssionID
函数功能：释放会话号   					
函数参数：sid:会话号        								
返回值  ：成功返回TRUE，失败返回FALSE
创建时间: 2008-5-5 15:45
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT BOOL ReleaseSenssionID(WORD SenssionID)
{
	if(SenssionID>=MESSAGE_MAX)
		return FALSE;
	return(SetSenssionId(SenssionID));
}

/********************************************************************
函数名称：Add_Message
函数功能: 添加消息
参数定义: pMsg:消息内容
返回定义: 成功返回消息的偏移量,是报返回-1
创建时间: 2007-11-8 20:05
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
int Add_Message(void *pMsg,int len)
{
	int iNo = -1;

	iNo = GetOffsetId();
	if(iNo < 0)
	{
		return -1;
	}
	else
	{
		if(iNo<MESSAGE_MAX)
		{
			if(len<=sizeof(MESSAGE))
			  memcpy(g_Message+iNo,pMsg,len);
			else
			  memcpy(g_Message+iNo,pMsg,sizeof(MESSAGE));			
		}
		else
		{
			return -1;
		}
	}
	return iNo;
}
/********************************************************************
函数名称：Update_Message
函数功能: 消息内容更新，包括删除和更新判断
参数定义: pMsg:消息内容,wNumber:消息偏移量，
bDelete:TRUE为清空消息，FALSE为更新消息
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2007-11-8 20:07
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL Update_Message_COM(MESSAGE *pMsg,WORD wNumber,BOOL bDelete)
{
	BOOL bTrue=FALSE;
	if(g_Message==NULL)
	{
		if(!Share_Merry())
		{
			//			YF_LOG "[Update_Message]%s ","共享内存没找到,update消息失败");
			return FALSE;
		}
	}
	if(MESSAGE_MAX<=wNumber)
	{
		return bTrue;
	}
	if(bDelete)//清空消息
	{
		//call clean message
		bTrue=Clean_Message(wNumber);
	}
	else//更新消息
	{
		//call updatemessage;
		bTrue=UpdateMessage(wNumber,pMsg);
	}
	return  bTrue;
}
/********************************************************************
函数名称：FreeDll
函数功能: 释放共享内存
参数定义: 无
返回定义: 无
创建时间: 2007-11-20 10:24
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
DLLIMPORT void FreeDll()
{
	Detach_Merry();
}
/********************************************************************
函数名称：Clean_Message
函数功能: 清空消息内容
参数定义: wNumber:消息偏移量
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2007-11-8 20:10
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

BOOL Clean_Message(WORD wNumber)
{
	if(MESSAGE_MAX<=wNumber)
	{
		return FALSE;
	}
	memset((g_Message+wNumber),0,sizeof(MESSAGE));
	SetOffsetId(wNumber);
	return TRUE;
}
/********************************************************************
函数名称：UpdateMessage
函数功能: 更新消息
参数定义: wNumber:消息偏移量，pMsg:更新的消息内容
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2007-11-8 20:11
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
BOOL UpdateMessage(WORD wNumber,const MESSAGE *pMsg)
{
	if(MESSAGE_MAX<=wNumber)
	{
		return FALSE;
	}
	memcpy(g_Message+wNumber,pMsg,sizeof(MESSAGE));
	return TRUE;
}
/********************************************************************
函数名称：Share_Merry
函数功能: 申请共享内存
参数定义: 无
返回定义: 成功返回TRUE，失败返回FALSE
创建时间: 2007-11-8 19:48
函数作者: 刘定文
注意事项: 无	
*********************************************************************/

BOOL Share_Merry()
{
	
	hPublicMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_PUBLIC);
	if(hPublicMapping==NULL)
	{
		// 		MessageBox(NULL,"openfile map errno","warn",MB_ICONERROR);
		return FALSE;
	}
	g_Public=(PUBLIC *)MapViewOfFile(hPublicMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Public==NULL)
	{
		CloseHandle(hPublicMapping);
		return FALSE;
		
	}
	hMessageMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_MESSAGE);
	if(hMessageMapping==NULL)
	{
		return FALSE;
	}
	g_Message=(MESSAGE *)MapViewOfFile(hMessageMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Message==NULL)
	{
		CloseHandle(hMessageMapping);
		return FALSE;	
	}
	hNodeMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_NODE);
	if(hNodeMapping==NULL)
	{
		return FALSE;
	}
	g_Node=(NID *)MapViewOfFile(hNodeMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Message==NULL)
	{
		CloseHandle(hNodeMapping);
		return FALSE;	
	}
	hModuleMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_MODULE);
	if(hModuleMapping==NULL)
	{
		return FALSE;
	}
	g_Module=(MID *)MapViewOfFile(hModuleMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Module==NULL)
	{
		CloseHandle(hModuleMapping);
		return FALSE;	
	}
	hSidMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_SID);
	if(hSidMapping==NULL)
	{
		return FALSE;
	}
	g_Sid_Position=(SID_POSITION *)MapViewOfFile(hSidMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Sid_Position==NULL)
	{
		CloseHandle(hSidMapping);
		return FALSE;
	}
	hOffsetMapping=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,SPP_OFFSET);
	if(hOffsetMapping==NULL)
	{
		return FALSE;
	}
	g_Offset_Position=(SID_POSITION *)MapViewOfFile(hOffsetMapping,FILE_MAP_ALL_ACCESS,0,0,0);
	if(g_Offset_Position==NULL)
	{
		CloseHandle(hOffsetMapping);
		return FALSE;
	}
	return TRUE;
	
}
/********************************************************************
函数名称：Detach_Merry
函数功能: 释放共享内存
参数定义: 无
返回定义: 无
创建时间: 2007-11-8 19:51
函数作者: 刘定文
注意事项: 无	
*********************************************************************/
void Detach_Merry()
{
	UnmapViewOfFile(g_Message);
	UnmapViewOfFile(g_Node);
	UnmapViewOfFile(g_Module);
	UnmapViewOfFile(g_Sid_Position);
	UnmapViewOfFile(g_Offset_Position);
	g_Message=NULL;
	g_Node = NULL;
	g_Module = NULL;
	g_Sid_Position = NULL;
	g_Offset_Position = NULL;
	CloseHandle(hMessageMapping);
	CloseHandle(hNodeMapping);
	CloseHandle(hModuleMapping);
	CloseHandle(hReBufferMapping);
	CloseHandle(hSenssionMapping);
	CloseHandle(hSidMapping);
	CloseHandle(hOffsetMapping);
	CloseHandle(hMutex);
}

/************************************************************************/

BOOL SetSenssionId(WORD offset)
{
	if(offset < MESSAGE_MAX)
	{
		g_Sid_Position->bSign[offset] = 0;
		return TRUE;
	}
	else
		return FALSE;
}

int GetSenssionNumber()
{
	int i=0;
	int offset = -1;

	WaitForSingleObject(hMutex,INFINITE);
	while(i<MESSAGE_MAX)
	{
		if(g_Sid_Position->offset<MESSAGE_MAX)
		{
			if(g_Sid_Position->bSign[g_Sid_Position->offset]==0)
			{
				offset = g_Sid_Position->offset;
				g_Sid_Position->bSign[g_Sid_Position->offset] = 1;
				if(g_Sid_Position->offset == (MESSAGE_MAX - 1))
				{
					g_Sid_Position->offset = 0;
				}
				else
				{
					g_Sid_Position->offset ++;
				}
				break;
			}
			if(g_Sid_Position->offset == (MESSAGE_MAX - 1))
			{
				g_Sid_Position->offset = 0;
			}
			else
			{
				g_Sid_Position->offset ++;
			}
		}
		else
		{
			g_Sid_Position->offset = 0;
		}
		
		i++;
	}
   ReleaseMutex(hMutex);
   return offset;
}
void SetOffsetId(WORD offset)
{
	if(offset < MESSAGE_MAX)
	{
		g_Offset_Position->bSign[offset] = 0;
	}
}
int GetOffsetId()
{
	
	int i=0;
	int offset = -1;
	
	/***********2014-03-10 change by dyz*********************/
    WaitForSingleObject(hMutex,INFINITE);
	while(i<MESSAGE_MAX)
	{
		if(g_Offset_Position->offset<MESSAGE_MAX)
		{
			if(g_Offset_Position->bSign[g_Offset_Position->offset]==0)
			{
				offset = g_Offset_Position->offset;
				g_Offset_Position->bSign[g_Offset_Position->offset] = 1;
				if(g_Offset_Position->offset == (MESSAGE_MAX - 1))
				{
					g_Offset_Position->offset = 0;
				}
				else
				{
					g_Offset_Position->offset ++;
				}
				break;
			}
			if(g_Offset_Position->offset == (MESSAGE_MAX - 1))
			{
				g_Offset_Position->offset = 0;
			}
			else
			{
				g_Offset_Position->offset ++;
			}
		}
		else
		{
			g_Offset_Position->offset = 0;
		}
		i++;
	}
	ReleaseMutex(hMutex);
	return offset;
}
