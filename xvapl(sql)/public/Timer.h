#ifndef _TIMER_H_
#define _TIMER_H_
#if TIMER_H
# define DLLIMPORT __declspec (dllimport)
#else /* Not COMM_DLL */
# define DLLIMPORT __declspec (dllexport)
#endif /* Not COMM_DLL */

//#define LOG

#include <windows.h>
#include <Mmsystem.h>
#include "TimerDll.h"
#include "../public/Commdll.h"
#include "../public/comm.h"

#define MAX_TIMERS 8192  //8192:��Լ�ʱ����Ŀ
#define MAX_ABS_TIMERS 10   //10:���Լ�ʱ����Ŀ

#pragma  pack(1)
typedef struct 
{
	long    ldTime;    /*��ʱ���Ĵ���ʱ��*/
	WORD	event;		/* ��ʱ����ʱ���������¼� */
	UINT    mId;       /* ģ���*/
	WORD	ref;		/* �ο�ֵ�����������Լ����壬����ʱԭֵ���� */	
	WORD    tmID;       //��ʱ����ID.
}YF_TIMER;

typedef struct tagTMCtrl
{
	YF_TIMER    timer;
	struct tagTMCtrl  *prev,*next;
}YF_TmCtrl;

typedef struct 
{
	WORD index;
	YF_TmCtrl* vec[10];
}timer_vec;
#pragma  pack()

/*********************************��Զ�ʱ���õ�������***************************************/
YF_TmCtrl* szHeadNode[51];
YF_TmCtrl  szHead[51] = {0};

static short int yf_revise = 0;//��������ʱ��ƫ��
static long yf_beginYfTime = 0;//��������ʱ��ƫ��
static long yf_curYfTime = 0;//��������ʱ��ƫ��
static long yf_beginRealTime = 0;//��������ʱ��ƫ��
static long yf_curRealTime = 0;//��������ʱ��ƫ��

static long yf_YfTime = 0;//��ʱ,��0.1��Ϊ��λ
static YF_TmCtrl* pHead = NULL;//ָ���ʱ�������Ŀ�ʼ��
static YF_TmCtrl* pCurrent = NULL;//ָ���ʱ�������Ŀ�ʼ��
static WORD yf_curTimerCursor = 0;//ָ���ʱ�������ĵ�ǰ��
static YF_TmCtrl* yf_szTimer[MAX_TIMERS];//ÿ����ʱ���ĵ�ַ.
static YF_TIMER* yf_szAbsTimer[MAX_ABS_TIMERS];//ÿ�����Լ�ʱ���ĵ�ַ.

static  timer_vec tv1;//0.1�뼶
static  timer_vec tv2;//1�뼶 
static  timer_vec tv3;//10�뼶 
static  timer_vec tv4;//100�뼶 
static  timer_vec tv5;//1000�뼶���ʱ��ԼΪ��10000/60/60 �� 2.7Сʱ��
static  timer_vec *  yf_tvecs[] = {&tv1,&tv2,&tv3,&tv4,&tv5};


/********************************���Զ�ʱ���õ�������****************************************/
YF_TmCtrl* pabsHead = NULL,*pabsCur = NULL,*pabsTail = NULL;

CRITICAL_SECTION sec;

CRITICAL_SECTION absSec;

//����ʱ��ƫ��:yf_curTime
static /*inline*/ void revise_time();

static /*inline*/ void init_timer(/**/);

static int get_tmid(/*YF_TmCtrl * pTm*/);//��yf_timerList����һ��δ�õ�ID���������±귵��

static void del_tmid(WORD tmid);// 

static /*inline*/ int add_timer(YF_TIMER timer);

static int transformnAddTimer(YF_TIMER,WORD); 

//static /*inline*/ void kill_timer(WORD tmID);

static /*inline*/ int internal_add_timer(YF_TIMER,WORD);

static /*inline*/ void internal_mod_timer(YF_TIMER timer,WORD tmID);

static /*inline*/ void mod_timer(YF_TIMER timer,WORD tmID);

static /*inline*/ void cascade_timers(/*YF_TmCtrl* tv*/);//��ʱ����Ǩ�Ʋ���

static void TriggleTimer(YF_TIMER timer);

unsigned __stdcall timerProc(void *lpP);//��Զ�ʱ����ѭ���߳�

static /*inline*/ int add_abs_timer(YF_TIMER timer);
static int get_abs_tmid();
static void del_abs_tmid(WORD tmid);
static void TriggleAbsTimer(YF_TIMER timer);
unsigned __stdcall absTimerProc(void *lpP);//���Զ�ʱ����ѭ���߳�

void CALLBACK TimeProc(UINT uID,UINT uMsg, DWORD dwUser,DWORD dw1,DWORD dw2);

void WriteLog(const char *fmt,...);

#endif