package com.iflytek.testspeechsuitenative.cata;

import android.util.Log;

import com.iflytek.cata.CataNativeHandle;
import com.iflytek.cata.libisscata;
import com.iflytek.testspeechsuitenative.Tool;



public class TestCATA {
    private String Tag = "TestCATA";
    CataNativeHandle mNativeHandle;
    DefCATA def;
    Tool tool;

    TestCATA() {
        def = new DefCATA();
        tool = new Tool();
        mNativeHandle = new CataNativeHandle();
    }
}
