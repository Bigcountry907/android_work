#ifdef     YFCOMM_DYZ_2009_5_19

#define    YFCOMM_DYZ_2009_5_19


#else


#define MAXLINE   10*1024*1024  
#define NORMAL     0
#define WARN       1 
#define TRACE      2             


#pragma   pack(1)
typedef struct 
{		
	BYTE  type;               // 0:normal 1��warn  2��trace ��           
    WORD  lines;              // ��ʶ��        
	char  funname[100];       // ������ 
    BYTE  warnstep;           // �澯�ȼ�  
	char  content[CHAR_NUMBER-105];       // ��־����

}Log_Message;


typedef  struct               //�����ļ��Ļ�������
{ 

   int   index;               //��ʶ��ǰ�ļ������к�
   char  filename[30];        //��ʶ��ǰ�ļ��� 

}FileAtt;


typedef struct  
{
  FileAtt  nomal_file;        //��ʶ������־������

  FileAtt  Trace_file;         //��ʶ������־������

}ATT_FILE;     

#pragma   pack()

int   Find_MaxIndex(ATT_FILE* all_file);
int   Write_DBAPWarnLog(Log_Message ulog);
int   WriteLogMsg(WORD wparam,ATT_FILE *all_file);
void  logProc(PVOID lp);
int   Change_char_To(char* buf);
int   Judge_FileName(char* name,FileAtt*fileatt,Log_Message  ulog);

#endif  YFCOMM_DYZ_2009_5_19 