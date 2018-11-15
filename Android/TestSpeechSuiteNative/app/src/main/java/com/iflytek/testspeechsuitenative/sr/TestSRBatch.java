package com.iflytek.testspeechsuitenative.sr;

import android.os.Environment;
import android.util.Log;

import com.iflytek.testspeechsuitenative.Tool;
import com.iflytek.speech.libisssr;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;


public class TestSRBatch {
    private final String Tag = "TestSRBatch";
    DefSR def = new DefSR();
    Tool tool = new Tool();

    public TestSRBatch() {
        libisssr.setMachineCode("test_speechsuite_machinecode");

        libisssr.create(def.resDir, def.mSRListener);
        while (!def.msgInitSuccess) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * 测试上传词典的时间
     */
    public void test_upLoadDictTime(int ifOnlyUploadToCloud, String... dictPaths) {
        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(new FileWriter(new File(Environment
                    .getExternalStorageDirectory()
                    .getAbsolutePath() + "/TestRes/sr/upload_time/ret.txt")));
        } catch (IOException e) {
            e.printStackTrace();
        }

        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());
        Date start = null;

        tool.sleep(10000);

        for (String dictPath : dictPaths) {
            try {
                start = dfs.parse(dfs.format(new Date()));
            } catch (Exception e) {
                e.printStackTrace();
            }

            libisssr.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                    .getAbsolutePath() + dictPath), ifOnlyUploadToCloud);

            if (ifOnlyUploadToCloud == 0) {
                // &&: 上传云端  ||: 上传云+端
                while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                    tool.sleep(10);
                }
            } else {
                while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                    tool.sleep(10);
                }
            }

            long between_cloud = def.uploadDictToCloud_time.getTime() - start.getTime();
            long between_local = def.uploadDictToLocal_time.getTime() - start.getTime();
            Log.i(Tag, "the time of uploading dict to cloud:" + between_cloud);
            Log.i(Tag, "the time of uploading dict to local:" + between_local);

            printWriter.println(dictPath);
            printWriter.println("local time: " + between_local);
            printWriter.println("cloud time: " + between_cloud);
            printWriter.println();

            def.initMsg();
        }

        tool.sleep(10000);

        libisssr.destroy();
        printWriter.close();
        Log.i(Tag, "测试结束");
    }
}
