package com.iflytek.nativetoolwrapper;

import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.util.Log;

import com.iflytek.nativetool.AiuiToolMainEntry;
import com.iflytek.nativetool.AiuiToolCallback;

public class MainActivity extends Activity {
	private static final String tag = "NativeToolWrapper";
	private Button mStartMainFuncBtn = null;
	private PowerManager powerManger = null;
	private WakeLock wakeLock = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.d(tag, "onCreate in =>");

		setContentView(R.layout.activity_main);
		mStartMainFuncBtn = (Button) (findViewById(R.id.start_main_func));
		mStartMainFuncBtn.setOnClickListener(new startMainFuncBtnListener());

		powerManger = (PowerManager) getSystemService(Context.POWER_SERVICE);
		wakeLock = powerManger.newWakeLock(PowerManager.FULL_WAKE_LOCK, tag);

		AiuiToolMainEntry.setListenerAiuiToolCallback(new onNativeAiuiToolCallback());
		Log.d(tag, "onCreate out <=");
	}

	class startMainFuncBtnListener implements OnClickListener {
		@Override
		public void onClick(View arg0) {
			AiuiToolMainEntry.startNativeMainFunc();
			mStartMainFuncBtn.setEnabled(false);
			mStartMainFuncBtn
					.setText("com.iflytek.nativetoolwrapper 已启动。为了保证持续运行，程序会保持车机屏幕常亮，可以按返回back键退出程序。");
		}
	}

	class onNativeAiuiToolCallback implements AiuiToolCallback {
		@Override
		public void onFinished() {
			Log.d(tag, "onNativeAiuiToolCallback onFinished in =>");
			mStartMainFuncBtn = (Button) findViewById(R.id.start_main_func);
			mStartMainFuncBtn.post(
				new Runnable() {
					@Override
					public void run() {
						mStartMainFuncBtn
								.setText("Aiui的测试工具跑完啦，如果想重新跑一次，可以按返回back键退出程序后 再启动一次程序。");
					}
				}
			);
			Log.d(tag, "onNativeAiuiToolCallback onFinished out <=");
		}
	}

	@Override
	protected void onPause() {
		super.onPause();
		Log.d(tag, "onPause in =>");
		wakeLock.release();
		Log.d(tag, "onPause out <=");
	}

	@Override
	protected void onResume() {
		super.onResume();
		Log.d(tag, "onResume in =>");
		wakeLock.acquire();
		Log.d(tag, "onResume out <=");
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.d(tag, "onDestroy in =>");
		Log.d(tag, "onDestroy out, will call System.exit(0) to be terminated <=");
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.exit(0);
	}
}
