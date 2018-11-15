package com.iflytek.testspeechsuiteapi.sr;


import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.telephony.TelephonyManager;

import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.sr.SrSession;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;


public class TestStabilitySR {
    private Context context;
    private Activity activity;
    private SrSession srSession;
    private Tool tool = new Tool();
    private ToolSR toolSR = new ToolSR();
    private DefSR def = new DefSR();
    private Random random = new Random();

    private Date endAudioData_time;
    private SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
            .getDefault());
    private PrintWriter printWriter;

    private String ret_path = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/sr/stability/sr_ret.txt";

    private String Tag = "TestStabilitySR";
    private String imei;
    private int err = -1;
    private long runTime; // 运行时间，以秒为单位
    public int ifWriteToActivity = 1;

    public TestStabilitySR(Context context) {
        this.context = context;

        try {
            printWriter = new PrintWriter(new FileWriter(new File(ret_path)));
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }

        TelephonyManager tm = (TelephonyManager) context
                .getSystemService(Context.TELEPHONY_SERVICE);
        imei = tm.getDeviceId();
        if (imei == null || imei.length() == 0) {
            imei = "ifly" + Math.random() + Math.random() + Math.random()
                    + Math.random() + Math.random() + Math.random()
                    + Math.random() + Math.random() + Math.random()
                    + Math.random() + "tek";
        }

        ILog.i(Tag, "IMEI::" + imei, ifWriteToActivity);
    }

    public TestStabilitySR(Context context, long runTime) {
        this.context = context;
        this.runTime = runTime;

        try {
            printWriter = new PrintWriter(new FileWriter(new File(ret_path)));
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }

        TelephonyManager tm = (TelephonyManager) context
                .getSystemService(Context.TELEPHONY_SERVICE);
        imei = tm.getDeviceId();
        if (imei == null || imei.length() == 0) {
            imei = "ifly" + Math.random() + Math.random() + Math.random()
                    + Math.random() + Math.random() + Math.random()
                    + Math.random() + Math.random() + Math.random()
                    + Math.random() + "tek";
        }
    }

    public TestStabilitySR(Context context, Activity activity, long runTime) {
        this.context = context;
        this.runTime = runTime;
        this.activity = activity;

        try {
            printWriter = new PrintWriter(new FileWriter(new File(ret_path)));
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }

        TelephonyManager tm = (TelephonyManager) context
                .getSystemService(Context.TELEPHONY_SERVICE);
        imei = tm.getDeviceId();
        if (imei == null || imei.length() == 0) {
            imei = "ifly" + Math.random() + Math.random() + Math.random()
                    + Math.random() + Math.random() + Math.random()
                    + Math.random() + Math.random() + Math.random()
                    + Math.random() + "tek";
        }
    }

    /**
     * 批量识别音频，生成的结果在TestRes/sr/stability/sr_ret.txt
     *
     * @param scene   识别场景
     * @param mode    识别模式
     * @param pcmList 音频列表路径
     */
    public void testManySR(String scene, int mode, String pcmList) {
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        BufferedReader br = null;

        try {
            br = new BufferedReader(new InputStreamReader(new FileInputStream(pcmList), "utf8"));
            String pcm_path = null;
            while ((pcm_path = br.readLine()) != null) {
                err = srSession.start(scene, mode, null);
                HandleRet.handlStartRet(err);

                ILog.i(Tag, "pcm: " + pcm_path, 1);
                tool.loadPcmAndAppendAudioData(srSession, pcm_path, def);

                err = srSession.endAudioData();
                if (err == 0 || err == 10000) {
                    try {
                        endAudioData_time = dfs.parse(dfs.format(new Date()));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    int wait_time = 0;
                    while (!def.msgResult && wait_time++ < 1000) {
                        tool.sleep(10);
                    }
                    def.msgResult = false;

                    if (wait_time < 1000) {
                        long time_resume = def.result_time.getTime() - Math.min(endAudioData_time
                                .getTime(), def.speechend_time);
                        printWriter.println(pcm_path + "\t" + def.srResult.replace("\n", "") + "\t" +
                                time_resume);
                        ILog.i(Tag, "srResult: " + def.srResult, 1);
                    } else {
                        printWriter.println(pcm_path + "\t" + def.srErrCode);
                    }
                } else {
                    printWriter.println(pcm_path);
                }
                printWriter.flush();
            }
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        printWriter.close();
        System.out.println("Test over");
    }

    public void testRandomExit(int tag) {
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        ComponentName cn = new ComponentName("com.iflytek.teststressspeechsuitesr", "com" +
                ".iflytek.teststressspeechsuitesr.MainActivity");
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
        activity.startActivityForResult(intent, 0);
    }

    /**
     * 随机接口调用。
     */
    public void testRandom() {
        def.isWriteToActivity = ifWriteToActivity;
        tool.isWriteToActivity = 0;

        // 初始化
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;
        if (!def.msgInitStatus_success) {
            return;
        }

        tool.setRunTime(runTime);

//        new Thread(new ThreadRollUploadDict()).start();
//        new Thread(new ThreadRollAppendAudioData()).start();

        while (true) {
            switch (random.nextInt(5)) {
                // start
                case 0:
                    String scene = getStartScene();
                    int mode = getStartMode();
                    String szCmd = getSzCmd();

//                    ILog.i(Tag, "调用start接口，scene：" + sceneNum2Str(scene) + "，mode：" + modeNum2Str
//                            (mode) + "，szCmd：" + (szCmd == null || szCmd.length() <= 0 ? "null" :
//                            szCmd), ifWriteToActivity);
                    err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
//                    ILog.i(Tag, "start接口返回：" + err, ifWriteToActivity);
                    break;
                // stop
//                case 1:
//                    int t = random.nextInt(30);
//                    if (t < 10) {
////                        ILog.i(Tag, "调用stop接口", ifWriteToActivity);
//                        err = srSession.stop();
////                        ILog.i(Tag, "stop接口返回：" + err, ifWriteToActivity);
//                    }
//                    break;
                // getInstance
//                case 2:
////                    ILog.i(Tag, "调用getInstance接口", ifWriteToActivity);
////                    srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
////                    ILog.i(Tag, "退出getInstance接口", ifWriteToActivity);
//                    break;
                // endAudioData
//                case 3:
//                    t = random.nextInt(30);
//                    if (t < 10) {
////                        ILog.i(Tag, "调用endAudioData接口", ifWriteToActivity);
//                        err = srSession.endAudioData();
////                        ILog.i(Tag, "endAudioData接口返回：" + err, ifWriteToActivity);
//                    }
//                    break;
                // setParam
//                case 4:
//                    String szParam = getSzParam();
//                    String szParamValue = getSzParamValue();
//
////                    ILog.i(Tag, "调用setParam接口，szParam：" + szParamNum2Str(szParam) +
////                            "，szPatamValue：" + szParamValue, ifWriteToActivity);
//                    err = srSession.setParam(szParam, szParamValue);
////                    ILog.i(Tag, "setParam接口返回：" + err, ifWriteToActivity);
//                    break;
            }

            // 判断是否结束测试
            if (tool.isTimeUp()) {
                break;
            }
        }
        ILog.i(Tag, "测试结束");
    }

    /**
     * 循环调用接口。
     */
    public void testRollApi() {
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;
        if (!def.msgInitStatus_success) {
            return;
        }

        tool.setRunTime(runTime);

        while (true) {
            err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
            err = srSession.stop();

            if (tool.isTimeUp())
                return;
        }
    }

    /**
     * 开启log，一个线程上传词典，一个线程送录音，一个线程循环start-stop
     */
    public void test_multiThreading1() {
        ExecutorService exe = Executors.newCachedThreadPool();
        def.isWriteToActivity = 0;
        tool.isWriteToActivity = 0;

        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        exe.execute(new ThreadRollUploadDict());
        exe.execute(new ThreadRollStartStop());
        exe.execute(new ThreadRollAppendAudioData());

        exe.shutdown();
        while (!exe.isTerminated())
            tool.sleep(10);
    }

    /**
     * 开启log，一个线程上传词典，一个线程正常送录音识别
     */
    public void test_multiThreading2() {
        ExecutorService exe = Executors.newCachedThreadPool();
        def.isWriteToActivity = 0;
        tool.isWriteToActivity = 0;

        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        exe.execute(new ThreadRollUploadDict());
        exe.execute(new ThreadRollSr());

        exe.shutdown();
        while (!exe.isTerminated())
            tool.sleep(10);
    }

    private class ThreadRollSr implements Runnable {
        @Override
        public void run() {
            Tool tool = new Tool();
            tool.isWriteToActivity = 0;

            tool.setRunTime(runTime);
            while (true) {
                srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);

                tool.loadPcmAndAppendAudioData(srSession, def.srPcm_confirm, def);

                synchronized (this) {
                    while (!def.msgResult) {
                        tool.sleep(10);
                    }
                    def.msgResult = false;
                }

                if (tool.isTimeUp())
                    break;
            }
        }
    }

    private class ThreadRollStartStop implements Runnable {

        @Override
        public void run() {
            Tool tool = new Tool();
            tool.isWriteToActivity = 0;
            tool.setRunTime(runTime);

            while (true) {
                srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
                srSession.stop();

                if (tool.isTimeUp())
                    break;
            }
        }
    }

    private class ThreadRollAppendAudioData implements Runnable {
        @Override
        public void run() {
            Tool tool = new Tool();
            tool.isWriteToActivity = 0;
            tool.setRunTime(runTime);

            while (true) {
                tool.loadPcmAndAppendAudioData(srSession, def.srPcm_callBaiYaWei);
                tool.sleep(3000);

                if (tool.isTimeUp())
                    break;
            }
        }
    }

    /**
     * 循环上传词典线程类。
     */
    class ThreadRollUploadDict implements Runnable {
        @Override
        public void run() {
            String dict;
            int ifOnlyUploadToCloud = 0;
            Tool tool = new Tool();
            tool.isWriteToActivity = 0;
            tool.setRunTime(runTime);

            while (true) {
//                switch (random.nextInt(3)) {
//                    case 0:
//                        dict = def.dict_500;
//                        break;
//                    case 1:
//                        dict = def.dict_1000;
//                        break;
//                    default:
//                        dict = def.dict_5000;
//                }
                ifOnlyUploadToCloud = random.nextInt(2);

                ILog.i(Tag, "调用uploadDict接口，ifOnlyUploadToCloud：" + ifOnlyUploadToCloud,
                        ifWriteToActivity);
                err = srSession.uploadDict(def.dict_500, ifOnlyUploadToCloud);
                ILog.i(Tag, "uploadDict接口返回：" + err, ifWriteToActivity);

                try {
                    TimeUnit.SECONDS.sleep(5);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                if (tool.isTimeUp())
                    break;
            }
        }
    }

    class ThreadUploadDict implements Runnable {
        private String dict;

        public ThreadUploadDict(String dict) {
            this.dict = dict;
        }

        @Override
        public void run() {
            srSession.uploadDict(dict, 0);
        }
    }

    private String getStartScene() {
        switch (random.nextInt(11)) {
            case 0:
                return SrSession.ISS_SR_SCENE_SELECT_MVW;
            case 1:
                return SrSession.ISS_SR_SCENE_ANSWER_CALL;
            case 2:
                return SrSession.ISS_SR_SCENE_ANSWER_CALL_MVW;
            case 3:
                return SrSession.ISS_SR_SCENE_BUILD_GRM_MVW;
            case 4:
                return SrSession.ISS_SR_SCENE_CMDLIST_WITHALL;
            case 5:
                return SrSession.ISS_SR_SCENE_CONFIRM;
            case 6:
                return SrSession.ISS_SR_SCENE_CONFIRM_MVW;
            case 7:
                return SrSession.ISS_SR_SCENE_CONTACTS;
            case 8:
                return SrSession.ISS_SR_SCENE_POI;
            case 9:
                return SrSession.ISS_SR_SCENE_SELECT;
            default:
                return SrSession.ISS_SR_SCENE_ALL;
        }
    }

    private int getStartMode() {
        switch (random.nextInt(5)) {
            case 0:
                return SrSession.ISS_SR_MODE_CLOUD_REC;
            case 1:
                return SrSession.ISS_SR_MODE_LOCAL_CMDLIST;
            case 2:
                return SrSession.ISS_SR_MODE_LOCAL_NLP;
            case 3:
                return SrSession.ISS_SR_MODE_LOCAL_REC;
            default:
                return SrSession.ISS_SR_MODE_MIX_REC;
        }
    }

    private String getSzCmd() {
        switch (random.nextInt(4)) {
            case 0:
                return "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": " +
                        "\"司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝\", \"id\": 0 }]}]}";
            case 1:
                return "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [\"name\": " +
                        "\"司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝\", \"id\": 0 }]}]}";
            case 2:
                return "{\"grm\": \"a\"}";
            default:
                return null;
        }
    }

    private String getSzParam() {
        switch (random.nextInt(11)) {
            case 0:
                return SrSession.ISS_SR_PARAM_LONGTITUDE;
            case 1:
                return SrSession.ISS_SR_PARAM_LATITUDE;
            case 2:
                return SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT;
            case 3:
                return SrSession.ISS_SR_PARAM_IAT_EXTEND_PARAMS;
            case 4:
                return SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT;
            case 5:
                return SrSession.ISS_SR_PARAM_SPEECH_TAIL;
            case 6:
                return SrSession.ISS_SR_PARAM_CITY;
            case 7:
                return SrSession.ISS_SR_PARAM_TRACE_LEVEL;
            case 8:
                return SrSession.ISS_SR_MVW_PARAM_AEC;
            case 9:
                return SrSession.ISS_SR_MVW_PARAM_LSA;
            default:
                return SrSession.ISS_SR_PARAM_ACOUS_LANG;
        }
    }

    private String getSzParamValue() {
        switch (random.nextInt(12)) {
            case 0:
                return "102.4";
            case 1:
                return "青岛";
            case 2:
                return "陶喆";
            case 3:
                return "△╲Ⅸ❽‰にオУ┿夊Óū";
            case 4:
                return "" + random.nextInt(7000);
            case 5:
                return "on";
            case 6:
                return "off";
            case 7:
                return SrSession.ISS_SR_PARAM_ACOUS_LANG_VALUE_MANDARIN;
            case 8:
                return SrSession.ISS_SR_PARAM_ACOUS_LANG_VALUE_ENGLISH;
            case 9:
                return SrSession.ISS_SR_PARAM_ACOUS_LANG_VALUE_CANTONESE;
            case 10:
                return SrSession.ISS_SR_PARAM_ACOUS_LANG_VALUE_HENANESE;
            default:
                return "=.=";
        }
    }

    private String sceneNum2Str(String scene) {
        switch (scene) {
            case SrSession.ISS_SR_SCENE_SELECT_MVW:
                return "ISS_SR_SCENE_SELECT_MVW";
            case SrSession.ISS_SR_SCENE_ANSWER_CALL:
                return "ISS_SR_SCENE_ANSWER_CALL";
            case SrSession.ISS_SR_SCENE_ANSWER_CALL_MVW:
                return "ISS_SR_SCENE_ANSWER_CALL_MVW";
            case SrSession.ISS_SR_SCENE_BUILD_GRM_MVW:
                return "ISS_SR_SCENE_BUILD_GRM_MVW";
            case SrSession.ISS_SR_SCENE_CMDLIST_WITHALL:
                return "ISS_SR_SCENE_CMDLIST_WITHALL";
            case SrSession.ISS_SR_SCENE_CONFIRM:
                return "ISS_SR_SCENE_CONFIRM";
            case SrSession.ISS_SR_SCENE_CONFIRM_MVW:
                return "ISS_SR_SCENE_CONFIRM_MVW";
            case SrSession.ISS_SR_SCENE_CONTACTS:
                return "ISS_SR_SCENE_CONTACTS";
            case SrSession.ISS_SR_SCENE_POI:
                return "ISS_SR_SCENE_POI";
            case SrSession.ISS_SR_SCENE_SELECT:
                return "ISS_SR_SCENE_SELECT";
            default:
                return "ISS_SR_SCENE_ALL";
        }
    }

    private String modeNum2Str(int num) {
        switch (num) {
            case 0:
                return "ISS_SR_MODE_CLOUD_REC";
            case 1:
                return "ISS_SR_MODE_LOCAL_CMDLIST";
            case 2:
                return "ISS_SR_MODE_LOCAL_NLP";
            case 3:
                return "ISS_SR_MODE_LOCAL_REC";
            default:
                return "ISS_SR_MODE_MIX_REC";
        }
    }

    private String szParamNum2Str(String param) {
        switch (param) {
            case SrSession.ISS_SR_PARAM_LONGTITUDE:
                return "ISS_SR_PARAM_LONGTITUDE";
            case SrSession.ISS_SR_PARAM_LATITUDE:
                return "ISS_SR_PARAM_LATITUDE";
            case SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT:
                return "ISS_SR_PARAM_SPEECH_TIMEOUT";
            case SrSession.ISS_SR_PARAM_IAT_EXTEND_PARAMS:
                return "ISS_SR_PARAM_IAT_EXTEND_PARAMS";
            case SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT:
                return "ISS_SR_PARAM_RESPONSE_TIMEOUT";
            case SrSession.ISS_SR_PARAM_SPEECH_TAIL:
                return "ISS_SR_PARAM_SPEECH_TAIL";
            case SrSession.ISS_SR_PARAM_CITY:
                return "ISS_SR_PARAM_CITY";
            case SrSession.ISS_SR_PARAM_TRACE_LEVEL:
                return "ISS_SR_PARAM_TRACE_LEVEL";
            default:
                return "ISS_SR_PARAM_ACOUS_LANG";
        }
    }

}

