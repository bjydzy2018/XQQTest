package com.iflytek.nativetool;

import com.iflytek.speech.sr.ISRListener;

/**
 * Created by qqxiong on 2017/9/3.
 */

public class onSRMsgListener implements ISRListener {
    @Override
    public void onSRMsgProc_(long var1, long var3, String var5) {
        AiuiToolMainEntry.onSRNativeMsgProc(var1, var3, var5);
    }
}
