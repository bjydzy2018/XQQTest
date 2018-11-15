package com.iflytek.testspeechsuiteapi.tts;

import android.os.Environment;

import com.iflytek.testspeechsuiteapi.Def;
import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.speech.ISSErrors;
import com.iflytek.tts.ITtsInitListener;
import com.iflytek.tts.ITtsListener;

import java.io.UnsupportedEncodingException;
import java.util.Random;

public class DefTTS extends Def {
    public String resDir = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res/tts";  // 合成资源路径
    public String resDir_wrong = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res";  // 合成资源错误路径
    public String ttsList = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/test.hcliu/tts/tts_list.txt";

    public boolean msgInitStatus;
    public boolean msgInitStatus_Success;
    public boolean msgInitStatus_INVALID_PARA;
    public boolean msgInitStatus_REMOTE_EXCEPTION;
    public boolean msgInitStatus_BIND_FAILED;
    public boolean msgInitStatus_FILE_NOT_FOUND;

    public boolean msgOnPlayBegin;
    public boolean msgOnPlayCompleted;
    public boolean msgOnPlayInterrupted;
    public boolean msgOnProgressReturn;

    public ITtsInitListener iTtsInitListener = new ITtsInitListener() {
        @Override
        public void onTtsInited(boolean state, int errId) {
            msgInitStatus = true;
            if (state) {
                msgInitStatus_Success = true;
                ILog.i("TtsSession", "初始化成功", isWriteToActivity);
            } else {
                switch (errId) {
                    case ISSErrors.ISS_ERROR_INVALID_PARA:
                        msgInitStatus_INVALID_PARA = true;
                        ILog.e("TtsSession.onTtsInited", "初始化失败，ISS_ERROR_INVALID_PARA", isWriteToActivity);
                        break;
                    case ISSErrors.REMOTE_EXCEPTION:
                        msgInitStatus_REMOTE_EXCEPTION = true;
                        ILog.e("TtsSession.onTtsInited", "初始化失败，REMOTE_EXCEPTION", isWriteToActivity);
                        break;
                    case ISSErrors.BIND_FAILED:
                        msgInitStatus_BIND_FAILED = true;
                        ILog.e("TtsSession.onTtsInited", "初始化失败，BIND_FAILED", isWriteToActivity);
                        break;
                    case ISSErrors.ISS_ERROR_FILE_NOT_FOUND:
                        msgInitStatus_FILE_NOT_FOUND = true;
                        ILog.e("TtsSession.onTtsInited", "初始化失败，FILE_NOT_FOUND", isWriteToActivity);
                        break;
                    default:
                        ILog.e("TtsSession.onTtsInited", "初始化失败，errId:" + errId, isWriteToActivity);
                }
            }
        }
    };

    public ITtsListener iTtsListener = new ITtsListener() {
        @Override
        public void onPlayBegin() {
            msgOnPlayBegin = true;
//            ILog.i("ITtsListener", "onPlayBegin", isWriteToActivity);
        }

        @Override
        public void onPlayCompleted() {
            msgOnPlayCompleted = true;
            ILog.i("ITtsListener", "onPlayCompleted", isWriteToActivity);
        }

        @Override
        public void onPlayInterrupted() {
            msgOnPlayInterrupted = true;
//            ILog.i("ITtsListener", "onPlayInterrupted", isWriteToActivity);
        }

        @Override
        public void onProgressReturn(int textindex, int textlen) {
            msgOnProgressReturn = true;
//            ILog.i("ITtsListener", "onProgressReturn, textindex:" + textindex + ", textlen:" + textlen, isWriteToActivity);
        }
    };

    /**
     * 随机生成中文字符。
     *
     * @param length 需要的字符串长度。
     * @return 生成的字符串。
     */
    public String getRandomCHStr(int length) {
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < length; i++) {
            String str = null;

            int hightPos, lowPos; // 定义高低位
            Random random = new Random();
            hightPos = (176 + Math.abs(random.nextInt(39)));//获取高位值
            lowPos = (161 + Math.abs(random.nextInt(93)));//获取低位值
            byte[] b = new byte[2];
            b[0] = (new Integer(hightPos).byteValue());
            b[1] = (new Integer(lowPos).byteValue());
            try {
                str = new String(b, "GBk");//转成中文
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            sb.append(str);
        }
        return sb.toString();
    }

    /**
     * 随机生成英文+数字组合字符串，可能只包含英文或只包含数字。
     *
     * @param length 需要的字符串长度。
     * @return 生成的字符串。
     */
    public String getRandomENStr(int length) {

        String val = "";
        Random random = new Random();

        //参数length，表示生成几位随机数
        for (int i = 0; i < length; i++) {
            String charOrNum = random.nextInt(2) % 2 == 0 ? "char" : "num";
            //输出字母还是数字
            if ("char".equalsIgnoreCase(charOrNum)) {
                //输出是大写字母还是小写字母
                int temp = random.nextInt(2) % 2 == 0 ? 65 : 97;
                val += (char) (random.nextInt(26) + temp);
            } else if ("num".equalsIgnoreCase(charOrNum)) {
                val += String.valueOf(random.nextInt(10));
            }
        }
        return val;
    }

    public void initMsg() {
        msgInitStatus = false;
        msgInitStatus_Success = false;
        msgInitStatus_INVALID_PARA = false;
        msgInitStatus_REMOTE_EXCEPTION = false;
        msgInitStatus_BIND_FAILED = false;

        msgOnPlayBegin = false;
        msgOnPlayCompleted = false;
        msgOnPlayInterrupted = false;
        msgOnProgressReturn = false;
    }
}

