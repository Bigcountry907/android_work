package com.yfcomm.mpos.codec;



/**
 * ��������
 * @author qc
 *
 */
public enum TrxType {
	
	/**
	 * ��ѯ
	 */
	QUERY((byte)0x01,"����ѯ"),
	
	/**
	 * Ԥ��Ȩ
	 */
	PRE_AUTHORIZATION((byte)0x10,"Ԥ��Ȩ"),
	
	/**
	 * ׷��Ԥ��Ȩ
	 */
	ADD_PRE_AUTHORIZATION((byte)0x10,"׷��Ԥ��Ȩ"),
	
	/**
	 * Ԥ��Ȩ����
	 */
	PRE_AUTHORIZATION_REVOKE((byte)0x11,"Ԥ��Ȩ����"),
	
	/**
	 * Ԥ��Ȩ��ɣ�������
	 */
	PRE_AUTHORIZATION_ONLINE_COMPLETE((byte)0x20,"Ԥ��Ȩ���"),
	
	/**
	 * Ԥ��Ȩ��ɳ���
	 */
	PRE_AUTHORIZATION_COMPLETE_REVOKE((byte)0x21,"��ɳ���"),
	
	
	/**
	 * Ԥ��Ȩ��ɣ����ߣ�
	 */
	PRE_AUTHORIZATION_OFF_COMPLETE((byte)0x24,"Ԥ��Ȩ���"),

	/**
	 * ����
	 */
	PURCHASE((byte)0x22,"����"),
	
	/**
	 * ���ѳ���
	 */
	REVOKE((byte)0x23,"���ѳ���"),
	
	/**
	 * �˻�
	 */
	REFUND((byte)0x25,"�˿�"),
	
	/**
	 * ����
	 */
	SETTLEMENT((byte)0x30,"����"),
	
	/**
	 * �������
	 */
	SETTLEMENT_ADJUSTMENT((byte)0x32,"�������"),
	
	/**
	 * ���������׷��С�ѣ�
	 */
	SETTLEMENT_ADJUSTMENT_TIP((byte)0x34,"����׷��С��"),
	
	/**
	 * ��������
	 */
	OFF_PURCHASE((byte)0x36,"����"),
	
	/**
	 * ����PBOC ����Ǯ����IC ��ָ���˻�Ȧ��
	 */
	IC_STORE((byte)0x40,"�˻�Ȧ��");
	
	
	private final byte value;
	private final String messsage;
	
	TrxType(byte value,String messsage) {
		this.value = value;
		this.messsage = messsage;
	}

	public byte getValue() {
		return value;
	}

	public String getMesssage() {
		return messsage;
	}
	
	
	public static  TrxType convert(int type) {
		for(TrxType trxType : TrxType.values()) {
			if(trxType.value == (byte)type) {
				return trxType;
			}
		}
		return TrxType.QUERY;
		
	}
	
}
