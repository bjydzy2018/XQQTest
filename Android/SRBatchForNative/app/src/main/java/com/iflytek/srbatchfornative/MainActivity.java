package com.iflytek.srbatchfornative;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.ToggleButton;
import android.widget.AdapterView.OnItemSelectedListener;

import com.iflytek.srbatchfornative.SpeechService.myService;
import com.iflytek.speech.ISSErrors;
import com.iflytek.speech.sr.ISRListener;

public class MainActivity extends Activity {
    public static final String PcmIndexPath = Environment.getExternalStorageDirectory()
            .getAbsolutePath() + "/issTest/pcmList/";
    public static final String DictPath = Environment.getExternalStorageDirectory()
            .getAbsolutePath() + "/issTest/dict/";
    //public static final String TAG = "TestTool";
    private static final int MSG_TYPE_SR = 30;
    public static int ISS_SR_MODE;
    public static int SrSessionStart = 100;
    public static int appendAudioDataResult;
    public static int ISS_SR_SCENE;

    public boolean bSelectJson = true;

    SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SS");// 设置日期格式
    private RadioButton cloudButton;                    //设置各个按钮的。
    RadioButton localButton;
    RadioButton mixedButton;
    RadioButton allButton;
    RadioButton poiButton;
    RadioButton contactButton;
    RadioButton selectButton;
    RadioButton confirmButton;
    RadioButton selectXmlButton;    //zhfu
    RadioButton selectJsonButton;    //zhfu
    Button uploadDict;
    private RadioButton singleTrackButton;
    RadioButton doubleTrackButton;
    private Spinner langspinner = null;

    private ArrayAdapter<String> adapter;
    private ArrayAdapter<String> adapterDict;
    private ArrayAdapter<String> langAdpter;
    Handler mHandler = null;
    TextView mTextView;
    public long startTime = 0;
    public long createTime = 0;

    private static String TAG = "TestTool";

    private myService binder = null;

    SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
            .getDefault());

    private ServiceConnection conn = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            // TODO Auto-generated method stub
            Log.d(TAG, "onServiceConnected");
            binder = (myService) service;
            binder.setListener(isrListener);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            // TODO Auto-generated method stub
            Log.d(TAG, "onServiceDisconnected");
            binder = null;
        }

    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // 创建识别会话

        final Spinner spinner = (Spinner) findViewById(R.id.filechooser);
        final Spinner spinnerDict = (Spinner) findViewById(R.id.dictchooser);
        cloudButton = (RadioButton) findViewById(R.id.Cloud_mode);
        localButton = (RadioButton) findViewById(R.id.Local_mode);
        mixedButton = (RadioButton) findViewById(R.id.Mixed_mode);
        mTextView = (TextView) findViewById(R.id.mTextView);
        mTextView.setMinLines(15);
        uploadDict = (Button) findViewById(R.id.ditbtn);
        allButton = (RadioButton) findViewById(R.id.scene_all);
        poiButton = (RadioButton) findViewById(R.id.scene_poi);
        contactButton = (RadioButton) findViewById(R.id.scene_Contact);
        selectButton = (RadioButton) findViewById(R.id.scene_select);
        confirmButton = (RadioButton) findViewById(R.id.scene_confirm);
        singleTrackButton = (RadioButton) findViewById(R.id.singletrack);
        doubleTrackButton = (RadioButton) findViewById(R.id.doubletrack);
        langspinner = (Spinner) findViewById(R.id.lang);

        switch (SpeechService.mode) {
            case 0:
                cloudButton.setChecked(true);
                break;
            case 1:
                localButton.setChecked(true);
                break;
            case 2:
                mixedButton.setChecked(true);
                break;
            default:
                localButton.setChecked(true);
                break;
        }
        switch (SpeechService.scene) {
            case 0:
                allButton.setChecked(true);
                break;
            case 1:
                poiButton.setChecked(true);
                break;
            case 2:
                contactButton.setChecked(true);
                break;
            case 3:
                selectButton.setChecked(true);
                break;
            case 4:
                confirmButton.setChecked(true);
                break;
            default:
                allButton.setChecked(true);
                break;
        }
        switch (SpeechService.sleepInterval) {
            case 10:
                singleTrackButton.setChecked(true);
                break;
            case 5:
                doubleTrackButton.setChecked(true);
                break;
        }

        final ToggleButton toggleButton = (ToggleButton) findViewById(R.id.on_off);
        if (SpeechService.quitOnce) {
            toggleButton.setChecked(false);
        } else {
            toggleButton.setChecked(true);
        }
        List<String> pcmNameList = new ArrayList<String>();
        List<String> dictNameList = new ArrayList<String>();
        List<String> langlist = new ArrayList<String>();
        pcmNameList = readDirFile(PcmIndexPath);
        dictNameList = readDirFile(DictPath);
        langlist.add("chinese");
        langlist.add("english");
        langlist.add("cantonese");

        if(pcmNameList!=null){
            adapter = new ArrayAdapter<String>(this,
                    android.R.layout.simple_spinner_item, pcmNameList);
            spinner.setAdapter(adapter);
        }else{
            Log.d(TAG, "onCreate : pcmNameList is null");
        }
        if(dictNameList!=null){
            adapterDict = new ArrayAdapter<String>(this,
                    android.R.layout.simple_spinner_item, dictNameList);
            spinnerDict.setAdapter(adapterDict);
        }
        else{
            Log.d(TAG, "onCreate : dictNameList is null");
        }

        langAdpter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, langlist);
        langspinner.setAdapter(langAdpter);
        langspinner.setOnItemSelectedListener(new OnItemSelectedListener() {

            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1, int pos,
                                       long arg3) {
                String lang = langspinner.getSelectedItem().toString();
                binder.ReCreate(lang);
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0) {

            }

        });

        uploadDict.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View arg0) {
                String dictFile = spinnerDict.getSelectedItem().toString();
                binder.upLoadDict(dictFile);
            }
        });
        toggleButton.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                if (toggleButton.isChecked()) {
                    ISS_SR_MODE = selectmode();
                    ISS_SR_SCENE = selectsecene();
                    String txtPath = spinner.getSelectedItem().toString();

                    binder.setSleepInterval(setTrack());
                    binder.batchRec(ISS_SR_MODE, ISS_SR_SCENE, txtPath);
                } else {
                    binder.stopRec();
                }
            }
        });
        mHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what) {
                    case MSG_TYPE_SR: {
                        Bundle b = msg.getData();
                        long uMsg = b.getLong("uMsg");
                        long wParam = b.getLong("wParam");
                        String lParam = b.getString("lParam");
                        switch ((int) uMsg) {
                            case 1111111111: {
                                mTextView.append("音频文件不存在\n");
                            }
                            case 20000: {
                                if (wParam == ISSErrors.ISS_SUCCESS) {
                                    Date initEndDate = null;
                                    try {
                                        initEndDate = dfs.parse(dfs.format(new Date()));
                                        long time = initEndDate.getTime() - binder.initDate
                                                .getTime();
                                        mTextView.append("成功创建识别会话,初始化时间=" + time + "\n");
                                    } catch (Exception e) {
                                        e.printStackTrace();
                                    }
                                    Log.d("ISS_SR_MSG_InitStatus", "初始化成功");
                                } else {
                                    if (wParam == ISSErrors.ISS_ERROR_FAIL)
                                        mTextView.append("创建识别会话失败,尝试重新创建.\n");
                                    if (wParam == ISSErrors.ISS_ERROR_OUT_OF_MEMORY)
                                        mTextView.append("创建识别会话失败,内存不足。尝试重新创建.\n");
                                }
                                break;
                            }
                            case 20001: {
                                if (wParam == ISSErrors.ISS_SUCCESS) {
                                    mTextView.append("本地个性化数据上传成功\n");
                                    mTextView.append(lParam);
                                    Log.d("zhfu local dict", lParam);
                                }
                                if (wParam == ISSErrors.ISS_ERROR_INVALID_JSON_FMT) {
                                    mTextView.append("输入的Json格式有问题\n");
                                }
                                if (wParam == ISSErrors.ISS_ERROR_INVALID_JSON_INFO) {
                                    mTextView.append("没有从Json输入中提取到必要的个性化数据\n");
                                }
                                break;
                            }

                            case 20002: {
                                if (wParam == ISSErrors.ISS_SUCCESS) {
                                    mTextView.append("云端个性化数据上传成功\n");
                                    mTextView.append(lParam);
                                    Log.d("zhfu cloud dict", lParam);
                                }
                                if (wParam == ISSErrors.ISS_ERROR_INVALID_JSON_FMT)
                                    mTextView.append("输入的Json格式有问题\n");
                                if (wParam == ISSErrors.ISS_ERROR_INVALID_JSON_INFO)
                                    mTextView.append("没有从Json输入中提取到必要的个性化数据\n");
                                if (wParam >= ISSErrors.ISS_ERROR_NET_GENERAL
                                        && wParam <= ISSErrors.ISS_ERROR_NET_DNS)
                                    mTextView.append("网络错误:" + String.valueOf(wParam)
                                            + "\n");
                                if (wParam == ISSErrors.ISS_ERROR_FAIL)
                                    mTextView.append("云端个性化数据上传失败\n");
                                break;
                            }
                            case 20004: {
                                mTextView.setText("");
                                mTextView.append("响应超时,没有在预定的时间内检测到语音"
                                        + df.format(new Date()) + "\n");
                                break;
                            }
                            case 20005: {
                                mTextView.setText("");

                                mTextView.append("检测到语音开始_SpeechStart:"
                                        + df.format(new Date()) + "\n");
                                break;
                            }
                            case 20006: {
                                mTextView
                                        .append("语音超时，正在进行识别处理,不需要再写入数据_SpeechTimeOut:"
                                                + df.format(new Date()) + "\n");
                                break;
                            }
                            case 20007: {
                                mTextView
                                        .append("检测到语音结束点，正在进行识别处理,不需要再写入数据_SpeechEnd:"
                                                + df.format(new Date()) + "\n");
                                break;
                            }
                            case 20008: {
                                mTextView.append("出现其它错误，识别引擎停止  MSG_Error:" + "wParam"
                                        + wParam + df.format(new Date()) + "\n");
                                break;
                            }
                            case 20009: {
                                mTextView.setText("---识别结果--" + df.format(new Date())
                                        + "---\n" + lParam + "\n---识别结束------\n");
                                break;
                            }
                            case 20003:
                                break;
                            default:
                                mTextView.append("Unhandled Msg： " + uMsg + ", wparam: " + wParam
                                        + "\n");
                                System.out.println("Unhandled Msg： " + uMsg + ", wparam: " + wParam);
                        }
                        break;
                    }
                    default: {
                        mTextView.append("收到未定义消息类型\n");
                    }
                }
            }
        };

        startService(new Intent(this, SpeechService.class));
    }

    @Override
    protected void onPause() {
        // TODO Auto-generated method stub
        super.onPause();
        unbindService(conn);
        binder = null;
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();

        bindService(new Intent(this, SpeechService.class), conn, BIND_AUTO_CREATE);
    }

    public int selectmode() {
        if (cloudButton.isChecked()) {
            return 0;
        } else if (localButton.isChecked()) {
            return 1;
        } else if (mixedButton.isChecked()) {
            return 2;
        } else {
            return 3;
        }
    }

    public int selectsecene() {
        if (allButton.isChecked()) {
            return 0;
        } else if (poiButton.isChecked()) {
            return 1;
        } else if (contactButton.isChecked()) {
            return 2;
        } else if (selectButton.isChecked()) {
            return 3;
        } else if (confirmButton.isChecked()) {
            return 4;
        } else {
            return 0;
        }
    }

    public int setTrack() {
        if (doubleTrackButton.isChecked()) {
            return 5;
        } else {
            return 10;
        }
    }


    @Override
    protected void onStop() {
        // TODO Auto-generated method stub
        super.onStop();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

    }

    ISRListener isrListener = new ISRListener() {
        @Override
        public void onSRMsgProc_(long uMsg, long wParam, String lParam) {
            Message msg = new Message();
            msg.what = MSG_TYPE_SR;
            Bundle b = new Bundle();
            b.putLong("uMsg", uMsg);
            b.putLong("wParam", wParam);
            b.putString("lParam", lParam);
            msg.setData(b);
            msg.setTarget(mHandler);
            msg.sendToTarget();
        }
    };

    private List<String> readDirFile(String dirPath) {
        File dictDir = null;
        File[] dictFile = null;
        dictDir = new File(dirPath);
        dictFile = dictDir.listFiles();
        if (dictFile == null) {
            return null;
        }

        List<String> fileList = new ArrayList<String>();

        for (int i = 0; i != dictFile.length; i++) {
            fileList.add(dictFile[i].getAbsolutePath());
        }
        return fileList;
    }
}

