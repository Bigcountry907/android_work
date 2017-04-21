package com.yfcomm.businesshall;

import com.yfcomm.public_define.OperterData;
import com.yfcomm.public_define.public_define;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class selectplanActivety extends Activity {

	private TextView tv;
	private GridView myGirdView;
	private MyPlanGridAdapter planAdpater = null;
	private OperterData.Operplannames plan = null;
    public  final static  String OverPlan="�ײͳ����շѹ�����������0.19Ԫ/���ӣ��̲���0.1Ԫ/������������0.29Ԫ/M";
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.choicenum);

		tv = (TextView) findViewById(R.id.text_v);
		tv.setText("ѡ���ײ�");
		myGirdView = (GridView) findViewById(R.id.gridview);
		init();
	}

	public void init() {
		planAdpater = new MyPlanGridAdapter(this, MainActivity.operdata[MainActivity.operter_sel].plannames);
		myGirdView.setAdapter(planAdpater);
		myGirdView.setNumColumns(1);
		
		myGirdView.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

				MyPlanGridAdapter adpter = (MyPlanGridAdapter) myGirdView.getAdapter();
				if (adpter.setItemsel(position)) {
					plan = adpter.getItem(position);
					CustomDialog dialog;
					CustomDialog.Builder customBuilder = new CustomDialog.Builder(selectplanActivety.this);
					customBuilder.setMessage("��ѡ�ײ�: " + plan.planname + "\r\n" + plan.note+"\r\nע��"+OverPlan).setPositiveButton("ȷ��",
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int which) {
									dialog.dismiss();
									sendResylt();
								}
							});
					dialog = customBuilder.create(0);
					Window window = dialog.getWindow();
					window.setGravity(Gravity.BOTTOM);
					dialog.show();
				}
			}
		});
	}

	public void sendResylt() {

		Intent mIntent = new Intent();
		Bundle arguments = new Bundle();
		arguments.putInt(public_define.SevName, public_define.selplan);
		arguments.putString(public_define.planname, plan.planname);
		arguments.putString(public_define.plannote, plan.note);
		arguments.putString(public_define.price, plan.prcie);
		mIntent.putExtra(public_define.SelphonenumInfo, arguments);
		setResult(RESULT_OK, mIntent);
		this.finish();
	}

}