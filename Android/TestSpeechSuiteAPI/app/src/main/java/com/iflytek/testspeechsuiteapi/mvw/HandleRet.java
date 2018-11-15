package com.iflytek.testspeechsuiteapi.mvw;


import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.speech.ISSErrors;


public class HandleRet {
    public static void handleStartRet(int err) {
        String tag = "MvwSession.start";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(tag, "成功", 0);
                break;
            case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                ILog.e(tag, "无效唤醒句柄");
                break;
            case ISSErrors.ISS_ERROR_INVALID_CALL:
                ILog.e(tag, "错误调用");
                break;
            default:
                ILog.e(tag, "UnHandled MSG:" + err);
        }
    }

    public static void handleSetThresholdRet(int err) {
        String tag = "MvwSession.setThreshold";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(tag, "成功", 0);
                break;
            case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                ILog.e(tag, "无效唤醒句柄");
                break;
            case ISSErrors.ISS_ERROR_INVALID_CALL:
                ILog.e(tag, "错误调用");
                break;
            default:
                ILog.e(tag, "UnHandled MSG:" + err);
        }
    }

    public static void handleSetMvwKeyWordsRet(int err) {
        String tag = "MvwSession.setMvwKeyWords";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(tag, "成功", 0);
                break;
            case ISSErrors.ISS_ERROR_INVALID_PARA:
                ILog.e(tag, "参数非法");
                break;
            default:
                ILog.e(tag, "UnHandled MSG:" + err);
        }
    }

    public static void handleSetMvwDefaultKeyWordsRet(int err) {
        String tag = "MvwSession.setMvwDefaultKeyWords";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(tag, "成功", 0);
                break;
            case ISSErrors.ISS_ERROR_INVALID_PARA:
                ILog.e(tag, "参数非法");
                break;
            default:
                ILog.e(tag, "UnHandled MSG:" + err);
        }
    }
}

