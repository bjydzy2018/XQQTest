package com.iflytek.testspeechsuitenative.sr;

import android.os.Environment;
import android.util.Log;

import com.iflytek.testspeechsuitenative.Def;
import com.iflytek.speech.libisssr;
import com.iflytek.speech.sr.ISRListener;

import java.util.Date;


public class DefSR implements Def {
    public String resDir = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res/sr";

    boolean msgInitSuccess = false;
    boolean msgResult = false;
    boolean msgSpeechEnd = false;
    boolean msgUploadDictLocal = false;
    boolean msgUploadDictCloud = false;
    Date uploadDictToCloud_time = null;
    Date uploadDictToLocal_time = null;
    String srResult = "";

    ISRListener mSRListener = new ISRListener() {
        String Tag = "ISRListener";

        @Override
        public void onSRMsgProc_(long uMsg, long wParam, String lParam) {
            if (uMsg != 20003)
                Log.i(Tag, "uMsg: " + uMsg + " wParam: " + wParam + " lParam: " + (lParam != null ? lParam : "null"));

            switch ((int) uMsg) {
                case 20000: // ISS_SR_MSG_InitStatus
                    if (wParam == 0) {
                        Log.i(Tag, "识别初始化成功");
                        msgInitSuccess = true;
                    } else {
                        Log.e(Tag, "识别初始化失败, wParam: " + wParam);
                    }
                    break;
                case 20007: // ISS_SR_MSG_SpeechEnd
                    msgSpeechEnd = true;
                    break;
                case 20009: // ISS_SR_MSG_Result
                    Log.i(Tag, "识别结果：" + lParam);
                    srResult = lParam;
                    msgResult = true;
                    break;
                case 20001: // ISS_SR_MSG_UpLoadDictToLocalStatus
                    try {
                        uploadDictToLocal_time = dfs.parse(dfs.format(new Date()));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    msgUploadDictLocal = true;
                    break;
                case 20002: // ISS_SR_MSG_UpLoadDictToCloudStatus
                    try {
                        uploadDictToCloud_time = dfs.parse(dfs.format(new Date()));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    msgUploadDictCloud = true;
                    break;
            }
        }
    };

    @Override
    public void initMsg() {
        msgInitSuccess = false;
        msgResult = false;
        msgSpeechEnd = false;
        msgUploadDictLocal = false;
        msgUploadDictCloud = false;
        uploadDictToCloud_time = null;
        uploadDictToLocal_time = null;
        srResult = "";
    }
}

