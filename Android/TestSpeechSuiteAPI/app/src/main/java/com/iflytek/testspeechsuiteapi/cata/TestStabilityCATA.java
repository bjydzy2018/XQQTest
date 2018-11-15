package com.iflytek.testspeechsuiteapi.cata;

import android.content.Context;

import com.iflytek.cata.CataIndexInst;
import com.iflytek.cata.CataSearchInst;
import com.iflytek.cata.CataSession;
import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Vector;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 批量CATA测试工具
 */

public class TestStabilityCATA {
    private CataSession cataSession;
    private CataIndexInst cataIndexInst;
    private CataSearchInst cataSearchInst;
    private DefCATA def = new DefCATA();
    private Tool tool = new Tool();
    private Random random = new Random();

    private String Tag = "TestStabilityCATA";
    private int err = -1;
    private long runTime; // 运行时间，以秒为单位
    public int ifWriteToActivity = 1;

    public TestStabilityCATA(Context context) {
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

    public TestStabilityCATA(Context context, long runTime) {
        this.runTime = runTime;
        def.isWriteToActivity = 0;
        tool.isWriteToActivity = 0;

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

    /**
     * 循环创建、搜索
     */
    public void test() {
        // CataSearchInst.create
        err = cataSearchInst.create("Test.bin", def.iCataListener);
        HandleRet.handleSearchCreateRet(err);

        for (int i = 0; i < 5000; i++) {
            // CataIndexInst.reCreate
            err = cataIndexInst.reCreate("Test.bin", def.iCataListener);
            HandleRet.handleIndexCreateRet(err);

            err = cataIndexInst.drop();
            HandleRet.handleIndexDropRet(err);

            // CataIndexInst.addIdxEntity
            Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
            for (Vector<SegValue> segValues : segValuesList) {
                ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), ifWriteToActivity);
                err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
                HandleRet.handleIndexAddIdxEntityRet(err);
            }

            // CataIndexInst.endIdxEntity
            err = cataIndexInst.endIdxEntity();
            HandleRet.handleIndexEndIdxEntityRet(err);

            // CataIndexInst.destroy
            err = cataIndexInst.destroy();
            HandleRet.handleIndexDestroyRet(err);

            // CataSearchInst.searchSync
            String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
            ILog.e(Tag, "searchSync ret:" + searchRet, ifWriteToActivity);
            if (searchRet.equals("")) {
                ILog.e(Tag, "搜索结果为空", ifWriteToActivity);
            }
        }
    }

    public void test1() {
        // CataSearchInst.create
        err = cataSearchInst.create("mandarin2py,Test.bin", def.iCataListener);
        HandleRet.handleSearchCreateRet(err);

        List<String[]> keywordsList = new ArrayList<>();
        keywordsList.add(new String[]{"七佰佳蛋糕（高新区店）", "潜水东路26号", "山洼子祥瑞楼",
                "创新大道与望江西路交口北200米,创新公寓A座与B座之间二层,祥源城对面", "皖北地锅",
                "创新大道与潜水东路交口西200米", "卡宾（CABIN）木屋", "枫林路与合六路交口向前1000米大蜀山森林公园入口"});

        keywordsList.add(new String[]{"皖玉香黄焖鸡米饭（高新区西站店）", "百草街海亮九玺M-109",
                "詹姆仕汉堡", "蜀南庭院商业街门面房14栋103（蜀南庭院合家福超市西100米）",
                "蜀锦香厕所串串（蜀南庭院店）", "望江西路蜀南庭苑商业街满天星超市往西50米（蜀南庭苑西街9栋104）",
                "佰家宴", "望江西路蜀南庭苑西区商业街16栋109号（满天星超市往西100米）"});

        keywordsList.add(new String[]{"豪大大鸡排（蜀南庭院店）", "望江西路西路蜀南庭院美食广场一楼",
                "胖嫂面馆", "望江西路蜀南庭院美食广场一楼外商铺（美特好超市旁）",
                "曼果甜品站", "望江西路619号（蜀南庭苑商业街7号楼113商铺满天星超市旁）",
                "老街口重庆小面", "枫林路蜀南庭苑商业街"});

        keywordsList.add(new String[]{"华莱士炸鸡汉堡（蜀南庭院店）", "望江西路与枫林路交汇处蜀南庭院商业街6栋104",
                "N多寿司（蜀南庭苑店）", "望江路蜀南庭苑",
                "采蝶轩（蜀南庭院店）", "望江西路蜀南庭苑广场对面（蜀南庭院商业街内）",
                "动力鸡车", "高新区望江西路蜀南庭苑商业街东街"});

        keywordsList.add(new String[]{"艾比客（柏堰店）", "桃花镇长安路柏堰商业服务中心A3栋11-12号",
                "大脸鸡排（蜀南庭苑店）", "枫林路桃园社区南行50米",
                "速吧百味旋转小火锅（蜀山店）", "大别山路与天龙路交口东南侧搭档网咖旁"});

        String[] searchContent = new String[]{"(field=item,query=七佰佳蛋糕,fuzzy=1,searchType=SR)",
                "(field=item,query=詹姆仕汉堡,fuzzy=1,searchType=SR)",
                "(field=item,query=重庆小面,fuzzy=1,searchType=SR)",
                "(field=item,query=采蝶轩,fuzzy=1,searchType=SR)",
                "(field=item,query=大脸鸡排,fuzzy=1,searchType=SR)"};

        for (int j = 0; j < 200; j++) {
            for (int i = 0; i < 5; i++) {
                // CataIndexInst.reCreate
                err = cataIndexInst.reCreate("Test.bin", def.iCataListener);
                HandleRet.handleIndexCreateRet(err);

                err = cataIndexInst.drop();
                HandleRet.handleIndexDropRet(err);

                for (String key :
                        keywordsList.get(i)) {
                    Vector<SegValue> segValues = new Vector<>();
                    Vector<String> text = new Vector<>();
                    text.add(key);
                    SegValue segValue = new SegValue("item", 1, text);
                    segValues.add(segValue);

                    // CataIndexInst.addIdxEntity
                    err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
                    HandleRet.handleIndexAddIdxEntityRet(err);

                    // CataIndexInst.endIdxEntity
                    err = cataIndexInst.endIdxEntity();
                    HandleRet.handleIndexEndIdxEntityRet(err);
                }

                // CataIndexInst.destroy
                err = cataIndexInst.destroy();
                HandleRet.handleIndexDestroyRet(err);

                // CataSearchInst.searchSync
                String searchRet = cataSearchInst.searchSync(searchContent[i]);
                ILog.e(Tag, "searchSync ret:" + searchRet, ifWriteToActivity);
                if (searchRet.equals("")) {
                    ILog.e(Tag, "搜索结果为空", ifWriteToActivity);
                }
            }
        }
    }

    /**
     * 接口随机调用
     */
    public void testRandom() {
        String segValue = def.segValues2JSON(def.createSegValuesList(def.segValuePath_1, 0).get(0));
        int param;
        int paramValue;
        String text;

        tool.setRunTime(runTime);

        while (true) {
            switch (random.nextInt(7)) {
                // addIdxEntity
                case 0:
                    ILog.i(Tag, "调用addIdxEntity接口", ifWriteToActivity);
                    cataIndexInst.addIdxEntity(segValue);
                    break;
                // delIdxEntity
                case 1:
                    ILog.i(Tag, "调用delIdxEntity接口", ifWriteToActivity);
                    cataIndexInst.delIdxEntity(segValue);
                    break;
                // drop
                case 2:
                    ILog.i(Tag, "调用drop接口", ifWriteToActivity);
                    cataIndexInst.drop();
                    break;
                // endIdxEntity
                case 3:
                    ILog.i(Tag, "调用endIdxEntity接口", ifWriteToActivity);
                    cataIndexInst.endIdxEntity();
                    break;
                // searchAsync
                case 4:
                    text = getRandomSearchText();
                    ILog.i(Tag, "调用searchAsync接口，text：" + text, ifWriteToActivity);
                    cataSearchInst.searchAsync(text);
                    break;
                // searchSync
                case 5:
                    text = getRandomSearchText();
                    ILog.i(Tag, "调用searchSync接口，text：" + text, ifWriteToActivity);
                    cataSearchInst.searchSync(text);
                    break;
                // setParam
                case 6:
                    param = random.nextInt(5);
                    paramValue = random.nextInt(5);
                    ILog.i(Tag, "调用setParam接口，param：" + param + "，paramValue：" + paramValue, ifWriteToActivity);
                    cataSearchInst.setParam(param, paramValue);
                    break;
            }

            // 判断是否结束测试
            if (tool.isTimeUp())
                break;
        }
    }

    /**
     * 接口循环调用
     */
    public void testRollApi() {
        String segValue = def.segValues2JSON(def.createSegValuesList(def.segValuePath_1, 0).get(0));

        for (int i = 0; i < 1; i++) {
            tool.setRunTime(runTime);

//            cataIndexInst.reCreate("123.bin", def.iCataListener);
            cataIndexInst.create("123.bin", def.iCataListener);
//            cataSearchInst.create("Test.bin", def.iCataListener);

            while (true) {
                cataIndexInst.addIdxEntity(segValue);
                cataIndexInst.delIdxEntity(segValue);
                cataIndexInst.endIdxEntity();
//            cataIndexInst.drop();

//            cataIndexInst.reCreate("Test.bin", def.iCataListener);
//            err = cataIndexInst.destroy();

                // 判断是否结束测试
                if (tool.isTimeUp())
                    break;
            }

//            cataIndexInst.endIdxEntity();
//            cataIndexInst.drop();
            err = cataIndexInst.destroy();
        }

        ILog.i(Tag, "destroy return:" + err, ifWriteToActivity);
        ILog.i(Tag, "test over", ifWriteToActivity);
    }

    /**
     * 开启log，在多个线程中构建索引
     */
    public void test_multiThreading1() {
        ExecutorService exe = Executors.newCachedThreadPool();

        for (int i = 0; i < 500; i++) {
            exe.execute(new ThreadCreateIndex(def.createSegValuesList(def.segValuePath_20, 0)));
        }
        exe.shutdown();

        while (!exe.isTerminated())
            tool.sleep(10);
    }

    private class ThreadCreateIndex implements Runnable {
        private Vector<Vector<SegValue>> segValuesList;
        private CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        public ThreadCreateIndex(Vector<Vector<SegValue>> segValuesList) {
            this.segValuesList = segValuesList;
        }

        @Override
        public void run() {
            cataIndexInst.reCreate(Thread.currentThread().getName(), new DefCATA().iCataListener);
            for (Vector<SegValue> segValues : segValuesList) {
                ILog.i(Tag, "adding segValues:" + new DefCATA().getSegValuesContent(segValues), ifWriteToActivity);
                err = cataIndexInst.addIdxEntity(new DefCATA().segValues2JSON(segValues));
            }

            err = cataIndexInst.endIdxEntity();
        }
    }

    class ThreadSearchAsync implements Runnable {
        private String text;

        public ThreadSearchAsync(String text) {
            this.text = text;
        }

        @Override
        public void run() {
            err = cataSearchInst.searchAsync(text);
            HandleRet.handleSearchAsyncRet(err);
        }
    }

    private String getRandomSearchText() {
        String searchText = "";
        switch (random.nextInt(6)) {
            case 0:
                searchText = "(field=name,query=交通台,fuzzy=1,searchType=SR)";
                break;
            case 1:
                searchText = "";
                break;
            case 2:
                searchText = "(field=name,query=交通台,fuzzy=1,searchType=SR";
                break;
            case 3:
                searchText = "(field=name,query=交通台,fuzzy=-3,searchType=SR)";
                break;
            case 4:
                searchText = "(field=name,query=交通台,fuzzy=1,searchType=BHIDSFG)";
                break;
            case 5:
                searchText = "△╲Ⅸ❽‰にオУ┿夊Óū";
                break;
        }
        return searchText;
    }
}

