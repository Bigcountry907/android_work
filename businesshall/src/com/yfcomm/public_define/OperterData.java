package com.yfcomm.public_define;

import java.util.List;

public class OperterData {

	public List<Operplannames> plannames;// ��Ӫ��������
	public List<numberpool> numberpool;// ���������

	public static class Operplannames {

		public String planname; // �ײ�����
		public String prcie; // �ײͼ۸�
		public String note; // �ײͱ�ע
		public boolean is_sel = false;// �Ƿ�ѡ��
	}

	public static class numberpool {

		public String phonenum; // ����
		public String prcie; // �۸�
		public boolean is_sel = false;// �Ƿ�ѡ��
		public boolean isUsed = false; // �Ƿ�ռ��
	}

}
