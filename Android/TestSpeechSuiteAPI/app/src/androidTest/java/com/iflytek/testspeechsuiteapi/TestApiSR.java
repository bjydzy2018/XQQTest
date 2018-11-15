package com.iflytek.testspeechsuiteapi;

import android.content.Context;
import android.os.Environment;
import android.telephony.TelephonyManager;
import android.test.AndroidTestCase;
import com.iflytek.testspeechsuiteapi.mvw.DefMVW;
import com.iflytek.testspeechsuiteapi.sr.DefSR;
import com.iflytek.testspeechsuiteapi.sr.TestStabilitySR;
import com.iflytek.testspeechsuiteapi.tts.DefTTS;
import com.iflytek.speech.ISSErrors;
import com.iflytek.sr.SrSession;
import org.json.JSONObject;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

/**
 * 接口测试用例。
 * 以test开头的用例都会被执行。
 */
public class TestApiSR extends AndroidTestCase {

    private String resDir;
    int err = -1;

    Tool tool;
    DefSR def;



    @Override
    protected void setUp() throws Exception {
        super.setUp();

        tool = TestApiTool.tool;
        def = TestApiTool.defSR;
        resDir = TestApiTool.defSR.resDir;
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();

        TestApiTool.defSR.initMsg();
        if (TestApiTool.srSession != null) {
            TestApiTool.srSession.release();
            TestApiTool.srSession = null;
        }
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v
     * appendAudioData: 中文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     */
    public void test_sr_101() throws Exception {
        int i = 0;
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(new JSONObject(def.srResult).getString("text").contains("我想关闭电话"));

        def.initMsg();
        srSession.release();
    }

    /**
     * getInstance: context=v(application context), srListener=v, resDir=v,imei=v
     * appendAudioData: 中文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     */
    public void atest_sr_102() throws Exception {
        int i = 0;
        SrSession srSession = SrSession.getInstance(getContext().getApplicationContext(), def
                .isrListener, resDir, getIMEI());
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("我想关闭电话"));
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=iv(资源不存在)
     * <p>
     * 返回错误码
     */
    public void atest_sr_103() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, def
                .resDir_wrong, getIMEI());

        int i = 0;
        while (!def.msgFileNotFound && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgFileNotFound);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/test.hcliu/dict/dict_M-500.txt"), 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.sleep(8000);
        assertFalse(def.msgUploadDictCloud);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=iv(资源路径用反斜杠)
     *
     * @throws Exception
     */
    public void atest_sr_104() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, Environment
                .getExternalStorageDirectory().getAbsolutePath()
                + "\\iflytek\\res\\sr", getIMEI());

        int i = 0;
        while (!def.msgFileNotFound && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgFileNotFound);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/test.hcliu/dict/dict_M-500.txt"), 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.sleep(8000);
        assertFalse(def.msgUploadDictCloud);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v("")
     * appendAudioData: 中文音频
     * <p>
     * 返回错误码
     * <p>
     * fail IMEI问题
     */
    public void abort_test_sr_105() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, def.resDir, "");

        int i = 0;
        while (!def.msgFileNotFound && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgFileNotFound);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/test.hcliu/dict/dict_M-500.txt"), 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.sleep(8000);
        assertFalse(def.msgUploadDictCloud);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v, imei=v("")
     * appendAudioData: 中文音频
     * <p>
     * 返回错误码
     * <p>
     * fail IMEI问题
     */
    public void abort_test_sr_106() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, def.resDir, "");

        int i = 0;
        while (!def.msgFileNotFound && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(def.msgFileNotFound);
        tool.sleep(50);
        assertFalse(def.msgError);
        def.msgInitStatus = false;

        srSession.release();

        srSession = SrSession.getInstance(getContext(), def.isrListener, resDir, getIMEI());
        i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("我想关闭电话"));
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v("")
     * appendAudioData: 中文音频
     * <p>
     * 该用例需在atest_sr_105之后执行。
     * <p>
     * fail. 当设置了IMEI再release之后，获取实例时IMEI传入空仍然能正常识别，release并没有将该值清掉。
     *
     * @throws Exception
     */
    public void abort_test_sr_107() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir, "");

        int i = 0;
        while (!def.msgFileNotFound && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(def.msgFileNotFound);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * getInstance: context=v(activity context), srListener=v, iAcousLang=v
     * (ISS_SR_ACOUS_LANG_VALUE_ENGLISH),resDir=v,imei=v
     * appendAudioData: 英文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     */
    public void atest_sr_201() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_ENGLISH, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_en, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("mobile number"));
    }

    /**
     * getInstance: context=v(application context), srListener=v, iAcousLang=v
     * (ISS_SR_ACOUS_LANG_VALUE_ENGLISH),resDir=v,imei=v
     * appendAudioData: 英文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     */
    public void atest_sr_202() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext().getApplicationContext(), def
                .isrListener, SrSession.ISS_SR_ACOUS_LANG_VALUE_ENGLISH, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_en, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("mobile number"));
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=iv(资源不存在)
     * <p>
     * 返回错误码
     */
    public void atest_sr_203() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir_wrong, getIMEI());

        int i = 0;
        while (!def.msgFileNotFound && i++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgFileNotFound);
        tool.sleep(50);
        assertFalse(def.msgError);
        def.msgFileNotFound = false;

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/test.hcliu/dict/dict_M-500.txt"), 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.sleep(8000);
        assertFalse(def.msgUploadDictCloud);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * getInstance: context=v(activity context), srListener=v, iAcousLang=v
     * (ISS_SR_ACOUS_LANG_VALUE_ENGLISH),resDir=iv(路径用反斜杠),imei=v
     * appendAudioData: 英文音频
     */
    public void atest_sr_204() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_ENGLISH, Environment.getExternalStorageDirectory()
                .getAbsolutePath()
                + "\\iflytek\\res\\sr", getIMEI());

        int i = 0;
        while (!def.msgFileNotFound && i++ < 500) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgFileNotFound);
        tool.sleep(50);
        assertFalse(def.msgError);
        def.msgFileNotFound = false;

        err = srSession.uploadDict(tool.readFile(Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/test.hcliu/dict/dict_M-500.txt"), 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.sleep(8000);
        assertFalse(def.msgUploadDictCloud);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * getInstance: context=v(activity context), srListener=v, iAcousLang=v
     * (ISS_SR_ACOUS_LANG_VALUE_MANDARIN),resDir=v,imei=v
     * appendAudioData: 中文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     */
    public void atest_sr_205() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("我想关闭电话"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_POI),iMode =v(ISS_SR_MODE_MIX_REC),szCmd =v(null)
     * appendAudioData: 科大讯飞
     */
    public void atest_sr_301() throws Exception {
        int i = 0;
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_SELECT, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_iflytek);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("科大讯飞"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_POI),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 打电话给张三
     * uploadDict: 带有联系人张三的词典
     */
    public void atest_sr_302() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict1, 0);
        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_POI, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_callZhangSan);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("打电话给张三"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("map"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_POI),iMode =v(ISS_SR_MODE_MIX_REC),szCmd =v(null)
     * appendAudioData: 张三
     * uploadDict: 带有联系人张三的词典
     */
    public void atest_sr_303() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict1, 0);
        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_CONTACTS, SrSession.ISS_SR_MODE_LOCAL_REC,
                null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_zhangSan);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("张三"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("telephone"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_CONTACTS),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 科大讯飞
     */
    public void atest_sr_304() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONTACTS, SrSession.ISS_SR_MODE_LOCAL_REC,
                null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_iflytek);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("科大讯飞"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("telephone"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_SELECT),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 第一个
     *
     * @throws Exception
     */
    public void atest_sr_305() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_SELECT, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_first);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("第一个"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("itemSelect"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_SELECT),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 科大讯飞
     *
     * @throws Exception fail 给出的service为stock
     */
    public void fail_test_sr_306() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_SELECT, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_iflytek);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("科大讯飞"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("itemSelect"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_SELECT),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 确认
     *
     * @throws Exception
     */
    public void atest_sr_307() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM, SrSession.ISS_SR_MODE_LOCAL_REC,
                null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_confirm);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("确定"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("cmd"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_SELECT),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 科大讯飞
     *
     * @throws Exception fail 给出的service为stock
     */
    public void fail_test_sr_308() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM, SrSession.ISS_SR_MODE_LOCAL_REC,
                null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_iflytek);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("科大讯飞"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("cmd"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_SELECT),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 接听
     *
     * @throws Exception
     */
    public void atest_sr_309() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ANSWER_CALL, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_answer);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("接听"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("cmd"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_SELECT),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 科大讯飞
     *
     * @throws Exception
     */
    public void atest_sr_310() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ANSWER_CALL, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_iflytek);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("科大讯飞"));
        assertTrue(new JSONObject(def.srResult).getString("service").contains("stock"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_SELECT_MVW),iMode =v(ISS_SR_MODE_CLOUD_REC),szCmd =v(null)
     * appendAudioData: 第一个
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     * c. 识别结果来自本地
     */
    public void atest_sr_311() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_SELECT_MVW, SrSession.ISS_SR_MODE_LOCAL_REC,
                null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_first);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("第一个"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_SELECT_MVW),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v(null)
     * appendAudioData: 科大讯飞
     * <p>
     * 无识别结果
     *
     * @throws Exception
     */
    public void atest_st_312() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_SELECT_MVW, SrSession.ISS_SR_MODE_LOCAL_REC,
                null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_iflytek);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertFalse(def.msgResult);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_CONFIRM_MVW),iMode =v(ISS_SR_MODE_CLOUD_REC),szCmd =v(null)
     * appendAudioData: 确定
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     * c. 识别结果来自本地
     */
    public void atest_sr_313() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_confirm, def);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("确定"));
    }

    public void atest_sr_314() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_iflytek, def);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertFalse(def.msgResult);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ANSWER_CALL_MVW),iMode =v(ISS_SR_MODE_CLOUD_REC),szCmd =v(null)
     * appendAudioData: 接听
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     * c. 识别结果来自本地
     */
    public void atest_sr_315() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ANSWER_CALL_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_answer, def);

        err = srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1000) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("接听"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ANSWER_CALL_MVW),iMode =v(ISS_SR_MODE_CLOUD_REC),szCmd =v(null)
     * appendAudioData: 科大讯飞
     * <p>
     * 无识别结果
     *
     * @throws Exception
     */
    public void atest_sr_316() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ANSWER_CALL_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_iflytek, def);

        err = srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1000) {
            tool.sleep(10);
        }

        assertFalse(def.msgResult);
    }

    /**
     * szScene=iv("awneifq")
     */
    public void atest_sr_317() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start("awneifq", SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * szScene=iv("")
     */
    public void atest_sr_318() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start("", SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * szScene=iv(null)
     */
    public void atest_sr_319() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(null, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * szScene=iv(50个字符)
     */
    public void atest_sr_320() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(new DefTTS().getRandomCHStr(50), SrSession.ISS_SR_MODE_LOCAL_REC,
                null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * 命令词超长
     * <p>
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =v({"grm":
     * [{"dictname":"cmd","dictcontant": [{ "name": "司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝", "id": 0 }]}]})
     * appendAudioData: 司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝
     * <p>
     * 未收到error回调
     */
    public void fail_test_sr_321() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": " +
                        "\"司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝\", \"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(100);
        assertTrue(def.msgError);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_INFO, def.srErrCode);
    }

    /**
     * 测试各场景的命令词模式，看结果是否一致（命令词模式会忽略场景）
     *
     * @throws Exception
     */
    public void abort_test_sr_322() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_REC,
                "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十似\", " +
                        "\"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
        def.initMsg();

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM, SrSession
                .ISS_SR_MODE_LOCAL_CMDLIST, "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{" +
                " \"name\": \"司小四要拿四十似\", \"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
        def.initMsg();

        err = srSession.start(SrSession.ISS_SR_SCENE_SELECT, SrSession.ISS_SR_MODE_LOCAL_CMDLIST,
                "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十似\", " +
                        "\"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
        def.initMsg();

        err = srSession.start(SrSession.ISS_SR_SCENE_POI, SrSession.ISS_SR_MODE_LOCAL_CMDLIST,
                "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十似\", " +
                        "\"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
        def.initMsg();

        err = srSession.start(SrSession.ISS_SR_SCENE_ANSWER_CALL, SrSession
                .ISS_SR_MODE_LOCAL_CMDLIST, "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{" +
                " \"name\": \"司小四要拿四十似\", \"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
        def.initMsg();

        err = srSession.start(SrSession.ISS_SR_SCENE_ANSWER_CALL_MVW, SrSession
                .ISS_SR_MODE_LOCAL_CMDLIST, "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{" +
                " \"name\": \"司小四要拿四十似\", \"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
        def.initMsg();

        err = srSession.start(SrSession.ISS_SR_SCENE_SELECT_MVW, SrSession
                .ISS_SR_MODE_LOCAL_CMDLIST, "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{" +
                " \"name\": \"司小四要拿四十似\", \"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
        def.initMsg();

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_CMDLIST, "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{" +
                " \"name\": \"司小四要拿四十似\", \"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
        def.initMsg();
    }

    /**
     * iMode=iv(-1)
     */
    public void atest_sr_323() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, -1, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * iMode=iv(17)
     */
    public void atest_sr_324() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, 17, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * iMode=iv(Integer.MIN_VALUE)
     */
    public void atest_sr_325() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, Integer.MIN_VALUE, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * iMode=iv(Integer.MAX_VALUE)
     */
    public void atest_sr_326() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, Integer.MAX_VALUE, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * ({"grm": [{"dictname":"cmd","dictcontant": ["name": "司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝", "id": 0
     * }]}]})
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     */
    public void atest_sr_327() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format);
        assertEquals(10106, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     */
    public void atest_sr_328() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format1);
        assertEquals(10106, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回error回调
     * <p>
     * 命令词没有name字段也会返回成功
     * <p>
     */
    public void atest_sr_329() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format2);

//        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_FMT, err);
        tool.sleep(100);
//        assertTrue(def.msgError);
//        assertEquals(10002, def.srErrCode);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     * <p>
     * fail 崩溃
     */
    public void crash_test_sr_330() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format3);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_FMT, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     * <p>
     * fail 崩溃
     */
    public void crash_test_sr_331() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format4);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_FMT, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     * <p>
     * fail 崩溃
     */
    public void crash_test_sr_332() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format5);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_FMT, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     * <p>
     * fail 崩溃
     */
    public void crash_test_sr_333() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format6);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_FMT, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     */
    public void abort_test_sr_334() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format7);
        assertEquals(10106, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     */
    public void abort_test_sr_335() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format8);
        assertEquals(10106, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =iv格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_FMT
     */
    public void abort_test_sr_336() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_wrong_format9);
        assertEquals(10106, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =v(10000条)
     * <p>
     * start接口中编命令词过程是异步的，因此如果命令词上传过多，识别不会有结果
     */
    public void abort_test_sr_337() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                def.srCmd_10000);
        assertEquals(10106, err);

        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);

        err = srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertFalse(def.msgResult);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_REC),szCmd =v({"grm":
     * [{"dictname":"cmd","dictcontant": [{ "name": "司小四要拿四十似", "id": 0 }]}]})
     * appendAudioData: 司小四要拿四十似
     *
     * @throws Exception
     */
    public void abort_test_sr_338() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_REC,
                "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十似\", " +
                        "\"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_CLOUD_REC),szCmd =v({"grm":
     * [{"dictname":"cmd","dictcontant": [{ "name": "司小四要拿四十似", "id": 0 }]}]})
     * appendAudioData: 司小四要拿四十似
     *
     * @throws Exception
     */
    public void atest_sr_339() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_CLOUD_REC,
                "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十似\", " +
                        "\"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertFalse(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_CMDLIST),szCmd =v({"grm":
     * [{"dictname":"cmd","dictcontant": [{ "name": "司小四要拿四十似", "id": 0 }]}]})
     * appendAudioData: 司小四要拿四十似
     *
     * 废弃 cmdlist功能不要了
     *
     * @throws Exception
     */
    public void abort_test_sr_340() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CMDLIST_WITHALL, SrSession
                        .ISS_SR_MODE_LOCAL_CMDLIST,
                "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十似\", " +
                        "\"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);
        srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_NLP),szCmd =v({"nlptext":
     * "今天天气怎么样？"})
     *
     * @throws Exception
     */
    public void atest_sr_341() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_NLP,
                "{\"nlptext\": \"今天天气怎么样？\"}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 格式错误
     * <p>
     * fail destroy卡住
     */
    public void atest_sr_342() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_NLP, def
                .srNlp_wrong_format);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_FMT, err);
    }

    /**
     * 格式错误
     */
    public void atest_sr_343() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_NLP, def
                .srNlp_wrong_format1);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_FMT, err);
    }

    /**
     * 格式错误
     */
    public void atest_sr_344() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_NLP, def
                .srNlp_wrong_format2);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_INFO, err);
    }

    /**
     * 格式错误
     */
    public void atest_sr_345() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_NLP, def
                .srNlp_wrong_format3);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_INFO, err);
    }

    /**
     * 格式错误
     */
    public void atest_sr_346() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_NLP, def
                .srNlp_wrong_format4);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_INFO, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_ALL),iMode =v(ISS_SR_MODE_LOCAL_NLP),
     * szCmd =v({"grm": [{"dictname":"cmd","dictcontant": [{ "name": "司小四要拿四十似", "id": 0 }]}]})
     */
    public void atest_sr_347() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_NLP,
                "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十似\", " +
                        "\"id\": 0 }]}]}");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_JSON_INFO, err);
    }

    /**
     * start: szScene=v(ISS_SR_SCENE_BUILD_GRM_MVW),iMode =v(ISS_SR_MODE_LOCAL_REC),
     * szCmd =v({"Keywords": [{"KeyWordId":0,"KeyWord":"灵犀灵犀","DefaultThreshold40":0},
     * {"KeyWordId":1,"KeyWord" : "司小四要拿四十似","DefaultThreshold40": 0}]})
     */
    public void atest_sr_348() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_BUILD_GRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, "{\"Keywords\": [{\"KeyWordId\":0,\"KeyWord\":\"灵犀灵犀\"," +
                "\"DefaultThreshold40\":0}, {\"KeyWordId\":1,\"KeyWord\" : \"知豆你好\"," +
                "\"DefaultThreshold40\": 0}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, new DefMVW().mvwPcm_LingXi, def);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("灵犀灵犀"));
        def.initMsg();

        err = srSession.start(SrSession.ISS_SR_SCENE_BUILD_GRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, "{\"Keywords\": [{\"KeyWordId\":0,\"KeyWord\":\"灵犀灵犀\"," +
                "\"DefaultThreshold40\":0}, {\"KeyWordId\":1,\"KeyWord\" : \"司小四要拿四十似\"," +
                "\"DefaultThreshold40\": 0}]}");
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_siXiaoSi, def);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("司小四要拿四十似"));
    }

    /**
     * uploadDict: szList=v({"grm": [{"dictname": "contact","dictcontant": [{ "name": "白亚伟",
     * "id": 0 }]}]}), bOnlyUploadToCloud=0
     * appendAudioData: 打电话给白亚伟
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     */
    public void atest_sr_401() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict("{\"grm\": [{\"dictname\": \"contact\",\"dictcontant\": [{ " +
                "\"name\": \"白亚伟\", \"id\": 0 }]}]}", 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_CONTACTS, SrSession.ISS_SR_MODE_LOCAL_REC,
                null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_callBaiYaWei, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("打个电话给白亚伟"));
    }

    /**
     * uploadDict: szList=iv词典类别不支持({"grm": [{"dictname": "asdf","dictcontant": [{ "name": "白亚伟",
     * "id": 0 }]}]}), bOnlyUploadToCloud=0
     * <p>
     * 返回ISS_ERROR_INVALID_JSON_INFO
     *
     * 初始化成功后上传错误格式词典，小概率出现uoload接口返回10108无效句柄
     */
    public void atest_sr_402() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 2000) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict("{\"grm\": [{\"dictname\": \"asdf\",\"dictcontant\": [{ " +
                "\"name\": \"白亚伟\", \"id\": 0 }]}]}", 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO);
    }

    /**
     * uploadDict: szList=iv词典格式错误({"grm": {"dictname": "contact","dictcontant": [{ "name":
     * "白亚伟", "id": 0 }]}}), bOnlyUploadToCloud=0
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_403() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict("{\"grm\": {\"dictname\": \"contact\",\"dictcontant\": [{ " +
                "\"name\": \"白亚伟\", \"id\": 0 }]}}", 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_404() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_FMT);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_FMT);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_405() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format1, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     *
     * fail 返回success
     */
    public void fail_test_sr_406() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format2, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_FMT);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_FMT);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_407() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format3, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_408() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format4, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
//        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_FMT);
//        if (def.uploadDictCloudErrCode != 10106)
//            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_FMT);

        srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, "");
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_callBaiYaWei, def);

        err = srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     *
     * fail  返回success
     */
    public void fail_test_sr_409() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format5, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_FMT);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_FMT);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void fail_test_sr_410() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format6, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_FMT);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_FMT);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_411() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format7, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_412() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format8, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO);
    }

    /**
     * uploadDict: szList=iv词典格式错误
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_413() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_wrong_format9, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }
        assertTrue(def.msgUploadDictLocal_ISS_ERROR_INVALID_JSON_INFO);
        if (def.uploadDictCloudErrCode != 10106)
            assertTrue(def.msgUploadDictCloud_ISS_ERROR_INVALID_JSON_INFO);
    }

    /**
     * uploadDict: szList=iv(null), bOnlyUploadToCloud=0
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_414() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(null, 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

    }

    /**
     * uploadDict: szList=iv(""), bOnlyUploadToCloud=0
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void atest_sr_415() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict("", 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

    }

    /**
     * uploadDict: szList=v({"grm": [{"dictname": "contact","dictcontant": [{ "name": "白亚伟",
     * "id": 0 }]}]}), bOnlyUploadToCloud=1000
     * appendAudioData: 打电话给白亚伟
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     * c. 词典只上传到云端
     */
    public void atest_sr_416() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict("{\"grm\": [{\"dictname\": \"contact\",\"dictcontant\": [{ " +
                "\"name\": \"白亚伟\", \"id\": 0 }]}]}", 1000);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        // &&: 上传云端  &&: 上传云+端
//        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
//            tool.sleep(10);
//        }
        tool.sleep(25000);
        assertFalse(def.msgUploadDictLocal);
        assertTrue(def.msgUploadDictCloud);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONTACTS, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_callBaiYaWei, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("白亚伟"));
    }

    /**
     * uploadDict: szList=v({"grm": [{"dictname": "contact","dictcontant": [{ "name": "白亚伟",
     * "id": 0 }]}]}), bOnlyUploadToCloud=-1
     * <p>
     * 返回SUCCESS
     */
    public void atest_sr_417() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict("{\"grm\": [{\"dictname\": \"contact\",\"dictcontant\": [{ " +
                "\"name\": \"白亚伟\", \"id\": 0 }]}]}", -1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * 持续送大音频
     */
    public void atest_sr_601() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_MANDARIN, resDir, getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONTACTS, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int bytesEachTimeSend = 140000;

        for (int j = 0; j < 150; j++) {
            byte[] byteData = null;
            try {
                DataInputStream data = new DataInputStream(new FileInputStream(new DefMVW()
                        .mvwPcm_KaiYi_long));
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
                int ret = srSession.appendAudioData(mid);
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

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * szParam: ISS_SR_PARAM_CITY
     * szParamValue: "BEIJING"
     * appendAudioData: 附近的加油站
     * <p>
     * fail 设置不生效
     *
     * @throws Exception
     */
    public void atest_sr_701() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_CITY, "beijing");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_fuJinJiaYouZhan, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
    }

    /**
     * szParam: ISS_SR_PARAM_CITY
     * szParamValue: "HNDF#R"
     * appendAudioData: 附近的加油站
     * <p>
     * fail 异常值未检查
     *
     * @throws Exception
     */
    public void fail_test_sr_702() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_CITY, "HNDF#R");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * szParam: ISS_SR_PARAM_CITY
     * szParamValue: "123"
     * appendAudioData: 附近的加油站
     * <p>
     * fail 异常值未检查
     *
     * @throws Exception
     */
    public void fail_test_sr_703() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_CITY, "123");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);
    }

    /**
     * szParam: ISS_SR_PARAM_LONGTITUDE
     * szParamValue: "116,23"
     * szParam: ISS_SR_PARAM_LATITUDE
     * szParamValue: "39,54"
     * appendAudioData: 附近的加油站
     *
     * @throws Exception
     */
    public void atest_sr_704() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_LONGTITUDE, "116.23");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_LATITUDE, "39.54");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_fuJinJiaYouZhan, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
    }

    /**
     * szParam: ISS_SR_PARAM_LONGTITUDE
     * szParamValue: "116,23"
     * appendAudioData: 附近的加油站
     * <p>
     * fail 语义中有一个『state:fail』字段，待查
     *
     * @throws Exception
     */
    public void atest_sr_705() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_LONGTITUDE, "116.23");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_fuJinJiaYouZhan, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
    }

    /**
     * szParam: ISS_SR_PARAM_LATITUDE
     * szParamValue: "39,54"
     * appendAudioData: 附近的加油站
     * <p>
     * fail 语义中有一个『state:fail』字段，待查
     *
     * @throws Exception
     */
    public void atest_sr_706() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_LATITUDE, "39.54");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_fuJinJiaYouZhan, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
    }

    /**
     * szParam: ISS_SR_PARAM_LONGTITUDE
     * szParamValue: "500"
     * szParam: ISS_SR_PARAM_LATITUDE
     * szParamValue: "500"
     * appendAudioData: 附近的加油站
     * <p>
     * fail 给出了佳木斯市的结果
     *
     * @throws Exception
     */
    public void fail_test_sr_707() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_LONGTITUDE, "500");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_LATITUDE, "500");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_fuJinJiaYouZhan, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(false);
    }

    /**
     * szParam: ISS_SR_PARAM_LONGTITUDE
     * szParamValue: "-500"
     * szParam: ISS_SR_PARAM_LATITUDE
     * szParamValue: "-500"
     * appendAudioData: 附近的加油站
     * <p>
     * 语义中有fail字段
     *
     * @throws Exception
     */
    public void fail_test_sr_708() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_LONGTITUDE, "-500");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_LATITUDE, "-500");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_fuJinJiaYouZhan, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "20000"
     * appendAudioData: 语音开始前有21秒的静音段
     * <p>
     * 送入的音频超过20s后appendAudioData接口返回INVALID_CALL，回调ResponseTimeOut消息
     *
     * @throws Exception
     */
    public void atest_sr_709() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "20000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_21, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 20000 && time_appendAudioData < 21000);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgResponseTimeOut);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "20000"
     * appendAudioData: 语音开始前有21秒的静音段
     * start之后设置参数
     * <p>
     * 送入的音频超过20s后appendAudioData接口返回INVALID_CALL，回调ResponseTimeOut消息
     *
     * @throws Exception
     */
    public void atest_sr_709_1() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "20000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_21, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 20000 && time_appendAudioData < 21000);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgResponseTimeOut);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "1000"
     * appendAudioData: 语音开始前有1.5秒的静音段
     * <p>
     * 送入的音频超过1s后appendAudioData接口返回INVALID_CALL，回调ResponseTimeOut消息
     *
     * @throws Exception
     */
    public void atest_sr_710() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "1000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_1_5, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 1000 && time_appendAudioData < 1300);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgResponseTimeOut);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "2800"
     * <p>
     * 送入的音频超过30000s后appendAudioData接口返回INVALID_CALL，回调ResponseTimeOut消息
     * <p>
     * fail 不会收到ResponseTimeOut消息
     *
     * @throws Exception
     */
    public void fail_test_sr_711() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "28000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_100, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 29000 && time_appendAudioData < 31000);

        err = srSession.endAudioData();
        tool.sleep(50);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgResponseTimeOut);
    }

    /**
     * ISS_SR_PARAM_RESPONSE_TIMEOUT异常参数值
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "100001"
     *
     * @throws Exception
     */
    public void fail_test_sr_712() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "999");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * ISS_SR_PARAM_RESPONSE_TIMEOUT异常参数值
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "100001"
     * <p>
     * fail 接口不会返回fail，并且送长时间空音频也不会回调ResponseTimeOut消息
     *
     * @throws Exception
     */
    public void fail_test_sr_713() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "100001");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);

        // 以下只是为了看是否能收到ResponseTimeOut消息
        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_100, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 29000 && time_appendAudioData < 31000);

        err = srSession.endAudioData();
        tool.sleep(50);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgResponseTimeOut);
    }

    /**
     * ISS_SR_PARAM_RESPONSE_TIMEOUT异常参数值
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "-10"
     *
     * @throws Exception
     */
    public void atest_sr_714() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "-10");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * ISS_SR_PARAM_RESPONSE_TIMEOUT异常参数值
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "asdf"
     * <p>
     * fail 接口不会返回fail，并且送长时间空音频也不会回调ResponseTimeOut消息
     *
     * @throws Exception
     */
    public void fail_test_sr_715() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "asdf");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);

        // 以下只是为了看是否能收到ResponseTimeOut消息
        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_100, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 29000 && time_appendAudioData < 31000);

        err = srSession.endAudioData();
        tool.sleep(50);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgResponseTimeOut);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "1000.123"
     *
     * @throws Exception
     */
    public void atest_sr_716() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_RESPONSE_TIMEOUT, "1000.123");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_1_5, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 1000 && time_appendAudioData < 1300);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgResponseTimeOut);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "3000"
     * appendAudioData: 语音持续3.1秒
     * <p>
     * 送入的音频超过3s后回调SpeechTimeOut消息
     *
     * @throws Exception
     */
    public void atest_sr_717() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT, "3000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_SpeechTimeOut_3_1, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 3000 && time_appendAudioData < 3500);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgSpeechTimeOut);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "20000"
     * appendAudioData: 语音持续21秒
     * <p>
     * 送入的音频超过20s后回调SpeechTimeOut消息
     *
     * @throws Exception
     */
    public void atest_sr_718() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT, "20000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_SpeechTimeOut_21, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 20000 && time_appendAudioData < 21000);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgSpeechTimeOut);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "2900"
     *
     * @throws Exception
     */
    public void atest_sr_719() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT, "2900");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "20001"
     *
     * @throws Exception
     */
    public void atest_sr_720() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT, "20001");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "-10"
     *
     * @throws Exception
     */
    public void atest_sr_721() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT, "-10");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "asdf"
     *
     * @throws Exception
     */
    public void atest_sr_722() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT, "asdf");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * szParam: ISS_SR_PARAM_RESPONSE_TIMEOUT
     * szParamValue: "3000.123"
     * appendAudioData: 语音持续3.1秒
     * <p>
     * 送入的音频超过3s后回调SpeechTimeOut消息
     *
     * @throws Exception
     */
    public void atest_sr_723() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TIMEOUT, "3000.123");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_SpeechTimeOut_3_1, def);
        System.out.println("送录音时长：:" + time_appendAudioData);
        assertTrue(time_appendAudioData > 3000 && time_appendAudioData < 3500);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        // 回调消息收到的较晚，在这之前appendAudioData已经返回INVALID_CALL，此时也已经停止送录音
        tool.sleep(300);
        assertTrue(def.msgSpeechTimeOut);
    }

    /**
     * szParam: ISS_SR_PARAM_SPEECH_TAIL
     * szParamValue: "650"
     * 送的音频分两段，两段之间有【650ms+300ms误差量】静音
     * <p>
     * 识别结果只是第一段语音的
     * 总共送的音频超过第一段音频长度+650ms
     *
     * @throws Exception
     */
    public void atest_sr_724() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TAIL, "650");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.uploadDict(def.dict1, 0);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_SpeechTail0_65, def);
        assertTrue(time_appendAudioData > 1700);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(new JSONObject(def.srResult).getString("text").contains("张三"));
        assertEquals("张三", new JSONObject(def.srResult).getString("text"));
    }

    /**
     * szParam: ISS_SR_PARAM_SPEECH_TAIL
     * szParamValue: "5000"
     * 送的音频分两段，两段之间有【5000ms+300ms误差量】静音
     * <p>
     * 识别结果只是第一段语音的
     * 总共送的音频超过第一段音频长度+5s
     *
     * @throws Exception
     */
    public void atest_sr_725() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TAIL, "5000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.uploadDict(def.dict1, 0);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_SpeechTail5, def);
        System.out.println(time_appendAudioData);
        assertTrue(time_appendAudioData > 5140);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(new JSONObject(def.srResult).getString("text").contains("张三"));
        assertEquals("张三", new JSONObject(def.srResult).getString("text"));
    }

    /**
     * szParam: ISS_SR_PARAM_SPEECH_TAIL
     * szParamValue: "5000"
     * 送的音频分两段，两段之间有【5000ms+100ms误差量】静音
     * <p>
     * 识别结果只是第一段语音的
     * 总共送的音频超过第一段音频长度+5s
     *
     * @throws Exception
     */
    public void atest_sr_725_1() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict1, 0);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TAIL, "5000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_SpeechTail5, def);
        System.out.println(time_appendAudioData);
        assertTrue(time_appendAudioData > 5000);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(new JSONObject(def.srResult).getString("text").contains("张三"));
        assertEquals("张三", new JSONObject(def.srResult).getString("text"));
    }

    /**
     * szParam: ISS_SR_PARAM_SPEECH_TAIL
     * szParamValue: "649"
     * <p>
     * fail 小于最小值650仍然生效
     *
     * @throws Exception
     */
    public void fail_test_sr_726() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TAIL, "649");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);

        err = srSession.uploadDict(def.dict1, 0);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_SpeechTail5, def);
        System.out.println(time_appendAudioData);
        assertTrue(time_appendAudioData > 5000);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(new JSONObject(def.srResult).getString("text").contains("张三"));
        assertEquals("张三", new JSONObject(def.srResult).getString("text"));
    }

    /**
     * szParam: ISS_SR_PARAM_SPEECH_TAIL
     * szParamValue: "5001"
     *
     * @throws Exception
     */
    public void atest_sr_727() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TAIL, "5001");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * szParam: ISS_SR_PARAM_SPEECH_TAIL
     * szParamValue: "-10"
     *
     * @throws Exception
     */
    public void atest_sr_728() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TAIL, "-10");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * szParam: ISS_SR_PARAM_SPEECH_TAIL
     * szParamValue: "asdf"
     *
     * @throws Exception
     */
    public void atest_sr_729() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TAIL, "asdf");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);
    }

    /**
     * szParam: ISS_SR_PARAM_SPEECH_TAIL
     * szParamValue: "5000.123"
     * 送的音频分两段，两段之间有【第一段语音的后端点+5000ms+50ms误差量】静音
     * <p>
     * 识别结果只是第一段语音的
     *
     * @throws Exception
     */
    public void atest_sr_730() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam(SrSession.ISS_SR_PARAM_SPEECH_TAIL, "5000.123");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.uploadDict(def.dict1, 0);

        // &&: 上传云端  ||: 上传云+端
        while (!def.msgUploadDictCloud || !def.msgUploadDictLocal) {
            tool.sleep(10);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_SpeechTail5, def);
        System.out.println(time_appendAudioData);
        assertTrue(time_appendAudioData > 5000);

        err = srSession.endAudioData();
        tool.sleep(50);

        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(new JSONObject(def.srResult).getString("text").contains("张三"));
        assertEquals("张三", new JSONObject(def.srResult).getString("text"));
    }

    /**
     * 使用唤醒模式
     * szParam: ISS_SR_PARAM_MVW_TIMEOUT
     * szParamValue: "1"
     * appendAudioData: 语音结束前有2毫秒长度
     *
     * @throws Exception
     */
    public void atest_sr_731() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam("mvwtimeout", "1");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_MvwTimeout2, def);
        System.out.println(time_appendAudioData);
        assertTrue(time_appendAudioData > 1000);

        tool.sleep(100);
        assertTrue(def.msgError);
        assertTrue(def.srErrCode == 10015);
    }

    /**
     * 使用唤醒模式
     * szParam: ISS_SR_PARAM_MVW_TIMEOUT
     * szParamValue: "5000"
     * appendAudioData: 语音结束前有5000毫秒长度
     *
     * @throws Exception
     */
    public void atest_sr_732() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam("mvwtimeout", "10000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_MvwTimeout10, def);
        System.out.println(time_appendAudioData);
        assertTrue(time_appendAudioData > 10000);

        tool.sleep(100);
        assertTrue(def.msgError);
        assertTrue(def.srErrCode == 10015);
    }

    /**
     * 使用唤醒模式
     * szParam: ISS_SR_PARAM_MVW_TIMEOUT
     * szParamValue: "10000"
     * appendAudioData: 语音结束前有10000毫秒长度
     *
     * @throws Exception
     */
    public void atest_sr_732_1() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.setParam("mvwtimeout", "10000");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_MvwTimeout10, def);
        System.out.println(time_appendAudioData);
        assertTrue(time_appendAudioData > 10000);

        tool.sleep(100);
        assertTrue(def.msgError);
        assertTrue(def.srErrCode == 10015);
    }

    /**
     * 使用唤醒模式
     * szParam: ISS_SR_PARAM_MVW_TIMEOUT
     * szParamValue: "0"
     * appendAudioData: 语音结束前有100秒长度
     *
     * @throws Exception
     */
    public void atest_sr_733() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam("mvwtimeout", "0");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_100, def);
        System.out.println(time_appendAudioData);

        tool.sleep(100);
        assertFalse(def.srErrCode == 10015);
    }

    /**
     * 使用唤醒模式
     * szParam: ISS_SR_PARAM_MVW_TIMEOUT
     * szParamValue: "-10"
     * <p>
     * fail 设置负数和设置0的效果一样
     *
     * @throws Exception
     */
    public void fail_test_sr_734() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.setParam("mvwtimeout", "-10");
        assertEquals(ISSErrors.ISS_ERROR_FAIL, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def
                .srPcm_ResponseTimeOut_100, def);
        System.out.println(time_appendAudioData);

        assertFalse(def.srErrCode == 10015);
    }

    /**
     * 使用唤醒模式
     * szParam: ISS_SR_MVW_PARAM_AEC
     * szParamValue: ISS_SR_PARAM_VALUE_ON
     * <p>
     * log中输入的音频（文件名有org）不为空
     *
     * @throws Exception
     */
    public void atest_sr_735() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        String vwLogPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/iflytek/res/Active/MVWRes/log/Vw";
        File file = new File(vwLogPath);
        if (file.exists()) {
            tool.deleteDirectory(vwLogPath);
        }
        file.mkdir();

        err = srSession.setParam("mvw_enable_aec", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def.srPcm_confirm,
                def);
        System.out.println(time_appendAudioData);

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
     * 使用唤醒模式
     * szParam: ISS_SR_MVW_PARAM_AEC
     * szParamValue: ISS_SR_PARAM_VALUE_ON
     * szParam: ISS_SR_MVW_PARAM_AEC
     * szParamValue: ISS_SR_PARAM_VALUE_OFF
     * <p>
     * log中输入的音频（文件名有org）不为空
     *
     * @throws Exception
     */
    public void atest_sr_736() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        String vwLogPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/iflytek/res/Active/MVWRes/log/Vw";
        File file = new File(vwLogPath);
        if (file.exists()) {
            tool.deleteDirectory(vwLogPath);
        }
        file.mkdir();

        err = srSession.setParam("mvw_enable_lsa", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.setParam("mvw_enable_lsa", "off");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def.srPcm_confirm,
                def);
        System.out.println(time_appendAudioData);

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
     * 使用唤醒模式
     * szParam: ISS_SR_MVW_PARAM_LSA
     * szParamValue: ISS_SR_PARAM_VALUE_ON
     * <p>
     * log中输入的音频（文件名有org）不为空
     *
     * @throws Exception
     */
    public void atest_sr_737() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        String vwLogPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/iflytek/res/Active/MVWRes/log/Vw";
        File file = new File(vwLogPath);
        if (file.exists()) {
            tool.deleteDirectory(vwLogPath);
        }
        file.mkdir();

        err = srSession.setParam("mvw_enable_lsa", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def.srPcm_confirm,
                def);
        System.out.println(time_appendAudioData);

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
     * 使用唤醒模式
     * szParam: ISS_SR_MVW_PARAM_LSA
     * szParamValue: ISS_SR_PARAM_VALUE_ON
     * szParam: ISS_SR_MVW_PARAM_LSA
     * szParamValue: ISS_SR_PARAM_VALUE_OFF
     * <p>
     * log中输入的音频（文件名有org）不为空
     *
     * @throws Exception
     */
    public void atest_sr_738() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        String vwLogPath = Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/iflytek/res/Active/MVWRes/log/Vw";
        File file = new File(vwLogPath);
        if (file.exists()) {
            tool.deleteDirectory(vwLogPath);
        }
        file.mkdir();

        err = srSession.setParam("mvw_enable_lsa", "on");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.setParam("mvw_enable_lsa", "off");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = srSession.start(SrSession.ISS_SR_SCENE_CONFIRM_MVW, SrSession
                .ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        int time_appendAudioData = tool.loadPcmAndAppendAudioData(srSession, def.srPcm_confirm,
                def);
        System.out.println(time_appendAudioData);

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
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v
     * appendAudioData: 中文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常识别
     * <p>
     * fail 第二次初始化无法成功
     */
    public void fail_test_sr_501() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);
        def.initMsg();

        err = srSession.initService();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("我想关闭电话"));
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v
     * <p>
     * start返回ISS_ERROR_INVALID_CALL
     */
    public void atest_sr_502() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        assertFalse(def.msgInitStatus);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v
     * start: szScene=v(ISS_SR_SCENE_POI),iMode =v(ISS_SR_MODE_MIX_REC),szCmd =v(null)
     * appendAudioData: 两个不同的中文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 第二次正常识别
     */
    public void atest_sr_503() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_callBaiYaWei, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        assertFalse(def.msgResult);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_callZhangSan);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("打电话给张三"));
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v
     * start: szScene=v(ISS_SR_SCENE_POI),iMode =v(ISS_SR_MODE_MIX_REC),szCmd =v(null)
     * appendAudioData: 两个不同的中文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 第二次正常识别
     */
    public void atest_sr_504() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int j = 0;
        while (!def.msgInitStatus && j++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.srPcm_callBaiYaWei));
            byte[] byteData;
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            int location = 0;
            int position = byteData.length;
            int i = 0;

            while (location < byteData.length && location < position && i++ < 5) {
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                byte[] mid = new byte[1600];
                if (end > 1600) {
                    System.arraycopy(byteData, location, mid, 0, 1600);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                err = srSession.appendAudioData(mid);

                Thread.sleep(50);
                location = location + 1600;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertFalse(def.msgResult);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_callZhangSan);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        j = 0;
        while (!def.msgResult && j++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("打电话给张三"));
    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v
     * start: szScene=v(ISS_SR_SCENE_POI),iMode =v(ISS_SR_MODE_MIX_REC),szCmd =v(null)
     * appendAudioData: 两个不同的中文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. stop后不会收到识别结果回调消息
     */
    public void atest_sr_505() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int j = 0;
        while (!def.msgInitStatus && j++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.srPcm_callBaiYaWei));
            byte[] byteData;
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            int location = 0;
            int position = byteData.length;
            int i = 0;

            while (location < byteData.length && location < position && i++ < 5) {
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                byte[] mid = new byte[1600];
                if (end > 1600) {
                    System.arraycopy(byteData, location, mid, 0, 1600);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                err = srSession.appendAudioData(mid);

                Thread.sleep(50);
                location = location + 1600;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertFalse(def.msgResult);

        err = srSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.sleep(8000);
        assertFalse(def.msgResult);

    }

    /**
     * getInstance: context=v(activity context), srListener=v, resDir=v,imei=v
     * start: szScene=v(ISS_SR_SCENE_POI),iMode =v(ISS_SR_MODE_MIX_REC),szCmd =v(null)
     * appendAudioData: 两个不同的中文音频
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. stop后不会收到识别结果回调消息
     */
    public void atest_sr_506() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int j = 0;
        while (!def.msgInitStatus && j++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.srPcm_callBaiYaWei));
            byte[] byteData;
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            int location = 0;
            int position = byteData.length;
            int i = 0;

            while (location < byteData.length && location < position && i++ < 5000) {
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                byte[] mid = new byte[1600];
                if (end > 1600) {
                    System.arraycopy(byteData, location, mid, 0, 1600);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                err = srSession.appendAudioData(mid);
                if (i <= 10) {
                    assertEquals(ISSErrors.ISS_SUCCESS, err);
                    assertFalse(def.msgError);
                } else {
                    assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
                }
                Thread.sleep(50);
                location = location + 1600;

                if (i == 10) {
                    err = srSession.stop();
                    assertEquals(ISSErrors.ISS_SUCCESS, err);
                    assertFalse(def.msgError);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        tool.sleep(8000);
        assertFalse(def.msgResult);

    }

    /**
     * 识别时stop。
     * <p/>
     * getInstance: context=v(activity context), srListener=v, resDir=v, imei=v
     * appendAudioData: 中文音频
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. stop后不会收到识别结果回调消息
     */
    public void atest_sr_507() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.stop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        assertFalse(def.msgResult);

        tool.sleep(8000);
        assertFalse(def.msgResult);

    }

    /**
     * uploadDict: szList=v({"grm": [{"dictname": "contact","dictcontant": [{ "name": "白亚伟",
     * "id": 0 }]}]}), bOnlyUploadToCloud=0
     * <p>
     * a. 接口全部返回ISS_SUCCESS
     * b. 能收到云端和本地上传词典成功的消息
     */
    public void atest_sr_508() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        tool.sleep(20);

        err = srSession.uploadDict(def.dict1, 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        tool.sleep(50);
        assertFalse(def.msgError);

    }

    public void atest_sr_509() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());

        tool.sleep(20);

        err = srSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    public void atest_sr_510() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());

        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.uploadDict(def.dict_5000, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        // &&: 上传云端  ||: 上传云+端
        while ((!def.msgUploadDictCloud || !def.msgUploadDictLocal) && i++ < 10) {
            tool.sleep(10);
        }
        assertFalse(def.msgUploadDictLocal);

        err = srSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
    }

    public void atest_sr_511() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int j = 0;
        while (!def.msgInitStatus && j++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.srPcm_callBaiYaWei));
            byte[] byteData;
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            int location = 0;
            int position = byteData.length;
            int i = 0;

            while (location < byteData.length && location < position && i++ < 5000) {
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                byte[] mid = new byte[1600];
                if (end > 1600) {
                    System.arraycopy(byteData, location, mid, 0, 1600);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                int ret = srSession.appendAudioData(mid);
                Thread.sleep(50);
                location = location + 1600;

                if (i == 6) {
                    err = srSession.release();
                    assertEquals(ISSErrors.ISS_SUCCESS, err);
                    assertFalse(def.msgError);
                    break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        tool.sleep(8000);
        assertFalse(def.msgResult);
    }

    public void atest_sr_512() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.sleep(8000);

        assertFalse(def.msgResult);

    }

    public void atest_sr_513() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        err = srSession.stop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

    }

    public void atest_sr_514() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        err = srSession.appendAudioData(null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

    }

    public void atest_sr_515() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.endAudioData();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);

    }

    public void atest_sr_516() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.release();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        err = srSession.uploadDict("{\"grm\": [{\"dictname\": \"contact\",\"dictcontant\": [{ " +
                "\"name\": \"白亚伟\", \"id\": 0 }]}]}", 0);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_CALL, err);
    }

    /**
     * (0,12)*100
     */
    public void atest_sr_524() throws Exception {
        for (int j = 0; j < 20; j++) {
            SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                    getIMEI());
//            int i = 0;
//            while (!def.msgInitStatus && i++ < 1200) {
//                tool.sleep(10);
//            }
//            TestApiTool.srSession = srSession;
//            assertTrue(def.msgInitStatus_success);
//            def.initMsg();
            tool.sleep(1000);

            err = srSession.release();
//            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
    }

    /**
     * 0,(5,6)*100,5,7,8,12
     */
    public void atest_sr_525() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        for (int j = 0; j < 100; j++) {
            err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = srSession.stop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(new JSONObject(def.srResult).getString("text").contains("我想关闭电话"));
    }

    /**
     * a. 送音频时stop
     * b. 0,(5,7,6)*1000,12
     */
    public void atest_sr_526() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        int k = 0;
        while (!def.msgInitStatus && k++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        for (int j = 0; j < 100; j++) {
            err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            try {
                DataInputStream data = new DataInputStream(new FileInputStream(def
                        .srPcm_callBaiYaWei));
                byte[] byteData;
                byteData = new byte[data.available()];
                data.read(byteData);
                data.close();

                int location = 0;
                int position = byteData.length;
                int i = 0;

                while (location < byteData.length && location < position && i++ < 5) {
                    int end1 = byteData.length - location;
                    int end2 = position - location;
                    int end = 0;
                    if (end1 >= end2)
                        end = end2;
                    else {
                        end = end1;
                    }

                    byte[] mid = new byte[1600];
                    if (end > 1600) {
                        System.arraycopy(byteData, location, mid, 0, 1600);
                    } else {
                        System.arraycopy(byteData, location, mid, 0, end);
                    }
                    int ret = srSession.appendAudioData(mid);
                    assertEquals(ISSErrors.ISS_SUCCESS, ret);
                    Thread.sleep(50);
                    location = location + 1600;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            srSession.stop();
        }
    }

    /**
     * 开启log，一个线程上传词典，一个线程送录音，一个线程循环start-stop
     */
    public void atest_sr_mThreads1() {
        TestStabilitySR t = new TestStabilitySR(getContext(), 600);
        t.ifWriteToActivity = 0;

        t.test_multiThreading1();
    }

    /**
     * 开启log，一个线程上传词典，一个线程正常送录音识别
     */
    public void atest_sr_mThreads2() {
        TestStabilitySR t = new TestStabilitySR(getContext(), 600);
        t.ifWriteToActivity = 0;

        t.test_multiThreading2();
    }

    public void atest_sr_random() {
        TestStabilitySR t = new TestStabilitySR(getContext(), 600);
        t.ifWriteToActivity = 0;

        t.testRandom();
    }

    public void atest_sr_stopBeforeSpeechEnd() throws Exception {
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, def.resDir,
                getIMEI());
        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        def.msgInitStatus = false;

        byte[] byteData = null;
        try {
            DataInputStream data = new DataInputStream(new FileInputStream(def.srPcm_callZhang));
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);

        int location = 0;
        int position = byteData.length;

        int bytesEachTimeSend = 320; // 每次送的音频数
        int sleepTimeEachTimeSend = 10; // 每次送音频后的间隔时间，废弃
        int appendTimes = 0; // 送音频次数
        int ret_appendAudioData = ISSErrors.ISS_SUCCESS; // appendAudioData接口的返回值

        while (location < byteData.length && location < position) {
            appendTimes++;
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
            ret_appendAudioData = srSession.appendAudioData(mid);
            switch (ret_appendAudioData) {
                case ISSErrors.ISS_SUCCESS:
//                            ILog.i("SrSession.appendAudio", "ISS_SUCCESS", 1);
                    break;
                case ISSErrors.ISS_ERROR_INVALID_PARA:
                    ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_PARA",
                            1);
                    break;
                case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                    ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_HANDLE",
                            1);
                    break;
                case ISSErrors.ISS_ERROR_INVALID_CALL:
                    ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_CALL",
                            1);
                    break;
                case ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER:
                    ILog.e("SrSession.appendAudio", "ISS_ERROR_NO_ENOUGH_BUFFER",
                            1);
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
            location = location + bytesEachTimeSend;
            if (location >= 1800 * 32)
                break;
            tool.sleep(10);
        }

        srSession.stop();
    }

    /**
     * 中文识别后release，再构建英文实例进行识别
     * <p>
     * fail 英文识别无结果
     *
     * @throws Exception
     */
    public void atest_sr_ch_en() throws Exception {
        int i = 0;
        SrSession srSession = SrSession.getInstance(getContext(), def.isrListener, resDir,
                getIMEI());
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_MIX_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);
        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_closePhone, def);

        err = srSession.endAudioData();

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }
        assertTrue(new JSONObject(def.srResult).getString("text").contains("我想关闭电话"));
        srSession.release();
        def.initMsg();

        srSession = SrSession.getInstance(getContext(), def.isrListener, SrSession
                .ISS_SR_ACOUS_LANG_VALUE_ENGLISH, resDir, getIMEI());
        i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }
        TestApiTool.srSession = srSession;
        assertTrue(def.msgInitStatus_success);

        err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_LOCAL_REC, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
        tool.sleep(50);
        assertFalse(def.msgError);

        tool.loadPcmAndAppendAudioData(srSession, def.srPcm_en, def);

        err = srSession.endAudioData();
        tool.sleep(50);
        assertFalse(def.msgError);

        i = 0;
        while (!def.msgResult && i++ < 1200) {
            tool.sleep(10);
        }

        assertTrue(new JSONObject(def.srResult).getString("text").contains("mobile number"));

        srSession.release();

        def.initMsg();


    }

    private String getIMEI() throws Exception {
        TelephonyManager tm = (TelephonyManager) (getContext().getSystemService(Context
                .TELEPHONY_SERVICE));
        String imei = tm.getDeviceId();
        if (imei == null || imei.length() == 0) {
            imei = "ifly" + Math.random() + Math.random() + Math.random()
                    + Math.random() + Math.random() + Math.random()
                    + Math.random() + Math.random() + Math.random()
                    + Math.random() + "tek";
        }

        return imei;
    }

}
