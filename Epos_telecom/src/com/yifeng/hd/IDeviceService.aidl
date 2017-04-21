package com.yifeng.hd;
import com.yifeng.hd.FontStyle;
import com.yifeng.hd.TransRequest;

interface IDeviceService{

    /**
    * ��ʼ���豸
    */
    int init();
    
    /**
    * ��mac��ʼ���豸
    */
    int initForMAC(String mac);
    
    int initForName(String name);
    /**
     * ��ȡ�ն˰汾��
     * @param version  �汾����
     */
	int getVersion(out byte[] version);
	
	/**
     * ��ȡpos��ַ
     */
	String getPOSAddress();
	
	/**
	 * ����
	 */
	int clearScreen();
	
	/**
	 * ���ָ����
	 *@param line      �ڼ���
	 */ 
	int clearScreenLine(int line);
	
	/**
	 * ���ն���Ļ����ʾ�ַ���
	 * @param line   �кţ�0 �C 3�� ��ÿ��ռ16���ظߣ�
	 * @param col    �кţ�0 �C 127������λ�����أ�
	 * @param str    ����ʾ���ַ��� 
	 * 
	 */
	int screenDisplay(int line,int col,String str);
	 
	
    /**
	 *  ���ִ�ӡ
	 * <li>����ΪGB2312</li>
	 *
	 * @param offset   ��ӡ�����ڴ�ӡֽ�ϵ�ƫ��λ��
	 * @param size     �����С     2�ִ�С��0��16*16,1:24*24��
	 * @param info     Ҫ��ӡ�����ݣ�ÿ��Ԫ��Ϊһ��
	 * @return   ����Ӧ����
	 */
	int printASCII(int offset,int size,in List<FontStyle> info);
	
	/**
     * ͼƬ��ӡ
     * @param str   λͼ����
     */
	int printBitmap(in byte[] bitmap);
	
	/**
     * ��ӡ������ 
     * <li>Ŀǰ֧�ִ�ӡ��Ʒ��EAN-13��code 128�루A���е�����ASCLL��֧�֣� ��code 39��</li>
     * @param offset   ��ӡ�������ڴ�ӡֽ�ϵ�ƫ��λ��  (0 -- 0x01 0x80 )
     * @param type     ��ӡ���� 0: 128�� ,1: 39�� , 2: ��Ʒ��  
     * @param check    ��ӡ���ݰ�������
     * @param barcode  ��ӡ����   GB2312 ����
     */
	int printBarcode(int offset,int type,boolean check,in byte[] barcode);
	
	/**
	 * ��ֽ
	 * @param step  ������
	 */
	int movePaper(int step);
	
    /**
	 * IC����ȡ
	 * @param  ic  ic������
	 * @return 
	 */
	int readIC(out byte[] ic);
	
	/**
	 * ��ȡ RFID ����
	 * @param row   ��ʾ��ֵ��ʼ��(0-4)
	 * @param col   ��ʾ��ֵ��ʼ��(1-0x80)
	 * @param type  0 ��ȡ����  1  ��ȡ�������ŵ���������  
	 * @param data   ������ ������
	 */
	int readRFID(int row,int col,int type,out byte[] data);
	
	/**
	 * ˢ����������
	 * @param type  0 ��ȡ����  1  ��ȡ�������ŵ���������  
	 * @param data   ������ ������
	 */
	int readBankCard(int type,out byte[] data);
	/**
	 * ���ó�ʱʱ��   
	 * @param time  ��λ��
	 * @return
	 */
	int setWaitTimeout(int time);
	
	/**
	 * ��ȡ���  
	 * @param row    ��ʾ��ֵ��ʼ�� (1-128)
     * @param col    ��ʾ��ֵ��ʼ�� (0-4)
     * @param min    ��С����
     * @param max    ��󳤶� (<=12)
	 * @param data    ������ ��λΪ��
	 */
	int getMoney(int row,int col,int min,int max,out byte[] data);
	
	 /**
	 * ��ʾ�ַ���
	 * @param line    ��ʾ��ʼ�У�0--4��
     * @param col     ��ʾ��ʼ�У�1--0x80��
	 * @param data    ��ʾ���� GB2312
	 */
	int displayASCII(int line,int col,in byte[] data);
	
	 /**
	 * ��ʾ����
	 * @param bline    ��ʾ��ʼ�У�0--4��
     * @param bcol     ��ʾ��ʼ�У�1--0x80��
     * @param eline    ��ʾ�����У�1--0x80��
     * @param ecol     ��ʾ�����У�1--0x80��
	 * @param data     ��ʾ����
	 */
	int displayDotMatrix(int bline,int bcol,int eline,int ecol,in byte[] data);
	
	/**
	 * ����logo���豸
	 * @param len     ���ݳ���
	 * @param data    logo����
	 */
	int downLoadLogo(int len,in byte[] data);
	
	/**
	 * ��������ӿ�
	 * @param messageType  ��������
	 * @param inData       ��������
	 * @param outData      �������
	 */
	int transRequest(in TransRequest req,out byte[] outData);
	
	/**
	 * ����Ӧ���Ľ����ӿ�
	 * @param inData     ��������
	 * @param outData    �������
	 */
	int transResponse(in byte[] inData,out byte[] outData);
	
	/**
	 * ���ײ�������
	 * @param inData     ��ȡ����
	 */
	int transParams(in byte[] inData,out byte[] outData);
	
	/**
	 * ��ӡ���һ�ʽ���
	 */
	int printLastTrans();
	
	void close();
	/**
	 * ���� ����
	 */
	void endCmd();
}