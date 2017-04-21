/********************************************************************
��˾���ƣ���������ͨѶ���޹�˾
����ʱ��: 2008-04-12   14:26
�ļ�����: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\SPP\trace.c
�ļ�·��: E:\��Ʒ��Ϣ��ѯ����ϵͳҵ���߼�������Ŀ\Դ����\SPP
file base:SPP_SLP
file ext: c
author:	  ������

purpose:	���Ը��ٴ���
*********************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.h"

#define MAXFILELEN 10*1024*1024

/******************************************************************************
*  Function Name:  WriteFilelog()
*  Description	:  write a log information to the log file. 
*  INPUT	:      error messages--errmsg,filename--file, and line count--line.
*  OUTPUT	:      if error return false,else return true. 
*  Note		:
*  Modify	:  
*******************************************************************************/
BOOL WriteFileLog_1(const char * logname, const char *cfile, const int line,const char *fmt,...)
{
	va_list	args;	
	SYSTEMTIME     Clock;
	FILE	    *fp=NULL;
	char	 	fileName[255];
	char	    buf[1024];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

 	memset(buf,0,sizeof(buf));

	va_start(args,fmt);
	_vsnprintf(buf,sizeof(buf)-1,fmt,args);
	va_end(args);

	if (!logname) 
		return FALSE;

	GetLocalTime(&Clock);

	sprintf(fileName,"%s%04d%02d%02d.log",logname,Clock.wYear,Clock.wMonth, Clock.wDay);

	fp = fopen (fileName,"a");

	if ( fp == NULL )
		return FALSE;

	 _splitpath( cfile, drive, dir, fname, ext ); 

	fprintf (fp, "%02d:%02d:%02d:%03d F[%8s%4s] L[%4d] %s\n",
		    Clock.wHour, Clock.wMinute, Clock.wSecond, Clock.wMilliseconds,
			fname,ext,line, buf );
	fclose (fp);
	return TRUE;
}
