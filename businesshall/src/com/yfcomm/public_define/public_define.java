package com.yfcomm.public_define;

import java.io.IOException;
import java.lang.reflect.Method;

import com.yfcomm.pos.SerialPort;

import android.app.ActivityManager;
import android.content.Context;
import android.util.Log;

public class public_define {

	public static int[] Keyboardcoordinates = new int[4];// ���������������

	public static final String SelphonenumInfo = "sel_numinfo"; // ѡ�еĺ�����Ϣ
	public static final String SevName = "choice type";// ҵ������
	public static final int selplan = 1; // ѡ���ײ�
	public static final int readcard = 2;// �����֤
	public static final int readmoney = 3;// �����

	public static final String TRADEMONEY = "money";// ���׽��
	public static final String Tradetype = "TradeType";// ҵ������ 1���ǻ��ѳ�ֵ 2���ǽɷ�
														// 3���̻���ֵ 4:�����ź�
	public static final String PaymentOper = "PaymentOper";// ��Ӫ������

	public static final String yfdb = "CustomInfos.db";
	public static final int ver = 1;
	public static final String TBNAME_CUSTOMINFOS = "CustomInfos";

	public static final String plannote = "plannote"; // �ײ�����
	public static final String planname = "planname"; // �ײ���
	public static final String phonenum = "phonenum"; // �ֻ���
	public static final String price = "price"; // �ײͼ۸�
	public static final String acount = "acount"; // �˻����

	public static final String custom_name = "name"; // �û���
	public static final String custom_certno = "certno";// ���֤����
	public static final String custom_address = "address";// �û���ַ
	public static final String attribute_operters = "belongto";// �����ĸ���Ӫ��
	public static final String Cardissuingbank = "Cardissuingbank";// ������

	public static SerialPort serialport = null;
	public final static String TAG = "public_define";

	public static SerialPort getSerialPort(SerialPort.SerialPortListener listener, Context context) throws IOException {

		try {
			forceStopPackage("com.yifengcom.yfpos", context);
			Log.e(TAG, "com.yifengcom.yfpos succ");
		} catch (Exception e) {
			Log.e(TAG, "error:" + e.getMessage());
		}
		if (serialport == null) {

			serialport = new SerialPort("/dev/ttyUSB1", 115200, listener);

		} else {
			serialport.setlistener(listener);
		}
		return serialport;
	}

	public static void close() {

		if (serialport != null) {

			serialport.close();
			serialport = null;
		}
	}

	private static void forceStopPackage(String pkgName, Context context) throws Exception {
		ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
		Method method = Class.forName("android.app.ActivityManager").getMethod("forceStopPackage", String.class);
		method.invoke(am, pkgName);
	}

}