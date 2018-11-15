package com.iflytek.testspeechsuiteapi.tts;

import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.speech.ISSErrors;

public class HandleRet {
    public static void handleSessionStartRet(int err) {
        String Tag = "TtsSession.sessionStart";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(Tag, "sessionStart成功");
                break;
            case ISSErrors.ISS_ERROR_INVALID_PARA:
                ILog.e(Tag, "sessionStart失败，参数无效");
                break;
            case ISSErrors.ISS_ERROR_FILE_NOT_FOUND:
                ILog.e(Tag, "sessionStart失败，未找到资源文件");
                break;
            case ISSErrors.ISS_ERROR_OUT_OF_MEMORY:
                ILog.e(Tag, "sessionStart失败，内存不足");
                break;
            default:
                ILog.e(Tag, "sessionStart失败，UnHandled Msg:" + err);
        }
    }

    public static void handleStartSpeakRet(int err) {
        String Tag = "TtsSession.startSpeak";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(Tag, "startSpeak成功");
                break;
            case ISSErrors.ISS_ERROR_INVALID_PARA:
                ILog.e(Tag, "startSpeak失败，参数无效");
                break;
            case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                ILog.e(Tag, "startSpeak失败，TTS句柄无效");
                break;
            case ISSErrors.ISS_ERROR_INVALID_CALL:
                ILog.e(Tag, "startSpeak失败，无效调用");
                break;
            case ISSErrors.INVALID_SESSION:
                ILog.e(Tag, "startSpeak失败，初始化失败或未完成");
                break;
            default:
                ILog.w(Tag, "startSpeak失败，UnHandled Msg:" + err);
        }
    }

    public static void handleStopSpeakRet(int err) {
        String Tag = "TtsSession.stopSpeak";

        switch (err) {
            case 0:
                ILog.i(Tag, "stopSpeak成功");
                break;
            default:
                ILog.w(Tag, "stopSpeakRet失败，UnHandled Msg:" + err);
        }
    }
}

