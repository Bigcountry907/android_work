#ifdef     XVAPL_ODI_123
#define    XVAPL_ODI_123 

#else

#define    R_run                1  

#pragma  pack(1)

typedef struct 
{
 
  char  userNum[32];     // �û�����
  char  acessNum[32];    // �������
  char  reversed[64];    // ����������
  WORD  bkey;            // �������Ϣ��ҵ��� 

}strODIACCESS;

#pragma  pack()

int   Deal_ODI_Msg(WORD  offset);
int   Start_Server_Msg(void);

#endif