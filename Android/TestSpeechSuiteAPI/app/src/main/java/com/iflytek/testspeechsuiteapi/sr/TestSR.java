package com.iflytek.testspeechsuiteapi.sr;


import android.content.Context;
import android.os.Environment;
import android.telephony.TelephonyManager;

import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.MainActivity;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.speech.ISSErrors;
import com.iflytek.sr.SrSession;


import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Random;

public class TestSR {
    private Context context;
    private MainActivity mainActivity;
    private SrSession srSession;
    private Tool tool;
    private ToolSR toolSR;
    private DefSR def = new DefSR();

    private String Tag = "TestSR";
    private String imei = "";

    SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault());

    Date endAudioData_time = null;

    private int err = -1;

    public TestSR(MainActivity mainActivity, Context context) {
        this.mainActivity = mainActivity;
        this.context = context;

        tool = new Tool();
        toolSR = new ToolSR();

//        TelephonyManager tm = (TelephonyManager) context
//                .getSystemService(Context.TELEPHONY_SERVICE);
//        imei = tm.getDeviceId();
//        if (imei == null || imei.length() == 0) {
//            imei = "ifly" + Math.random() + Math.random() + Math.random()
//                    + Math.random() + Math.random() + Math.random()
//                    + Math.random() + Math.random() + Math.random()
//                    + Math.random() + "tek";
//        }
        //获取imei，可以设置一个假的
        imei = "ThisIsAFakeIMEI";

        ILog.i(Tag, "IMEI::" + imei);
    }

    //冒烟测试，送入一条音频进行识别
    public void test_temp() {
        int i = 0;
        //识别初始化，def.isrListener是回调函数，会打印回调消息
        SrSession srSession = SrSession.getInstance(context, def.isrListener, def.resDir,
                imei);
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }

        //开始识别start
        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
        tool.sleep(50);
        //送入音频
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        //结束音频送入
        err = srSession.endAudioData();

        //等待结果
        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        ILog.i(Tag, def.srResult);
        def.initMsg();
        srSession.release();
    }

    /**
     * 批量测试识别的同时上传词典。
     * 传入音频列表，传入词典列表
     */
    public void test_manySR_and_uploadDict(String pcmPathList, String dictPathList) {
        String testTag = Tag + ".Test";

        final List<String> dicts = getDictList(dictPathList);

        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        //在该线程里面一直上传词典，死循环
        new Thread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; ; i++) {
                    // SrSession.uploadDict
                    err = srSession.uploadDict(tool.readFile(dicts.get(i % dicts.size())), 0);
                    HandleRet.handleUploadDictRet(err);

                    while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                        tool.sleep(10);
                    }
                    def.msgUploadDictCloud = false;
                    def.msgUploadDictLocal = false;
                }
            }
        }).start();

        //toolSR.start
        for (int i = 0; i < 350; i++) {
            ILog.i("toolSR.start num::", "" + i);
            toolSR.start(srSession, def, pcmPathList, SrSession.ISS_SR_SCENE_ALL, SrSession
                    .ISS_SR_MODE_MIX_REC, null);
        }
        //mainActivity.finish();
    }

    /**
     * 测试上传词典的时间
     */
    public void test_upLoadDictTime(int ifOnlyUploadToCloud, String... dictPaths) {
        String Tag = this.Tag + ".test_upLoadDictTime";

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

        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        tool.sleep(10000);

        for (String dictPath : dictPaths) {
            try {
                start = dfs.parse(dfs.format(new Date()));
            } catch (Exception e) {
                e.printStackTrace();
            }

            err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                    .getAbsolutePath() + dictPath), ifOnlyUploadToCloud);
            HandleRet.handleUploadDictRet(err);

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
            ILog.i(Tag, "the time of uploading dict to cloud:" + between_cloud);
            ILog.i(Tag, "the time of uploading dict to local:" + between_local);

            printWriter.println(dictPath);
            printWriter.println("local time: " + between_local);
            printWriter.println("cloud time: " + between_cloud);
            printWriter.println();

            def.initMsg();
        }

        tool.sleep(10000);

        srSession.release();
        printWriter.close();
        ILog.i(Tag, "测试结束", 1);
    }

    /**
     * 测试多语种识别。
     */
    public void test_createEx() {
        srSession = SrSession.getInstance(context, def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        toolSR.start(srSession, def, def.srPcm_en, SrSession.ISS_SR_SCENE_ALL, SrSession
                .ISS_SR_MODE_MIX_REC, null);

        //结束音频送入
        err = srSession.endAudioData();

        //等待结果
        int i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        ILog.i(Tag, def.srResult);
        def.initMsg();
        srSession.release();
    }

    /**
     * 获取云端语义，不上传词典。
     *
     * @param searchText     需要获取语义的文本
     * @param mspSearchParam 获取云端语义的额外参数
     */
    public void test_mspSearch(String searchText, String mspSearchParam) {
        String Tag = this.Tag + ".test_mspSearch";
        srSession = SrSession.getInstance(context, def.isrListener, "/sdcard/iflytek/res/sr", imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        ILog.i(Tag, srSession.mspSearch(searchText, mspSearchParam));

        def.initMsg();
        srSession.release();
    }

    /**
     * 获取云端语义，上传词典文件。
     *
     * @param searchText          需要获取语义的文本
     * @param mspSearchParam      获取云端语义的额外参数
     * @param dictPath            上传词典的路径，相对于SD卡根目录
     * @param ifOnlyUploadToCloud 0：上传云端+本地，1：上传云端
     */
    public void test_mspSearch(String searchText, String mspSearchParam, String dictPath, int
            ifOnlyUploadToCloud) {
        String Tag = this.Tag + ".test_mspSearch";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + dictPath), ifOnlyUploadToCloud);
        HandleRet.handleUploadDictRet(err);

        // &&: 上传云端  ||: 上传云+端
        if (ifOnlyUploadToCloud == 0) {
            while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        } else {
            while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        }

        ILog.i(Tag, srSession.mspSearch(searchText, mspSearchParam));
        def.initMsg();
        srSession.release();
    }

    /**
     * 获取云端语义，上传词典内容字符串。
     *
     * @param searchText          需要获取语义的文本
     * @param mspSearchParam      获取云端语义的额外参数
     * @param ifOnlyUploadToCloud 0：上传云端+本地，1：上传云端
     * @param dictContent         上传的词典内容字符串
     */
    public void test_mspSearch(String searchText, String mspSearchParam, int ifOnlyUploadToCloud,
                               String dictContent) {
        String Tag = this.Tag + ".test_mspSearch";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        err = srSession.uploadDict(dictContent, ifOnlyUploadToCloud);
        HandleRet.handleUploadDictRet(err);

        // &&: 上传云端  ||: 上传云+端
        if (ifOnlyUploadToCloud == 0) {
            while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        } else {
            while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        }

        ILog.i(Tag, srSession.mspSearch(searchText, mspSearchParam));

        def.initMsg();
        srSession.release();
    }

    /**
     * 获取本地语义，不上传词典
     *
     * @param searchText    用来搜索语义的文本
     * @param localNliParam localNli的额外参数，场景
     */
    public void test_localNli(String searchText, String localNliParam) {
        String Tag = this.Tag + ".test_localNli";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        ILog.i(Tag, srSession.localNli(searchText, localNliParam));

        def.initMsg();
        srSession.release();
    }

    /**
     * 获取本地语义，上传词典文件
     *
     * @param searchText          用来搜索语义的文本
     * @param localNliParam       localNli的额外参数，场景
     * @param dictPath            词典路径
     * @param ifOnlyUploadToCloud 0：上传云端+本地，1：上传云端
     */
    public void test_localNli(String searchText, String localNliParam, String dictPath, int
            ifOnlyUploadToCloud) {
        String Tag = this.Tag + ".test_localNli";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + dictPath), ifOnlyUploadToCloud);
        HandleRet.handleUploadDictRet(err);

        // &&: 上传云端  ||: 上传云+端
        if (ifOnlyUploadToCloud == 0) {
            while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        } else {
            while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        }

        ILog.i(Tag, srSession.localNli(searchText, localNliParam));

        def.initMsg();
        srSession.release();
    }

    /**
     * 获取本地语义，上传词典内容字符串
     *
     * @param searchText          用来搜索语义的文本
     * @param localNliParam       localNli的额外参数，场景
     * @param ifOnlyUploadToCloud 0：上传云端+本地，1：上传云端
     * @param dictContent         词典内容字符串
     */
    public void test_localNli(String searchText, String localNliParam, int ifOnlyUploadToCloud,
                              String dictContent) {
        String Tag = this.Tag + ".test_localNli";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = srSession.uploadDict(dictContent, ifOnlyUploadToCloud);
        HandleRet.handleUploadDictRet(err);

        // &&: 上传云端  ||: 上传云+端
        if (ifOnlyUploadToCloud == 0) {
            while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        } else {
            while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        }

        ILog.i(Tag, srSession.localNli(searchText, localNliParam));

        def.initMsg();
        srSession.release();
    }

    /**
     * 批量获取云端或本地语义，不上传词典。
     *
     * @param mspOrLocal         0：获取云端语义，1：获取本地语义
     * @param param              获取语义时的额外参数
     * @param searchTextListPath 文本标注路径
     * @param searchRetPath      搜索结果路径
     */
    public void test_manyNli(int mspOrLocal, String param, String searchTextListPath, String
            searchRetPath) {
        String path_obj = Environment.getExternalStorageDirectory().getAbsolutePath() +
                searchTextListPath;
        String path_ret = Environment.getExternalStorageDirectory().getAbsolutePath() +
                searchRetPath;

        String Tag = this.Tag + ".test_manyNli";

        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault());
        Date start = null;
        Date end = null;

        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }


        PrintWriter printWriter = null;

        try {
            BufferedReader bReader = new BufferedReader(new FileReader(new File(path_obj)));
            printWriter = new PrintWriter(new FileWriter(path_ret));

            String line = null;
            String ret = null;
            while ((line = bReader.readLine()) != null) {
                try {
                    start = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }

                if (mspOrLocal == 0)
                    ret = srSession.mspSearch(line, param);
                else
                    ret = srSession.localNli(line, param);

                try {
                    end = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }

                long between = end.getTime() - start.getTime();
                ILog.i(Tag, between+", "+ret, 1);

                // 每次获取云端语义需要等待一会儿，否则搜索结果会重复
                if (mspOrLocal == 0)
                    tool.sleep(60);

                printWriter.println(line + "\t" + between + "\t" + ret);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            printWriter.close();
        }
        ILog.i(Tag, "Test over", 1);

        def.initMsg();
        srSession.release();
    }

    /**
     * 批量获取云端或本地语义，上传词典文件。
     *
     * @param mspOrLocal          0：获取云端语义，1：获取本地语义
     * @param param               获取语义时的额外参数
     * @param searchTextListPath  文本标注路径
     * @param searchRetPath       搜索结果路径
     * @param dictPath            上传词典的路径
     * @param ifOnlyUploadToCloud 0：上传词典到云端+本地，1：上传词典到云端
     */
    public void test_manyNli(int mspOrLocal, String param, String searchTextListPath, String
            searchRetPath, String dictPath, int ifOnlyUploadToCloud) {
        String path_obj = Environment.getExternalStorageDirectory().getAbsolutePath() +
                searchTextListPath;
        String path_ret = Environment.getExternalStorageDirectory().getAbsolutePath() +
                searchRetPath;

        String Tag = this.Tag + ".test_manyNli";

        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault());
        Date start = null;
        Date end = null;

        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + dictPath), ifOnlyUploadToCloud);
        HandleRet.handleUploadDictRet(err);

        // &&: 上传云端  ||: 上传云+端
        if (ifOnlyUploadToCloud == 0) {
            while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        } else {
            while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        }

        PrintWriter printWriter = null;

        try {
            BufferedReader bReader = new BufferedReader(new FileReader(new File(path_obj)));
            printWriter = new PrintWriter(new FileWriter(path_ret));

            String line = null;
            String ret = null;
            String last_ret = "";

            while ((line = bReader.readLine()) != null) {
                int loopNum = 0;

                while (true) {
                    start = dfs.parse(dfs.format(new Date()));

                    if (mspOrLocal == 0)
                        ret = srSession.mspSearch(line, param);
                    else
                        ret = srSession.localNli(line, param);

                    end = dfs.parse(dfs.format(new Date()));

                    if (!ret.equals(last_ret)) {
                        break;
                    } else {
                        if (loopNum++ < 10) {
                            System.out.println("loop:" + loopNum);
                            tool.sleep(20);
                        } else
                            break;
                    }
                }
                last_ret = ret;

                long between = end.getTime() - start.getTime();
                ILog.i(Tag, between + "," + ret);

                printWriter.println(line + "\t" + between + "\t" + ret);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            printWriter.close();
        }

        def.initMsg();
        srSession.release();
    }

    /**
     * 批量获取云端或本地语义，上传词典内容字符串。
     *
     * @param mspOrLocal          0：获取云端语义，1：获取本地语义
     * @param param               获取云端语义时的额外参数
     * @param searchTextListPath  文本标注路径
     * @param searchRetPath       搜索结果路径
     * @param ifOnlyUploadToCloud 0：上传词典到云端+本地，1：上传词典到云端
     * @param dictContent         上传的词典内容字符串
     */
    public void test_manyNli(int mspOrLocal, String param, String searchTextListPath, String
            searchRetPath, int ifOnlyUploadToCloud, String dictContent) {
        String path_obj = Environment.getExternalStorageDirectory().getAbsolutePath() +
                searchTextListPath;
        String path_ret = Environment.getExternalStorageDirectory().getAbsolutePath() +
                searchRetPath;

        String Tag = this.Tag + ".test_manyNli";

        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault());
        Date start = null;
        Date end = null;

        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = srSession.uploadDict(dictContent, ifOnlyUploadToCloud);
        HandleRet.handleUploadDictRet(err);

        // &&: 上传云端  ||: 上传云+端
        if (ifOnlyUploadToCloud == 0) {
            while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        } else {
            while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        }

        PrintWriter printWriter = null;

        try {
            BufferedReader bReader = new BufferedReader(new FileReader(new File(path_obj)));
            printWriter = new PrintWriter(new FileWriter(path_ret));

            String line = null;
            String ret = null;

            while ((line = bReader.readLine()) != null) {
                try {
                    start = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }
                if (mspOrLocal == 0)
                    ret = srSession.mspSearch(line, param);
                else
                    ret = srSession.localNli(line, param);
                try {
                    end = dfs.parse(dfs.format(new Date()));
                } catch (Exception e) {
                    e.printStackTrace();
                }

                long between = end.getTime() - start.getTime();
                ILog.i(Tag, between + "," + ret);

                // 每次获取云端语义需要等待一会儿，否则搜索结果会重复
                if (mspOrLocal == 0)
                    tool.sleep(60);

                printWriter.println(line + "\t" + between + "\t" + ret);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            printWriter.close();
        }

        def.initMsg();
        srSession.release();
    }

    /**
     * 识别一条音频，不上传词典。
     *
     * @param pcmPath 音频路径
     * @param scene   识别场景
     * @param mode    识别模式
     * @param szCmd   命令词
     */
    public void test_sr(String pcmPath, String scene, int mode, String szCmd) {
        String Tag = this.Tag + ".test_sr";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        toolSR.start(srSession, def, Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/" + pcmPath, scene, mode, szCmd);
    }

    /**
     * 识别一条音频，上传词典文件。
     *
     * @param pcmPath             音频路径
     * @param scene               识别场景
     * @param mode                识别模式
     * @param szCmd               命令词
     * @param dictPath            词典路径
     * @param ifOnlyUploadToCloud 0：上传云端+本地，1：上传云端
     */
    public void test_sr(String pcmPath, String scene, int mode, String szCmd, String dictPath,
                        int ifOnlyUploadToCloud) {
        String Tag = this.Tag + ".test_sr";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + dictPath), ifOnlyUploadToCloud);
        HandleRet.handleUploadDictRet(err);

        // &&: 上传云端  ||: 上传云+端
        if (ifOnlyUploadToCloud == 0) {
            while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        } else {
            while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        }

        toolSR.start(srSession, def, Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/" + pcmPath, scene, mode, szCmd);

        def.initMsg();
        srSession.release();
    }

    /**
     * 识别一条音频，上传词典内容字符串。
     *
     * @param pcmPath             音频路径
     * @param scene               识别场景
     * @param mode                识别模式
     * @param szCmd               命令词
     * @param ifOnlyUploadToCloud 0：上传云端+本地，1：上传云端
     * @param dictContent         词典内容字符串
     */
    public void test_sr(String pcmPath, String scene, int mode, String szCmd, int
            ifOnlyUploadToCloud, String dictContent) {
        String Tag = this.Tag + ".test_sr";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = srSession.uploadDict(dictContent, ifOnlyUploadToCloud);
        HandleRet.handleUploadDictRet(err);

        // &&: 上传云端  ||: 上传云+端
        if (ifOnlyUploadToCloud == 0) {
            while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        } else {
            while (!def.msgUploadDictCloud && !def.msgUploadDictLocal) {
                tool.sleep(10);
            }
        }

        toolSR.start(srSession, def, Environment.getExternalStorageDirectory().getAbsolutePath()
                + "/" + pcmPath, scene, mode, szCmd);
        def.initMsg();
        srSession.release();
    }

    public void test_ResourceBreak() {
        String Tag = this.Tag + ".test_ResourceBreak";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        toolSR.start(srSession, def, def.srPcm_confirm, SrSession.ISS_SR_SCENE_ALL, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);

        def.initMsg();
        srSession.release();
    }


    /**
     * 测试setparam接口设置responsetimeou效果。
     */
    public void test_setParam_ResponseTimeout() {
        String testTag = Tag + ".test_setParam_ResponseTimeout";

        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        err = srSession.setParam(SrSession.ISS_SR_PARAM_TRACE_LEVEL, SrSession
                .ISS_SR_PARAM_TRACE_LEVEL_VALUE_DEBUG);
        HandleRet.handleSetParamRet(err);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "3000");
        HandleRet.handleSetParamRet(err);

        toolSR.start(srSession, def, def.srPcm_responseTimeOut, SrSession.ISS_SR_SCENE_ALL,
                SrSession.ISS_SR_MODE_MIX_REC, null);

        def.initMsg();
        srSession.release();

    }

    /**
     * 存在这一个场景，开启识别后，第一次会送入1-2s的音频，然后再正常取录音送入，发现VAD后端点检测和本地识别响应时间变长。
     */
    public void test_first_large_pcm() {
        String Tag = this.Tag + ".test_first_large_pcm";
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, imei);
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        long time_sum = 0;
        int test_time = 0;

        int first_send = 48000;
        int common_send = 1600;
        int flag = 0;  // 控制第一次是否送大音频; 0 送   1 不送
        String pcm_name = "HuShiCiShen.pcm";
        String cmd = "{\n" +
                "    \"grm\": [\n" +
                "    {\n" +
                "        \"dictname\": \"cmd\",\n" +
                "        \"dictcontant\": [\n" +
                "           { \"name\": \"糊世刺身（夫子庙店）\", \"id\": 0 },\n" +
                "           { \"name\": \"慧园街6—5号（好又多超市旁）\", \"id\": 1 }\n" +
                "           ]\n" +
                "    }\n" +
                "    ]\n" +
                "}";
        ILog.i(Tag, cmd);

        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(new FileWriter(new File(Environment
                    .getExternalStorageDirectory().getAbsolutePath() +
                    "/TestRes/sr/first_large_pcm/" + pcm_name + "_" + flag + "_" + (flag == 0
                    ? first_send : common_send) + ".txt")));
        } catch (Exception e) {
            e.printStackTrace();
        }

        for (int i = 0; i < 100; i++) {
            err = srSession.start(SrSession.ISS_SR_SCENE_SELECT, SrSession.ISS_SR_MODE_LOCAL_REC,
                    cmd);
            HandleRet.handlStartRet(err);

            try {
                DataInputStream data = new DataInputStream(new FileInputStream(Environment
                        .getExternalStorageDirectory().getAbsolutePath()
                        + "/TestRes/pcm_src/sr/" + pcm_name));
                byte[] byteData;
                byteData = new byte[data.available()];
                data.read(byteData);
                data.close();

                int location = 0;
                int position = byteData.length;

                while (location < byteData.length && location < position) {
                    int end1 = byteData.length - location;
                    int end2 = position - location;
                    int end = 0;
                    if (end1 >= end2)
                        end = end2;
                    else {
                        end = end1;
                    }

                    if (flag == 0) {
                        byte[] mid = new byte[first_send];
                        if (end > first_send) {
                            System.arraycopy(byteData, location, mid, 0, first_send);
                        } else {
                            System.arraycopy(byteData, location, mid, 0, end);
                        }
                        err = srSession.appendAudioData(mid);

                    } else {
                        byte[] mid = new byte[common_send];
                        if (end > common_send) {
                            System.arraycopy(byteData, location, mid, 0, common_send);
                        } else {
                            System.arraycopy(byteData, location, mid, 0, end);
                        }
                        err = srSession.appendAudioData(mid);
                    }
                    switch (err) {
                        case ISSErrors.ISS_SUCCESS:
//                            ILog.i("SrSession.appendAudio", "ISS_SUCCESS", 1);
                            break;
                        case ISSErrors.ISS_ERROR_INVALID_PARA:
                            ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_PARA", 1);
                            break;
                        case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                            ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_HANDLE", 1);
                            break;
                        case ISSErrors.ISS_ERROR_INVALID_CALL:
                            ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_CALL", 1);
                            break;
                        case ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER:
                            ILog.e("SrSession.appendAudio", "ISS_ERROR_NO_ENOUGH_BUFFER", 1);
                            break;
                        case ISSErrors.INVALID_SESSION:
                            ILog.e("SrSession.appendAudio", "INVALID_SESSION", 1);
                            break;
                        case ISSErrors.REMOTE_EXCEPTION:
                            ILog.e("SrSession.appendAudio", "REMOTE_EXCEPTION", 1);
                            break;
                        default:
                            ILog.e("SrSession.appendAudio", "UnHandled MSG", 1);
                    }

                    Thread.sleep(50);
                    if (flag == 0) {
                        flag = 1;
                        location = location + first_send;
                    } else {
                        location = location + common_send;
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            srSession.endAudioData();
            try {
                endAudioData_time = dfs.parse(dfs.format(new Date()));
            } catch (Exception e) {
                e.printStackTrace();
            }
            ILog.i(Tag, "call endAudioData");

            while (!def.msgResult) {
                tool.sleep(10);
            }

            long time_resume = def.result_time.getTime() - Math.min(endAudioData_time.getTime(),
                    def.speechend_time);
            time_sum += time_resume;
            test_time++;
            printWriter.println(time_resume);

            ILog.i(Tag, "SpeechEndTime: " + def.speechend_time + ", EndAudioDataTime: "
                    + endAudioData_time.getTime() + ", GetResultTime: " + def.result_time.getTime
                    ());
            ILog.i(Tag, "识别结果响应时间：" + time_resume + "ms");
            def.msgResult = false;
        }

        ILog.i(Tag, "平均识别响应时间：" + (time_sum / test_time));
        printWriter.println("\n平均识别响应时间：" + (time_sum / test_time));
        printWriter.close();

    }

    private List<String> getDictList(String dictPathList) {
        List<String> dicts = new ArrayList<>();

        try {
            BufferedReader bReader = new BufferedReader(new FileReader(dictPathList));
            String line = null;
            while ((line = bReader.readLine()) != null) {
                dicts.add(Environment.getExternalStorageDirectory().getAbsolutePath() + line);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return dicts;
    }
}

