package com.yfcomm.businesshall;

import java.text.DecimalFormat;
import java.util.Timer;
import java.util.TimerTask;

import com.yfcomm.businesshall.SwapCard.SwapCardlistener;
import com.yfcomm.mpos.codec.CardModel;
import com.yfcomm.mpos.codec.TrxType;
import com.yfcomm.public_define.public_define;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnCancelListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;
import android.widget.TextView;
import android.widget.Toast;

public class consumptionaddandopen extends Activity implements SwapCardlistener {

	private String Payname, acount;
	private TextView sel_payoper, moneyslc, peoplename, peoplecertno;
	private int selId = 2; // 1������ 2���ǳ�ֵ
	private LinearLayout account_sel, pay_readcard;
	private boolean isreadcard = false;
	private Button gm_order;
	private ProgressDialog progressDialog = null;
	private SwapCard swapCard;
    private MyHandler myHandler;
    private ImageView pageupload;
     
     
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.consumption);

		Payname = getIntent().getExtras().getString(public_define.PaymentOper, null);

		sel_payoper = (TextView) findViewById(R.id.sel_payoper);
		moneyslc = (TextView) findViewById(R.id.moneyslc);
		peoplename = (TextView) findViewById(R.id.peoplename);
		peoplecertno = (TextView) findViewById(R.id.peoplecertno);

		account_sel = (LinearLayout) findViewById(R.id.account_sel);
		pay_readcard = (LinearLayout) findViewById(R.id.pay_readcard);
		gm_order = (Button) findViewById(R.id.gm_order);

		pageupload= (ImageView)findViewById(R.id.pageupload);
		sel_payoper.setText(Payname);

		myHandler=new MyHandler(this);
		RadioGroup group = (RadioGroup) this.findViewById(R.id.radioGroup);
		// ��һ������������
		group.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(RadioGroup arg0, int arg1) {

				int radioButtonId = arg0.getCheckedRadioButtonId();

				if (radioButtonId == R.id.radioButton1)
					selId = 1;
				else
					selId = 2;
			}
		});

		account_sel.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				startActivityForResult(new Intent(consumptionaddandopen.this, PurchaseActivity.class), 0);
			}
		});

		pay_readcard.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				IdentityAuthentication.isReturn=true;
				Intent intent = new Intent();
				intent.setAction("android.intent.action.IdentityAuthentication");
				startActivityForResult(intent, 0);
			}
		});

		gm_order.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {

				if (moneyslc.getText().toString().isEmpty()) {
					Toast.makeText(consumptionaddandopen.this, "��������", Toast.LENGTH_SHORT).show();
				} else if (!isreadcard) {
					Toast.makeText(consumptionaddandopen.this, "���ȶ�ȡ���֤", Toast.LENGTH_SHORT).show();
				} else {
					progressDialog = new ProgressDialog(consumptionaddandopen.this);
					progressDialog.setMessage("�����հ�IC��...");
					progressDialog.setCancelable(false);
					progressDialog.setOnCancelListener(new OnCancelListener() {
						@Override
						public void onCancel(DialogInterface dialog) {
							// ȡ��ˢ��
							// mSerialPort.getOutputStream(PackageBuilder.syn(PackageBuilder.CMD_CANCEL).getPackData());
						}
					});
					progressDialog.show();
					swapCard = new SwapCard(consumptionaddandopen.this);
					swapCard.startswap(120, 100 * 100, 0, TrxType.PURCHASE, consumptionaddandopen.this, true);
				}

			}
		});
		
		pageupload.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
		     	finish();
			}
		});

	}
	private static class MyHandler extends Handler {
		private consumptionaddandopen activity;

		MyHandler(consumptionaddandopen activity) {
			this.activity = activity;
		}

		@Override
		public void handleMessage(Message msg) {

			switch (msg.what) {

			case 1:
				activity.StatPayActivity();
				break;
			}
		}
	}
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {

		super.onActivityResult(requestCode, resultCode, data);

		if (resultCode != RESULT_OK) {
			return;
		}

		Bundle dl = data.getBundleExtra(public_define.SelphonenumInfo);
		int type = dl.getInt(public_define.SevName);
		switch (type) {
		case public_define.readmoney:
			acount = dl.getString(public_define.TRADEMONEY);
		//	String money = getMoney(acount);
			moneyslc.setText(acount + "Ԫ");

			break;

		case public_define.readcard:
			if (IdentityAuthentication.cardZ != null) {
				peoplename.setText(IdentityAuthentication.cardZ.name);
				peoplecertno.setText(IdentityAuthentication.cardZ.cardNo);
				isreadcard = true;
			}
			break;
		}

	}

	public String getMoney(String money) {
		float size = Float.parseFloat(money) / (100);
		DecimalFormat decimalFormat = new DecimalFormat("0.00");// ���췽�����ַ���ʽ�������С������2λ,����0����.
		return decimalFormat.format(size);// format ���ص����ַ���

	}

	public void StatPayActivity(){

		progressDialog.dismiss();
	
		Intent intent = new Intent();
		intent.setAction("android.intent.action.com.yfcomm.businesshall.paymentActivety");
		Bundle arguments = new Bundle();
		arguments.putString(public_define.acount, String.valueOf(acount));
		arguments.putString(public_define.Cardissuingbank, Payname);

		if (IdentityAuthentication.cardZ != null) {
			peoplename.setText(IdentityAuthentication.cardZ.name);
			arguments.putString(public_define.custom_name, IdentityAuthentication.cardZ.name);
			arguments.putString(public_define.custom_certno, IdentityAuthentication.cardZ.cardNo);
		}
		
		
		if (selId == 1)
			arguments.putInt(public_define.Tradetype, 5);
		else {
			arguments.putInt(public_define.Tradetype, 6);
		}
		intent.putExtra(public_define.SelphonenumInfo, arguments);
		startActivity(intent);
	}

	@Override
	public void onDetectIc() {
		progressDialog.setMessage("��⵽IC��������д�����Ժ�");
	}

	@Override
	public void onInputPin() {
		// progressDialog.setMessage("д��հ׿��ɹ�����������֧���������Ժ�...");

	}

	@Override
	public void onSwiperSuccess(CardModel cardModel) {
		if (progressDialog != null)
			progressDialog.setMessage("д���ɹ�...��������֧��.");
		myHandler.sendEmptyMessageDelayed(1, 2000);
       

	}

	@Override
	public void onError(int errorCode, String errorMessage) {
		if (progressDialog != null)
			progressDialog.dismiss();
		new AlertDialog.Builder(consumptionaddandopen.this).setTitle("��ʾ")
				.setMessage("����ʧ�ܣ������룺" + errorCode + " ��Ϣ:" + errorMessage).setPositiveButton("ȷ��", null).show();
		closeKeybord();
	}

	@Override
	public void onTimeout() {
		if (progressDialog != null)
			progressDialog.setMessage("д���ɹ�...��������֧��.");

		new AlertDialog.Builder(this).setTitle("����").setMessage("ˢ����ʱ").setPositiveButton("ȷ��", null).show();
	
	}

	@Override
	public void onTradeCancel() {
		if (progressDialog != null)
			progressDialog.dismiss();

		new AlertDialog.Builder(consumptionaddandopen.this).setTitle("����").setMessage("ȡ��ˢ��")
				.setPositiveButton("ȷ��", null).show();
		closeKeybord();
	}

	@Override
	public void onShowKeyPad(byte[] keyvalue) {
		if (progressDialog != null)
			progressDialog.dismiss();
		startKeybord();
	}

	@Override
	public void OnChangeKeyBord(byte[] body) {

		// myhandler.obtainMessage(4, body[0]).sendToTarget();

	}

	@Override
	public void OnCloseKeyBord() {
		closeKeybord();
	}

	private void closeKeybord() {

	}

	private void startKeybord() {
		progressDialog.setMessage("д���ɹ�");
	}

	@Override
	public void OnPrintOK() {
		// myhandler.obtainMessage(1).sendToTarget();
	}

}
