package com.iflytek.nativetool;

import com.iflytek.cata.ICataListener;

/**
 * Created by qqxiong on 2017/8/27.
 */

public class onICataListener implements ICataListener {
    public long Native_point = 0L;
    public void setNative_point(long native_point) {
        this.Native_point = native_point;
    }

    @Override
    public void onMsgProc(int var1, int var2, String var3) {
        AiuiToolMainEntry.onICataNativeListener(Native_point, var1, var2, var3);
    }
}
