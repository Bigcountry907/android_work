package com.yfcomm.mpos.tlv;

import java.io.IOException;
import java.io.OutputStream;
import java.io.Serializable;



public abstract class TLV<V extends Serializable> implements Serializable {
	
 
	private static final long serialVersionUID = 5941502245502818169L;
	
	private int tag;
	
	private V value;
	
	/***
	 * ת����byte
	 * @return
	 */
	public abstract byte[] getBytes();
	
	/**
	 * ��ȡ TLV ת���ɵ�byte
	 * @return
	 */
	public  void getTLVBytes(OutputStream os) {
		try {
			//����Ϊ2
			os.write(this.tag>>8);
			os.write(this.tag);
			
			//����
			os.write(this.length());
			
			//����
			if(this.length()>0) {
				os.write(this.getBytes());
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * ���ݳ���
	 * @return
	 */
	public abstract int length();
	
	/**
	 * ����tlv ֵ 
	 * @param data   ������������
	 * @param offset  
	 * @param len
	 */
	public abstract void setValue(byte[] data,int offset,int len);
	
	/**
	 * ��ȡ����
	 * <b> tag(2b)+ len(1b)+ data(n)</b>
	 * @param data
	 * @param offset  ��ʼλ��
	 * @return  ��ȡ���� 
	 */
	public int read(byte[] data,int offset) {
		int count = data.length;
		//tag �� len ���ܽ���   tag(2b)+ len(1b)
		if(count - offset < 3) {
			return 0;
		}
		
		int temp = offset;
		//tag
		int tag = (data[temp] & 0xFF) <<8  | (data[++temp] & 0xFF); 
		//����
		int len = data[++temp] & 0xFF; 
		
		if( (temp + len) <= (count-1) ) {
			//����value
			this.setValue(data, ++temp, len);
			this.setTag(tag);
			return 3 + len;
			
		} else {
			//���ݳ��Ȳ���ȷ
			return 0;
		}
	}
	
	public V getValue() {
		return value;
	}

	public void setValue(V value) {
		this.value = value;
	}


	public int getTag() {
		return tag;
	}

	public void setTag(int tag) {
		this.tag = tag;
	}
}
