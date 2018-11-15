package com.iflytek.testspeechsuiteapi.mvw;

import android.os.Environment;

import com.iflytek.testspeechsuiteapi.Def;
import com.iflytek.testspeechsuiteapi.ILog;

import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.mvw.IMvwListener;
import com.iflytek.mvw.MvwSession;
import com.iflytek.speech.ISSErrors;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

/**
 * 此类对公共变量进行定义。
 */
public class DefMVW extends Def {
    public String Tag = "TestMVW";

    public VwWakeupInfo vwWakeupInfo = new VwWakeupInfo();
    Tool tool = new Tool();

    /**
     * 1: ISS_MVW_SCENE_GLOBAL
     * 0: 你好语音助理
     * <p>
     * 2: ISS_MVW_SCENE_CONFIRM
     * 0: 确定
     * 1: 取消
     * <p>
     * 4: ISS_MVW_SCENE_SELECT
     * 0: 第一个
     * 1: 第二个
     * 2: 第三个
     * 3: 第四个
     * 4: 第五个
     * 5: 第六个
     * <p>
     * 8: ISS_MVW_SCENE_ANSWER_CALL
     * 0: 接听
     * 1: 挂断
     * 2: 取消
     */
    public String resDir = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res/mvw/FirstRes";  // 唤醒资源路径
    public String resDir_second = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res/mvw/SecondRes";  // 唤醒资源路径
    public String resDir_wrong = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res/mvw/First";  // 唤醒资源路径

    // 唤醒音频路径
    public String mvwPcm_global = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/mvwword.pcm";
    public String mvwPcm_confirm = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/confirm.pcm";
    public String mvwPcm_cancel = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/cancel.pcm";
    public String mvwPcm_select = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/select.pcm";
    public String mvwPcm_answer = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/answer.wav";
    public String mvwPcm_hangup = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/Hungup.pcm";
    public String mvwPcm_first = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/first.pcm";
    public String mvwPcm_second = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/second.pcm";
    public String mvwPcm_third = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/third.pcm";
    public String mvwPcm_fourth = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/forth.pcm";
    public String mvwPcm_fifth = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/fifth.pcm";
    public String mvwPcm_sixth = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/sixth.pcm";
    public String mvwPcm_first_en = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/first_en.pcm";
    public String mvwPcm_confirm_en = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/confirm_en.pcm";
    public String mvwPcm_last = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/last.pcm";


    public String mvwPcm_KaiYi = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/KaiYiNiHao.pcm";
    public String mvwPcm_KaiYi_long = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/KaiYiNiHao_long.wav";
    public String mvwPcm_ZhiDou = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/ZhiDouNiHao.pcm";
    public String mvwPcm_ChenSheng = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/ChenSheng.pcm";
    public String mvwPcm_DaLianJiPai = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/DaLianJiPai.pcm";
    public String mvwPcm_HuShiCiShen = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/HuShiCiShen.pcm";
    public String mvwPcm_ChongQingXiaoMian = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/ChongQingXiaoMian.pcm";
    public String mvwPcm_NavigateIflytek = Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/pcm_src/sr/NavigateIflytek.pcm";
    public String mvwPcm_iflytek = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/iflytek.pcm";
    public String mvwPcm_ShenTong = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/ShenTong.pcm";
    public String mvwPcm_ZhengKai = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/ZhengKai.pcm";
    public String mvwPcm_PhoneZhangsan = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/PhoneZhangsan.wav";
    public String mvwPcm_PhoneBaiYawei = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/sr/PhoneBaiYawei.wav";
    public String mvwPcm_ZhiDou_long = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/ZhiDouNiHao_long.wav";
    public String mvwPcm_LingXi = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/LingXiLingXi.pcm";
    public String mvwPcm_LingXi_long = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/LingXiLingXi_long.wav";
    public String mvwPcm_123 = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/123.pcm";
    public String mvwPcm_HelloHello = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/pcm_src/mvw/HelloHello.wav";

    // 自定义唤醒词路径
    public String mvwWord_LingXi = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/LingXiLingXi.txt");
    public String mvwWord_KaiYi = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/KaiYiNiHao.txt");
    public String mvwWord_KaiYi_long = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/KaiYiNiHao_long.txt");
    public String mvwWord_ZhiDou = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/ZhiDouNiHao.txt");
    public String mvwWord_KaiYi_LingXi = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/KaiYi&LingXi.txt");
    public String mvwWord_KaiYi_LingXi_ZhiDou_long = tool.readFile(Environment
            .getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/vw_word/KaiYi_LingXi_ZhiDou_long.txt");
    public String mvwWord_many_words = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/many_words.txt");
    public String mvwWord_wrong_format = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_format.txt");
    public String mvwWord_wrong_format1 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_format1.txt");
    public String mvwWord_wrong_format2 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_format2.txt");
    public String mvwWord_wrong_format3 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_format3.txt");
    public String mvwWord_wrong_format4 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_format4.txt");
    public String mvwWord_wrong_format5 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_format5.txt");
    public String mvwWord_wrong_id_1 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_id_-1.txt");
    public String mvwWord_wrong_id_str = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_id_str.txt");
    public String mvwWord_wrong_id_100 = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_id_100.txt");
    public String mvwWord_wrong_id_3003003003003003009 = tool.readFile(Environment
            .getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/vw_word/wrong_id_3003003003003003009.txt");
    public String mvwWord_wrong_id_order = tool.readFile(Environment.getExternalStorageDirectory
            ().getAbsolutePath()
            + "/TestRes/vw_word/wrong_id_order.txt");
    public String mvwWord_wrong_id_same = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/wrong_id_same.txt");
    public String mvwWord_wrong_word_same = tool.readFile(Environment.getExternalStorageDirectory
            ().getAbsolutePath()
            + "/TestRes/vw_word/wrong_word_same.txt");
    public String mvwWord_123_num = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/123_num.txt");
    public String mvwWord_123_str = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/123_str.txt");
    public String mvwWord_HelloHello = tool.readFile(Environment.getExternalStorageDirectory()
            .getAbsolutePath()
            + "/TestRes/vw_word/HelloHello.txt");

    public String szRlt;  // 唤醒成功的唤醒词
    public String wakeupRet;

    public boolean msgInitStatus;
    public boolean msgInitStatus_success;
    public boolean msgInitStatus_fail;
    public boolean msgInitStatus_fileNotFound;
    public boolean msgVwWakeup;
    public boolean msgVwWakeup_scene10;
    public boolean msgVwWakeup_scene20;
    public boolean msgVwWakeup_scene21;

    public IMvwListener iMvwListener = new IMvwListener() {
        @Override
        public void onVwInited(boolean state, int errId) {
            msgInitStatus = true;
            if (state) {
                msgInitStatus_success = true;
                ILog.i(Tag, "初始化成功", isWriteToActivity);
            } else {
                msgInitStatus_fail = true;

                if (errId == ISSErrors.ISS_ERROR_FILE_NOT_FOUND) {
                    msgInitStatus_fileNotFound = true;
                    ILog.e(Tag, "初始化失败，ISS_ERROR_FILE_NOT_FOUND", isWriteToActivity);
                } else
                    ILog.e(Tag, "初始化失败，errId=" + errId, isWriteToActivity);
            }
        }

        @Override
        public void onVwWakeup(int nMvwScene, int nMvwId, int nMvwScore, String lParam) {
            ILog.i(Tag, "唤醒成功", isWriteToActivity);
            vwWakeupInfo.setVwTime();

            if (nMvwScene == MvwSession.ISS_MVW_SCENE_GLOBAL) {
                if (nMvwId == 0) {
                    msgVwWakeup_scene10 = true;
                    szRlt = "你好语音助理";
                } else {
                    szRlt = "" + nMvwId;
                }
            } else if (nMvwScene == MvwSession.ISS_MVW_SCENE_CONFIRM) {
                if (nMvwId == 0) {
                    msgVwWakeup_scene20 = true;
                    szRlt = "确定";
                } else if (nMvwId == 1) {
                    msgVwWakeup_scene21 = true;
                    szRlt = "取消";
                } else {
                    szRlt = "" + nMvwId;
                }
            } else if (nMvwScene == MvwSession.ISS_MVW_SCENE_SELECT) {
                if (nMvwId == 0) {
                    szRlt = "第一个";
                } else if (nMvwId == 1) {
                    szRlt = "第二个";
                } else if (nMvwId == 2) {
                    szRlt = "第三个";
                } else if (nMvwId == 3) {
                    szRlt = "第四个";
                } else if (nMvwId == 4) {
                    szRlt = "第五个";
                } else if (nMvwId == 5) {
                    szRlt = "第六个";
                } else if (nMvwId == 6) {
                    szRlt = "最后一个";
                } else if (nMvwId == 7) {
                    szRlt = "取消";
                } else {
                    szRlt = "" + nMvwId;
                }
            } else if (nMvwScene == MvwSession.ISS_MVW_SCENE_ANSWER_CALL) {
                if (nMvwId == 0) {
                    szRlt = "接听";
                } else if (nMvwId == 1) {
                    szRlt = "挂断";
                } else if (nMvwId == 2) {
                    szRlt = "取消";
                } else {
                    szRlt = "" + nMvwId;
                }
            }

            wakeupRet = "nMvwScene:" + nMvwScene + ",nMvwId:" + nMvwId + ",nMvwRlt:" +
                    szRlt + ",nMvwScore:" + nMvwScore + ",lParam:" + lParam;
            vwWakeupInfo.setnMvwScene(nMvwScene);
            vwWakeupInfo.setnMvwId(nMvwId);
            vwWakeupInfo.setSzRlt(szRlt);
            vwWakeupInfo.setnMvwScore(nMvwScore);
            vwWakeupInfo.setlParam(lParam);

            ILog.i("onVwWakeup", wakeupRet, isWriteToActivity);
            msgVwWakeup = true;
        }
    };

    public int getnMvwScene() {
        return vwWakeupInfo.getnMvwScene();
    }

    public int getnMvwId() {
        return vwWakeupInfo.getnMvwId();
    }

    public void initMsg() {
        msgInitStatus = false;
        msgInitStatus_success = false;
        msgInitStatus_fail = false;
        msgVwWakeup = false;
        msgVwWakeup_scene10 = false;
        msgVwWakeup_scene20 = false;
        msgVwWakeup_scene21 = false;

        vwWakeupInfo.initMsg();
    }

    class VwWakeupInfo {
        /**
         * 保存唤醒成功时的信息。
         */
        private int nMvwScene = -1;
        private int nMvwId = -1;
        private int nMvwScore = -1;
        private String szRlt;
        private String lParam;
        private Date vwDate;
        private SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());

        public String getlParam() {
            return lParam;
        }

        public void setlParam(String lParam) {
            this.lParam = lParam;
        }

        public int getnMvwScene() {
            return nMvwScene;
        }

        public void setnMvwScene(int nMvwScene) {
            this.nMvwScene = nMvwScene;
        }

        public int getnMvwId() {
            return nMvwId;
        }

        public void setnMvwId(int nMvwId) {
            this.nMvwId = nMvwId;
        }

        public int getnMvwScore() {
            return nMvwScore;
        }

        public void setnMvwScore(int nMvwScore) {
            this.nMvwScore = nMvwScore;
        }

        public String getSzRlt() {
            return szRlt;
        }

        public void setSzRlt(String szRlt) {
            this.szRlt = szRlt;
        }

        public void setVwTime() {
            try {
                this.vwDate = dfs.parse(dfs.format(new Date()));
            } catch (ParseException e) {
                e.printStackTrace();
            }
        }

        public Date getVwDate() {
            return vwDate;
        }

        public void initMsg() {
            nMvwScene = -1;
            nMvwId = -1;
            nMvwScore = -1;
            szRlt = null;
        }
    }
}

