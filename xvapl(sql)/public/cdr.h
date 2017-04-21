#ifdef  DYZ_YF_CDR_BILL_H 
#define DYZ_YF_CDR_BILL_H

#else 

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include "comm.h"
#include "Commdll.h"
#include "public_define.h"
#include "event.h"
#include "define.h"
#include "yftrace.h"




#pragma  pack(1)

typedef struct 
 {
  char    callnum[32];      //���к���
  char    callednum[32];    //���к���   
  char    s_time[30];       //��ʼʱ�� 
  char    f_time[30];       //����ʱ�� 
  DWORD   l_time;           //ͨ��ʱ��                                       
  char    usernum[20];      //�û�����
  WORD    bkey;             //ҵ���
  WORD    cost;             //����    
  WORD    FlowRate;         //����  
  char    fir_msg[40];      //������Ϣ1
  char    se_msg[40];       //������Ϣ2
  char    tid_msg[40];      //������Ϣ3
  char    fth_msg[40];      //������Ϣ4
}CDR;

typedef struct 
{
 
   int   cdrmode;  //��¼�����ļ�ѡ��ķ�ʽ  0����ʾͨ���ļ���С����,1��  ��ʾͨ��ʱ������
   int   size;     //��cdrmodeΪ0ʱ����ʾ�ļ���С ��λΪ k;��g_modeΪ1ʱ����ʾʱ��Ĵ�С��λΪ�� 
   int   index;    //��¼�ļ����   

}MODE_CDR;

typedef struct 
{
  FILE    *fp;           //��ʶ��ǰ�ļ��򿪵ľ��
  char    filename[40];  //��ʶ��ǰ�ļ���
  int     index;         //��ʶ��ǰ�ļ������õ����
  int     bkey;          //��ǰ�ļ���ҵ���
  time_t  stime;         //���������ʱ��¼����   
  char    temp[40];      //��ʶ��������ʱ������
}CDR_FILE;

typedef struct 
{
	BYTE    btrace;         //0:��ʾ�رգ�1��ʾ��
    BYTE    type;           // 1Ϊ���У�   2Ϊ���У�3��ҵ�����
	BYTE    Match;          //1��ʾ��ƥ�䣬2��ʾ��ƥ�䣬3��ʾȫƥ�䣬4��ʾȫ���绰����
	WORD    bkey;           //ҵ�����Ϊ0��ʾ�������е�ҵ������
    char    Telephone[32];  //�绰����
	
}CDR_TRACE;                  //��������


#pragma  pack()



#define    FPATH            "C:\\yfcomm\\CDR\\"    
#define    P_PATH           "C:\\yfcomm\\ini\\CDR.ini"
#define    F_FORMAT         "%s | %s | %d | %s | %s | %s | %d | %d | %d | %s | %s | %s | %s \n" 

#define    CTRACE_NAME     "callingnum:%s callednum:%s bkey:%d usernum:%s s_time:%s  f_time:%s l_time:%d  cost:%d FlowRate:%d fir_msg:%s se_msg:%s tid_msg:%s fth_msg:%s \n" 

unsigned  _stdcall Cdr_proc(LPVOID lp);

int   Deal_Cdr_Msg( CDR*cdr,MESSAGE*cmsg,MODE_CDR* cmode,CDR_FILE*cdrfile);
int   judge_filename(WORD wapram,WORD lparam,CDR_FILE* cdrfile,MODE_CDR* cmode);
int   choose_mode(MODE_CDR* cmode);
void  Get_Cdr_TraceCondition(CDR_TRACE cdrtrace,CDR cdr,WORD lparam);
void  Judge_Cdr_Match(BYTE  type,char* filename, char* callnum,CDR cdr,WORD lparam);

#endif DYZ_YF_CDR_BILL_H