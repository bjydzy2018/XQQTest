package com.iflytek.nativetool;

import com.iflytek.speech.mvw.IMVWListener;

/**
 * Created by qqxiong on 2017/8/27.
 */

public class onIMVWListener implements IMVWListener {
    public long Native_point = 0L;
    public void setNative_point(long native_point) {
        this.Native_point = native_point;
    }

    @Override
    public void onMVWWakeup(int var1, int var2, int var3, String var4) {
        AiuiToolMainEntry.onMVWNativeWakeup(Native_point, var1, var2, var3, var4);
    }
    @Override
    public void onMVWMsgProc_(long var1, long var3, String var5) {
        AiuiToolMainEntry.onMVWNativeMsgProc(Native_point, var1, var3, var5);
    }
}