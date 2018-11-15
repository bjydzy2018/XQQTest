package com.iflytek.testspeechsuiteapi.tts;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;

import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.tts.ESpeaker;
import com.iflytek.tts.TtsSession;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class TestStabilityTTS {
    private Tool tool = new Tool();
    private DefTTS def = new DefTTS();
    private TtsSession ttsSession;
    private Context context;
    private Activity activity;

    private int err = -1;
    private String Tag = "TestStabilityTTS";
    private long runTime; // 运行时间，以秒为单位
    public int ifWriteToActivity = 0;

    public TestStabilityTTS(Context context) {
        this.context = context;
    }

    /**
     * @param context 程序的Context
     * @param runTime 运行时间，以秒为单位
     */
    public TestStabilityTTS(Context context, long runTime) {
        this.context = context;
        this.runTime = runTime;
    }

    public TestStabilityTTS(Context context, Activity activity, long runTime) {
        this.context = context;
        this.runTime = runTime;
        this.activity = activity;
    }

    public void testRandomExit(int tag) {
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        ComponentName cn = new ComponentName("com.iflytek.hcliu.teststressspeechsuite30tts", "com" +
                ".iflytek.hcliu.teststressspeechsuite30tts.MainActivity");
        intent.setComponent(cn);

        System.out.println("start activity");
        if (tag == 0) {
            tool.setRunTime(runTime);
        } else {
            // 判断是否结束测试
            if (tool.isTimeUp()) {
                Bundle bundle1 = new Bundle();
                bundle1.putBoolean("isEnd", true);
                intent.putExtras(bundle1);
                activity.startActivity(intent);
                return;
            }
        }

        Bundle bundle = new Bundle();
        bundle.putBoolean("isEnd", false);
        intent.putExtras(bundle);
        activity.startActivityForResult(intent, 1);
    }

    /**
     * 批量合成，合成文本列表：/test.hcliu/tts/tts_list.txt
     */
    public void testManyTts() {
        def.isWriteToActivity = 0;
        tool.isWriteToActivity = 0;

        String Tag = this.Tag + ".testManyTts";
        ttsSession = new TtsSession(context, def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;
//        tool.sleep(10000);

        err = ttsSession.sessionStart(def.iTtsListener, 3);
//        tool.sleep(10000);

        ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_SPEAKER, ESpeaker.ivTTS_ROLE_XIAOJIE);
//        ttsSession.setParamEx(ESpeaker.ISS_TTS_PARAM_TMP_LOG_DIR, Environment
//                .getExternalStorageDirectory().getAbsolutePath() + "/iss_speak_type/全场景");

        BufferedReader bReader = null;
        ArrayList<String> lines = new ArrayList<>();

        try {
            bReader = new BufferedReader(new FileReader(def.ttsList));
            String line = null;
            while ((line = bReader.readLine()) != null) {
                lines.add(line);
            }
            bReader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        for (int i = 0; i < 1; i++) {
            for (int j = 0; j < lines.size(); j++) {
                err = ttsSession.startSpeak(lines.get(j));

                int k = 0;
                while (!def.msgOnPlayCompleted && k++ < 500) {
                    tool.sleep(10);
                }
                def.msgOnPlayCompleted = false;
            }

            System.out.println("第" + i + "遍");

//            err = ttsSession.startSpeak("123");
//
//            while (!def.msgOnPlayCompleted) {
//                tool.sleep(10);
//            }
//            def.msgOnPlayCompleted = false;
//
//            tool.sleep(50);
        }
//        ttsSession.sessionStop();
    }

    /**
     * 随机调用接口。
     */
    public void testRandom() {
        Random random = new Random();
        String text;
        int param = 0;
        int paramValue = 0;

        def.isWriteToActivity = ifWriteToActivity;
        tool.isWriteToActivity = ifWriteToActivity;

        ttsSession = new TtsSession(context, def.iTtsInitListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        tool.setRunTime(runTime);

        while (true) {
            switch (random.nextInt(7)) {
                // sessionStart
                case 0:
                    int audioType = random.nextInt(6);
                    ILog.i(Tag, "调用sessionStart接口，audioType：" + audioType, ifWriteToActivity);
                    ttsSession.sessionStart(def.iTtsListener, audioType);
                    break;
                // sessionStop
                case 1:
                    ILog.i(Tag, "调用sessionStop接口", ifWriteToActivity);
                    ttsSession.sessionStop();
                    break;
                // pauseSpeak
                case 2:
                    ILog.i(Tag, "调用pauseSpeak接口", ifWriteToActivity);
                    ttsSession.pauseSpeak();
                    break;
                // resumeSpeak
                case 3:
                    ILog.i(Tag, "调用resumeSpeak接口", ifWriteToActivity);
                    ttsSession.resumeSpeak();
                    break;
                // startSpeak
                case 4:
                    text = def.getRandomCHStr(10);
                    ILog.i(Tag, "调用startSpeak接口，text：" + text, ifWriteToActivity);
                    ttsSession.startSpeak(text);
                    break;
                // stopSpeak
                case 5:
                    ILog.i(Tag, "调用stopSpeak接口", ifWriteToActivity);
                    ttsSession.stopSpeak();
                    break;
                // setParam
                case 6:
                    switch (random.nextInt(6)) {
                        case 0:
                            param = ESpeaker.ISS_TTS_PARAM_SPEAKER;
                            break;
                        case 1:
                            param = ESpeaker.ISS_TTS_PARAM_VOICE_SPEED;
                            break;
                        case 2:
                            param = ESpeaker.ISS_TTS_PARAM_VOICE_PITCH;
                            break;
                        case 3:
                            param = ESpeaker.ISS_TTS_PARAM_VOLUME;
                            break;
                        case 4:
                            param = ESpeaker.ISS_TTS_PARAM_USERMODE;
                            break;
                        case 5:
                            param = random.nextInt(5000);
                            break;
                    }

                    switch (random.nextInt(4)) {
                        case 0:
                            paramValue = -32768;
                            break;
                        case 1:
                            paramValue = 32767;
                            break;
                        case 2:
                            paramValue = random.nextInt();
                        case 3:
                            paramValue = random.nextInt(10);
                    }

                    ILog.i(Tag, "调用setParam接口，param：" + param + "，paramValue：" + paramValue,
                            ifWriteToActivity);
                    ttsSession.setParam(param, paramValue);
                    break;
            }

            // 判断是否结束测试
            if (tool.isTimeUp())
                break;
        }
    }

    /**
     * 循环调用接口。
     */
    public void testRollApi() {
        tool.setRunTime(runTime);

//        ttsSession.sessionStart(def.iTtsListener, 1);
        String text = def.getRandomCHStr(10);

        while (true) {
            TtsSession ttsSession = new TtsSession(context, def.iTtsInitListener, def.resDir);
            ttsSession.sessionStart(def.iTtsListener, 5);

//            ttsSession.startSpeak(text);
//            ttsSession.pauseSpeak();
//            ttsSession.resumeSpeak();
//            ttsSession.stopSpeak();
//            ttsSession.setParam(ESpeaker.ISS_TTS_PARAM_VOICE_SPEED, 32767);

            ttsSession.sessionStop();

            // 判断是否结束测试
            if (tool.isTimeUp())
                break;
        }

        ttsSession.sessionStop();
    }

    public void test_multiThreading1() {
        ExecutorService exe = Executors.newFixedThreadPool(5);

        for (int i = 0; i < 500; i++) {
            exe.execute(new ThreadRollCreate());
        }

        exe.shutdown();
        while (!exe.isTerminated())
            tool.sleep(10);

        System.out.println("Test over");
    }

    class ThreadTTS implements Runnable {
        private TtsSession ttsSession;
        private DefTTS def = new DefTTS();
        private Tool tool = new Tool();

        public ThreadTTS() {
            def.isWriteToActivity = 0;
            ttsSession = new TtsSession(context, def.iTtsInitListener, def.resDir);

            synchronized (this) {
                while (!def.msgInitStatus) {
                    tool.sleep(10);
                }
                def.msgInitStatus = false;
            }
        }

        @Override
        public void run() {
            ttsSession.sessionStart(def.iTtsListener, 5);
            ttsSession.startSpeak(def.getRandomCHStr(7));
            tool.sleep(new Random().nextInt(3));
            ttsSession.sessionStop();
        }
    }

    class ThreadRollCreate implements Runnable{
        private DefTTS def = new DefTTS();
        private Tool tool = new Tool();

        public ThreadRollCreate() {
            def.isWriteToActivity = 0;
        }

        @Override
        public void run() {
            while (true) {
                TtsSession ttsSession = new TtsSession(context, def.iTtsInitListener, def.resDir);
                while (!def.msgInitStatus) {
                    tool.sleep(10);
                }
                def.msgInitStatus = false;

                ttsSession.sessionStart(def.iTtsListener, 5);
                ttsSession.sessionStop();
            }
        }
    }
}
