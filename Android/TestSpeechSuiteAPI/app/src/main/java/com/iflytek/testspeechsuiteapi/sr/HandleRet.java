package com.iflytek.testspeechsuiteapi.sr;


import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.speech.ISSErrors;

/**
 * 此类专门处理各个接口返回的结果。
 */
public class HandleRet {
    public static void handlStartRet(int err) {
        String tag = "SrSession.start";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(tag, "start::ISS_SUCCESS");
                break;
            case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                ILog.e(tag, "start::ISS_ERROR_INVALID_HANDLE");
                break;
            case ISSErrors.ISS_ERROR_INVALID_PARA:
                ILog.e(tag, "start::ISS_ERROR_INVALID_PARA");
                break;
            case ISSErrors.ISS_ERROR_INVALID_JSON_FMT:
                ILog.e(tag, "start::ISS_ERROR_INVALID_JSON_FMT");
                break;
            case ISSErrors.ISS_ERROR_INVALID_JSON_INFO:
                ILog.e(tag, "start::ISS_ERROR_INVALID_JSON_INFO");
                break;
            case ISSErrors.ISS_ERROR_INVALID_CALL:
                ILog.e(tag, "start::ISS_ERROR_INVALID_CALL");
                break;
            case ISSErrors.ISS_ERROR_BUILDING_GRM:
                ILog.e(tag, "start::ISS_ERROR_BUILDING_GRM");
                break;
            case ISSErrors.INVALID_SESSION:
                ILog.e(tag, "start::INVALID_SESSION");
                break;
            case ISSErrors.REMOTE_EXCEPTION:
                ILog.e(tag, "start::REMOTE_EXCEPTION");
                break;
            default:
                ILog.e(tag, "start::UnHandled MSG");
        }
    }

    public static void handleGetActiveKeyRet(int err) {
        String Tag = "SrSession.getActiveKey";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(Tag, "getActiveKey::ISS_SUCCESS");
                break;
            case ISSErrors.ISS_ERROR_FAIL:
                ILog.e(Tag, "getActiveKey::ISS_ERROR_FAIL");
                break;
            case ISSErrors.ISS_ERROR_MACHINE_CODE_NOT_SET:
                ILog.e(Tag, "getActiveKey::ISS_ERROR_MACHINE_CODE_NOT_SET");
                break;
            case ISSErrors.ISS_ERROR_INVALID_MACHINE_CODE:
                ILog.e(Tag, "getActiveKey::ISS_ERROR_INVALID_MACHINE_CODE");
                break;
            case ISSErrors.ISS_ERROR_ACTIVATE_TO_MANY_TIMES:
                ILog.e(Tag, "getActiveKey::ISS_ERROR_ACTIVATE_TO_MANY_TIMES");
                break;
            default:
                ILog.e(Tag, "getActiveKey::ISS_ERROR_NET_XXXX::" + err);
        }
    }

    public static void handleUploadDictRet(int err) {
        String tag = "SrSession.uploadDict";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(tag, "ISS_SUCCESS");
                break;
            case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                ILog.e(tag, "ISS_ERROR_INVALID_HANDLE");
                break;
            case ISSErrors.ISS_ERROR_INVALID_PARA:
                ILog.e(tag, "ISS_ERROR_INVALID_PARA");
                break;
            case ISSErrors.ISS_ERROR_INVALID_CALL:
                ILog.e(tag, "ISS_ERROR_INVALID_CALL");
                break;
            case ISSErrors.INVALID_SESSION:
                ILog.e(tag, "INVALID_SESSION");
                break;
            case ISSErrors.REMOTE_EXCEPTION:
                ILog.e(tag, "REMOTE_EXCEPTION ");
                break;
            case ISSErrors.ISS_ERROR_FAIL:
                ILog.e(tag, "ISS_ERROR_FAIL ");
                break;
            default:
                ILog.e(tag, "UnHandled Msg: "+err);
        }
    }

    public static void handleSetParamRet(int err) {
        String tag = "SrSession.setParam";

        switch (err) {
            case ISSErrors.ISS_SUCCESS:
                ILog.i(tag, "ISS_SUCCESS");
                break;
            case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                ILog.e(tag, "ISS_ERROR_INVALID_HANDLE");
                break;
            case ISSErrors.ISS_ERROR_INVALID_PARA:
                ILog.e(tag, "ISS_ERROR_INVALID_PARA");
                break;
            case ISSErrors.ISS_ERROR_INVALID_CALL:
                ILog.e(tag, "ISS_ERROR_INVALID_CALL");
                break;
            default:
                ILog.e(tag, "UnHandled Msg: "+err);
        }
    }
}
