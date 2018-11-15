package com.iflytek.testspeechsuitenative.mvw;

import android.os.Environment;
import android.util.Log;

import com.iflytek.testspeechsuitenative.Tool;
import com.iflytek.speech.NativeHandle;
import com.iflytek.speech.libissmvw;


public class TestMVW {
    private String Tag = "TestMVW";
    NativeHandle mNativeHandle;
    DefMVW def;
    Tool tool;

    public TestMVW() {
        mNativeHandle = new NativeHandle();
        def = new DefMVW();
        tool = new Tool();

        int errId = libissmvw.create(mNativeHandle, def.resDir, def.iMvwListener);
        Log.d(Tag, "create libissmvw return " + errId);
    }

    public void test() {
        libissmvw.start(mNativeHandle, 1);
        tool.loadBigPcmAndAppendAudioData("mvw", mNativeHandle, Environment.getExternalStorageDirectory().getAbsolutePath() + "/test.wav", def);
//        tool.loadBigPcmAndAppendAudioData("mvw", mNativeHandle, def.mvwPcm_global, def);
    }
}
