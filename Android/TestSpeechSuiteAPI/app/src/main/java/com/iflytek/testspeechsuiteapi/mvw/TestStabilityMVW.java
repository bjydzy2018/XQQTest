package com.iflytek.testspeechsuiteapi.mvw;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;

import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.mvw.MvwSession;
import com.iflytek.speech.ISSErrors;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TestStabilityMVW {
    private MvwSession mvwSession;
    private Context context;  // MainActivity的context
    private Activity activity;
    private Tool tool = new Tool();
    public DefMVW def = new DefMVW();
    private int err = -1;
    private long runTime; // 运行时间，以秒为单位

    private String Tag = "TestStabilityMVW";
    public int isWriteToActivity = 1;

    private boolean isRunningRandomTest = false;

    // 供批量唤醒(一致性)使用, 测试每个场景只需修改scene即可
    private int scene = 1;
    private String mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/SceneId-" + scene + "" +
            ".txt";  // 音频文件列表
    private String vwRetName = "SceneId-" + scene + "/SceneId-" + scene + "_ret.txt";

    private String vwTimeRetName = "SceneId-" + scene + "/SceneId-" + scene + "_ret.csv";

    public TestStabilityMVW(Context context) {
        this.context = context;

        mvwSession = new MvwSession(context, def.iMvwListener, def.resDir);
        ILog.i(Tag, "开始初始化", isWriteToActivity);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
    }

    public TestStabilityMVW(Context context, long runTime) {
        this.context = context;
        this.runTime = runTime;
    }

    public TestStabilityMVW(Context context, Activity activity, long runTime) {
        this.context = context;
        this.runTime = runTime;
        this.activity = activity;
    }

    /**
     * 按场景测试批量唤醒
     *
     * @param isWriteRetToFile  是否写入结果到文件，性能测试时建议关闭。
     * @param isWriteToActivity 是否将结果显示到车机屏幕
     * @param scenes            需要测试的场景，可作为多个参数传入。
     */
    public void testManyMVWByScene(boolean isWriteRetToFile, boolean isWriteToActivity, int...
            scenes) {
        def.isWriteToActivity = isWriteToActivity ? 1 : 0;
        tool.isWriteToActivity = isWriteToActivity ? 1 : 0;
        this.isWriteToActivity = isWriteToActivity ? 1 : 0;

        for (int i = 0; i < scenes.length; i++) {
            scene = scenes[i];
            mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                    + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/SceneId-" +
                    scene + ".txt";
            vwRetName = "SceneId-" + scene + "/SceneId-" + scene + "_ret.txt";

            testManyMVW("scene(" + scene + ")", isWriteRetToFile);
        }
    }

    /**
     * 按唤醒词测试批量。
     *
     * @param isWriteRetToFile  是否将结果写入到文件，测试性能时建议关闭。
     * @param isWriteToActivity 是否将结果显示到界面，测试性能时建议关闭。
     */
    public void testManyMVWByWord(boolean isWriteRetToFile, boolean isWriteToActivity) {
        def.isWriteToActivity = isWriteToActivity ? 1 : 0;
        tool.isWriteToActivity = isWriteToActivity ? 1 : 0;
        this.isWriteToActivity = isWriteToActivity ? 1 : 0;

        scene = 1;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/SceneId-" + scene +
                ".txt";
        vwRetName = "SceneId-" + scene + "/你好语音助理_ret.txt";
        testManyMVW("1-你好语音助理", isWriteRetToFile);

        scene = 2;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/确定.txt";
        vwRetName = "SceneId-" + scene + "/确定_ret.txt";
        testManyMVW("2-确定", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/取消.txt";
        vwRetName = "SceneId-" + scene + "/取消_ret.txt";
        testManyMVW("2-取消", isWriteRetToFile);

        scene = 4;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第一个.txt";
        vwRetName = "SceneId-" + scene + "/第一个_ret.txt";
        testManyMVW("4-第一个", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第二个.txt";
        vwRetName = "SceneId-" + scene + "/第二个_ret.txt";
        testManyMVW("4-第二个", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第三个.txt";
        vwRetName = "SceneId-" + scene + "/第三个_ret.txt";
        testManyMVW("4-第三个", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第四个.txt";
        vwRetName = "SceneId-" + scene + "/第四个_ret.txt";
        testManyMVW("4-第四个", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第五个.txt";
        vwRetName = "SceneId-" + scene + "/第五个_ret.txt";
        testManyMVW("4-第五个", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第六个.txt";
        vwRetName = "SceneId-" + scene + "/第六个_ret.txt";
        testManyMVW("4-第六个", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/最后一个.txt";
        vwRetName = "SceneId-" + scene + "/最后一个_ret.txt";
        testManyMVW("4-最后一个", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/取消.txt";
        vwRetName = "SceneId-" + scene + "/取消_ret.txt";
        testManyMVW("4-取消", isWriteRetToFile);

        scene = 8;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/接听.txt";
        vwRetName = "SceneId-" + scene + "/接听_ret.txt";
        testManyMVW("8-接听", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/挂断.txt";
        vwRetName = "SceneId-" + scene + "/挂断_ret.txt";
        testManyMVW("8-挂断", isWriteRetToFile);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/取消.txt";
        vwRetName = "SceneId-" + scene + "/取消_ret.txt";
        testManyMVW("8-取消", isWriteRetToFile);

        mvwSession.stop();
        mvwSession.release();
    }

    /**
     * 批量读取音频进行唤醒
     *
     * @param vwLabel          唤醒标签，传入如场景名或唤醒词，便于不写文件时在屏幕上看结果
     * @param isWriteRetToFile 是否将结果写到文件
     * @param test_type        测试类型，可不传，默认为效果。0：效果，1：响应时间
     */
    private void testManyMVW(String vwLabel, boolean isWriteRetToFile, int... test_type) {
        def.initMsg();
        String wakeupRetPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/test.hcliu/mvw/效果/" + vwRetName;
        if (test_type.length > 0) {
            if (test_type[0] == 1)
                wakeupRetPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                        "/test.hcliu/mvw/响应时间/" + vwRetName;
        }
        int vwNum = 0; // 尝试唤醒次数
        int vwSuccess = 0; // 唤醒成功次数
        int vwSuccessWithVwdEnd = 0; // 含有vad后端点音频唤醒成功次数
        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());
        Date startAppendAudioDataDate = null;
        long vadEndTime = 0;
        long vwTime = 0; // 唤醒响应时间
        long vwTimeSum = 0; // 总的唤醒响应时间， 用于计算平均值

        for (int j = 0; j < 1; j++) {
            try {
                BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream
                        (mvwPcmListPath), "utf-8"));
                PrintWriter printWriter = null;
                DecimalFormat df = (DecimalFormat) DecimalFormat.getInstance();
                df.applyPattern("0.00");
                if (isWriteRetToFile) {
                    printWriter = new PrintWriter(new FileWriter(wakeupRetPath));
                }

                String p = null;
                List<String> pcmPaths = new ArrayList<>();
                while ((p = br.readLine()) != null) {
                    pcmPaths.add(p);
                }
                br.close();

                err = mvwSession.start(scene);

                for (String pcmPath : pcmPaths) {
                    String currentFilePath = pcmPath;
                    vwNum++;
                    ILog.i(Tag, "当前测试序号：" + vwNum + "，成功率：" + df.format(vwNum == 1 ? 0 : (
                            (double) vwSuccess) / (vwNum - 1) * 100) + "%", isWriteToActivity);
                    if (printWriter != null)
                        printWriter.print(currentFilePath);
                    pcmPath = Environment.getExternalStorageDirectory().getAbsolutePath() + pcmPath;

                    vadEndTime = (long) (getVadEndTime(pcmPath) * 1000);
                    ILog.i(Tag, "VadEnd:" + vadEndTime, isWriteToActivity);

                    startAppendAudioDataDate = dfs.parse(dfs.format(new Date()));
                    // appendAudioData
                    tool.loadPcmAndAppendAudioData(mvwSession, pcmPath, def);

                    if (def.msgVwWakeup) {
                        vwSuccess++;
                        String vwRet = "\t" + def.wakeupRet;
                        if (vadEndTime != 0) {
                            vwTime = def.vwWakeupInfo.getVwDate().getTime() -
                                    startAppendAudioDataDate.getTime
                                            () - vadEndTime;
                            if (vwTime > 0) {
                                vwSuccessWithVwdEnd++;
                                vwTimeSum += vwTime;
                            }
                            vwRet += "\t" + vwTime;
                        }
                        if (printWriter != null)
                            printWriter.println(vwRet);
                        ILog.i(Tag, vwRet, isWriteToActivity);
                    } else {
                        ILog.e(Tag, "唤醒失败", isWriteToActivity);
                        if (printWriter != null)
                            printWriter.println();
                    }
                    def.msgVwWakeup = false;
                }
                String ret = "";
                if (vwTimeSum != 0)
                    ret = "vwLabel：" + vwLabel + ", 尝试唤醒次数：" + vwNum + ", 唤醒成功次数：" + vwSuccess
                            + "，成功率：" + df.format(((double) vwSuccess) / vwNum * 100) + "%" +
                            "，平均唤醒响应时间：" + df.format(((double) vwTimeSum) / vwSuccessWithVwdEnd);
                else
                    ret = "vwLabel：" + vwLabel + ", 尝试唤醒次数：" + vwNum + ", 唤醒成功次数：" + vwSuccess
                            + "，成功率：" + df.format(((double) vwSuccess) / vwNum * 100) + "%";
                ILog.i(Tag, ret, 1);
                if (printWriter != null)
                    printWriter.println(ret);

                if (printWriter != null)
                    printWriter.close();

                ILog.i(Tag, "测试结束", 1);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void testRandomExit(int tag) {
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        ComponentName cn = new ComponentName("com.iflytek.hcliu.teststressspeechsuite30mvw", "com" +
                ".iflytek.hcliu.teststressspeechsuite30mvw.MainActivity");
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
        activity.startActivityForResult(intent, 2);
    }

    /**
     * 测试唤醒响应时间。
     * 计算直到收到唤醒回调前appendAudioData的次数，以计算一共送了多长时间的音频，然后用这个时间
     * 减去唤醒回调中的后端点时间
     */
    public void test_mvwTime(boolean isWriteRetToFile, boolean isWriteToActivity, int...
            scenes) {
        def.isWriteToActivity = isWriteToActivity ? 1 : 0;
        tool.isWriteToActivity = isWriteToActivity ? 1 : 0;
        this.isWriteToActivity = isWriteToActivity ? 1 : 0;

        for (int i = 0; i < scenes.length; i++) {
            scene = scenes[i];
            mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                    + "/MVWTestSuite/MVWTestTime/SceneId-" + scene + "/SceneId-" + scene + ".txt";
            vwRetName = "SceneId-" + scene + "/SceneId-" + scene + "_ret.txt";

            testManyMVW("scene(" + scene + ")", isWriteRetToFile, 1);
        }
    }

    /**
     * 随机接口调用。
     * 构建实例时若传入runTime参数，则会运行该指定时间；否则会始终运行。
     */
    public void testRandom() {
        isRunningRandomTest = true;
        Random random = new Random();
        def.isWriteToActivity = isWriteToActivity;
        tool.isWriteToActivity = isWriteToActivity;

        mvwSession = new MvwSession(context, def.iMvwListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        tool.setRunTime(runTime);

        new Thread(new ThreadAppendAudioData(mvwSession, def, def.mvwPcm_global)).start();

        while (true) {
            switch (random.nextInt(9)) {
                // start
                case 0:
                    int scene = random.nextInt(25);
                    ILog.i(Tag, "调用start接口，scene：" + scene, isWriteToActivity);
                    mvwSession.start(scene);
                    break;
                // addStartScene
                case 1:
                    scene = random.nextInt(25);
                    ILog.i(Tag, "调用addStartScene接口，scene：" + scene, isWriteToActivity);
                    mvwSession.addStartScene(scene);
                    break;
                // stop
                case 2:
                    if (random.nextInt(5) == 3) {
                        ILog.i(Tag, "调用stop接口", isWriteToActivity);
                        mvwSession.stop();
                    }
                    break;
                // stopScene
                case 3:
                    if (random.nextInt(5) == 3) {
                        scene = random.nextInt(25);
                        ILog.i(Tag, "调用stopScene接口，scene：" + scene, isWriteToActivity);
                        mvwSession.stopScene(scene);
                    }
                    break;
                // setThreshold
                case 4:
                    scene = random.nextInt(25);
                    int id = random.nextInt(8);
                    int threshold = random.nextInt(1000);
                    ILog.i(Tag, "调用setThreshold接口，scene：" + scene + "，id：" + id + "，threshold：" +
                            threshold, isWriteToActivity);
                    mvwSession.setThreshold(scene, id, threshold);
                    break;
                // setMvwKeyWords
                case 5:
                    scene = random.nextInt(25);
                    ILog.i(Tag, "调用setMvwKeyWords接口，scene：" + scene, isWriteToActivity);
                    mvwSession.setMvwKeyWords(scene, def.mvwWord_KaiYi);
                    break;
                // setParam
                case 6:
                    String param = "";
                    String paramValue = "";
                    switch (random.nextInt(4)) {
                        case 0:
                            param = "mvw_enable_aec";
                            break;
                        case 1:
                            param = "mvw_enable_lsa";
                            break;
                        case 2:
                            param = "asdfaw";
                            break;
                        case 3:
                            param = "";
                            break;
                    }
                    switch (random.nextInt(4)) {
                        case 0:
                            paramValue = "on";
                            break;
                        case 1:
                            paramValue = "off";
                            break;
                        case 2:
                            paramValue = "asdfaw";
                            break;
                        case 3:
                            paramValue = "";
                            break;
                    }
                    ILog.i(Tag, "调用setParam接口，param：" + param + "，paramValue：" + paramValue,
                            isWriteToActivity);
                    mvwSession.setParam(param, paramValue);
                    break;
                // setMvwDefaultKeyWords
                case 7:
                    scene = random.nextInt(20);
                    ILog.i(Tag, "调用setMvwDefaultKeyWords接口，scene：" + scene, isWriteToActivity);
                    mvwSession.setMvwDefaultKeyWords(scene);
                    break;
//                case 8:
//                    ILog.i(Tag, "调用appendAudioData接口");
//                    new Thread(new Runnable() {
//                        @Override
//                        public void run() {
//                            tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi);
//                        }
//                    }).start();
//                    tool.sleep(500);
//                    break;
            }

//            tool.sleep(300);

            // 判断是否结束测试
            if (tool.isTimeUp())
                break;
        }

        isRunningRandomTest = false;
        mvwSession.setParam("mvw_enable_aec", "off");
        mvwSession.setParam("mvw_enable_lsa", "off");
        mvwSession.setMvwDefaultKeyWords(1);
        mvwSession.setThreshold(1, 0, 0);

        def.initMsg();
        mvwSession.start(1);
        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        mvwSession.release();
    }

    /**
     * 循环接口调用。
     */
    public void testRollApi() {
        while (true) {
            mvwSession = new MvwSession(context, def.iMvwListener, def.resDir);

            while (!def.msgInitStatus) {
                tool.sleep(10);
            }
            def.msgInitStatus = false;

            mvwSession.release();
        }
    }

    /**
     * 多线程调用接口。
     */
    public void test_multiThreading() throws InterruptedException {
        ExecutorService threadPool = Executors.newCachedThreadPool();

        DefMVW def = new DefMVW();
        def.isWriteToActivity = 0;
        MvwSession mvwSession = new MvwSession(context, def.iMvwListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        DefMVW def1 = new DefMVW();
        def1.isWriteToActivity = 0;
        MvwSession mvwSession1 = new MvwSession(context, def1.iMvwListener, def1.resDir_second);

        while (!def1.msgInitStatus) {
            tool.sleep(10);
        }

        threadPool.execute(new ThreadInitMvwSession(mvwSession));
        threadPool.execute(new ThreadInitMvwSession(mvwSession1));
        threadPool.shutdown();

        while (!threadPool.isTerminated())
            tool.sleep(10);
    }

    public void test_multiThreading1(){
        def.isWriteToActivity = this.isWriteToActivity;

        ExecutorService threadPool = Executors.newCachedThreadPool();

        MvwSession mvwSession = new MvwSession(context, def.iMvwListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        tool.sleep(10000);

        for (int i = 0; i < 100; i++) {
            threadPool.execute(new ThreadInitMvwSession(mvwSession));
        }
        threadPool.shutdown();

        while (!threadPool.isTerminated())
            tool.sleep(10);
    }

    private double getVadEndTime(String s) {
        Pattern pattern = Pattern.compile(".*-(\\d([(\\.\\d)|\\d])*)s.[pcm|wav]");
        Matcher matcher = pattern.matcher(s);

        if (matcher.find()) {
            return Double.parseDouble(matcher.group(1));
        }

        return 0;
    }

    private class ThreadInitMvwSession extends Thread {
        private MvwSession mvwSession;
        private Tool tool = new Tool();

        ThreadInitMvwSession(MvwSession mvwSession) {
            this.mvwSession = mvwSession;
            tool.isWriteToActivity = 0;
        }

        @Override
        public void run() {
            int err = -1;
            tool.setRunTime(runTime);

            while (true) {
                err = mvwSession.start(1);
//                if (err != 0)
//                    fun("start ret:"+err);

                err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);

                err = mvwSession.setMvwDefaultKeyWords(1);

                err = mvwSession.setParam("mvw_enable_aec", "on");

                err = mvwSession.setParam("mvw_enable_aec", "off");

                err = mvwSession.stop();

                // 判断是否结束测试
                if (tool.isTimeUp())
                    break;
            }
        }

        private void fun(String msg) throws RetNotSuccessException {
            throw new RetNotSuccessException("start ret:" + err);
        }

        class RetNotSuccessException extends Exception {
            public RetNotSuccessException(String msg) {
                super(msg);
            }
        }
    }

    private class ThreadAppendAudioData implements Runnable {
        private MvwSession mvwSession;
        private DefMVW def;
        private String pcmPath;

        public ThreadAppendAudioData(MvwSession mvwSession, DefMVW def, String pcmPath) {
            this.mvwSession = mvwSession;
            this.def = def;
            this.pcmPath = pcmPath;
        }

        @Override
        public void run() {
            while (isRunningRandomTest) {
                ILog.i(Tag, "调用appendAudioData接口", isWriteToActivity);
                tool.loadPcmAndAppendAudioData(this.mvwSession, this.pcmPath, this.def);
            }
        }
    }
}


