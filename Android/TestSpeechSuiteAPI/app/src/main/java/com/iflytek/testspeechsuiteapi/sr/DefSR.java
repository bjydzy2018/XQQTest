package com.iflytek.testspeechsuiteapi.sr;

import android.os.Environment;

import com.iflytek.testspeechsuiteapi.Def;
import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.speech.ISSErrors;
import com.iflytek.sr.IIsrListener;
import com.iflytek.sr.SrSession;

import java.text.SimpleDateFormat;
import java.util.Date;

public class DefSR extends Def {
    Tool tool = new Tool();

    public String resDir = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res/sr";
    public String resDir_wrong = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res";

    public String srPcm_responseTimeOut = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/ResponseTimeout.pcm";
    public String srPcm_closePhone = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/closePhone.pcm";
    public String srPcm_en = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/en.pcm";
    public String srPcm_iflytek = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/iflytek.wav";
    public String srPcm_callZhangSan = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/PhoneZhangsan.wav";
    public String srPcm_callZhang = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/CallZhang.wav";
    public String srPcm_zhangSan = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/Zhangsan.wav";
    public String srPcm_callBaiYaWei = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/PhoneBaiYawei.wav";
    public String srPcm_goiflytek = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/NavigateIflytek.wav";
    public String srPcm_raoKouLing = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/Raokouling.wav";
    public String srPcm_siXiaoSi = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/SiXiaoSi.wav";
    public String srPcm_fuJinJiaYouZhan = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/FuJinDeJiaYouZhan.wav";
    public String srPcm_ResponseTimeOut_21 = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/ResponseTimeOut21.wav";
    public String srPcm_ResponseTimeOut_151 = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/ResponseTimeOut151.wav";
    public String srPcm_ResponseTimeOut_100 = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/ResponseTimeOut100.wav";
    public String srPcm_ResponseTimeOut_1_5 = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/ResponseTimeOut1.5.wav";
    public String srPcm_ResponseTimeOut_0_1 = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/ResponseTimeOut0.1.wav";
    public String srPcm_ResponseTimeOut_0_11 = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/ResponseTimeOut0.11.wav";
    public String srPcm_SpeechTimeOut_3_1 = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/SpeechTimeOut3.1.wav";
    public String srPcm_SpeechTimeOut_21 = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/SpeechTimeOut21.wav";
    public String srPcm_SpeechTail0_65 = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/SpeechTail0.65.wav";
    public String srPcm_SpeechTail5 = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/SpeechTail5.wav";
    public String srPcm_MvwTimeout2 = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/MvwTimeout2.wav";
    public String srPcm_MvwTimeout10 = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/MvwTimeout10.wav";

    public String srPcm_first = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/first.pcm";
    public String srPcm_confirm = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/confirm.wav";
    public String srPcm_answer = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/answer.wav";

    public String dict1 = tool.readFile(Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/dict/dict_list_nlp.txt");
    public String dict_500 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/dict_M-500.txt");
    public String dict_1000 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/dict_M-1000.txt");
    public String dict_5000 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/dict_M-5000.txt");
    public String dict_wrong_format = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format.txt");
    public String dict_wrong_format1 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format1.txt");
    public String dict_wrong_format2 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format2.txt");
    public String dict_wrong_format3 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format3.txt");
    public String dict_wrong_format4 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format4.txt");
    public String dict_wrong_format5 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format5.txt");
    public String dict_wrong_format6 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format6.txt");
    public String dict_wrong_format7 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format7.txt");
    public String dict_wrong_format8 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format8.txt");
    public String dict_wrong_format9 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/dict/wrong_format9.txt");

    public String stability_pcms_list = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/stability";

    public String srCmd_10000 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_10000.txt");
    public String srCmd_siXiaoSi = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_SiXiaoSi.txt");
    public String srCmd_wrong_format = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format.txt");
    public String srCmd_wrong_format1 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format1.txt");
    public String srCmd_wrong_format2 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format2.txt");
    public String srCmd_wrong_format3 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format3.txt");
    public String srCmd_wrong_format4 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format4.txt");
    public String srCmd_wrong_format5 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format5.txt");
    public String srCmd_wrong_format6 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format6.txt");
    public String srCmd_wrong_format7 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format7.txt");
    public String srCmd_wrong_format8 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format8.txt");
    public String srCmd_wrong_format9 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/cmd_wrong_format9.txt");

    public String srNlp_wrong_format = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/nlp_wrong_format.txt");
    public String srNlp_wrong_format1 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/nlp_wrong_format1.txt");
    public String srNlp_wrong_format2 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/nlp_wrong_format2.txt");
    public String srNlp_wrong_format3 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/nlp_wrong_format3.txt");
    public String srNlp_wrong_format4 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/sr/cmd/nlp_wrong_format4.txt");

    public boolean msgInitStatus;
    public boolean msgInitStatus_success;
    public boolean msgInitStart;
    public boolean msgResult;
    public boolean msgUploadDictCloud;
    public boolean msgUploadDictCloud_ISS_SUCCESS;
    public boolean msgUploadDictCloud_ISS_ERROR_INVALID_JSON_FMT;
    public boolean msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO;
    public boolean msgUploadDictLocal;
    public boolean msgUploadDictLocal_ISS_SUCCESS;
    public boolean msgUploadDictLocal_ISS_ERROR_INVALID_JSON_FMT;
    public boolean msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO;
    public boolean msgSpeechEnd;
    public boolean msgFileNotFound;
    public boolean msgError;
    public boolean msgResponseTimeOut;
    public boolean msgSpeechTimeOut;

    public String srResult;
    public long srErrCode;
    public long uploadDictCloudErrCode;

    SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-ddHH:mm:ss.SSS");
    long speechend_time = 0;
    Date result_time = null;
    Date uploadDictToCloud_time = null;
    Date uploadDictToLocal_time = null;

    public IIsrListener isrListener = new IIsrListener() {
        @Override
        public void onSrMsgProc(long uMsg, long wParam, String lParam) {
            String onSrMsgProcTag = "SrSession.onSrMsgProc";

            if (uMsg == ISSErrors.ISS_ERROR_FILE_NOT_FOUND) {
                ILog.e(onSrMsgProcTag, setOnSrMsgProcText("ISS_ERROR_FILE_NOT_FOUND", "", lParam)
                        , isWriteToActivity);
                msgFileNotFound = true;
            } else if (uMsg == SrSession.ISS_SR_MSG_InitStatus) {
                if (wParam == ISSErrors.ISS_SUCCESS) {
                    msgInitStatus_success = true;
                    ILog.i(onSrMsgProcTag, setOnSrMsgProcText("ISS_SR_MSG_InitStatus",
                            "ISS_SUCCESS", lParam), isWriteToActivity);
                    ILog.i("ISS_SR_MSG_InitStatus", "初始化成功", isWriteToActivity);
                } else if (wParam == ISSErrors.ISS_ERROR_FAIL) {
                    ILog.e(onSrMsgProcTag, setOnSrMsgProcText("ISS_SR_MSG_InitStatus",
                            "ISS_ERROR_FAIL", lParam), isWriteToActivity);
                } else if (wParam == ISSErrors.ISS_ERROR_OUT_OF_MEMORY) {
                    ILog.e(onSrMsgProcTag, setOnSrMsgProcText("ISS_SR_MSG_InitStatus",
                            "ISS_ERROR_OUT_OF_MEMORY", lParam), isWriteToActivity);
                } else {
                    ILog.e(onSrMsgProcTag, "初始化出错："+wParam, isWriteToActivity);
                }
                msgInitStatus = true;
            } else if (uMsg == SrSession.ISS_SR_MSG_UpLoadDictToCloudStatus) {
                try {
                    uploadDictToCloud_time = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }
                msgUploadDictCloud = true;
                if (wParam == ISSErrors.ISS_SUCCESS) {
                    msgUploadDictCloud_ISS_SUCCESS = true;
                    ILog.i(onSrMsgProcTag, setOnSrMsgProcText
                                    ("ISS_SR_MSG_UpLoadDictToCloudStatus", "ISS_SUCCESS", lParam),
                            isWriteToActivity);
                } else if (wParam == ISSErrors.ISS_ERROR_INVALID_JSON_FMT) {
                    msgUploadDictCloud_ISS_ERROR_INVALID_JSON_FMT = true;
                    ILog.e(onSrMsgProcTag, setOnSrMsgProcText
                            ("ISS_SR_MSG_UpLoadDictToCloudStatus", "ISS_ERROR_INVALID_JSON_FMT",
                                    lParam), isWriteToActivity);
                } else if (wParam == ISSErrors.ISS_ERROR_INVALID_JSON_INFO) {
                    msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO = true;
                    ILog.e(onSrMsgProcTag, setOnSrMsgProcText
                            ("ISS_SR_MSG_UpLoadDictToCloudStatus", "ISS_ERROR_INVALID_JSON_INFO",
                                    lParam), isWriteToActivity);
                } else if (wParam == ISSErrors.ISS_ERROR_FAIL) {
                    ILog.e(onSrMsgProcTag, setOnSrMsgProcText
                                    ("ISS_SR_MSG_UpLoadDictToCloudStatus", "ISS_ERROR_FAIL", lParam),
                            isWriteToActivity);
                } else {
                    uploadDictCloudErrCode = wParam;
                    ILog.e(onSrMsgProcTag, setOnSrMsgProcText
                            ("ISS_SR_MSG_UpLoadDictToCloudStatus", "ISS_ERROR_NET_XXXX:" +
                                    wParam, lParam), isWriteToActivity);
                }
            } else if (uMsg == SrSession.ISS_SR_MSG_UpLoadDictToLocalStatus) {
                try {
                    uploadDictToLocal_time = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }
                msgUploadDictLocal = true;
                if (wParam == ISSErrors.ISS_SUCCESS) {
                    msgUploadDictLocal_ISS_SUCCESS = true;
                    ILog.i(onSrMsgProcTag, setOnSrMsgProcText
                                    ("ISS_SR_MSG_UpLoadDictToLocalStatus", "ISS_SUCCESS", lParam),
                            isWriteToActivity);
                } else if (wParam == ISSErrors.ISS_ERROR_INVALID_JSON_FMT) {
                    msgUploadDictLocal_ISS_ERROR_INVALID_JSON_FMT = true;
                    ILog.e(onSrMsgProcTag, setOnSrMsgProcText
                            ("ISS_SR_MSG_UpLoadDictToLocalStatus", "ISS_ERROR_INVALID_JSON_FMT",
                                    lParam), isWriteToActivity);
                } else if (wParam == ISSErrors.ISS_ERROR_INVALID_JSON_INFO) {
                    msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO = true;
                    ILog.e(onSrMsgProcTag, setOnSrMsgProcText
                            ("ISS_SR_MSG_UpLoadDictToLocalStatus", "ISS_ERROR_INVALID_JSON_INFO",
                                    lParam), isWriteToActivity);
                }
            } else if (uMsg == SrSession.ISS_SR_MSG_VolumeLevel) {
                //setOnSrMsgProcText("ISS_SR_MSG_VolumeLevel", "" + wParam, lParam);
            } else if (uMsg == SrSession.ISS_SR_MSG_ResponseTimeout) {
                msgResponseTimeOut = true;
                ILog.e(onSrMsgProcTag, "ISS_SR_MSG_ResponseTimeout", isWriteToActivity);
            } else if (uMsg == SrSession.ISS_SR_MSG_SpeechStart) {
                ILog.i(onSrMsgProcTag, "ISS_SR_MSG_SpeechStart, VAD前端点: " + wParam,
                        isWriteToActivity);
            } else if (uMsg == SrSession.ISS_SR_MSG_SpeechTimeOut) {
                msgSpeechTimeOut = true;
                ILog.e(onSrMsgProcTag, "ISS_SR_MSG_SpeechTimeOut", isWriteToActivity);
            } else if (uMsg == SrSession.ISS_SR_MSG_SpeechEnd) {
                speechend_time = wParam;
                msgSpeechEnd = true;
                ILog.i(onSrMsgProcTag, "ISS_SR_MSG_SpeechEnd, VAD后端点: " + wParam,
                        isWriteToActivity);
            } else if (uMsg == SrSession.ISS_SR_MSG_WaitingForCloudResult) {
                ILog.i(onSrMsgProcTag, "ISS_SR_MSG_WaitingForCloudResult", isWriteToActivity);
            } else if (uMsg == SrSession.ISS_SR_MSG_Error) {
                srErrCode = wParam;
                ILog.e(onSrMsgProcTag, setOnSrMsgProcText("ISS_SR_MSG_Error", "" + wParam,
                        lParam), isWriteToActivity);
                msgError = true;
            } else if (uMsg == SrSession.ISS_SR_MSG_Result) {
                try {
                    result_time = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }
                msgResult = true;
                srResult = lParam;
                ILog.i(onSrMsgProcTag, setOnSrMsgProcText("ISS_SR_MSG_Result", "" + wParam,
                        lParam), isWriteToActivity);
            } else if (uMsg == SrSession.ISS_SR_MSG_ErrorDecodingAudio) {
                ILog.e(onSrMsgProcTag, setOnSrMsgProcText("ISS_SR_MSG_ErrorDecodingAudio", "",
                        ""), isWriteToActivity);
            } else {
                ILog.w(onSrMsgProcTag, "UnHandled MSG:" + uMsg, isWriteToActivity);
            }
        }

        @Override
        public void onSrInited(boolean state, int errId) {
            String onSrInitedTag = "SrSession.onSrInited";

            if (state) {
                msgInitStart = true;
                ILog.i("SrSession", "开始初始化", isWriteToActivity);
            } else {
                if (errId == ISSErrors.REMOTE_EXCEPTION)
                    ILog.e(onSrInitedTag, "state::false, errId::REMOTE_EXCEPTION",
                            isWriteToActivity);
                else if (errId == ISSErrors.BIND_FAILED)
                    ILog.e(onSrInitedTag, "state::false, errId::BIND_FAILED", isWriteToActivity);
                else
                    ILog.e(onSrInitedTag, "state::false, errId::UnHandled MSG", isWriteToActivity);
            }
        }
    };

    public void initMsg() {
        msgInitStatus = false;
        msgInitStatus_success = false;
        msgResult = false;
        msgUploadDictCloud = false;
        msgUploadDictCloud_ISS_SUCCESS = false;
        msgUploadDictCloud_ISS_ERROR_INVALID_JSON_FMT = false;
        msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO = false;
        msgUploadDictLocal = false;
        msgUploadDictLocal_ISS_SUCCESS = false;
        msgUploadDictLocal_ISS_ERROR_INVALID_JSON_FMT = false;
        msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO = false;
        msgSpeechEnd = false;
        msgFileNotFound = false;
        msgError = false;
        msgInitStart = false;

        srResult = "";
    }

    private String setOnSrMsgProcText(String uMsg, String wParam, String lParam) {
        return ("uMsg::" + uMsg + ", wParam::" + wParam + ", lParam::" + (lParam == null ||
                lParam.equals("") ? "" : lParam));
    }
}

