package com.iflytek.testspeechsuiteapi;

import android.test.AndroidTestCase;
import android.os.Environment;
import com.iflytek.testspeechsuiteapi.tts.DefTTS;
import com.iflytek.testspeechsuiteapi.tts.TestStabilityTTS;
import com.iflytek.speech.ISSErrors;
import com.iflytek.tts.ESpeaker;
import com.iflytek.tts.TtsSession;

/**
 * 接口测试用例。
 * 以test开头的用例都会被执行。
 */
public class TestApiTTS extends AndroidTestCase {

    DefTTS def;
    Tool tool;
    int err = -1;

    @Override
    public void setUp() throws Exception {
        super.setUp();

        def = TestApiTool.defTTS;
        tool = TestApiTool.tool;
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();

        TestApiTool.defTTS.initMsg();

        if (TestApiTool.ttsSession != null) {
            TestApiTool.ttsSession.sessionStop();
            TestApiTool.ttsSession = null;
        }

        if (TestApiTool.ttsSession1 != null) {
            TestApiTool.ttsSession1.sessionStop();
            TestApiTool.ttsSession1 = null;
        }
    }

    /**
     * context、iTtsInitListener、resDir均传入合法参数
     * TtsSession: context=v(activity context), iTtsInitListener=v, resDir=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常合成
     */
    public void test_tts_101() throws InterruptedException {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def
                .resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 5);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(500);
    }

    /**
     * context、iTtsInitListener、resDir均传入合法参数
     * TtsSession: context=v(application context), iTtsInitListener=v, resDir=v
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常合成
     */
    public void test_tts_102() throws InterruptedException {
        TtsSession ttsSession = new TtsSession(getContext().getApplicationContext(), def
                .iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(500);
    }

    /**
     * context传null参数，iTtsInitListener、resDir传合法参数
     * TtsSession: context=iv(null), iTtsInitListener=v, resDir=v
     * <p>
     * a. TtsSession实例返回不为空
     * b. startSpeak接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_tts_103() throws InterruptedException {
        TtsSession ttsSession = new TtsSession(null, def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_INVALID_PARA);

        err = ttsSession.startSpeak("123");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * resDir传null参数，context、iTtsInitListener传合法参数
     * context=v(activity context), iTtsInitListener=v, resDir=iv(null)
     * <p>
     * a. TtsSession实例返回不为空
     * b. startSpeak接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_tts_105() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, null);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_INVALID_PARA);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 删除resDir，context、iTtsInitListener传合法参数
     * context=v(activity context), iTtsInitListener=v, resDir=iv(资源不存在)
     * <p/>
     * a. TtsSession实例返回不为空
     * b. startSpeak接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_tts_106() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def
                .resDir_wrong);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_FILE_NOT_FOUND);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 资源路径中使用反斜杠
     */
    public void test_tts_107() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, Environment
                .getExternalStorageDirectory().getAbsolutePath()
                + "\\iflytek\\res\\tts");

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_FILE_NOT_FOUND);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * ttsListener传null参数
     * iv(null)
     * <p/>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void test_tts_201() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(null, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * audioType传-1参数
     * iv(-1)
     * <p/>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void test_tts_202() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, -1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * audioType传1000000参数
     * iv(1000000)
     * <p/>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void test_tts_203() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 1000000);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * 各种参数组合
     */
    public void abort_test_tts_301() {
        int[] modes = new int[]{
                ESpeaker.ISS_TTS_VOLUME_USE_NORMAL, ESpeaker.ISS_TTS_VOLUME_USE_NAVIGATION,
                ESpeaker.ISS_TTS_VOLUME_USE_MOBILE, ESpeaker.ISS_TTS_VOLUME_USE_EDUCATION,
                ESpeaker.ISS_TTS_VOLUME_USE_TV};

        int[] speakers = new int[]{
                ESpeaker.ivTTS_ROLE_CATHERINE, ESpeaker.ivTTS_ROLE_JIAJIA,
                ESpeaker.ivTTS_ROLE_JOHN, ESpeaker.ivTTS_ROLE_LAOMA,
                ESpeaker.ivTTS_ROLE_NANNAN, ESpeaker.ivTTS_ROLE_TERRY,
                ESpeaker.ivTTS_ROLE_XIAOFENG, ESpeaker.ivTTS_ROLE_XIAOJIE,
                ESpeaker.ivTTS_ROLE_JINGER, ESpeaker.ivTTS_ROLE_XIAOKUN,
                ESpeaker.ivTTS_ROLE_XIAOLIN, ESpeaker.ivTTS_ROLE_XIAOMEI,
                ESpeaker.ivTTS_ROLE_XIAOMENG, ESpeaker.ivTTS_ROLE_XIAOQIAN,
                ESpeaker.ivTTS_ROLE_XIAOQIANG, ESpeaker.ivTTS_ROLE_XIAORONG,
                ESpeaker.ivTTS_ROLE_XIAOSHI, ESpeaker.ivTTS_ROLE_XIAOXUE,
                ESpeaker.ivTTS_ROLE_XIAOYAN
        };

        int[] volumes = new int[]{
                ESpeaker.ISS_TTS_VOLUME_MIN, ESpeaker.ISS_TTS_VOLUME_NORMAL,
                ESpeaker.ISS_TTS_VOLUME_MAX_DEFAULT
        };

        int[] pitches = new int[]{
                ESpeaker.ISS_TTS_PITCH_MIN, ESpeaker.ISS_TTS_PITCH_NORMAL_DEFAULT,
                ESpeaker.ISS_TTS_PITCH_MAX
        };

        int[] speeds = new int[]{
                ESpeaker.ISS_TTS_SPEED_MIN, ESpeaker.ISS_TTS_SPEED_NORMAL_DEFAULT,
                ESpeaker.ISS_TTS_SPEED_MAX
        };

        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        // 设置模式
        for (int i = 0; i < modes.length; i++) {
            err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_USERMODE, modes[i]);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            // 设置发音人
            for (int j = 0; j < speakers.length; j++) {
                err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_SPEAKER, speakers[j]);
                assertEquals(ISSErrors.ISS_SUCCESS, err);

                // 设置音量
                for (int k = 0; k < volumes.length; k++) {
                    err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_VOLUME, volumes[k]);
                    assertEquals(ISSErrors.ISS_SUCCESS, err);

                    // 设置音调
                    for (int l = 0; l < pitches.length; l++) {
                        err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_VOICE_PITCH, pitches[l]);
                        assertEquals(ISSErrors.ISS_SUCCESS, err);

                        // 设置语速
                        for (int m = 0; m < speeds.length; m++) {
                            System.out.println("设置模式：" + modes[i]);
                            System.out.println("设置发音人：" + speakers[j]);
                            System.out.println("设置音量：" + volumes[k]);
                            System.out.println("设置音调：" + pitches[l]);
                            System.out.println("设置语速：" + speeds[m]);

                            err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_VOICE_SPEED,
                                    speeds[m]);
                            assertEquals(ISSErrors.ISS_SUCCESS, err);

                            err = ttsSession.startSpeak("参数测试");
                            assertEquals(ISSErrors.ISS_SUCCESS, err);
                            int t = 0;
                            while (!def.msgOnPlayBegin && t++ < 100) {
                                tool.sleep(10);
                            }
                            assertTrue(def.msgOnPlayBegin);
                            def.initMsg();
                            tool.sleep(50);

                            err = ttsSession.startSpeak("Can you can a can as a canner can can a " +
                                    "can");
                            assertEquals(ISSErrors.ISS_SUCCESS, err);
                            t = 0;
                            while (!def.msgOnPlayBegin && t++ < 100) {
                                tool.sleep(10);
                            }
                            assertTrue(def.msgOnPlayBegin);
                            tool.sleep(50);

                            ttsSession.stopSpeak();

                            def.initMsg();
                        }
                    }
                }
            }
        }
        ttsSession.sessionStop();
    }

    /**
     * id=iv(0), value=iv(1024)
     */
    public void test_tts_302() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParam(0, 1024);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * id=iv(-1), value=iv(-1024)
     */
    public void test_tts_303() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParam(-1, -1024);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * id=iv(Integer.MAX_VALUE), value=iv(Integer.MAX_VALUE)
     */
    public void test_tts_304() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParam(Integer.MIN_VALUE, Integer.MIN_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * id=iv(Integer.MIN_VALUE), value=iv(Integer.MIN_VALUE)
     */
    public void test_tts_305() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParam(Integer.MAX_VALUE, Integer.MAX_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * 各种参数组合
     */
    public void abort_test_tts_401() {
        String[] modes = new String[]{
                "0", "1",
                "2", "3",
                "4"
        };

        String[] speakers = new String[]{
                "catherine", "jiajia",
                "john", "laoma",
                "nannan", "terry",
                "xiaofeng", "xiaojie",
                "xiaojing", "xiaokun",
                "xiaolin", "xiaomei",
                "xiaomeng", "xiaoqian",
                "xiaoqiang", "xiaorong",
                "xiaoshi", "xiaoxue",
                "xiaoyan"
        };

        String[] volumes = new String[]{
                "-32768", "0",
                "32767"
        };

        String[] pitches = new String[]{
                "-32768", "0",
                "32767"
        };

        String[] speeds = new String[]{
                "-32768", "0",
                "32767"
        };

        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        // 设置模式
        for (int i = 0; i < modes.length; i++) {
            err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_USERMODE, modes[i]);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            // 设置发音人
            for (int j = 0; j < speakers.length; j++) {
                err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_SPEAKER, speakers[j]);
                assertEquals(ISSErrors.ISS_SUCCESS, err);

                // 设置音量
                for (int k = 0; k < volumes.length; k++) {
                    err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOLUME, volumes[k]);
                    assertEquals(ISSErrors.ISS_SUCCESS, err);

                    // 设置音调
                    for (int l = 0; l < pitches.length; l++) {
                        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOICE_PITCH, pitches[l]);
                        assertEquals(ISSErrors.ISS_SUCCESS, err);

                        // 设置语速
                        for (int m = 0; m < speeds.length; m++) {
                            System.out.println("设置模式：" + modes[i]);
                            System.out.println("设置发音人：" + speakers[j]);
                            System.out.println("设置音量：" + volumes[k]);
                            System.out.println("设置音调：" + pitches[l]);
                            System.out.println("设置语速：" + speeds[m]);

                            err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOICE_SPEED,
                                    speeds[m]);
                            assertEquals(ISSErrors.ISS_SUCCESS, err);

                            err = ttsSession.startSpeak("参数测试");
                            assertEquals(ISSErrors.ISS_SUCCESS, err);
                            int t = 0;
                            while (!def.msgOnPlayBegin && t++ < 100) {
                                tool.sleep(10);
                            }
                            assertTrue(def.msgOnPlayBegin);
                            def.initMsg();
                            tool.sleep(2);

                            err = ttsSession.startSpeak("Can you can a can as a canner can can a " +
                                    "can");
                            assertEquals(ISSErrors.ISS_SUCCESS, err);
                            t = 0;
                            while (!def.msgOnPlayBegin && t++ < 100) {
                                tool.sleep(10);
                            }
                            assertTrue(def.msgOnPlayBegin);
                            tool.sleep(2);

                            ttsSession.stopSpeak();

                            def.initMsg();
                        }
                    }
                }
            }
        }
        ttsSession.sessionStop();
    }

    /**
     * id=iv(134235145),value=v("xiaofeng")
     */
    public void test_tts_402() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(134235145, "xiaofeng");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * id=v(ISS_TTS_PARAM_SPEAKER),value=iv("asdf23")
     */
    public void test_tts_403() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_SPEAKER, "asdf23");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * id=v(ISS_TTS_PARAM_SPEAKER), value=iv("")
     */
    public void test_tts_404() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_SPEAKER, "");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * id=iv(ISS_TTS_PARAM_SPEAKER), value=iv("SFGIUHGW")
     */
    public void test_tts_405() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_SPEAKER, "SFGIUHGW");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * id=iv(Integer.MAX_VALUE), value=iv(null)
     */
    public void test_tts_406() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(Integer.MAX_VALUE, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * id=iv(Integer.MIN_VALUE), value=iv(50个字符)
     */
    public void test_tts_407() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(Integer.MIN_VALUE, def.getRandomCHStr(50));
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOICE_SPEED),value=v("asdf")
     * <p>
     * fail setParamEx接口对PITCH, VOLUME, SPEED参数未做异常参数值检查，始终返回成功
     * 该接口不做维护
     */
    public void test_tts_408() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOICE_SPEED, "asdf");
//        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOICE_PITCH),value=v("fqwef")
     * <p>
     * fail setParamEx接口对PITCH, VOLUME, SPEED参数未做异常参数值检查，始终返回成功
     * 该接口目前不做维护
     */
    public void test_tts_409() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOICE_PITCH, "fqwef");
//        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOLUME),value=v("asdf2w")
     * <p>
     * fail setParamEx接口对PITCH, VOLUME, SPEED参数未做异常参数值检查，始终返回成功
     * 该接口目前不做维护
     */
    public void test_tts_410() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOLUME, "asdf2w");
//        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOICE_SPEED),value=v("10000000000000")
     */
    public void test_tts_411() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOICE_SPEED, "10000000000000");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOICE_PITCH),value=v("10000000000000")
     */
    public void test_tts_412() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOICE_PITCH, "10000000000000");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOLUME),value=v("10000000000000")
     */
    public void test_tts_413() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOLUME, "10000000000000");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOICE_SPEED),value=v("-10000000000000")
     */
    public void test_tts_414() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOICE_SPEED, "-10000000000000");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOICE_PITCH),value=v("-10000000000000")
     */
    public void test_tts_415() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOICE_PITCH, "-10000000000000");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * setParamEx: id=v(ISS_TTS_PARAM_VOLUME),value=v("-10000000000000")
     */
    public void test_tts_416() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_VOLUME, "-10000000000000");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * ttsListener、audioType传入合法参数
     * TtsSession: context=v(activity context), iTtsInitListener=v, resDir=v
     * sessionStart:ttsListener=v，audioType=v(3)
     * pauseSpeak-resumeSpeak
     */
    public void test_tts_501() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(1000);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(2000);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

    }

    /**
     * 合成指定文本
     * 获取实例后直接调用其它接口
     */
    public void test_tts_502() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

        tool.sleep(1000);
        assertFalse(def.msgOnPlayBegin);
    }

    /**
     * id传入音量参数，value传入最低音量参数
     * TtsSession: context=v(activity context), iTtsInitListener=v, resDir=v
     * setParam: id=v(ISS_TTS_PARAM_VOLUME),value=v(ISS_TTS_VOLUME_MIN)
     * startSpeak: 司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝
     * <p/>
     * a. 最低音量播报
     * b. 所有接口返回ISS_SUCCESS
     */
    public void test_tts_503() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_VOLUME, ESpeaker.ISS_TTS_VOLUME_MIN);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 停止TTS播报
     */
    public void test_tts_505() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.stopSpeak();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * id、value传入合法参数
     * TtsSession: context=v(activity context), iTtsInitListener=v, resDir=v
     * sessionStart:ttsListener=v，audioType=v(3)
     * 初始化完成前调用其它接口
     */
    public void test_tts_506() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        TestApiTool.ttsSession = ttsSession;

        assertFalse(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 初始化完成前调用其它接口
     * <p>
     * 返回ISS_ERROR_INVALID_CALL
     */
    public void test_tts_507() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        TestApiTool.ttsSession = ttsSession;

        assertFalse(def.msgInitStatus_Success);

        err = ttsSession.startSpeak("123");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 初始化完成前调用其它接口
     * <p>
     * 返回ISS_ERROR_INVALID_CALL
     */
    public void test_tts_508() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        TestApiTool.ttsSession = ttsSession;

        assertFalse(def.msgInitStatus_Success);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 初始化完成前调用其它接口
     * <p>
     * 返回ISS_ERROR_INVALID_CALL
     */
    public void test_tts_509() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        TestApiTool.ttsSession = ttsSession;

        assertFalse(def.msgInitStatus_Success);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 初始化完成前调用其它接口
     */
    public void test_tts_510() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        TestApiTool.ttsSession = ttsSession;

        assertFalse(def.msgInitStatus_Success);

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 初始化完成前调用其它接口
     */
    public void test_tts_511() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        TestApiTool.ttsSession = ttsSession;

        assertFalse(def.msgInitStatus_Success);

        err = ttsSession.setParam(1, 1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 初始化完成前调用其它接口
     */
    public void test_tts_512() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        TestApiTool.ttsSession = ttsSession;

        assertFalse(def.msgInitStatus_Success);

        err = ttsSession.stopSpeak();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 播报时sessionStop
     */
    public void test_tts_513() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(100);

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

    }

    /**
     * 合成时（收到开始播报回调前）sessionStop
     */
    public void test_tts_514() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        assertFalse(def.msgOnPlayBegin);
        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 暂停时sessionStop
     *
     * fail 第二次sessionStop卡住出不来
     */
    public void test_tts_515() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(1000);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

    }

    /**
     * 播报时stopSpeak
     */
    public void test_tts_516() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(100);

        err = ttsSession.stopSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

    }

    /**
     * 合成时stopSpeak
     */
    public void test_tts_517() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        assertFalse(def.msgOnPlayBegin);
        err = ttsSession.stopSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 暂停时stopSpeak
     */
    public void test_tts_518() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(1000);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.stopSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * sessionStart*10000
     */
    public void atest_stress_tts_1() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        for (int i = 0; i < 10000; i++) {
            err = ttsSession.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(200);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * sessionStop*10000
     */
    public void atest_stress_tts_2() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(200);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);


        for (int j = 0; j < 10000; j++) {
            err = ttsSession.sessionStop();
        }
    }

    /**
     * startSpeak(收到onPlayBegin回调后) * 10000
     */
    public void atest_stress_tts_3() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 10000; i++) {
            err = ttsSession.startSpeak(def.getRandomCHStr(10));
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            int j = 0;
            while (!def.msgOnPlayBegin && j++ < 100) {
                tool.sleep(10);
            }
            assertTrue(def.msgOnPlayBegin);
            def.msgOnPlayBegin = false;
        }

        tool.sleep(200);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * startSpeak(收到onPlayBegin回调前 * 10000
     */
    public void atest_stress_tts_4() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 10000; i++) {
            def.msgOnPlayBegin = false;
            err = ttsSession.startSpeak(def.getRandomCHStr(100));

            assertFalse(def.msgOnPlayBegin);
        }

        tool.sleep(200);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * pauseSpeak * 10000
     */
    public void atest_stress_tts_5() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(200);

        for (int j = 0; j < 10000; j++) {
            err = ttsSession.pauseSpeak();
        }

        tool.sleep(200);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resumeSpeak * 10000
     */
    public void atest_stress_tts_6() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(200);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        for (int j = 0; j < 10000; j++) {
            err = ttsSession.resumeSpeak();
        }

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * stopSpeak*10000
     */
    public void atest_stress_tts_7() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(200);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);


        for (int j = 0; j < 10000; j++) {
            err = ttsSession.stopSpeak();
        }

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setParam*10000
     */
    public void atest_stress_tts_8() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 2500; i++) {
            err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_USERMODE, ESpeaker.ISS_TTS_VOLUME_USE_EDUCATION);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_SPEAKER, ESpeaker.ivTTS_ROLE_XIAOJIE);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_VOLUME, ESpeaker.ISS_TTS_SPEED_MAX);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_VOICE_PITCH, ESpeaker.ISS_TTS_PITCH_MAX);
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(200);

        err = ttsSession.pauseSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        err = ttsSession.resumeSpeak();
        assertEquals(ISSErrors.ISS_SUCCESS, err);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * sessionStart-sessionStop(初始化完成) * 10000
     */
    public void atest_stress_tts_9() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        for (int i = 0; i < 10000; i++) {
            err = ttsSession.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.sessionStop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * sessionStart-sessionStop(初始化未完成) * 10000
     */
    public void atest_stress_tts_10() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        TestApiTool.ttsSession = ttsSession;

        assertFalse(def.msgInitStatus_Success);

        for (int i = 0; i < 10000; i++) {
            err = ttsSession.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.sessionStop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * startSpeak(播报完成)-stopSpeak * 5000
     */
    public void atest_stress_tts_11() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 5000; i++) {
            err = ttsSession.startSpeak("1");
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            int j = 0;
            while (!def.msgOnPlayBegin && j++ < 100) {
                tool.sleep(10);
            }
            assertTrue(def.msgOnPlayBegin);
            def.msgOnPlayBegin = false;

            tool.sleep(500);

            err = ttsSession.stopSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.startSpeak("123");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * startSpeak(收到onPlayBegin回调后，播报完成前)-stopSpeak * 10000
     */
    public void atest_stress_tts_12() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 10000; i++) {
            err = ttsSession.startSpeak(def.getRandomCHStr(10));
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            int j = 0;
            while (!def.msgOnPlayBegin && j++ < 100) {
                tool.sleep(10);
            }
            assertTrue(def.msgOnPlayBegin);
            def.msgOnPlayBegin = false;

            tool.sleep(20);

            err = ttsSession.stopSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * startSpeak(收到onPlayBegin回调前)-stopSpeak * 10000
     */
    public void atest_stress_tts_13() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 10000; i++) {
            def.msgOnPlayBegin = false;
            err = ttsSession.startSpeak(def.getRandomCHStr(10));

            assertFalse(def.msgOnPlayBegin);

            err = ttsSession.stopSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);


        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 0,(5,9,6)*10000,5,9(合成时sessionStop)
     */
    public void atest_stress_tts_14() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        for (int i = 0; i < 10000; i++) {
            def.msgOnPlayBegin = false;
            err = ttsSession.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.startSpeak(def.getRandomCHStr(100));

            assertFalse(def.msgOnPlayBegin);

            err = ttsSession.sessionStop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * sessionStart-startSpeak(收到onPlayBegin回调后，播报完成前)-sessionStop * 10000
     */
    public void atest_stress_tts_15() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        for (int i = 0; i < 10000; i++) {
            err = ttsSession.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.startSpeak(def.getRandomCHStr(10));
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            int j = 0;
            while (!def.msgOnPlayBegin && j++ < 10000) {
                tool.sleep(10);
            }
            assertTrue(def.msgOnPlayBegin);
            def.msgOnPlayBegin = false;

            tool.sleep(10);

            err = ttsSession.sessionStop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * pauseSpeak-resumeSpeak * 10000
     */
    public void atest_stress_tts_16() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession.startSpeak(def.getRandomCHStr(30));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int j = 0;
        while (!def.msgOnPlayBegin && j++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        for (int i = 0; i < 10000; i++) {
            err = ttsSession.pauseSpeak();

            err = ttsSession.resumeSpeak();
        }

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * startSpeak(收到onPlayBegin回调后)-pauseSpeak-resumeSpeak-stopSpeak * 10000
     */
    public void atest_stress_tts_17() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 10000; i++) {
            err = ttsSession.startSpeak(def.getRandomCHStr(30));
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            int j = 0;
            while (!def.msgOnPlayBegin && j++ < 100) {
                tool.sleep(10);
            }
            assertTrue(def.msgOnPlayBegin);

            err = ttsSession.pauseSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.resumeSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.stopSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * startSpeak(收到onPlayBegin回调前)-pauseSpeak-resumeSpeak-stopSpeak * 10000
     */
    public void atest_stress_tts_18() {
        TtsSession ttsSession = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession = ttsSession;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 10000; i++) {
            def.msgOnPlayBegin = false;

            err = ttsSession.startSpeak(def.getRandomCHStr(100));

            assertFalse(def.msgOnPlayBegin);

            err = ttsSession.pauseSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.resumeSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession.stopSpeak();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = ttsSession.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 创建20个实例同时进行播报
     */
    public void test_tts_mInst1() {
        TtsSession[] sessions = new TtsSession[20];

        for (int i = 0; i < sessions.length; i++) {
            sessions[i] = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);

            while (!def.msgInitStatus) {
                tool.sleep(10);
            }
            assertTrue(def.msgInitStatus_Success);

            def.initMsg();

            err = sessions[i].sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        for (int i = 0; i < sessions.length; i++) {
            sessions[i].startSpeak(def.getRandomCHStr(10));
        }

        tool.sleep(7000);

        for (int i = 0; i < sessions.length; i++) {
            sessions[i].sessionStop();
        }
    }

    /**
     * 第一个实例创建失败，第二个实例创建成功，播报
     */
    public void test_tts_mInst2() {
        TtsSession ttsSession1 = new TtsSession(getContext(), def.iTtsInitListener, def
                .resDir_wrong);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.initMsg();
        TestApiTool.ttsSession1 = ttsSession1;

        err = ttsSession1.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

        TtsSession ttsSession2 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        TestApiTool.ttsSession = ttsSession2;

        err = ttsSession2.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession2.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(1000);
    }

    /**
     * 第一个实例创建成功，第二个实例创建失败，播报
     * <p>
     * tts资源句柄是单例的，初始化一次后就不会再次初始化了。因此第二次即使传入错误的资源路径也是
     * 不影响使用的。
     */
    public void test_tts_mInst3() {
        TtsSession ttsSession2 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        TestApiTool.ttsSession = ttsSession2;
        def.initMsg();

        err = ttsSession2.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        TtsSession ttsSession1 = new TtsSession(getContext(), def.iTtsInitListener, def
                .resDir_wrong);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = ttsSession1.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession1.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        assertTrue(def.msgOnPlayBegin);

        tool.sleep(3000);
    }

    /**
     * 创建两个实例，销毁一个，用另一个进行播报
     */
    public void test_tts_mInst4() {
        TtsSession ttsSession1 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        def.initMsg();

        err = ttsSession1.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        TestApiTool.ttsSession = ttsSession1;

        TtsSession ttsSession2 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        TestApiTool.ttsSession1 = ttsSession2;

        err = ttsSession2.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession2.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession1.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(3000);
    }

    /**
     * 一个实例播报时，另一个实例调用stopSpeak
     */
    public void test_tts_mInst5() {
        TtsSession ttsSession1 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        def.initMsg();
        TestApiTool.ttsSession = ttsSession1;

        err = ttsSession1.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        TtsSession ttsSession2 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        TestApiTool.ttsSession1 = ttsSession2;

        err = ttsSession2.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession2.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession1.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        ttsSession2.stopSpeak();

        tool.sleep(200);
        assertFalse(def.msgOnPlayInterrupted);
    }

    /**
     * 一个实例播报时，另一个实例调用sessionStop
     */
    public void test_tts_mInst6() {
        TtsSession ttsSession1 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        def.initMsg();
        TestApiTool.ttsSession = ttsSession1;

        err = ttsSession1.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        TtsSession ttsSession2 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.ttsSession1 = ttsSession2;
        assertTrue(def.msgInitStatus_Success);

        err = ttsSession2.sessionStart(def.iTtsListener, 3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession2.sessionStop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = ttsSession1.startSpeak(def.getRandomCHStr(10));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.sleep(200);

        ttsSession2.sessionStop();

        tool.sleep(200);
        assertFalse(def.msgOnPlayInterrupted);
    }

    /**
     * 循环调用两个实例的sessionStart-sessionStop
     */
    public void test_tts_mInst7() {
        TtsSession ttsSession1 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        def.initMsg();
        TestApiTool.ttsSession = ttsSession1;

        TtsSession ttsSession2 = new TtsSession(getContext(), def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_Success);
        TestApiTool.ttsSession1 = ttsSession2;

        for (int i = 0; i < 100; i++) {
            err = ttsSession1.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession1.sessionStop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession2.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession2.sessionStop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        for (int i = 0; i < 100; i++) {
            err = ttsSession1.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession2.sessionStart(def.iTtsListener, 3);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession1.sessionStop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = ttsSession2.sessionStop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
    }

    public void test_tts_mThreads1() {
        TestStabilityTTS t = new TestStabilityTTS(getContext(), 600);
        t.ifWriteToActivity = 0;

        t.test_multiThreading1();
    }

    public void test_tts_random() {
        TestStabilityTTS t = new TestStabilityTTS(getContext(), 1800);
        t.ifWriteToActivity = 0;

        t.testRandom();
    }
}
