package com.iflytek.testspeechsuiteapi.cata;


import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.speech.ISSErrors;

public class HandleRet {
    public static void handleIndexCreateRet(int err) {
        String Tag = "CataIndexInst.create";

        if (err == ISSErrors.ISS_ERROR_RES_DATA) {
            ILog.e(Tag, "失败,资源错误");
        } else if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败:" + err);
        }
    }

    public static void handleIndexAddIdxEntityRet(int err) {
        String Tag = "CataIndexInst.addIdxEntity";

        if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败:" + err);
        }
    }

    public static void handleIndexEndIdxEntityRet(int err) {
        String Tag = "CataIndexInst.endIdxEntity";
        if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败:" + err);
        }
    }

    public static void handleIndexDelIdxEntityRet(int err) {
        String Tag = "CataIndexInst.delIdxEntityErr";
        if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败：" + err);
        }
    }

    public static void handleIndexDropRet(int err) {
        String Tag = "CataIndexInst.drop";

        if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败：" + err);
        }
    }

    public static void handleIndexDestroyRet(int err) {
        String Tag = "CataIndexInst.destroy";

        if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败：" + err);
        }
    }

    public static void handleIndexReCreateRet(int err) {
        String Tag = "CataIndexInst.reCreate";
        if (err == ISSErrors.ISS_ERROR_RES_DATA) {
            ILog.e(Tag, "资源错误");
        } else if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败：" + err);
        }
    }

    public static void handleSearchCreateRet(int err) {
        String Tag = "CataSearchInst.create";

        if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败:" + err);
        }
    }

    public static void handleSearchAsyncRet(int err) {
        String Tag = "CataSearchInst.searchASync";

        if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "成功");
        } else {
            ILog.e(Tag, "失败:" + err);
        }
    }

    public static void handleSearchDestroyRet(int err) {
        String Tag = "CataSearchInst.destroy";

        if (err == ISSErrors.ISS_SUCCESS) {
            ILog.i(Tag, "CataSearchInst destroy成功");
        } else {
            ILog.e(Tag, "CataSearchInst destroy失败：" + err);
        }
    }
}
