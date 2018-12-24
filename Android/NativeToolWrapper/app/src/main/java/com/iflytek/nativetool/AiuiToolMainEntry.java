package com.iflytek.nativetool;

public class AiuiToolMainEntry {
    // NativeMainFunc
    public static void setListenerAiuiToolCallback(AiuiToolCallback listenerAiuiToolCallback_) {
        listenerAiuiToolCallback = listenerAiuiToolCallback_;
    }
    public static AiuiToolCallback listenerAiuiToolCallback = null;
    public static void onFinished() {
		if (listenerAiuiToolCallback != null) {
			listenerAiuiToolCallback.onFinished();
		}
	}
    public static native void startNativeMainFunc();

    // SR
    public static native void onSRNativeMsgProc(long var1, long var3, String var5);

    // TTS
    public static native void onNativeDataReady(long Native_point);

    // MVW
    public static native void onMVWNativeWakeup(long Native_point, int var1, int var2, int var3, String var4);
    public static native void onMVWNativeMsgProc(long Native_point, long var1, long var3, String var5);

    // CATA
    public static native void onICataNativeListener(long Native_point, int var1, int var2, String var3);

    static {
        System.loadLibrary("AiuiToolMainFuncCallJavaNativeApi");
    }
}
