package com.iflytek.testspeechsuitenative.tts;


import android.os.Environment;
import android.util.Log;

import com.iflytek.speech.libisstts;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Date;
import java.util.concurrent.TimeUnit;

/**
 * <p>
 * 测试合成回调时间。
 */

public class TestTTSBatchTime extends TestTTS {
    private String Tag = "TTS.TestTTSBatch";
    Date baseDate;
    Date callbackDate;
    long passTime;
    long sumPassTime = 0;
    int textNum = 0;
    long initTime = 0;
    long createTime = 0;

    public void test() {
        Date startDate;
        Date endDate;

        try {
            startDate = def.dfs.parse(def.dfs.format(new Date()));
            errCode = libisstts.initRes(def.resDir, 0);
            endDate = def.dfs.parse(def.dfs.format(new Date()));
            Log.i(Tag, "initRes errCode: " + errCode);

            initTime = endDate.getTime() - startDate.getTime();
            Log.i(Tag, "initRes time: " + initTime);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        try {
            startDate = def.dfs.parse(def.dfs.format(new Date()));
            libisstts.create(mNativeHandle, def.mTtsListener);
            endDate = def.dfs.parse(def.dfs.format(new Date()));
            Log.i(Tag, "initRes errCode: " + mNativeHandle.err_ret);

            createTime = endDate.getTime() - startDate.getTime();
            Log.i(Tag, "create time: " + createTime);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        test_tts_batch(Environment.getExternalStorageDirectory().getAbsolutePath() + "/TestRes/tts/file_handle/batch_ret" + ".txt");

        libisstts.destroy(mNativeHandle);
        libisstts.unInitRes();

        try {
            startDate = def.dfs.parse(def.dfs.format(new Date()));
            errCode = libisstts.initRes(def.resDir, 1);
            endDate = def.dfs.parse(def.dfs.format(new Date()));
            Log.i(Tag, "initRes errCode: " + errCode);

            initTime = endDate.getTime() - startDate.getTime();
            Log.i(Tag, "initRes time: " + initTime);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        try {
            startDate = def.dfs.parse(def.dfs.format(new Date()));
            libisstts.create(mNativeHandle, def.mTtsListener);
            endDate = def.dfs.parse(def.dfs.format(new Date()));
            Log.i(Tag, "initRes errCode: " + mNativeHandle.err_ret);

            createTime = endDate.getTime() - startDate.getTime();
            Log.i(Tag, "create time: " + createTime);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        test_tts_batch(Environment.getExternalStorageDirectory().getAbsolutePath() + "/TestRes/tts/mem/batch_ret" + ".txt");

    }

    private void test_tts_batch(String ret_path) {
        BufferedReader bReader = null;
        PrintWriter printWriter = null;
        ArrayList<String> ttsTests = new ArrayList<>();

        try {
            bReader = new BufferedReader(new FileReader(def.ttsList));
            printWriter = new PrintWriter(new FileWriter(ret_path));
            String line = null;
            while ((line = bReader.readLine()) != null) {
                ttsTests.add(line);
            }
            bReader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        printWriter.println("Init resume " + initTime + " ms");
        printWriter.println("Create resume " + createTime + " ms\n");

        for (String text :
                ttsTests) {
            textNum++;
            printWriter.println(text);

            try {
                libisstts.start(mNativeHandle, text);
                baseDate = def.dfs.parse(def.dfs.format(new Date()));
                Log.i(Tag, "start errCode: " + mNativeHandle.err_ret);

                while (!def.msgOnDataReady) {
                    try {
                        TimeUnit.MILLISECONDS.sleep(2);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                def.initMsg();

                callbackDate = def.dfs.parse(def.dfs.format(new Date()));

                passTime = callbackDate.getTime() - baseDate.getTime();
                printWriter.println("tts proc result resume " + passTime + " ms\n");
                Log.i(Tag, "tts proc result resume " + passTime);
                sumPassTime += passTime;
            } catch (ParseException e) {
                e.printStackTrace();
            }
        }
        Log.i(Tag, "the avg tts proc result resume " + sumPassTime / textNum);
        printWriter.println("the avg tts proc result resume " + sumPassTime / textNum);
        printWriter.close();
    }
}
