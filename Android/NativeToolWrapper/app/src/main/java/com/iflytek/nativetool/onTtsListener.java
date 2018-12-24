package com.iflytek.nativetool;

import com.iflytek.speech.ITtsListener;

/**
 * Created by qqxiong on 2017/8/27.
 */

public class onTtsListener implements ITtsListener {
    public long Native_point = 0L;
    public void setNative_point(long native_point) {
        this.Native_point = native_point;
    }

    @Override
    public void onDataReady() {
        AiuiToolMainEntry.onNativeDataReady(Native_point);
    }

    @Override
    public void onProgress(int var1, int var2) {
    }
}
