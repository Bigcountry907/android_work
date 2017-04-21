#ifndef _READ_CONFIG_H_
#define _READ_CONFIG_H_
#include <windows.h>
#include "comm.h"
#include "Commdll.h"
//#define LENGTH_KEY     32   /*�ؼ��ֳ���*/
//#define LENGTH_SECTION 64   /*�ڳ���*/
//#define LENGTH_VALUE   128  /*�������ݳ���*/
//#define LENGTH_COMMENT 128  /*ע�ͳ���*/
//#define LENGTH_BUFFER  256  /*�ַ������С*/
///*�ؼ��ʴ洢�ṹ*/
//typedef struct Key
//{
//    char key[LENGTH_KEY],value[LENGTH_VALUE],comment[LENGTH_COMMENT];/*�ؼ���\����\ע��*/
//    struct Key *next;/*��һ���*/
//}KEY,K;
///*�ڴ洢�ṹ*/
//typedef struct Section
//{
//   char section[LENGTH_SECTION],comment[LENGTH_COMMENT];/*������ע��*/
//   struct Key headKey;/*��Ӧ�ڹؼ����׽��,�׽�㲻������*/
//   struct Section *next;/*��һ���*/
//}SECTION,SEC,S;
///*�����ļ��ṹ*/
//typedef struct Ini
//{
//   struct Section headSection;/*���׽��,�׽�㲻������*/
//}INI,I;
/*��ʱ�������ڵ�������*/
char tmpBUFFER[LENGTH_BUFFER];
char tmpSection[LENGTH_SECTION];
char tmpKey[LENGTH_KEY];
char tmpValue[LENGTH_VALUE];
char tmpComment[LENGTH_COMMENT];
static char CommentSeparator=';';
/*��������*/
void initINI(INI *ini);
/*�ͷ�*/
//void freeINI(INI *ini);

//void loadINI(INI *ini,char *filename);/* ���� */
void saveINI(INI *ini,char *filename);/* ���� */

SECTION * addSection(INI *ini,char *name,char *comment);
/*���ӽ�,����Ϊ�����ļ�,���Ƽ�ע��,��ΪNULL,�ɹ����ص�ַ,
ʧ�ܷ���NULL,���Ѵ��ڽڽ��Զ����ش��ڵ�ַ�൱������,�����ִ�Сд*/
int insertSECTION(SECTION *newSEC,SECTION *preSEC);/*��preSEC�����newSEC*/

//SECTION *getSection(INI *ini,char *name);/*�������ƻ�ȡָ����*/
SECTION *getNextSection(SECTION *sec);/*��ȡָ����ַ��һԪ��*/
/*�������Ʒ��ؽڵ�ַ���ڲ���,ʧ�ܷ���NULL,�����ִ�Сд*/
int  removeSection(INI *ini,char *name);
/*��������ɾ��ָ����,ʧ�ܷ���0�ɹ�����1,�����ִ�Сд*/
void freeSecKey(SECTION *ptrSEC);/*�ͷ�ָ���������йؼ���*/

KEY *addKey(INI *ini,char *secName,char *keyName,char *keyValue,char *comment);
/*��ָ���������ӹؼ������Ƽ���������ע��,�ɹ����ص�ַ,ʧ�ܷ���NULL*/
int insertKEY(KEY *newKEY,KEY *preKEY);/*��preKEY�����newKEY*/
KEY *addSecKey(SEC *keySEC,char *keyName,char *keyValue,char *comment);
/*��ָ���ڵ�ַ���ӹؼ������Ƽ���������ע��,�ɹ����ص�ַ,ʧ�ܷ���NULL*/
KEY *getKey(INI *ini,char *secName,char *keyName);
KEY *getNextKey(KEY *key);/*ָ��key����һ��Ԫ��*/ 
char *getKeyValue(KEY *key);/*��ȡָ���ؼ��ֵ�����*/
/*��ָ���ڻ�ȡƥ�䵽�Ĺؼ���*/
KEY *getSecKey(SECTION *keySEC,char *keyName);
/*��ָ���ڵ�ַ��ȡƥ�䵽�Ĺؼ���*/
int removeKey(INI *ini,char *secName,char *keyName);
int removeSecKey(SECTION *keySEC,char *keyName);
/*ͳ�ƺ���*/
int getSectionCount(INI *ini);
int getKeyCount(SECTION *keySEC);
/*�޸Ľ����ơ�ע�ͣ��ؼ������ơ�ע��*/
int setSectionName(INI *ini,SECTION *sec,char *secName);
int setSectionComment(INI *ini,SECTION *sec,char *secComment);
int setKeyName(KEY *setKey,char *keyName);
int setKeyValue(KEY *setKey,char *keyValue);
int setKeyComment(KEY *setKey,char *keyComment);
/*��ӡ���*/
void printSectionList(INI *ini);
void printKeyList(SECTION *keySEC);
///*void*/char * GetKeyList(SECTION *keySEC,BYTE offset,BYTE index);
void printList(INI *ini);

/*����ռ�*/
SECTION * allocateSEC(char *name,char *comment);
KEY *allocateKEY(char *name,char *value,char *comment);
/*�ַ�������*/
int isIgnoreCaseChar(char ch1,char ch2);
int ignoreCaseCompare(char *str1,char *str2);
int isSpecial(char ch);
void RightTrim(char *str);
void LeftTrim(char *str);
void Trim(char *str);
#endif