package com.iflytek.testspeechsuiteapi;

import android.test.AndroidTestCase;

import android.os.Environment;
import com.iflytek.testspeechsuiteapi.mvw.*;
import com.iflytek.testspeechsuiteapi.tts.DefTTS;
import com.iflytek.mvw.MvwSession;
import com.iflytek.speech.ISSErrors;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.Random;
import static com.iflytek.testspeechsuiteapi.TestApiTool.mvwSession;

/**
 * 接口测试用例。
 * 以test开头的用例都会被执行。
 */
public class TestApiMVW extends AndroidTestCase {
    DefMVW def;
    Tool tool;
    int err = -1;


    @Override
    protected void setUp() throws Exception {
        super.setUp();

        def = TestApiTool.defMVW;
        tool = TestApiTool.tool;
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();

        TestApiTool.defMVW.initMsg();

        if (TestApiTool.mvwSession != null) {
            TestApiTool.mvwSession.setMvwDefaultKeyWords(1);
            TestApiTool.mvwSession.setMvwDefaultKeyWords(2);
            TestApiTool.mvwSession.setMvwDefaultKeyWords(4);
            TestApiTool.mvwSession.setMvwDefaultKeyWords(8);
            TestApiTool.mvwSession.release();
        }

        if (TestApiTool.mvwSession1 != null) {
            TestApiTool.mvwSession1.setMvwDefaultKeyWords(1);
            TestApiTool.mvwSession1.setMvwDefaultKeyWords(2);
            TestApiTool.mvwSession1.setMvwDefaultKeyWords(4);
            TestApiTool.mvwSession1.setMvwDefaultKeyWords(8);
            TestApiTool.mvwSession1.release();
        }

        TestApiTool.mvwSession = null;
        TestApiTool.mvwSession1 = null;
    }

    private void appendAudioDataInAllScenes(MvwSession mvwSession, boolean[] scenes_boolean) {
        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        if (scenes_boolean[3]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(1, def.getnMvwScene());
            assertEquals(0, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_confirm, def);
        if (scenes_boolean[2]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(2, def.getnMvwScene());
            assertEquals(0, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_cancel, def);
        if (scenes_boolean[2] || scenes_boolean[1] || scenes_boolean[0]) {
            assertTrue(def.msgVwWakeup);
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_first, def);
        if (scenes_boolean[1]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(4, def.getnMvwScene());
            assertEquals(0, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_second, def);
        if (scenes_boolean[1]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(4, def.getnMvwScene());
            assertEquals(1, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_third, def);
        if (scenes_boolean[1]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(4, def.getnMvwScene());
            assertEquals(2, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_fourth, def);
        if (scenes_boolean[1]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(4, def.getnMvwScene());
            assertEquals(3, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_fifth, def);
        if (scenes_boolean[1]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(4, def.getnMvwScene());
            assertEquals(4, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_sixth, def);
        if (scenes_boolean[1]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(4, def.getnMvwScene());
            assertEquals(5, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_answer, def);
        if (scenes_boolean[0]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(8, def.getnMvwScene());
            assertEquals(0, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_hangup, def);
        if (scenes_boolean[0]) {
            assertTrue(def.msgVwWakeup);
            assertEquals(8, def.getnMvwScene());
            assertEquals(1, def.getnMvwId());
            def.initMsg();
        } else
            assertFalse(def.msgVwWakeup);
    }


    public void test_mvw_addStartScene() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, null);

        err = mvwSession2.addStartScene(2);
        assertEquals(err, ISSErrors.ISS_ERROR_INVALID_HANDLE);

        err = mvwSession.addStartScene(0);
        assertEquals(err, ISSErrors.ISS_ERROR_INVALID_PARA_VALUE);

        err = mvwSession.addStartScene(2);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        err = mvwSession.addStartScene(3);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        err = mvwSession.addStartScene(50);
        assertEquals(err, ISSErrors.ISS_ERROR_INVALID_PARA_VALUE);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_cancel);
        int vw_timeout = 0;
        while (!def.msgVwWakeup && vw_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgVwWakeup = false;
        assertTrue(def.msgVwWakeup_scene21);
        def.msgVwWakeup_scene21 = false;

        mvwSession.release();
        mvwSession2.release();
    }

    /**
     * 多实例构建自定义唤醒词。
     * <p/>
     * 第二个实例setkeyWords接口会报ISS_ERROR_BUILDGRM_INIT_ERROR
     */
    public void test_mvw_setKeyWords_multiInst() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession1;

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession2);
        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession1 = mvwSession2;

        err = mvwSession1.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        err = mvwSession2.setMvwKeyWords(2, def.mvwWord_ZhiDou);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_KaiYi);
        int vw_timeout = 0;
        while (!def.msgVwWakeup && vw_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgVwWakeup = false;
        assertEquals(def.getnMvwScene(), 1);

        err = mvwSession2.addStartScene(2);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        err = mvwSession2.start(2);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        tool.loadPcmAndAppendAudioData(mvwSession2, def.mvwPcm_ZhiDou);
        vw_timeout = 0;
        while (!def.msgVwWakeup && vw_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgVwWakeup = false;
        assertEquals(def.getnMvwScene(), 2);

        mvwSession1.release();
        mvwSession2.release();
    }

    public void test_mvw_setKeyWords() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession1;

        err = mvwSession1.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        err = mvwSession1.start(1);
        assertEquals(err, ISSErrors.ISS_SUCCESS);

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_KaiYi);
        int vw_timeout = 0;
        while (!def.msgVwWakeup && vw_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgVwWakeup = false;
        assertEquals(def.getnMvwScene(), 1);
        assertEquals(def.getnMvwId(), 0);
    }

    /**
     * getInstance: context=v(activity context), mvwListener=v, resDir=v
     * start: nMvwScene=v(1)
     * appendAudioData: 你好语音助理
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常唤醒
     */
    public void test_mvw_101() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        mvwSession.stop();

        mvwSession.release();
    }

    /**
     * getInstance: context=v(application context), mvwListener=v, def.resDir=v
     * start: nMvwScene=v(1)
     * appendAudioData: 你好语音助理
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常唤醒
     */
    public void test_mvw_102() {

        MvwSession mvwSession = new MvwSession(getContext().getApplicationContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        mvwSession.stop();

        mvwSession.release();
    }

    /**
     * getInstance: context=v(activity context), mvwListener=v, resDir=iv(资源不存在)
     * start: nMvwScene=v(1)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. start接口返回ISS_ERROR_INVALID_HANDLE
     * c. 回调ISS_ERROR_FILE_NOT_FOUND
     */
    public void test_mvw_103() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        mvwSession.release();
    }

    /**
     * getInstance: context=v(activity context), mvwListener=v, resDir=iv(资源路径用反斜杠)
     * start: nMvwScene=v(1)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. start接口返回ISS_ERROR_INVALID_HANDLE
     * c. 回调ISS_ERROR_FILE_NOT_FOUND
     */
    public void test_mvw_104() {
        MvwSession mvwSession = MvwSession.getInstance(getContext(), def.iMvwListener, Environment.getExternalStorageDirectory().getAbsolutePath()
                + "\\iflytek\\res\\mvw\\First");

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        mvwSession.release();
    }

    public void test_mvw_105() {
        MvwSession.setMvwLanguage(MvwSession.ISS_MVW_LANG_ENG);
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_first_en, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(4, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        mvwSession.stop();

        mvwSession.release();
    }

    public void test_mvw_106() {
        MvwSession.setMvwLanguage(MvwSession.ISS_MVW_LANG_ENG);
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.start(15);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_confirm_en, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(2, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        mvwSession.stop();

        mvwSession.release();
    }

    /**
     * MvwSession: context=v(activity context), mvwListener=v, def.resDir=v
     * start: nMvwScene=v(1)
     * appendAudioData: 你好语音助理
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常唤醒
     */
    public void test_mvw_201() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        mvwSession.stop();

        mvwSession.release();
    }

    /**
     * MvwSession: context=v(application context), mvwListener=v, def.resDir=v
     * start: nMvwScene=v(1)
     * appendAudioData: 你好语音助理
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常唤醒
     */
    public void test_mvw_202() {
        MvwSession mvwSession = new MvwSession(getContext().getApplicationContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * MvwSession: context=iv(null), mvwListener=v, def.resDir=v
     * start: nMvwScene=v(1)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. start接口返回ISS_ERROR_INVALID_CALL
     */
    public void test_mvw_203() {
        MvwSession mvwSession = new MvwSession(null, def.iMvwListener, def.resDir);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_fail);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * MvwSession: context=v(activity context), mvwListener=iv(null), def.resDir=v
     * start: nMvwScene=v(1)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. start接口返回ISS_ERROR_INVALID_CALL
     */
    public void test_mvw_204() {
        MvwSession mvwSession = new MvwSession(getContext(), null, def.resDir);
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * MvwSession: context=v(activity context), mvwListener=v, def.resDir=iv(null)
     * start: nMvwScene=v(1)
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常唤醒
     */
    public void test_mvw_205() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, null);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * MvwSession: context=v(activity context), mvwListener=v, def.resDir=iv(资源不存在)
     * start: nMvwScene=v(1)
     * <p/>
     * a. start接口返回ISS_ERROR_INVALID_HANDLE
     * b. 回调ISS_ERROR_FILE_NOT_FOUND
     */
    public void test_mvw_206() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * MvwSession: context=v(activity context), mvwListener=v, def.resDir=iv(资源路径用反斜杠)
     * start: nMvwScene=v(1)
     * <p/>
     * a. start接口返回ISS_ERROR_INVALID_HANDLE
     * b. 回调ISS_ERROR_FILE_NOT_FOUND
     */
    public void test_mvw_207() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, Environment.getExternalStorageDirectory().getAbsolutePath()
                + "\\iflytek\\res\\mvw\\First");
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * start: nMvwScene=v(1~15)
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常唤醒
     */
    public void abort_test_mvw_301() {
        int[] scenes = new int[]{8, 4, 2, 1};

        for (int i = 1; i <= 15; i++) {
            boolean[] scenes_boolean = new boolean[4];

            MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
            assertNotNull(mvwSession);

            int init_timeout = 0;
            while (!def.msgInitStatus && init_timeout++ < 500) {
                tool.sleep(10);
            }
            assertTrue(def.msgInitStatus_success);
            TestApiTool.mvwSession = mvwSession;
            def.initMsg();

            err = mvwSession.start(i);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            System.out.println("start: " + i);

            int t = i;
            for (int k = 0; k < scenes.length; k++) {
                if (t - scenes[k] >= 0) {
                    scenes_boolean[k] = true;
                    t -= scenes[k];
                }
            }

            System.out.println(Arrays.toString(scenes_boolean));

            appendAudioDataInAllScenes(mvwSession, scenes_boolean);

            mvwSession.release();
        }
    }

    /**
     * nMvwScene=iv(0)
     * <p/>
     * start接口返回ISS_ERROR_INVALID_PARA
     */
    public void test_mvw_302() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=iv(64)
     * <p/>
     * start接口返回ISS_ERROR_INVALID_PARA
     */
    public void test_mvw_303() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(64);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=iv(-1)
     * <p/>
     * start接口返回ISS_ERROR_INVALID_PARA
     */
    public void test_mvw_304() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(-1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=iv(Integer.MAX_VALUE)
     * <p/>
     * start接口返回ISS_ERROR_INVALID_PARA
     */
    public void test_mvw_305() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(Integer.MAX_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=iv(Integer.MIN_VALUE)
     * <p/>
     * start接口返回ISS_ERROR_INVALID_PARA
     */
    public void test_mvw_306() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(Integer.MIN_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(1),nMvwId=v(0),threshold=v(300)
     * appendAudioData: 你好语音助理
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_401() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(1, 0, 3000);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(2),nMvwId=v(0),threshold=v(300)
     * appendAudioData: 确定
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_402() {

        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(2, 0, 3000);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_confirm, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(2),nMvwId=v(1),threshold=v(300)
     * appendAudioData: 取消
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_403() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(2, 1, 3000);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_cancel, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(4),nMvwId=v(0),threshold=v(300)
     * appendAudioData: 第一个
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_404() {

        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(4, 0, 3000);

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_first, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(4),nMvwId=v(1),threshold=v(300)
     * appendAudioData: 第二个
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_405() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(4, 1, 300);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_second, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(4),nMvwId=v(2),threshold=v(300)
     * appendAudioData: 第三个
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_406() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(4, 2, 300);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_third, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(4),nMvwId=v(3),threshold=v(300)
     * appendAudioData: 第四个
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_407() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(4, 3, 300);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_fourth, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(4),nMvwId=v(4),threshold=v(300)
     * appendAudioData: 第五个
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_408() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(4, 4, 300);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_fifth, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(4),nMvwId=v(5),threshold=v(300)
     * appendAudioData: 第六个
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_409() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(4, 5, 300);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(5);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_sixth, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(8),nMvwId=v(0),threshold=v(300)
     * appendAudioData: 接听
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_410() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(8, 0, 300);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_answer, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(8),nMvwId=v(1),threshold=v(300)
     * appendAudioData: 挂断
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_411() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(8, 1, 300);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_hangup, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(8),nMvwId=v(2),threshold=v(300)
     * appendAudioData: 取消
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b.无法唤醒
     */
    public void abort_test_mvw_412() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(8, 2, 300);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_cancel, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(1),nMvwId=v(0),threshold=v(-1)
     * <p/>
     * 返回success
     */
    public void abort_test_mvw_413() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(1, 0, -1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_cancel, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(8, def.getnMvwScene());
        assertEquals(2, def.getnMvwId());

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=v(-1),nMvwId=v(-1),threshold=v(-1)
     * <p/>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void abort_test_mvw_414() {

        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(-1, -1, -1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=iv(Integer.MAX_VALUE),nMvwId=iv(Integer.MAX_VALUE),
     * threshold=iv(Integer.MAX_VALUE)
     * <p/>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void abort_test_mvw_415() {

        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(Integer.MAX_VALUE, Integer.MAX_VALUE, Integer.MAX_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setThreadhold: nMvwScene=iv(Integer.MIN_VALUE),nMvwId=iv(Integer.MIN_VALUE),
     * threshold=iv(Integer.MIN_VALUE)
     * <p/>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void abort_test_mvw_416() {

        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(Integer.MIN_VALUE, Integer.MIN_VALUE, Integer.MIN_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords设置唤醒词，nMvwScene:唤醒场景，szWords:设定的唤醒词
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(凯翼你好，id=0)
     * appendAudioData: 你好语音助理; 凯翼你好
     * <p/>
     * a. 默认唤醒词无法唤醒
     * b. 自定义唤醒词可以唤醒
     * c. 接口全部返回ISS_SUCCESS
     */
    public void test_mvw_501() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(凯翼你好，id=0; 灵犀灵犀，id=1)
     * appendAudioData: 你好语音助理; 凯翼你好; 灵犀灵犀
     * <p>
     * a. 默认唤醒词无法唤醒
     * b. 自定义唤醒词可以唤醒
     * c. 接口全部返回ISS_SUCCESS
     */
    public void test_mvw_502() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi_LingXi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_LingXi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(2),szWords=v(凯翼你好，id=0)
     * appendAudioData: 确定; 凯翼你好
     * <p/>
     * a. 默认唤醒词无法唤醒
     * b. 自定义唤醒词可以唤醒
     * c. 接口全部返回ISS_SUCCESS
     */
    public void test_mvw_503() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(2, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_confirm, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(2, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(4),szWords=v(凯翼你好，id=0)
     * appendAudioData: 第一个; 凯翼你好
     * <p/>
     * a. 默认唤醒词无法唤醒
     * b. 自定义唤醒词可以唤醒
     * c. 接口全部返回ISS_SUCCESS
     */
    public void test_mvw_504() {
        MvwSession mvwSession = MvwSession.getInstance(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(4, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_first, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(4);
        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(4, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(8),szWords=v(凯翼你好，id=0)
     * appendAudioData: 接听; 凯翼你好
     * <p/>
     * a. 默认唤醒词无法唤醒
     * b. 自定义唤醒词可以唤醒
     * c. 接口全部返回ISS_SUCCESS
     */
    public void test_mvw_505() {
        MvwSession mvwSession = MvwSession.getInstance(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(8, def.mvwWord_KaiYi);

        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_answer);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(8, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 测试多实例设置自定义唤醒词。第一个实例用默认全局唤醒词，
     * 第二个实例设置1场景自定义唤醒词
     * <p/>
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(凯翼你好，id=0)
     * appendAudioData: 你好语音助理; 凯翼你好
     * <p/>
     * a. 默认唤醒词可以唤醒
     * b. 自定义唤醒词可以唤醒
     * c. 接口全部返回ISS_SUCCESS
     */
    public void test_mvw_506() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession1);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession1 = mvwSession1;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=iv(-1),szWords=v(凯翼你好，id=0)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_507() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(-1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=iv(73),szWords=v(凯翼你好，id=0)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_508() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(73, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=iv(Integer.MIN_VALUE),szWords=v(凯翼你好，id=0)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_509() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(Integer.MIN_VALUE, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=iv(Integer.MAX_VALUE),szWords=v(凯翼你好，id=0)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_510() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(Integer.MAX_VALUE, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(json格式错误)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_511() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_format);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(json格式错误)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_512() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_format1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(json格式错误)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_513() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_format2);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(json格式错误)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_514() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_format3);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(json格式错误)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_515() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_format4);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(json格式错误)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_516() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_format5);
//        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        mvwSession.start(1);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(空字符串)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_517() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, "");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(凯翼你好，id=-1)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_518() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_id_1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(凯翼你好，id=100)
     * <p/>
     * 返回错误码
     * <p>
     * 待测试支持的id最大值
     */
    public void test_mvw_519() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_id_100);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(凯翼你好，id=3003003003003003009)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_520() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_id_3003003003003003009);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(凯翼你好，id="0")
     * <p/>
     * 返回错误码
     */
    public void test_mvw_521() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_id_str);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv(123，id=0)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_522() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_123_num);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(1),szWords=iv("123"，id=0)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_523() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_123_str);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_123, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(凯翼你好，id=0; 灵犀灵犀，id=3)
     * appendAudioData: 你好语音助理; 凯翼你好; 灵犀灵犀
     */
    public void test_mvw_524() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_id_order);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(凯翼你好，id=0; 灵犀灵犀，id=0)
     * appendAudioData: 你好语音助理; 凯翼你好; 灵犀灵犀
     * <p>
     * 第二个唤醒词可以唤醒
     */
    public void test_mvw_525() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        mvwSession.setMvwDefaultKeyWords(1);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_id_same);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_LingXi, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(凯翼你好，id=0; 凯翼你好，id=1)
     * appendAudioData: 你好语音助理; 凯翼你好; 灵犀灵犀
     * <p>
     * 给出的id是第一个的
     */
    public void test_mvw_526() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_wrong_word_same);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(
     * 凯翼你好凯翼你好凯翼你好凯翼你好凯翼你好凯翼你好凯翼你好凯翼你好凯翼你好
     * 凯翼你好凯翼你好凯翼你好凯翼你好，id=0;
     * 知豆你好知豆你好知豆你好知豆你好知豆你好知豆你好知豆你好知豆你好知豆你好
     * 知豆你好知豆你好知豆你好知豆你好知豆你好，id=1;
     * 灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀
     * 灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀灵犀, id=2)
     * appendAudioData: 你好语音助理; 凯翼你好; 知豆你好;灵犀灵犀
     */
    public void test_mvw_527() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi_LingXi_ZhiDou_long);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi_long, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_ZhiDou_long, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_LingXi_long, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(2, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

//        err = mvwSession.release();
//        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(凯翼你好，id=0; 灵犀灵犀，id=1;
     * 知豆知斗，id=2; 确定，id=3；取消，id=4；第一个，id=5；第二个；id=6；第三个，id=7；
     * 第四个，id=8；第五个，id=9；第六个，id=10；最后一个，id=11；陈盛，id=12；重庆小面，id=13；
     * 大脸鸡排，id=14；胡氏刺身，id=15；导航到科大讯飞，id=16；打电话给白亚伟，id=17；
     * 打电话给张三，id=18；沈彤，id=19；郑凯，id=20)
     * appendAudioData: 你好语音助理; 凯翼你好; 灵犀灵犀（以上各唤醒词）
     */
    public void test_mvw_528() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_many_words);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_ZhiDou, def);
//        assertTrue(def.msgVwWakeup);
//        assertEquals(2, def.getnMvwId());
//        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_LingXi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_confirm, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(3, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_cancel, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(4, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_first, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(5, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_second, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(6, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_third, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(7, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_fourth, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(8, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_fifth, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(9, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_sixth, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(10, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_last, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(11, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_ChenSheng, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(12, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_ChongQingXiaoMian, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(13, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_DaLianJiPai, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(14, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_HuShiCiShen, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(15, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_NavigateIflytek, def);
//        assertTrue(def.msgVwWakeup);
//        assertEquals(16, def.getnMvwId());
//        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_PhoneBaiYawei, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(17, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_PhoneZhangsan, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(18, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_ShenTong, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(19, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_ZhengKai, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(20, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    // 设置英文唤醒词
    public void test_mvw_529() {
        //MvwSession.setMvwLanguage(MvwSession.ISS_MVW_LANG_ENG);
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_HelloHello);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_HelloHello, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * setMvwKeyWords: nMvwScene=v(1),szWords=v(凯翼你好，id=0)
     * setMvwDefaultKeyWords: nMvwScene=v(1)
     * appendAudioData: 你好语音助理; 凯翼你好
     * <p/>
     * a. 默认唤醒词可以唤醒
     * b. 自定义唤醒词无法唤醒
     * c. 接口全部返回ISS_SUCCESS
     */
    public void test_mvw_601() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.setMvwDefaultKeyWords(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    public void test_mvw_602() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(2, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(2, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.setMvwDefaultKeyWords(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_confirm, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(2, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    public void test_mvw_603() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(4, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(4, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.setMvwDefaultKeyWords(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_first, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(4, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    public void test_mvw_604() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(8, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(8, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.setMvwDefaultKeyWords(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_answer, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(8, def.getnMvwScene());
        def.initMsg();

        err = mvwSession.start(8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_KaiYi, def);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(-1)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_605() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwDefaultKeyWords(-1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(16)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_606() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwDefaultKeyWords(16);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(Integer.MAX_VALUE)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_607() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwDefaultKeyWords(Integer.MAX_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(Integer.MIN_VALUE)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_608() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwDefaultKeyWords(Integer.MIN_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * start(1~15), stopScene(1~15)的组合
     */
    public void abort_test_mvw_701() {
        int[] scenes = new int[]{8, 4, 2, 1};

        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                boolean[] scenes_boolean = new boolean[4];

                MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
                assertNotNull(mvwSession);

                int init_timeout = 0;
                while (!def.msgInitStatus && init_timeout++ < 500) {
                    tool.sleep(10);
                }
                assertTrue(def.msgInitStatus_success);
                TestApiTool.mvwSession = mvwSession;
                def.initMsg();

                err = mvwSession.start(i);
                assertEquals(ISSErrors.ISS_SUCCESS, err);

                err = mvwSession.stopScene(j);
                assertEquals(ISSErrors.ISS_SUCCESS, err);

                System.out.println("start: " + i + ", stopScene: " + j);

                int t = i;
                for (int k = 0; k < scenes.length; k++) {
                    if (t - scenes[k] >= 0) {
                        scenes_boolean[k] = true;
                        t -= scenes[k];
                    }
                }
                t = j;
                for (int k = 0; k < scenes.length; k++) {
                    if (t - scenes[k] >= 0) {
                        scenes_boolean[k] = false;
                        t -= scenes[k];
                    }
                }

                System.out.println(Arrays.toString(scenes_boolean));

                appendAudioDataInAllScenes(mvwSession, scenes_boolean);

                mvwSession.release();
            }
        }
    }

    /**
     * nMvwScene=v(-1)
     * <p/>
     * 返回错误码
     */
    public void test_mvw_702() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.stopScene(-1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * nMvwScene=v(64)
     * <p/>
     * 返回错误码
     *
     * fail 返回成功
     */
    public void fail_test_mvw_703() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.stopScene(64);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * start(1~15), addStartScene(1~15)的组合
     */
    public void abort_test_mvw_801() {
        int[] scenes = new int[]{8, 4, 2, 1};

        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                boolean[] scenes_boolean = new boolean[4];

                MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
                assertNotNull(mvwSession);

                int init_timeout = 0;
                while (!def.msgInitStatus && init_timeout++ < 500) {
                    tool.sleep(10);
                }
                assertTrue(def.msgInitStatus_success);
                TestApiTool.mvwSession = mvwSession;
                def.initMsg();

                err = mvwSession.start(i);
                assertEquals(ISSErrors.ISS_SUCCESS, err);

                err = mvwSession.addStartScene(j);
                assertEquals(ISSErrors.ISS_SUCCESS, err);

                System.out.println("start: " + i + ", addStartScene: " + j);

                int t = i;
                for (int k = 0; k < scenes.length; k++) {
                    if (t - scenes[k] >= 0) {
                        scenes_boolean[k] = true;
                        t -= scenes[k];
                    }
                }
                t = j;
                for (int k = 0; k < scenes.length; k++) {
                    if (t - scenes[k] >= 0) {
                        scenes_boolean[k] = true;
                        t -= scenes[k];
                    }
                }

                System.out.println(Arrays.toString(scenes_boolean));

                appendAudioDataInAllScenes(mvwSession, scenes_boolean);

                mvwSession.release();
            }
        }
    }

    /**
     * addStartScene: nMvwScene = iv(-1)
     */
    public void test_mvw_802() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.addStartScene(-1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * addStartScene: nMvwScene = iv(18)
     */
    public void test_mvw_803() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.addStartScene(18);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * addStartScene: nMvwScene = iv(Integer.MAX_VALUE)
     */
    public void test_mvw_804() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.addStartScene(Integer.MAX_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * addStartScene: nMvwScene = iv(Integer.MIN_VALUE)
     */
    public void test_mvw_805() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.addStartScene(Integer.MIN_VALUE);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * audioBuffer=iv(null)
     * <p/>
     * 无法唤醒
     */
    public void test_mvw_901() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        for (int i = 0; i < 100; i++) {
            err = mvwSession.appendAudioData(null);
            assertEquals(ISSErrors.ISS_SUCCESS, err);
            tool.sleep(10);
        }

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 连续送大音频
     */
    public void test_mvw_902() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int bytesEachTimeSend = 140000;

        for (int i = 0; i < 200; i++) {
            byte[] byteData = null;
            try {
                DataInputStream data = new DataInputStream(new FileInputStream(def.mvwPcm_KaiYi_long));
                byteData = new byte[data.available()];
                data.read(byteData);
                data.close();
            } catch (IOException e) {
                e.printStackTrace();
            }

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

                byte[] mid = new byte[bytesEachTimeSend];
                if (end > bytesEachTimeSend) {
                    System.arraycopy(byteData, location, mid, 0, bytesEachTimeSend);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                int ret = mvwSession.appendAudioData(mid);
                assertEquals(ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER, ret);
                switch (ret) {
                    case ISSErrors.ISS_SUCCESS:
//                            ILog.i("MvwSession.appendAudio", "ISS_SUCCESS", isWriteToActivity);
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_PARA:
                        ILog.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_PARA", 0);
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                        ILog.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_HANDLE", 0);
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_CALL:
                        ILog.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_CALL", 0);
                        break;
                    case ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER:
                        ILog.e("MvwSession.appendAudio", "ISS_ERROR_NO_ENOUGH_BUFFER", 0);
                        break;
                    default:
                        ILog.e("MvwSession.appendAudio", "UnHandled MSG", 0);
                }
                tool.sleep(20);
                location = location + bytesEachTimeSend;
            }
        }

    }

    /**
     * szParam=v(mvw_enable_aec), szParamValue=v(on)
     */
    public void test_mvw_1001() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession = mvwSession;

        String vwLogPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/iflytek/res/Active/MVWRes/log/Vw";
        File file = new File(vwLogPath);
        if (file.exists()) {
            tool.deleteDirectory(vwLogPath);
        }
        file.mkdir();

        err = mvwSession.setParam("mvw_enable_aec", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        mvwSession.setParam("mvw_enable_aec", "off");

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String[] fileNames = file.list();
        for (String logFile :
                fileNames) {
            if (logFile.contains("org")) {
                String orgPath = vwLogPath + "/" + logFile;
                File orgFile = new File(orgPath);
                assertTrue(orgFile.length() > 0);
                break;
            }
        }

        tool.deleteDirectory(vwLogPath);
    }

    /**
     * szParam=v(mvw_enable_aec), szParamValue=v(off)
     */
    public void test_mvw_1002() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        String vwLogPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/iflytek/res/Active/MVWRes/log/Vw";
        File file = new File(vwLogPath);
        if (file.exists()) {
            tool.deleteDirectory(vwLogPath);
        }
        file.mkdir();

        err = mvwSession.setParam("mvw_enable_aec", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.setParam("mvw_enable_aec", "off");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String[] fileNames = file.list();
        for (String logFile :
                fileNames) {
            if (logFile.contains("org")) {
                String orgPath = vwLogPath + "/" + logFile;
                File orgFile = new File(orgPath);
                assertTrue(orgFile.length() == 0);
                break;
            }
        }

        tool.deleteDirectory(vwLogPath);
    }

    /**
     * szParam=v(mvw_enable_lsa), szParamValue=v(on)
     */
    public void test_mvw_1003() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        String vwLogPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/iflytek/res/Active/MVWRes/log/Vw";
        File file = new File(vwLogPath);
        if (file.exists()) {
            tool.deleteDirectory(vwLogPath);
        }
        file.mkdir();

        err = mvwSession.setParam("mvw_enable_lsa", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        mvwSession.setParam("mvw_enable_lsa", "off");

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String[] fileNames = file.list();
        for (String logFile :
                fileNames) {
            if (logFile.contains("org")) {
                String orgPath = vwLogPath + "/" + logFile;
                File orgFile = new File(orgPath);
                assertTrue(orgFile.length() > 0);
                break;
            }
        }

        tool.deleteDirectory(vwLogPath);
    }

    /**
     * szParam=v(mvw_enable_lsa), szParamValue=v(off)
     */
    public void test_mvw_1004() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        String vwLogPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/iflytek/res/Active/MVWRes/log/Vw";
        File file = new File(vwLogPath);
        if (file.exists()) {
            tool.deleteDirectory(vwLogPath);
        }
        file.mkdir();

        err = mvwSession.setParam("mvw_enable_lsa", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.setParam("mvw_enable_lsa", "off");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String[] fileNames = file.list();
        for (String logFile :
                fileNames) {
            if (logFile.contains("org")) {
                String orgPath = vwLogPath + "/" + logFile;
                File orgFile = new File(orgPath);
                assertTrue(orgFile.length() == 0);
                break;
            }
        }

        tool.deleteDirectory(vwLogPath);
    }

    /**
     * szParam=v(null), szParamValue=v(off)
     */
    public void test_mvw_1005() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setParam(null, "off");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * szParam=v(mvw_enable_aec), szParamValue=v(null)
     */
    public void test_mvw_1006() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setParam("mvw_enable_aec", null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * szParam=v(null), szParamValue=v(null)
     */
    public void test_mvw_1007() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setParam(null, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * szParam=v(""), szParamValue=v("")
     * <p>
     * fail setParam接口无效值也返回成功
     */
    public void test_mvw_1008() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession = mvwSession;

        err = mvwSession.setParam("", "");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * szParam=v("测试参数"), szParamValue=v("测试参数值")
     * <p>
     * fail setParam接口无效值也返回成功
     */
    public void test_mvw_1009() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setParam("测试参数", "测试参数值");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * szParam=iv(50个字符), szParamValue=iv(50个字符)
     * <p>
     * fail setParam接口无效值也返回成功
     */
    public void test_mvw_1010() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        DefTTS defTTS = new DefTTS();
        err = mvwSession.setParam(defTTS.getRandomENStr(50), defTTS.getRandomENStr(50));
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resDir传入没有资源文件的路径
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     * c. 回调ISS_ERROR_FILE_NOT_FOUND
     */
    public void test_mvw_1101() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.addStartScene(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resDir传入null参数，context、iMvwListener传入合法参数
     * MvwSession: IMvwListener＝iv(null)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_mvw_1102() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resDir传入null参数，context、iMvwListener传入合法参数
     * MvwSession: IMvwListener＝iv(null)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_mvw_1103() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resDir传入null参数，context、iMvwListener传入合法参数
     * MvwSession: IMvwListener＝iv(null)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_mvw_1104() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resDir传入null参数，context、iMvwListener传入合法参数
     * MvwSession: IMvwListener＝iv(null)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_mvw_1105() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.setParam("mvw_enable_aec", "off");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resDir传入null参数，context、iMvwListener传入合法参数
     * MvwSession: IMvwListener＝iv(null)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_mvw_1106() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.setThreshold(1, 0, 10);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resDir传入null参数，context、iMvwListener传入合法参数
     * MvwSession: IMvwListener＝iv(null)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_mvw_1107() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resDir传入null参数，context、iMvwListener传入合法参数
     * MvwSession: IMvwListener＝iv(null)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_mvw_1108() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.stopScene(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resDir传入null参数，context、iMvwListener传入合法参数
     * MvwSession: IMvwListener＝iv(null)
     * <p/>
     * a. MvwSession实例返回不为空
     * b. 其它接口返回ISS_ERROR_INVALID_HANDLE
     */
    public void test_mvw_1109() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_fileNotFound);

        err = mvwSession.setMvwDefaultKeyWords(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * 未start调用addStartScene
     */
    public void test_mvw_1110() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.addStartScene(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_confirm, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(2, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 送录音时release
     */
    public void test_mvw_1111() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.mvwPcm_global));
            byte[] byteData;
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            int location = 0;
            int position = byteData.length;
            int i = 0;

            while (location < byteData.length && location < position && i++ < 70) {
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                byte[] mid = new byte[160];
                if (end > 160) {
                    System.arraycopy(byteData, location, mid, 0, 160);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                err = mvwSession.appendAudioData(mid);
                assertEquals(ISSErrors.ISS_SUCCESS, err);
                Thread.sleep(3);
                location = location + 160;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 送录音时stop
     */
    public void test_mvw_1112() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int location = 0;
        byte[] byteData = null;
        int position = 0;
        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.mvwPcm_global));
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            position = byteData.length;
            int i = 0;

            while (location < byteData.length && location < position && i++ < 70) {
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                byte[] mid = new byte[160];
                if (end > 160) {
                    System.arraycopy(byteData, location, mid, 0, 160);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                err = mvwSession.appendAudioData(mid);
                assertEquals(ISSErrors.ISS_SUCCESS, err);
                Thread.sleep(1);
                location = location + 160;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (location < byteData.length && location < position) {
            int end1 = byteData.length - location;
            int end2 = position - location;
            int end = 0;
            if (end1 >= end2)
                end = end2;
            else {
                end = end1;
            }

            byte[] mid = new byte[160];
            if (end > 160) {
                System.arraycopy(byteData, location, mid, 0, 160);
            } else {
                System.arraycopy(byteData, location, mid, 0, end);
            }
            err = mvwSession.appendAudioData(mid);
            assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
            tool.sleep(1);
            location = location + 160;
        }

        tool.sleep(1500);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 送录音时stopScene(本场景)
     */
    public void test_mvw_1113() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int location = 0;
        byte[] byteData = null;
        int position = 0;
        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.mvwPcm_global));
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            position = byteData.length;
            int i = 0;

            while (location < byteData.length && location < position && i++ < 70) {
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                byte[] mid = new byte[160];
                if (end > 160) {
                    System.arraycopy(byteData, location, mid, 0, 160);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                err = mvwSession.appendAudioData(mid);
                assertEquals(ISSErrors.ISS_SUCCESS, err);
                Thread.sleep(1);
                location = location + 160;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        err = mvwSession.stopScene(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (location < byteData.length && location < position) {
            int end1 = byteData.length - location;
            int end2 = position - location;
            int end = 0;
            if (end1 >= end2)
                end = end2;
            else {
                end = end1;
            }

            byte[] mid = new byte[160];
            if (end > 160) {
                System.arraycopy(byteData, location, mid, 0, 160);
            } else {
                System.arraycopy(byteData, location, mid, 0, end);
            }
            err = mvwSession.appendAudioData(mid);
            assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
            tool.sleep(1);
            location = location + 160;
        }

        tool.sleep(1500);
        assertFalse(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 送录音时stopScene(非本场景)
     */
    public void test_mvw_1114() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(3);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int location = 0;
        byte[] byteData = null;
        int position = 0;
        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.mvwPcm_global));
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            position = byteData.length;
            int i = 0;

            while (location < byteData.length && location < position) {
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                byte[] mid = new byte[160];
                if (end > 160) {
                    System.arraycopy(byteData, location, mid, 0, 160);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                err = mvwSession.appendAudioData(mid);
                assertEquals(ISSErrors.ISS_SUCCESS, err);
                Thread.sleep(1);
                location = location + 160;

                if (i == 70) {
                    err = mvwSession.stopScene(2);
                    assertEquals(ISSErrors.ISS_SUCCESS, err);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        tool.sleep(1500);
        assertTrue(def.msgVwWakeup);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * (MvwSession-release) * 100,4,11,3,12,6
     */
    public void test_mvw_1115() {
        for (int i = 0; i < 500; i++) {
            MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
            assertNotNull(mvwSession);

            while (!def.msgInitStatus) {
                tool.sleep(10);
            }
            assertTrue(def.msgInitStatus_success);
            TestApiTool.mvwSession = mvwSession;
            def.initMsg();

            err = mvwSession.release();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * (MvwSession-start-stop-release) * 100,4,11,3,12,6
     */
    public void test_mvw_1116() {
        for (int i = 0; i < 100; i++) {
            MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
            assertNotNull(mvwSession);

            while (!def.msgInitStatus) {
                tool.sleep(10);
            }
            assertTrue(def.msgInitStatus_success);
            TestApiTool.mvwSession = mvwSession;

            err = mvwSession.start(1);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.stop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.release();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 4,(11,12)*100,11,3,12,6
     */
    public void test_mvw_1117() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        for (int i = 0; i < 100; i++) {
            err = mvwSession.start(1);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.stop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        tool.sleep(5000);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * (4,2,10,13,11,12,6)*100
     */
    public void test_mvw_1118() {
        for (int i = 0; i < 100; i++) {
            MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
            assertNotNull(mvwSession);

            while (!def.msgInitStatus) {
                tool.sleep(10);
            }
            assertTrue(def.msgInitStatus_success);
            TestApiTool.mvwSession = mvwSession;

            err = mvwSession.start(1);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.addStartScene(4);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.setThreshold(4, 0, 10);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.stopScene(1);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.stop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.release();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
    }

    /**
     * 1, (7, 14)*1000, 11, 3
     */
    public void test_mvw_1119() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        for (int i = 0; i < 1000; i++) {
            err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = mvwSession.setMvwDefaultKeyWords(1);
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 1, 6, 2
     */
    public void test_mvw_1120() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.addStartScene(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 3
     */
    public void test_mvw_1121() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.appendAudioData(null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 1, 11, 3
     */
    public void test_mvw_1122() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        def.initMsg();

        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        mvwSession1.stop();

        mvwSession1.release();
    }

    /**
     * 1, 6, 4, 11, 3
     */
    public void test_mvw_1123() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        def.initMsg();

        MvwSession mvwSession1 = MvwSession.getInstance(getContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        mvwSession1.stop();

        mvwSession1.release();
    }

    /**
     * 1, 6, 5
     */
    public void test_mvw_1124() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        mvwSession.initService();
    }

    /**
     * 1, 6, 6
     */
    public void test_mvw_1125() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 7
     */
    public void test_mvw_1126() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 8
     */
    public void test_mvw_1127() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.setParam("mvw_enable_lsa", "off");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 10
     */
    public void test_mvw_1128() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.setThreshold(1, 0, 10);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 11
     */
    public void test_mvw_1129() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 12
     */
    public void test_mvw_1130() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 13
     */
    public void test_mvw_1131() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.stopScene(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6, 14
     */
    public void test_mvw_1132() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.setMvwDefaultKeyWords(1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * 1, 6
     */
    public void test_mvw_1133() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 1, 2, 6
     */
    public void test_mvw_1134() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.addStartScene(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 1, 11, 6
     */
    public void test_mvw_1135() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 1, 7, 6
     */
    public void test_mvw_1136() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 1, 8, 6
     */
    public void test_mvw_1137() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setParam("mvw_enable_lsa", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 1, 10, 6
     */
    public void test_mvw_1138() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setThreshold(1, 0, 10);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 1, 7, 14, 6
     */
    public void test_mvw_1139() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);

        err = mvwSession.setMvwKeyWords(1, def.mvwWord_KaiYi);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.setMvwDefaultKeyWords(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * start * 10000
     */
    public void atest_stress_mvw_1() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        for (int i = 0; i < 10000; i++) {
            err = mvwSession.start(1);
        }

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);

        assertTrue(def.msgVwWakeup);
        assertEquals(1, def.getnMvwScene());
        assertEquals(0, def.getnMvwId());

        mvwSession.stop();

        mvwSession.release();
    }

    /**
     * destroy * 10000
     */
    public void atest_stress_mvw_2() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;

        for (int i = 0; i < 10000; i++) {
            mvwSession.release();
        }
    }

    /**
     * start:nMvwScene=v(1)
     * start:nMvwScene=v(2)
     * addStartScene:nMvwScene=v(4)
     * appendAudioData: 接听
     * <p/>
     * 场景1,2,4可以唤醒
     */
    public void test_mvw_mInst1() {
        MvwSession mvwSession = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.mvwSession = mvwSession;
        assertTrue(def.msgInitStatus_success);
        def.initMsg();

        DefMVW def1 = new DefMVW();
        def1.isWriteToActivity = 0;
        MvwSession mvwSession1 = new MvwSession(getContext(), def1.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession1);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def1.msgInitStatus_success);
        TestApiTool.mvwSession1 = mvwSession1;

        err = mvwSession.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession1.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession1.addStartScene(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());
        def.initMsg();

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_confirm, def1);
        assertTrue(def1.msgVwWakeup);
        assertEquals(0, def1.getnMvwId());
        assertEquals(2, def1.getnMvwScene());
        def1.initMsg();

        assertFalse(def.msgVwWakeup);

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_first, def1);
        assertTrue(def1.msgVwWakeup);
        assertEquals(0, def1.getnMvwId());
        assertEquals(4, def1.getnMvwScene());
        def1.initMsg();

        mvwSession.release();
        mvwSession1.release();
    }

    /**
     * 一个实例stop，另一个实例唤醒
     */
    public void test_mvw_mInst2() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession1;
        def.initMsg();

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession2);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession1 = mvwSession2;

        mvwSession1.setMvwDefaultKeyWords(1);
        mvwSession2.setMvwDefaultKeyWords(1);

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession2.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession1.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession2, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        mvwSession1.release();
        mvwSession2.release();
    }

    /**
     * 一个实例stopScene，另一个实例唤醒
     */
    public void test_mvw_mInst3() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession1;
        def.initMsg();

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession2);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession1 = mvwSession2;

        mvwSession1.setMvwDefaultKeyWords(1);
        mvwSession2.setMvwDefaultKeyWords(1);

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession2.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession1.stopScene(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession2, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        mvwSession1.release();
        mvwSession2.release();
    }

    /**
     * 两个实例加载不同场景，相互唤醒不是自己的场景
     */
    public void test_mvw_mInst4() {
        DefMVW def1 = new DefMVW();
        DefMVW def2 = new DefMVW();

        def1.isWriteToActivity = 0;
        def2.isWriteToActivity = 0;

        MvwSession mvwSession1 = new MvwSession(getContext(), def1.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def1.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def1.msgInitStatus_success);
        mvwSession = mvwSession1;
        def1.initMsg();

        MvwSession mvwSession2 = new MvwSession(getContext(), def2.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession2);

        init_timeout = 0;
        while (!def2.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def2.msgInitStatus_success);
        TestApiTool.mvwSession1 = mvwSession2;

        mvwSession1.setMvwDefaultKeyWords(1);
        mvwSession2.setMvwDefaultKeyWords(1);

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession2.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession1, def1.mvwPcm_first, def2);
//        assertTrue(def2.msgVwWakeup);
//        assertEquals(0, def2.getnMvwId());
//        assertEquals(4, def2.getnMvwScene());
        assertFalse(def2.msgVwWakeup);
        def2.initMsg();

        tool.loadPcmAndAppendAudioData(mvwSession2, def2.mvwPcm_global, def1);
//        assertTrue(def1.msgVwWakeup);
//        assertEquals(0, def1.getnMvwId());
//        assertEquals(1, def1.getnMvwScene());
        assertFalse(def2.msgVwWakeup);

        mvwSession1.release();
        mvwSession2.release();
    }

    /**
     * 销毁一个实例再唤醒另一个
     */
    public void test_mvw_mInst5() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession1;
        def.initMsg();

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession2);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession1 = mvwSession2;

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession2.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        mvwSession1.release();
    }

    /**
     * 两个实例加载同一份资源
     */
    public void test_mvw_mInst6() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession1;
        def.initMsg();

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession2);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession1 = mvwSession2;

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession2.start(4);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        mvwSession1.release();
        mvwSession2.release();
    }

    /**
     * 两个实例连续轮流送音频
     */
    public void test_mvw_mInst7() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession1;
        def.initMsg();

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession2);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession1 = mvwSession2;

        mvwSession1.setMvwDefaultKeyWords(1);
        mvwSession2.setMvwDefaultKeyWords(2);

        err = mvwSession1.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = mvwSession2.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 20; i++) {
            tool.loadPcmAndAppendAudioData(mvwSession1, def.mvwPcm_global, def);
            assertTrue(def.msgVwWakeup);
            assertEquals(0, def.getnMvwId());
            assertEquals(1, def.getnMvwScene());
            def.initMsg();

            tool.loadPcmAndAppendAudioData(mvwSession2, def.mvwPcm_confirm, def);
            assertTrue(def.msgVwWakeup);
            assertEquals(0, def.getnMvwId());
            assertEquals(2, def.getnMvwScene());
            def.initMsg();
        }
    }

    /**
     * 第一个实例创建失败，第二个实例创建成功，唤醒
     */
    public void test_mvw_mInst8() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertFalse(def.msgInitStatus_success);
        assertTrue(def.msgInitStatus_fail);
        def.initMsg();

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession2);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession2;

        err = mvwSession2.start(2);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession2, def.mvwPcm_confirm, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(2, def.getnMvwScene());

        mvwSession1.release();
        mvwSession2.release();
    }

    /**
     * 第一个实例创建成功，第二个实例创建失败，分别唤醒
     */
    public void test_mvw_mInst9() {
        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir_second);
        assertNotNull(mvwSession2);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession2;
        def.initMsg();

        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir_wrong);
        assertNotNull(mvwSession1);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertFalse(def.msgInitStatus_success);
        assertTrue(def.msgInitStatus_fail);

        err = mvwSession2.start(1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(mvwSession2, def.mvwPcm_global, def);
        assertTrue(def.msgVwWakeup);
        assertEquals(0, def.getnMvwId());
        assertEquals(1, def.getnMvwScene());

        mvwSession1.release();
        mvwSession2.release();
    }

    public void test_mvw_mThreads() throws Exception {
        TestStabilityMVW t = new TestStabilityMVW(getContext(), 600);
        t.isWriteToActivity = 0;

        t.test_multiThreading();
    }

    public void test_mvw_mThreads1() throws Exception {
        TestStabilityMVW t = new TestStabilityMVW(getContext(), 600);
        t.isWriteToActivity = 0;

        t.test_multiThreading1();
    }

    public void test_mvw_random() {
        TestStabilityMVW t = new TestStabilityMVW(getContext(), 3600*15);
        t.isWriteToActivity = 0;

        t.testRandom();
        assertTrue(t.def.msgVwWakeup);
        assertEquals(0, t.def.getnMvwId());
        assertEquals(1, t.def.getnMvwScene());
    }

    /**
     * getInstance并初始化成功后release，再次调用getInstance，由于release接口中没有
     * 将MvwSession实例变量赋null，此时不会进行初始化，再调用setThreshold, stop,
     * release, addStartScene接口崩溃，调用start返回成功
     */
    public void atest_BUG2016083108242() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        mvwSession = mvwSession1;
        def.initMsg();

        mvwSession1.release();

        MvwSession mvwSession2 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession2);

        init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }

        mvwSession2.setThreshold(1, 0, 1);
        mvwSession2.start(1);
        mvwSession2.stop();
        mvwSession2.release();
    }

    /**
     * 设置不存在的nMvwId，多次调用setThreshold接口，随机出现崩溃
     */
    public void atest_BUG2016082204628() {
        MvwSession mvwSession1 = new MvwSession(getContext(), def.iMvwListener, def.resDir);
        assertNotNull(mvwSession1);

        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);
        TestApiTool.mvwSession = mvwSession1;
        def.initMsg();

        Random random = new Random();

        for (int i = 0; i < 500; i++) {
            int scene = random.nextInt(15) + 1;
            int id = random.nextInt(10);
            int threshold = random.nextInt(1000);
            mvwSession1.setThreshold(scene, id, threshold);
        }
    }

}
