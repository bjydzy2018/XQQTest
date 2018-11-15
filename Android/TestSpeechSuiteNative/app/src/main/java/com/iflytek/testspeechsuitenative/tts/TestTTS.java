package com.iflytek.testspeechsuitenative.tts;

import android.util.Log;

import com.iflytek.testspeechsuitenative.Test;
import com.iflytek.speech.NativeHandle;
import com.iflytek.speech.libisstts;

import java.text.ParseException;
import java.util.Date;

/**
 * <p>
 * 合成基础测试类。
 */

class TestTTS extends Test {
    private String Tag = "TestTTS";
    int ttsBytes = 320;
    byte[] buffer = new byte[ttsBytes];
    int[] bufferSize = new int[1];

    NativeHandle mNativeHandle;
    DefTTS def;

    TestTTS() {
        mNativeHandle = new NativeHandle();
        def = new DefTTS();


    }
}

