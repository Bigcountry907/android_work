package com.yifengcom.yfpos.model.syn;

public class WorkKey extends SynPackage {

	private static final long serialVersionUID = -1054726969111227931L;

	//TDK
	private byte tdkIndex = 0x00;
	private byte[] tdk;
	private byte[] tdkCheckValue;

	//MAK
	private byte makIndex = 0x00;
	private byte[] mak;
	private byte[] makCheckValue;

	//PIK
	private byte pikIndex = 0x00;
	private byte[] pik;
	private byte[] pikCheckValue;

	/**
	 * 编码数据
	 */
	@Override
	public byte[] encode() {
		byte[] body = new byte[68];
		//TDK索引
		body[2] = tdkIndex;
		body[3] = tdk==null ? 0x00 : (byte)0x10;
		if(tdk!=null) {
			System.arraycopy(tdk, 0, body, 4, tdk.length<16 ? tdk.length : 16);
		}
		if(tdk!=null && tdkCheckValue!=null) {
			System.arraycopy(tdkCheckValue, 0, body, 20,tdkCheckValue.length<4 ? tdkCheckValue.length : 4 );
		}
		
		//MAK
		body[24] = makIndex;
		body[25] = mak==null ? 0x00 :(byte)0x10;
		if(mak!=null) {
			System.arraycopy(mak, 0, body, 26, mak.length<16 ? mak.length : 16);
		}
		if(mak!=null && makCheckValue!=null) {
			System.arraycopy(makCheckValue, 0, body, 42, makCheckValue.length<4 ? makCheckValue.length : 4);
		}
		
		//PIK
		body[46] = pikIndex;
		body[47] = pik==null ? 0x00 :(byte)0x10;
		if(pik!=null) {
			System.arraycopy(pik, 0, body, 48, pik.length<16 ? pik.length : 16);
		}
		if(pik!=null && pikCheckValue!=null) {
			System.arraycopy(pikCheckValue, 0, body, 64, pikCheckValue.length<4 ? pikCheckValue.length : 4);
		}
		
		// 组包
		return body;
	}
	
	public byte getTdkIndex() {
		return tdkIndex;
	}

	public void setTdkIndex(byte tdkIndex) {
		this.tdkIndex = tdkIndex;
	}

	public byte[] getTdk() {
		return tdk;
	}

	public void setTdk(byte[] tdk) {
		this.tdk = tdk;
	}

	public byte[] getTdkCheckValue() {
		return tdkCheckValue;
	}

	public void setTdkCheckValue(byte[] tdkCheckValue) {
		this.tdkCheckValue = tdkCheckValue;
	}

	public byte getMakIndex() {
		return makIndex;
	}

	public void setMakIndex(byte makIndex) {
		this.makIndex = makIndex;
	}

	public byte[] getMak() {
		return mak;
	}

	public void setMak(byte[] mak) {
		this.mak = mak;
	}

	public byte[] getMakCheckValue() {
		return makCheckValue;
	}

	public void setMakCheckValue(byte[] makCheckValue) {
		this.makCheckValue = makCheckValue;
	}

	public byte getPikIndex() {
		return pikIndex;
	}

	public void setPikIndex(byte pikIndex) {
		this.pikIndex = pikIndex;
	}

	public byte[] getPik() {
		return pik;
	}

	public void setPik(byte[] pik) {
		this.pik = pik;
	}

	public byte[] getPikCheckValue() {
		return pikCheckValue;
	}

	public void setPikCheckValue(byte[] pikCheckValue) {
		this.pikCheckValue = pikCheckValue;
	}

}
