package com.iflytek.testspeechsuitenative.mvw;


import android.util.Log;

import com.iflytek.testspeechsuitenative.Tool;
import com.iflytek.speech.NativeHandle;
import com.iflytek.speech.libissmvw;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class TestMultiInstPerf {
    String Tag = "TestMultiInstPerf";

    public void testOneInst() {
        ExecutorService threadPool = Executors.newCachedThreadPool();
        threadPool.execute(new ThreadFunc(new DefMVW().resDir));
    }

    public void testTwoInst() {
        ExecutorService threadPool = Executors.newCachedThreadPool();
        threadPool.execute(new ThreadFunc(new DefMVW().resDir));
        threadPool.execute(new ThreadFunc(new DefMVW().resDir_second));
    }

    private class ThreadFunc implements Runnable {
        NativeHandle mNativeHandle;
        DefMVW def;
        Tool tool;

        ThreadFunc(String resDir) {
            mNativeHandle = new NativeHandle();
            def = new DefMVW();
            tool = new Tool();

            int errId = libissmvw.create(mNativeHandle, resDir, def.iMvwListener);
            Log.d(Tag, "create libissmvw return " + errId);

            errId = libissmvw.start(mNativeHandle, 1);
            Log.i(Tag, "start libissmvw return " + errId);
        }

        @Override
        public void run() {
            while (true) {
                tool.loadPcmAndAppendAudioData("mvw", mNativeHandle, def.mvwPcm_global, def);
            }
        }
    }
}
