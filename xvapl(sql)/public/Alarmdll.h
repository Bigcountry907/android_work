/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2007-11-27   14:51
�ļ�����: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\SPPAlarm\Alarmdll.h
�ļ�·��: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\SPPAlarm
file base:Alarmdll
file ext: h
author:	  ������

purpose:	Alarm �澯����
*********************************************************************/
#ifndef _ALARMDLL_H_
#define _ALARMDLL_H_
#include <windows.h>
#if _ALARM_DLL
# define DLLIMPORT __declspec (dllimport)
#else /* Not ALARM_DLL */
# define DLLIMPORT __declspec (dllexport)
#endif /* Not ALARM_DLL */


DLLIMPORT BOOL EventLog(const WORD wMid,const char *function_name,const char *reason);

#endif