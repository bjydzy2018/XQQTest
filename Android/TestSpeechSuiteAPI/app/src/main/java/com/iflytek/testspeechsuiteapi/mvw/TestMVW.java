package com.iflytek.testspeechsuiteapi.mvw;

import android.content.Context;
import android.os.Environment;

import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.mvw.*;
import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.speech.ISSErrors;

import org.json.JSONException;
import org.json.JSONObject;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import static junit.framework.Assert.assertEquals;

public class TestMVW {

    private MvwSession mvwSession;
    private Context context;  // MainActivity的context
    private Tool tool;
    DefMVW def = new DefMVW();

    private String Tag = "TestMVW";

    private String resDir = def.resDir;  // 唤醒资源路径

    int err = -1;

    public TestMVW(Context context) {
        this.context = context;
        tool = new Tool();
    }

    public void test_temp() {
        final MvwSession mvwSession = new MvwSession(context, def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true) {
                    mvwSession.setParam("mvw_threshold_level", "-2.4");
                    mvwSession.start(1);

                    mvwSession.stop();
                }
            }
        }).start();
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true) {
                    mvwSession.setParam("mvw_threshold_level", "1");
                    mvwSession.start(4);

                    mvwSession.stop();
                }
            }
        }).start();
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true) {
                    mvwSession.setParam("mvw_threshold_level", "0");
                    mvwSession.start(2);

                    mvwSession.stop();
                }
            }
        }).start();
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true) {
                    mvwSession.setParam("mvw_threshold_level", "3");
                    mvwSession.start(8);

                    mvwSession.stop();
                }
            }
        }).start();

        while(true)
            tool.sleep(10);

//        mvwSession.release();
    }

    /**
     * 测试一条音频的唤醒。
     *
     * @param scene 唤醒场景
     * @param path  音频路径
     */
    public void test_mvw(int scene, String path) {
        ILog.i(Tag, def.resDir);
        mvwSession = new MvwSession(context, def.iMvwListener, resDir);
        ILog.i(Tag, "实例化成功");

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

//        mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);

        // start
        err = mvwSession.start(scene);
        HandleRet.handleStartRet(err);

        // appendAudioData
        tool.loadPcmAndAppendAudioData(mvwSession, Environment.getExternalStorageDirectory()
                .getAbsolutePath() + path, def);

        mvwSession.stop();

        err = mvwSession.release();
        ILog.i(Tag, "Test over");
    }

    /**
     * 测试唤醒响应时间.
     *
     * @param scene   唤醒场景
     * @param pcmPath 唤醒音频地址，相对于sd卡根目录
     * @return 唤醒响应时间
     */
    public long testVwTime(int scene, String pcmPath) {
        Date startAppendAudioDataDate = null;
        long vadEndTime = 2000;
        long ret = 0;
        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());

        mvwSession = new MvwSession(context, def.iMvwListener, resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        mvwSession.start(scene);

        try {
            startAppendAudioDataDate = dfs.parse(dfs.format(new Date()));
        } catch (ParseException e) {
            e.printStackTrace();
        }

        tool.loadPcmAndAppendAudioData(mvwSession, Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/" + pcmPath, def);

        ret = def.vwWakeupInfo.getVwDate().getTime() - startAppendAudioDataDate.getTime() -
                vadEndTime;
        System.out.println("唤醒响应时间：" + ret);

        mvwSession.stop();
        mvwSession.release();

        return ret;
    }

    /**
     * 测试唤醒多实例功能。
     */
    public void test_multiInstance() {
        MvwSession mvwSession = new MvwSession(context, def.iMvwListener, def.resDir);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        MvwSession mvwSession1 = new MvwSession(context, def.iMvwListener, def.resDir_second);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        err = mvwSession.start(1);

        err = mvwSession1.start(10);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_answer, def);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_answer, def);
    }

    /**
     * 验证芯片加密功能过
     * <p>
     * 现在会在初始化时进行初始化，使用非芯片加密会初始化失败。
     */
    public void testChipEn() {
        mvwSession = new MvwSession(context, def.iMvwListener, resDir);
        ILog.i(Tag, "实例化成功");

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }

        // start
        err = mvwSession.start(1);
        HandleRet.handleStartRet(err);
    }

}

