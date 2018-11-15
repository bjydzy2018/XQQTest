package com.iflytek.testspeechsuitenative.cata;


import android.os.Environment;
import android.util.Log;

import com.iflytek.cata.libisscata;
import com.iflytek.speech.ISSErrors;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TestCATABatch extends TestCATA {
    private final String Tag = "TestCATABatch";

    public void test_consistent(boolean isAddIndex, boolean isNli) {
        String test_consistent_segvalue_path = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/TestRes/cata/music10000.txt";

        if (isAddIndex) {
            libisscata.IndexCreate(mNativeHandle, def.resDir, "test_consistent.bin", 1, def.iCataListener);
            Log.i(Tag, "IndexReCreate return " + mNativeHandle.err_ret);

            int i = 0;
            Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(test_consistent_segvalue_path, 10000, 2);
            Log.i(Tag, "共" + segValuesList.size() + "条记录");
            for (Vector<SegValue> segValues : segValuesList) {
                i++;
                Log.i(Tag, "adding segValues:" + i + "," + def.getSegValuesContent(segValues));

                libisscata.IndexAddIdxEntity(mNativeHandle, def.segValues2JSON(segValues));
                Log.i(Tag, "IndexAddIdxEntity return " + mNativeHandle.err_ret);
            }

            libisscata.IndexEndIdxEntity(mNativeHandle);
            Log.i(Tag, "IndexEndIdxEntity return " + mNativeHandle.err_ret);
        }

        String label = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/TestRes/cata/consistent/navi_sr_left.txt";
        String ret = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/TestRes/cata/consistent/ret.txt";

        if (isNli) {
            // 过语义
            libisscata.SetParam(mNativeHandle, 1, 1);
            if (mNativeHandle.err_ret != ISSErrors.ISS_SUCCESS) {
                Log.e(Tag, "setParam error:" + mNativeHandle.err_ret);
            }
        }

        BufferedReader bReader = null;
        PrintWriter printWriter = null;
        try {
            bReader = new BufferedReader(new FileReader(label));
            printWriter = new PrintWriter(new FileWriter(ret));
            String line = null;

            Pattern pattern = Pattern.compile("\\(.*\\)");
            while ((line = bReader.readLine()) != null) {
                libisscata.SearchCreate(mNativeHandle, def.resDir, "test_consistent.bin,alias,yellow_page,pinyin_fuzzy", def.iCataListener);

                Matcher matcher = pattern.matcher(line);
                if (matcher.find()) {
                    String matchRet = matcher.group();

                    String searchRet = libisscata.SearchSync(mNativeHandle, matchRet);

                    Log.i(Tag, "searching:" + matchRet);
                    Log.i(Tag, "searchSync ret:" + searchRet);

                    printWriter.println(line);
                    printWriter.println(searchRet);
                    printWriter.flush();
                }

                libisscata.SearchDestroy(mNativeHandle);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            printWriter.close();
        }
    }

    public void test_time() {
        String test_consistent_segvalue_path = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/TestRes/cata/music10000.txt";
        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());
        PrintWriter indesPrintWriter = null;

        try {
            indesPrintWriter = new PrintWriter(new FileWriter(new File(Environment
                    .getExternalStorageDirectory()
                    .getAbsolutePath() + "/TestRes/cata/time/addindex.csv")));
        } catch (IOException e) {
            e.printStackTrace();
        }

        Date start = null;
        Date end = null;

        indesPrintWriter.println("index_time,err_add_del,text");
        libisscata.IndexCreate(mNativeHandle, def.resDir, "test_consistent.bin", 1, def.iCataListener);
        Log.i(Tag, "IndexReCreate return " + mNativeHandle.err_ret);

        int i = 0;
        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(test_consistent_segvalue_path, 10000, 2);
        Log.i(Tag, "共" + segValuesList.size() + "条记录");
        for (Vector<SegValue> segValues : segValuesList) {
            i++;
            Log.i(Tag, "adding segValues:" + i + "," + def.getSegValuesContent(segValues));

            try {
                start = dfs.parse(dfs.format(new Date()));
            } catch (Exception e) {
                e.printStackTrace();
            }

            libisscata.IndexAddIdxEntity(mNativeHandle, def.segValues2JSON(segValues));
            Log.i(Tag, "IndexAddIdxEntity return " + mNativeHandle.err_ret);

            try {
                end = dfs.parse(dfs.format(new Date()));
            } catch (Exception e) {
                e.printStackTrace();
            }

            long t = end.getTime() - start.getTime();
            indesPrintWriter.println(t + "," + mNativeHandle.err_ret);
        }

        libisscata.IndexEndIdxEntity(mNativeHandle);
        Log.i(Tag, "IndexEndIdxEntity return " + mNativeHandle.err_ret);
        libisscata.IndexDestroy(mNativeHandle);

        tool.sleep(100000);

        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(new FileWriter(new File(Environment
                    .getExternalStorageDirectory()
                    .getAbsolutePath() + "/TestRes/cata/time/search.csv")));
        } catch (IOException e) {
            e.printStackTrace();
        }
        Date searchStart = null;
        Date searchEnd = null;
        printWriter.println("loadRes_time,srh_time,text");

        String label = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/TestRes/cata/consistent/navi_sr_left.txt";

        BufferedReader bReader = null;
        try {
            bReader = new BufferedReader(new FileReader(label));
            String line = null;

            Pattern pattern = Pattern.compile("\\(.*\\)");
            while ((line = bReader.readLine()) != null) {
                try {
                    searchStart = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }
                libisscata.SearchCreate(mNativeHandle, def.resDir, "test_consistent.bin,alias,yellow_page,pinyin_fuzzy", def.iCataListener);
                try {
                    searchEnd = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }

                long load_time = searchEnd.getTime() - searchStart.getTime();

                Matcher matcher = pattern.matcher(line);
                if (matcher.find()) {
                    String matchRet = matcher.group();
                    try {
                        searchStart = dfs.parse(dfs.format(new Date()));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    String searchRet = libisscata.SearchSync(mNativeHandle, matchRet);

                    try {
                        searchEnd = dfs.parse(dfs.format(new Date()));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    long srh_time = searchEnd.getTime() - searchStart.getTime();
                    printWriter.println(load_time + "," + srh_time);
                }

                libisscata.SearchDestroy(mNativeHandle);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            printWriter.close();
            indesPrintWriter.close();
            Log.i("Tag", "CATA响应时间测试结束");
        }
    }
}

