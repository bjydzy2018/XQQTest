package com.iflytek.testspeechsuitenative.tts;
import android.os.Environment;
import android.util.Log;

import com.iflytek.testspeechsuitenative.Def;
import com.iflytek.speech.ITtsListener;

/**
 * <p>
 * 合成模块相关定义类。
 */

class DefTTS implements Def {
    String resDir = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res/tts"; // 资源路径
    public String ttsList = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/tts/pro.txt"; // 合成一致性列表

    boolean msgOnDataReady;

    ITtsListener mTtsListener;

    public DefTTS() {
        mTtsListener = new ITtsListener() {
            String Tag = "ITtsListener";

            @Override
            public void onDataReady() {
                Log.i(Tag, "onDataReady");

                msgOnDataReady = true;
            }

            @Override
            public void onProgress(int nTextIndex, int nTextLen) {
            }
        };
    }

    @Override
    public void initMsg() {
        msgOnDataReady = false;
    }
}
