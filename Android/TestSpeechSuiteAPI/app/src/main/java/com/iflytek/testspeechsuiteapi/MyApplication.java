package com.iflytek.testspeechsuiteapi;

import android.app.Application;

/**
 * Created by qqxiong on 2018/10/22.
 */

public class MyApplication extends Application {
    private static MyApplication instance;

    public static MyApplication getInstance() {
        return instance;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
    }
}