package com.yfcomm.businesshall;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.widget.EditText;
import android.widget.Toast;

class EditChangedListener implements TextWatcher {
	private CharSequence temp;// ����ǰ���ı�
	private int editStart;// ��꿪ʼλ��
	private int editEnd;// ������λ��
	private final int charMaxNum;
	private EditText mEditTextMsg;
	Context context;

	EditChangedListener(EditText mEditTextMsg, Context context, int maxnum) {

		this.mEditTextMsg = mEditTextMsg;
		this.context = context;
		this.charMaxNum = maxnum;
	}

	@Override
	public void beforeTextChanged(CharSequence s, int start, int count, int after) {
		temp = s;
	}

	@Override
	public void onTextChanged(CharSequence s, int start, int before, int count) {
		
	}
	
	@Override
	public void afterTextChanged(Editable s) {

		/** �õ���꿪ʼ�ͽ���λ�� ,������������¼�ճ����������������п��� */
		editStart = mEditTextMsg.getSelectionStart();
		editEnd = mEditTextMsg.getSelectionEnd();
		if (temp.length() > charMaxNum) {
			Toast.makeText(context.getApplicationContext(), "��������Ѿ����������ƣ�", Toast.LENGTH_LONG).show();
			s.delete(editStart - 1, editEnd);
			int tempSelection = editStart;
			mEditTextMsg.setText(s);
			mEditTextMsg.setSelection(tempSelection);
		}

	}
};