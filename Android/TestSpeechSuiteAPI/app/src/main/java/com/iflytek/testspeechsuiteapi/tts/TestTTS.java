package com.iflytek.testspeechsuiteapi.tts;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;

import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.speech.ISSErrors;
import com.iflytek.tts.ESpeaker;
import com.iflytek.tts.TtsSession;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStream;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class TestTTS {
    private TtsSession ttsSession;
    private TtsSession ttsSession1;
    private Context context;
    private Activity activity;
    private String Tag = "TestTTS";
    private int err = -1;

    private DefTTS def = new DefTTS();
    private Tool tool = new Tool();

    public TestTTS(Context context) {
        this.context = context;
        ttsSession = new TtsSession(context, def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;
    }

    public TestTTS(Context context, Activity activity) {
        this.context = context;
        this.activity = activity;
        ttsSession = new TtsSession(context, def.iTtsInitListener, def.resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;
    }

    public void test_temp() {
        TtsSession ttsSession = new TtsSession(context, def.iTtsInitListener, def
                .resDir);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = ttsSession.sessionStart(def.iTtsListener, 5);

        err = ttsSession.startSpeak(def.getRandomCHStr(10));
        int i = 0;
        while (!def.msgOnPlayBegin && i++ < 100) {
            tool.sleep(10);
        }
        ILog.i(Tag, "msgOnPlayBegin"+def.msgOnPlayBegin, 1);

        tool.sleep(5000);
        ILog.i(Tag, "msgOnPlayComplate"+def.msgOnPlayBegin, 1);
    }

    /**
     * 首次合成响应时间的一致性对比，测试结果用脚本处理合成log得到。
     * 首次合成响应时间，含义为一次合成过程中，第一次“句子开始合成的时间到首次进入音频输出回调的时间”之间的时间差。
     */
    public void test_ttsTime(String ttsListPath) {
        String Tag = this.Tag + ".test_ttsTime";

        ttsSession.initService();

        // TtsSession.sessionStart
        err = ttsSession.sessionStart(def.iTtsListener, 3);
        HandleRet.handleSessionStartRet(err);

        try {
            BufferedReader bReader = new BufferedReader(new FileReader(new File(Environment.getExternalStorageDirectory().getAbsolutePath() + ttsListPath)));
            String line = null;
            while ((line = bReader.readLine()) != null) {
                ILog.i(Tag, "tts text:" + line);

                // TtsSession.startSpeak
                err = ttsSession.startSpeak(line);
                HandleRet.handleStartSpeakRet(err);

                tool.sleep(800);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 测试合成。
     *
     * @param text 合成文本。
     */
    public void test_tts(String text) {
        String Tag = this.Tag + ".test_tts";

        // TtsSession.sessionStart
        err = ttsSession.sessionStart(def.iTtsListener, 3);
        HandleRet.handleSessionStartRet(err);

        ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_SPEAKER, ESpeaker.ivTTS_ROLE_JIAJIA);

        // TtsSession.startSpeak
        err = ttsSession.startSpeak(text);
        HandleRet.handleStartSpeakRet(err);

        while (!def.msgOnPlayCompleted) {
            tool.sleep(10);
        }
    }

    /**
     * 在TTSRes目录删除xiaoyan.irf资源，在isstts.cfg中设置{"name": "xiaoyan.irf", "id": 3, "loadtype": -1}后初始化异常崩溃。
     */
    public void testInitCrash() {
        ttsSession.initService();
        while (def.msgInitStatus) {
            tool.sleep(10);
        }
    }

    /**
     * 测试多次循环start-stop后，onTTSPlayCompleted消息不会回调问题。
     * <p>
     * 2016.7.8  戚满斌提出该测试。
     * <p>
     * 调用逻辑：
     * （1）先调用start，然后立刻调用stop，接收到播报打断消息onTTSPlayInterrupted的回调时，立刻再次进行start，如此循环调用几次后，执行第（2）步；
     * （2）调用start，在接收到播报完成onTTSPlayCompleted的回调时，调用start，看看这时是否能接收到播报完成的回调消息
     */
    public void test_onTTSPlayCompletedAfterManyStartStop() {
        String Tag = this.Tag + ".Test";

        // TtsSession.sessionStart
        err = ttsSession.sessionStart(def.iTtsListener, 3);
        HandleRet.handleSessionStartRet(err);

        for (int i = 0; i < 1000; i++) {
            // TtsSession.startSpeak
            err = ttsSession.startSpeak("为你找到12条结果");
            HandleRet.handleStartSpeakRet(err);

            // TtsSession.stopSpeak
            err = ttsSession.stopSpeak();
            HandleRet.handleStopSpeakRet(err);

            while (!def.msgOnPlayInterrupted) {
                tool.sleep(10);
            }
            def.msgOnPlayInterrupted = false;
        }

        // TtsSession.startSpeak
        err = ttsSession.startSpeak("为你找到12条结果");
        HandleRet.handleStartSpeakRet(err);

        while (!def.msgOnPlayCompleted) {
            tool.sleep(10);
        }
        def.msgOnPlayCompleted = false;

        // TtsSession.startSpeak
        err = ttsSession.startSpeak("为你找到12条结果");
        HandleRet.handleStartSpeakRet(err);

        while (!def.msgOnPlayCompleted) {
            tool.sleep(10);
        }
        def.msgOnPlayCompleted = false;
    }

    /**
     * 验证芯片加密功能过
     * <p>
     * 现在会在初始化时进行初始化，使用非芯片加密会初始化失败。
     */
    public void testChipEn() {
        err = ttsSession.sessionStart(def.iTtsListener, 3);
        HandleRet.handleSessionStartRet(err);
    }
}

