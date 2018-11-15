package com.iflytek.testspeechsuitenative.mvw;


import android.os.Environment;
import android.util.Log;

import com.iflytek.testspeechsuitenative.MainActivity;
import com.iflytek.speech.libissmvw;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TestMVWBatch extends TestMVW {
    // 供批量唤醒(一致性)使用, 测试每个场景只需修改scene即可
    private int scene = 1;
    private String mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/SceneId-" + scene + "" +
            ".txt";  // 音频文件列表
    private String vwRetName = "SceneId-" + scene + "/SceneId-" + scene + "_ret.txt";

    private final String Tag = "TestMVWBatch";
    public MainActivity mainActivity;

    public TestMVWBatch(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }

    /**
     * 按场景测试批量唤醒
     *
     * @param isWriteRetToFile 是否写入结果到文件，性能测试时建议关闭。
     * @param scenes           需要测试的场景，可作为多个参数传入。
     */
    public void testMVWBatchByScene(boolean isWriteRetToFile, int... scenes) {

        for (int i = 0; i < scenes.length; i++) {
            scene = scenes[i];
            mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                    + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/SceneId-" +
                    scene + ".txt";
            vwRetName = "SceneId-" + scene + "/SceneId-" + scene + "_ret.txt";

            testManyMVW("scene(" + scene + ")", false, isWriteRetToFile);
        }
    }

    /**
     * 按唤醒词测试批量。
     *
     * @param isWriteRetToFile 是否将结果写入到文件，测试性能时建议关闭。
     */
    public void testMVWBatchByWord(boolean isWriteRetToFile) {
        scene = 1;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/SceneId-" + scene +
                ".txt";
        vwRetName = "SceneId-" + scene + "/你好语音助理_ret.txt";
        testManyMVW("1-你好语音助理", isWriteRetToFile, false);

        scene = 2;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/确定.txt";
        vwRetName = "SceneId-" + scene + "/确定_ret.txt";
        testManyMVW("2-确定", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/取消.txt";
        vwRetName = "SceneId-" + scene + "/取消_ret.txt";
        testManyMVW("2-取消", isWriteRetToFile, false);

        scene = 4;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第一个.txt";
        vwRetName = "SceneId-" + scene + "/第一个_ret.txt";
        testManyMVW("4-第一个", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第二个.txt";
        vwRetName = "SceneId-" + scene + "/第二个_ret.txt";
        testManyMVW("4-第二个", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第三个.txt";
        vwRetName = "SceneId-" + scene + "/第三个_ret.txt";
        testManyMVW("4-第三个", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第四个.txt";
        vwRetName = "SceneId-" + scene + "/第四个_ret.txt";
        testManyMVW("4-第四个", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第五个.txt";
        vwRetName = "SceneId-" + scene + "/第五个_ret.txt";
        testManyMVW("4-第五个", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/第六个.txt";
        vwRetName = "SceneId-" + scene + "/第六个_ret.txt";
        testManyMVW("4-第六个", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/最后一个.txt";
        vwRetName = "SceneId-" + scene + "/最后一个_ret.txt";
        testManyMVW("4-最后一个", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/取消.txt";
        vwRetName = "SceneId-" + scene + "/取消_ret.txt";
        testManyMVW("4-取消", isWriteRetToFile, false);

        scene = 8;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/接听.txt";
        vwRetName = "SceneId-" + scene + "/接听_ret.txt";
        testManyMVW("8-接听", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/挂断.txt";
        vwRetName = "SceneId-" + scene + "/挂断_ret.txt";
        testManyMVW("8-挂断", isWriteRetToFile, false);

        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-" + scene + "/取消.txt";
        vwRetName = "SceneId-" + scene + "/取消_ret.txt";
        testManyMVW("8-取消", isWriteRetToFile, false);

        int errId = libissmvw.destroy(mNativeHandle);
        Log.i(Tag, "destroy libissmvw return " + errId);
    }

    /**
     * 测试DN版本一致性 2.14
     */
    public void testDNConsistent() {
        scene = 1;
        mvwPcmListPath = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/ivw/ivw.txt";
        vwRetName = "DN一致性" + "/ivw_ret.txt";
        testManyMVW("1-DN", true, true);
    }

    /**
     * 批量读取音频进行唤醒
     *
     * @param vwLabel          唤醒标签，传入如场景名或唤醒词，便于不写文件时在屏幕上看结果
     * @param isWriteRetToFile 是否将结果写到文件
     * @param test_type        测试类型，可不传，默认为效果。0：效果，1：响应时间
     */
    private void testManyMVW(String vwLabel, boolean isWriteRetToFile, boolean isTestConsistent, int... test_type) {
        def.initMsg();
        String wakeupRetPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/TestRes/mvw/效果/" + vwRetName;
        if (test_type.length > 0) {
            if (test_type[0] == 1)
                wakeupRetPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                        "/TestRes/mvw/响应时间/" + vwRetName;
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

                if (!isTestConsistent) {
                    int errId = libissmvw.start(mNativeHandle, scene);
                    Log.i(Tag, "start libissmvw return " + errId);
                }

                for (String pcmPath : pcmPaths) {
                    if(isTestConsistent) {
                        int errId = libissmvw.start(mNativeHandle, scene);
                        Log.i(Tag, "start libissmvw return " + errId);
                    }

                    String currentFilePath = pcmPath;
                    vwNum++;
                    Log.i(Tag, "当前测试序号：" + vwNum + "，成功率：" + df.format(vwNum == 1 ? 0 : (
                            (double) vwSuccess) / (vwNum - 1) * 100) + "%");
                    if (printWriter != null)
                        printWriter.print(currentFilePath);
                    pcmPath = Environment.getExternalStorageDirectory().getAbsolutePath() + pcmPath;

                    vadEndTime = (long) (getVadEndTime(pcmPath) * 1000);
                    Log.i(Tag, "VadEnd:" + vadEndTime);

                    startAppendAudioDataDate = dfs.parse(dfs.format(new Date()));
                    // appendAudioData
                    tool.loadPcmAndAppendAudioData("mvw", mNativeHandle, pcmPath, def);

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
                        Log.i(Tag, vwRet);
                    } else {
                        Log.e(Tag, "唤醒失败");
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
                Log.i(Tag, ret);
                mainActivity.setText(ret);

                if (printWriter != null)
                    printWriter.println(ret);

                if (printWriter != null)
                    printWriter.close();

                Log.i(Tag, "测试结束");
                mainActivity.setText("测试结束");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private double getVadEndTime(String s) {
        Pattern pattern = Pattern.compile(".*-(\\d([(\\.\\d)|\\d])*)s.[pcm|wav]");
        Matcher matcher = pattern.matcher(s);

        if (matcher.find()) {
            return Double.parseDouble(matcher.group(1));
        }

        return 0;
    }
}
