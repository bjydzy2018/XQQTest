package com.iflytek.testspeechsuiteapi.cata;


import android.content.Context;
import android.os.Environment;

import com.iflytek.cata.CataIndexInst;
import com.iflytek.cata.CataSearchInst;
import com.iflytek.cata.CataSession;
import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
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
import java.util.regex.*;

import static junit.framework.Assert.assertEquals;

public class TestCATA {
    private CataSession cataSession;
    private CataIndexInst cataIndexInst;
    private CataSearchInst cataSearchInst;
    private DefCATA def = new DefCATA();

    private Context context;
    private String Tag = "TestCATA";
    private Tool tool = new Tool();

    private String segValuePath = def.segValuePath;
    private String segValueDelPath = def.segValueDelPath;
    private int err = -1;

    public TestCATA(Context context) {
        this.context = context;

        cataSession = CataSession.getInstance(context, def.iCataInitListener, def.resDir);
        while (!def.msgInitStatus) {
            try {
                Thread.sleep(10);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        def.msgInitStatus = false;

        cataIndexInst = cataSession.createCataIndexInst();
        cataSearchInst = cataSession.createCataSearchInst();
    }

    public void test_temp() {
        String smokeTestTag = Tag + ".smokeTest";

        // CataIndexInst.create
        err = cataIndexInst.create("Test.bin", def.iCataListener);
        HandleRet.handleIndexCreateRet(err);

        err = cataIndexInst.addIdxEntity("[\n" +
                " {\n" +
                " \"field\" : \"name\",\n" +
                " \"mode\" : 1,\n" +
                " \"val\" : " +
                "[\"收到收到给收到了卡松地收到了卡松地收到了卡松地收到了卡松地收到了卡松地噶我跟那说的话否否啊我那个跟那说的话否啊我那个啊送牌啊山东顾问西南风狗啊啊送牌否啊我那个跟那说的话否啊我那山东顾问西南风狗啊啊送牌啊山东顾问西南风狗啊二搜嘎结手动狗啊多少呢服务额啊农夫大概\" ]\n" +
                " }\n" +
                "]");

        // CataIndexInst.endIdxEntity
        err = cataIndexInst.endIdxEntity();
        HandleRet.handleIndexEndIdxEntityRet(err);

        // CataSearchInst.create
        err = cataSearchInst.create("Test.bin", def.iCataListener);
        HandleRet.handleSearchCreateRet(err);

        // CataSearchInst.searchSync
        String searchRet = cataSearchInst.searchSync("(field=name,query=卡松地,fuzzy=1,searchType=SR)");
        ILog.i(smokeTestTag, "searchSync ret:" + searchRet, 1);

        // CataSearchInst.searchAsync
        err = cataSearchInst.searchAsync("(field=name,query=经济台, fuzzy=1,searchType=SR)");
        HandleRet.handleSearchAsyncRet(err);
        while (!def.msgResult) {
            try {
                Thread.sleep(10);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        def.msgResult = false;

        // CataIndexInst.delIdxEntity
        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        HandleRet.handleIndexDelIdxEntityRet(err);

        // CataIndexInst.endIdxEntity
        err = cataIndexInst.endIdxEntity();
        HandleRet.handleIndexEndIdxEntityRet(err);

        // 再搜索一次确定是否删除的索引无法被搜索到了
        ILog.i(smokeTestTag, "searchSync ret:" +
                cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)"), 1);

        // CataSearchInst.destroy
        err = cataSearchInst.destroy();
        HandleRet.handleSearchDestroyRet(err);

        // CataIndexInst.drop
        err = cataIndexInst.drop();
        HandleRet.handleIndexDropRet(err);

        // CataIndexInst.destroy
        err = cataIndexInst.destroy();
        HandleRet.handleIndexDestroyRet(err);
    }

    public void smokeTest() {
        String smokeTestTag = Tag + ".smokeTest";

        // CataIndexInst.create
        err = cataIndexInst.create("Test.bin", def.iCataListener);
        HandleRet.handleIndexCreateRet(err);

        // CataIndexInst.addIdxEntity
        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            ILog.i(smokeTestTag, "adding segValues:" + def.segValues2JSON(segValues), 1);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            HandleRet.handleIndexAddIdxEntityRet(err);
        }

        // CataIndexInst.endIdxEntity
        err = cataIndexInst.endIdxEntity();
        HandleRet.handleIndexEndIdxEntityRet(err);

        // CataSearchInst.create
        err = cataSearchInst.create("Test.bin", def.iCataListener);
        HandleRet.handleSearchCreateRet(err);

        // CataSearchInst.searchSync
        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        ILog.i(smokeTestTag, "searchSync ret:" + searchRet, 1);

        // CataSearchInst.searchAsync
        err = cataSearchInst.searchAsync("(field=name,query=经济台, fuzzy=1,searchType=SR)");
        HandleRet.handleSearchAsyncRet(err);
        while (!def.msgResult) {
            try {
                Thread.sleep(10);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        def.msgResult = false;

        // CataIndexInst.delIdxEntity
        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        HandleRet.handleIndexDelIdxEntityRet(err);

        // CataIndexInst.endIdxEntity
        err = cataIndexInst.endIdxEntity();
        HandleRet.handleIndexEndIdxEntityRet(err);

        // 再搜索一次确定是否删除的索引无法被搜索到了
        ILog.i(smokeTestTag, "searchSync ret:" +
                cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)"), 1);

        // CataSearchInst.destroy
        err = cataSearchInst.destroy();
        HandleRet.handleSearchDestroyRet(err);

        // CataIndexInst.drop
        err = cataIndexInst.drop();
        HandleRet.handleIndexDropRet(err);

        // CataIndexInst.destroy
        err = cataIndexInst.destroy();
        HandleRet.handleIndexDestroyRet(err);
    }

    /**
     * 测试CataIndex两遍create是否崩溃。
     */
    public void test_create2() {
        // CataIndexInst.create
        err = cataIndexInst.create("Test1.bin", def.iCataListener);
        HandleRet.handleIndexCreateRet(err);

        // CataIndexInst.addIdxEntity
        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            ILog.i("test_create2", "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            HandleRet.handleIndexAddIdxEntityRet(err);
        }

        // CataIndexInst.endIdxEntity
        err = cataIndexInst.endIdxEntity();
        HandleRet.handleIndexEndIdxEntityRet(err);

        // CataIndexInst.create
        err = cataIndexInst.create("Test1.bin", def.iCataListener);
        HandleRet.handleIndexCreateRet(err);
    }

    /**
     * 测试英文功能。
     */
    public void test_ex() {
        String Tag = this.Tag + ".test_ex";

        // CataIndexInst.create
        err = cataIndexInst.createEx("TestEX.bin", CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        HandleRet.handleIndexCreateRet(err);

        // CataIndexInst.addIdxEntity
        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            HandleRet.handleIndexAddIdxEntityRet(err);
        }

        // CataIndexInst.endIdxEntity
        err = cataIndexInst.endIdxEntity();
        HandleRet.handleIndexEndIdxEntityRet(err);

        // CataSearchInst.create
        err = cataSearchInst.createEx("TestEX.bin", CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        HandleRet.handleSearchCreateRet(err);

        // CataSearchInst.searchSync
        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1,searchType=SR)");
        ILog.i(Tag, "searchSync ret:" + searchRet);

        // CataSearchInst.searchAsync
        err = cataSearchInst.searchAsync("(field=code,query=90.8,fuzzy=1,searchType=SR)");
        HandleRet.handleSearchAsyncRet(err);
        while (!def.msgResult) {
            try {
                Thread.sleep(10);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        def.msgResult = false;

        // CataIndexInst.delIdxEntity
        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        HandleRet.handleIndexDelIdxEntityRet(err);

        // CataIndexInst.endIdxEntity
        err = cataIndexInst.endIdxEntity();
        HandleRet.handleIndexEndIdxEntityRet(err);
        // 再搜索一次确定是否删除的索引无法被搜索到了
        ILog.i(Tag, "after del, searchSync ret:" +
                cataSearchInst.searchSync("(field=name,query=traffic,fuzzy=1,searchType=SR)"));
        ILog.i(Tag, "after del, searchSync ret:" +
                cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)"));

        // CataSearchInst.destroy
        err = cataSearchInst.destroy();
        HandleRet.handleSearchDestroyRet(err);

        // CataIndexInst.drop
        err = cataIndexInst.drop();
        HandleRet.handleIndexDropRet(err);

        // CataIndexInst.destroy
        err = cataIndexInst.destroy();
        HandleRet.handleIndexDestroyRet(err);
    }

    /**
     * 测试一次搜索。
     *
     * @param searchRsc  搜索的对象资源
     * @param searchText 搜索字符串
     */
    public void test_search(String searchRsc, String searchText) {
        String smokeTestTag = Tag + ".test_search";

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_temp, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            if (err != 0)
                ILog.e(Tag, "adding segValues " + def.getSegValuesContent(segValues) + " err: " + err);
        }

        err = cataIndexInst.endIdxEntity();

        err = cataSearchInst.create(searchRsc, def.iCataListener);
        HandleRet.handleSearchCreateRet(err);

        cataSearchInst.setParam(1, 1);

        String searchRet = cataSearchInst.searchSync(searchText);
        ILog.i(smokeTestTag, "searching:" + searchRet);
    }

    /**
     * 一致性测试。
     *
     * @param isAddIndex 是否创建搜索资源
     * @param isNli      是否过语义
     */
    public void test_consistent(boolean isAddIndex, boolean isNli) {
        String smokeTestTag = Tag + ".test_consistent";
        String test_consistent_segvalue_path = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/test.hcliu/cata/music10000.txt";

        if (isAddIndex) {
            PrintWriter printWriter = null;
            try {
                printWriter = new PrintWriter(new FileWriter(new File(Environment
                        .getExternalStorageDirectory()
                        .getAbsolutePath() + "/test.hcliu/cata/time/addindex.csv")));
            } catch (IOException e) {
                e.printStackTrace();
            }
            SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                    .getDefault());
            Date start = null;
            Date end = null;

            printWriter.println("index_time,err_add_del,text");
            err = cataIndexInst.reCreate("test_consistent.bin", def.iCataListener);
            HandleRet.handleIndexCreateRet(err);

            int i = 0;
            Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(test_consistent_segvalue_path, 10000, 2);
            ILog.i(Tag, "共" + segValuesList.size() + "条记录");
            for (Vector<SegValue> segValues : segValuesList) {
                i++;
                ILog.i(smokeTestTag, "adding segValues:" + i + "," + def.getSegValuesContent(segValues), 0);

                try {
                    start = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }

                err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
//                HandleRet.handleIndexAddIdxEntityRet(err);

                try {
                    end = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }

                long t = end.getTime() - start.getTime();
                printWriter.println(t + "," + err);
            }

            err = cataIndexInst.endIdxEntity();
            HandleRet.handleIndexEndIdxEntityRet(err);
        }

        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(new FileWriter(new File(Environment
                    .getExternalStorageDirectory()
                    .getAbsolutePath() + "/test.hcliu/cata/time/search.csv")));
        } catch (IOException e) {
            e.printStackTrace();
        }
        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());
        Date start = null;
        Date end = null;
        printWriter.println("loadRes_time,srh_time,text");

        String label = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/test.hcliu/cata/consistent/navi_sr_left.txt";
        String ret = Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/test.hcliu/cata/consistent/ret.txt";

        if (isNli) {
            // 过语义
            err = cataSearchInst.setParam(1, 1);
            if (err != ISSErrors.ISS_SUCCESS) {
                ILog.e(smokeTestTag, "setParam error:" + err);
            }
        }

        BufferedReader bReader = null;
//        PrintWriter printWriter = null;
        try {
            bReader = new BufferedReader(new FileReader(label));
//            printWriter = new PrintWriter(new FileWriter(ret));
            String line = null;

            Pattern pattern = Pattern.compile("\\(.*\\)");
            while ((line = bReader.readLine()) != null) {
                try {
                    start = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }
                err = cataSearchInst.create("test_consistent.bin,alias,yellow_page,pinyin_fuzzy", def.iCataListener);
                try {
                    end = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }

                long load_time = end.getTime() - start.getTime();
//                HandleRet.handleSearchCreateRet(err);

                Matcher matcher = pattern.matcher(line);
                if (matcher.find()) {
                    String matchRet = matcher.group();
                    try {
                        start = dfs.parse(dfs.format(new Date()));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    String searchRet = cataSearchInst.searchSync(matchRet);

                    try {
                        end = dfs.parse(dfs.format(new Date()));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    long srh_time = end.getTime() - start.getTime();
                    printWriter.println(load_time + "," + srh_time);
//                    ILog.i(smokeTestTag, "searching:" + matchRet);
//                    ILog.i(smokeTestTag, "searchSync ret:" + searchRet);

//                    printWriter.println(line);
//                    printWriter.println(searchRet);
//                    printWriter.flush();
                }

                cataSearchInst.destroy();
            }
            printWriter.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
//            printWriter.close();
        }
    }

    public void test_ResBreakIndex() {
        err = cataIndexInst.reCreate("Test.bin", def.iCataListener);
        HandleRet.handleIndexCreateRet(err);

        // CataIndexInst.addIdxEntity
        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            ILog.i("test_ResBreakIndex", "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            HandleRet.handleIndexAddIdxEntityRet(err);
        }

        // CataIndexInst.endIdxEntity
        err = cataIndexInst.endIdxEntity();
        HandleRet.handleIndexEndIdxEntityRet(err);
    }

    public void test_ResBreakSearch() {
        err = cataSearchInst.create("Test.bin,alias,yellow_page", def.iCataListener);
        HandleRet.handleSearchCreateRet(err);

        // CataSearchInst.searchSync
        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        ILog.i("test_ResBreakSearch", "searchSync ret:" + searchRet);
    }
}
