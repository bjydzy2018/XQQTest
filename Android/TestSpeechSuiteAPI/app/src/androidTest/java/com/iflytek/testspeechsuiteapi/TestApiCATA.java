package com.iflytek.testspeechsuiteapi;

import android.test.AndroidTestCase;


import android.os.Environment;
import com.iflytek.cata.CataIndexInst;
import com.iflytek.cata.CataSearchInst;
import com.iflytek.cata.CataSession;
import com.iflytek.testspeechsuiteapi.cata.DefCATA;
import com.iflytek.testspeechsuiteapi.cata.SegValue;
import com.iflytek.testspeechsuiteapi.cata.TestStabilityCATA;
import com.iflytek.speech.ISSErrors;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.Vector;

/**
 * 接口测试用例。
 * 以test开头的用例都会被执行。
 */
public class TestApiCATA extends AndroidTestCase {
    DefCATA def;
    Tool tool;
    int err = -1;
    String Tag = "TestApiCATA";


    @Override
    protected void setUp() throws Exception {
        super.setUp();

        def = TestApiTool.defCATA;
        tool = TestApiTool.tool;
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();

        TestApiTool.defCATA.initMsg();
    }

    /**
     * getInstance:context=v(activity context),cataInitListener=v,resDir=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_101() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        ILog.i(Tag, "after create index", 0);
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();
        ILog.i(Tag, "after create search", 0);

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

//        cataIndexInst.create("test.bin", def.iCataListener);
//        cataIndexInst.drop();

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
        System.out.println(3);
        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

//        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * getInstance:context=v(application context),cataInitListener=v,resDir=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_102() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext().getApplicationContext(),
                def.iCataInitListener, def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * getInstance:context=v(activity context),cataInitListener=iv(null),resDir=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_103() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), null, def.resDir);
        tool.sleep(500);

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * getInstance:context=v(activity context),cataInitListener=v,resDir=iv(null)
     * <p>
     * 创建cataSession时传入的资源路径并不会使用，创建索引时才会加载资源，因此即使参数出错也
     * 不会有初始化失败的回调。
     */
    public void test_cata_104() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                null);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * getInstance:context=v(activity context),cataInitListener=v,resDir=iv("")
     * <p>
     * 创建cataSession时传入的资源路径并不会使用，创建索引时才会加载资源，因此即使参数出错也
     * 不会有初始化失败的回调。
     */
    public void test_cata_105() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener, "");
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * getInstance:context=v(activity context),cataInitListener=v,resDir=iv(资源路径用反斜杠
     * <p>
     * 创建cataSession时传入的资源路径并不会使用，创建索引时才会加载资源，因此即使参数出错也
     * 不会有初始化失败的回调。
     */
    public void test_cata_106() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                Environment.getExternalStorageDirectory().getAbsolutePath()
                        + "\\iflytek\\res\\cata");
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        assertTrue(def.msgInitStatus_success);

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_FILE_NOT_FOUND, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * resName=iv(""), cataSearchListener=v
     * <p>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void test_cata_201() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=iv(null), cataSearchListener=v
     * <p/>
     * 返回ISS_ERROR_INVALID_PARA
     */
    public void test_cata_202() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create(null, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=v("sa8dgf89agwef626fg6q2g3f98agfh23897gf"), cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_203() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("sa8dgf89agwef626fg6q2g3f98agfh23897gf", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("sa8dgf89agwef626fg6q2g3f98agfh23897gf", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("4"), cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_204() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("4", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("4", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv("tes t.bin"), cataSearchListener=v
     */
    public void test_cata_205() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("tes t.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("tes t.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv("t|e?s\t.bin"), cataSearchListener=v
     * <p>
     * 错误码不合理
     */
    public void test_cata_206() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("t|e<>/\':;?s\\t.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_FILE_NOT_FOUND, err);
    }

    /**
     * resName=iv("测试.bin"), cataSearchListener=v
     */
    public void test_cata_207() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("测试.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("测试.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"),cataSearchListener=iv(null)
     * <p/>
     * 返回错误码
     */
    public void test_cata_208() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv(""), langType=v(ISS_CATA_LANG_ENGLISH ) , cataSearchListener=v
     * <p/>
     * 返回ISS_ERROR_RES_DATA
     */
    public void test_cata_301() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.createEx("", CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=iv(null), cataSearchListener=v
     * <p/>
     * 返回ISS_ERROR_RES_DATA
     */
    public void test_cata_302() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.createEx(null, CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=v("qy3249c7h23jt$J&%Y@$c284ctrwyq34my7#@&(#7YN4TFnq7AGHSIOU23<"), langType=v
     * (ISS_CATA_LANG_ENGLISH ) , cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_303() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("sa8dgf89agwef626fg6q2g3f98agfh23897gf", CataSession
                .ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("sa8dgf89agwef626fg6q2g3f98agfh23897gf", CataSession
                .ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("4"), langType=v(ISS_CATA_LANG_ENGLISH ) , cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_304() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("4", CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("4", CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv("tes t.bin"), langType=v(ISS_CATA_LANG_ENGLISH ) , cataSearchListener=v
     */
    public void test_cata_305() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("tes t.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("tes t.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv("t|e?s\t.bin"), langType=v(ISS_CATA_LANG_ENGLISH ) , cataSearchListener=v
     */
    public void test_cata_306() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.createEx("t|e?s\\t.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"), langType=v(ISS_CATA_LANG_MANDARIN) , cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_307() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_MANDARIN, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin", CataSession.ISS_CATA_LANG_MANDARIN, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"), langType=iv(-3) , cataSearchListener=v
     * <p/>
     * 返回错误码
     */
    public void test_cata_308() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.createEx("test.bin", -3, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=v("test.bin"), langType=iv(100) , cataSearchListener=v
     */
    public void test_cata_309() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.createEx("test.bin", 100, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=v("test.bin"), langType=v(ISS_CATA_LANG_ENGLISH ), cataSearchListener=iv(null)
     * <p/>
     * 返回错误码
     */
    public void test_cata_310() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            System.out.println("adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        System.out.println("searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv(""), cataSearchListener=v
     * <p/>
     * 返回ISS_ERROR_RES_DATA
     */
    public void test_cata_401() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=iv(null), cataSearchListener=v
     * <p/>
     * 返回ISS_ERROR_RES_DATA
     */
    public void test_cata_402() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate(null, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=v("sa8dgf89agwef626fg6q2g3f98agfh23897gf"), cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_403() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("sa8dgf89agwef626fg6q2g3f98agfh23897gf", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("sa8dgf89agwef626fg6q2g3f98agfh23897gf", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("4"), cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_404() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("4", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("4", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv("tes t.bin"), cataSearchListener=v
     */
    public void test_cata_405() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("tes t.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("tes t.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv("t|e?s\t.bin"), cataSearchListener=v
     */
    public void test_cata_406() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("t|e?s\\t.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"),cataSearchListener=iv(null)
     * <p/>
     * 返回错误码
     */
    public void test_cata_407() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=iv(""), langType=v(ISS_CATA_LANG_ENGLISH ) , cataSearchListener=v
     * <p/>
     * 返回ISS_ERROR_RES_DATA
     */
    public void test_cata_501() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreateEx("", CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=iv(null), cataSearchListener=v
     * <p/>
     * 返回ISS_ERROR_RES_DATA
     */
    public void test_cata_502() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreateEx(null, CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=v("qy3249c7h23jt$J&%Y@$c284ctrwyq34my7#@&(#7YN4TFnq7AGHSIOU23<"), langType=v
     * (ISS_CATA_LANG_ENGLISH ) , cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_503() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreateEx("sa8dgf89agwef626fg6q2g3f98agfh23897gf", CataSession
                .ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("sa8dgf89agwef626fg6q2g3f98agfh23897gf", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("4"), langType=v(ISS_CATA_LANG_ENGLISH ) , cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_504() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreateEx("4", CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("4", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("tes t.bin"), langType=v(ISS_CATA_LANG_ENGLISH) , cataSearchListener=v
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_505() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreateEx("tes t.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("tes t.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("t|e?s\t.bin"), langType=v(ISS_CATA_LANG_ENGLISH) , cataSearchListener=v
     * <p/>
     * 返回错误码
     */
    public void test_cata_506() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreateEx("t|e?s\\t.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"), langType=v(ISS_CATA_LANG_MANDARIN) , cataSearchListener=v
     */
    public void test_cata_507() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreateEx("test.bin", CataSession.ISS_CATA_LANG_MANDARIN, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"), langType=iv(-3) , cataSearchListener=v
     * <p/>
     * 返回错误码
     */
    public void test_cata_508() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreateEx("testt.bin", -3, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=v("test.bin"), langType=iv(30) , cataSearchListener=v
     *
     * @throws JSONException
     */
    public void test_cata_509() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreateEx("testt.bin", 100, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * resName=v("test.bin"), langType=v(ISS_CATA_LANG_ENGLISH ), cataSearchListener=iv(null)
     *
     * @throws JSONException
     */
    public void test_cata_510() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreateEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("123456789");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=code,query=2345,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * data=iv(null)
     * <p/>
     * 返回错误码
     */
    public void test_cata_601() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreateEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.addIdxEntity(null);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * data=iv("")
     */
    public void test_cata_602() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreateEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.addIdxEntity("");
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * data=iv:
     * [
     * {
     * "field" : "id",
     * "mode" : 2,
     * "val" : [ "0" ]
     * },
     * {
     * "field" : "song",
     * "mode" : 1,
     * "val" : [ "忘情水" ]
     * ]
     */
    public void test_cata_603() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   {\n" +
                "      \"field\" : \"id\",\n" +
                "      \"mode\" : 2,\n" +
                "      \"val\" : [ \"0\" ]\n" +
                "   },\n" +
                "   {\n" +
                "      \"field\" : \"song\",\n" +
                "      \"mode\" : 1,\n" +
                "      \"val\" : [ \"忘情水\" ]\n" +
                "]";
        err = cataIndexInst.addIdxEntity(index);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * data=iv:
     * {
     * "field" : "id",
     * "mode" : 2,
     * "val" : [ "0" ]
     * }
     */
    public void test_cata_604() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "{\n" +
                "      \"field\" : \"id\",\n" +
                "      \"mode\" : 2,\n" +
                "      \"val\" : [ \"0\" ]\n" +
                "}";
        err = cataIndexInst.addIdxEntity(index);

        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);
    }

    /**
     * data=iv:
     * [
     * [{
     * "field" : "id",
     * "mode" : 2,
     * "val" : [ "0" ]
     * }]
     * ]
     */
    public void test_cata_605() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   [{\n" +
                "      \"field\" : \"id\",\n" +
                "      \"mode\" : 2,\n" +
                "      \"val\" : [ \"0\" ]\n" +
                "   }]\n" +
                "]";
        try {
            err = cataIndexInst.addIdxEntity(index);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
        assertEquals(15060, err);
    }

    /**
     * data=iv:
     * [
     * {
     * "field" : "id",
     * "mode" : 2,
     * "val" :  "0"
     * }
     * ]
     * <p>
     * fail 返回success
     */
    public void test_cata_606() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   {\n" +
                "      \"field\" : \"id\",\n" +
                "      \"mode\" : 1,\n" +
                "      \"val\" :  \"0\" \n" +
                "   }\n" +
                "]";
        err = cataIndexInst.addIdxEntity(index);
        assertEquals(err, 15060);
    }

    /**
     * data=iv:
     * [
     * {
     * "field" : "id",
     * "mode" : 2,
     * "val" : [1.5]
     * }
     * ]
     */
    public void test_cata_607() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   {\n" +
                "      \"field\" : \"id\",\n" +
                "      \"mode\" : 2,\n" +
                "      \"val\" : [1.5]\n" +
                "   }\n" +
                "]";
        err = cataIndexInst.addIdxEntity(index);
        assertEquals(err, 15060);
    }

    /**
     * data=iv:
     * [
     * {
     * "field" : "id",
     * "mode" : "2",
     * "val" : ["0" ]
     * }
     * ]
     */
    public void test_cata_608() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   {\n" +
                "      \"field\" : \"id\",\n" +
                "      \"mode\" : \"2\",\n" +
                "      \"val\" : [\"0\" ]\n" +
                "   }\n" +
                "]";
        err = cataIndexInst.addIdxEntity(index);
        assertEquals(err, 15060);
    }

    /**
     * data=iv:
     * [
     * {
     * "field" : 5,
     * "mode" : 2,
     * "val" : ["0" ]
     * }
     * ]
     */
    public void test_cata_609() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   {\n" +
                "      \"field\" : 5,\n" +
                "      \"mode\" : 2,\n" +
                "      \"val\" : [\"0\" ]\n" +
                "   }\n" +
                "]";
        err = cataIndexInst.addIdxEntity(index);
        assertEquals(err, 15060);
    }

    /**
     * data=iv:
     * [
     * {
     * "mode" : 2,
     * "val" : ["0" ]
     * }
     * ]
     */
    public void test_cata_610() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   {\n" +
                "      \"mode\" : 2,\n" +
                "      \"val\" : [\"0\" ]\n" +
                "   }\n" +
                "]";
        err = cataIndexInst.addIdxEntity(index);
        assertEquals(15060, err);
    }

    /**
     * data=iv:
     * [
     * {
     * "field" : "name",
     * "mode" : 2.2,
     * "val" : ["0" ]
     * }
     * ]
     * <p>
     * fail 仍然能搜索到结果
     */
    public void test_cata_611() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   {\n" +
                "      \"field\" : \"name\",\n" +
                "      \"mode\" : 2.5,\n" +
                "      \"val\" : [\"0\" ]\n" +
                "   }\n" +
                "]";
        err = cataIndexInst.addIdxEntity(index);
        assertEquals(15060, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(15008, err);
    }

    /**
     * data=iv:
     * [
     * {
     * "field" : "name",
     * "mode" : 2,
     * "val" : ["收到了卡松地噶我跟那说的话否啊我那个啊送牌啊山东顾问西南风狗啊二搜嘎结手动狗啊多少呢服务额啊农夫大概" ]
     * }
     * ]
     * <p>
     * 字符串超过128个字符无法搜到
     */
    public void test_cata_612() throws JSONException {

        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String index = "[\n" +
                "   {\n" +
                "      \"field\" : \"name\",\n" +
                "      \"mode\" : 1,\n" +
                "      \"val\" : " +
                "[\"收到收到给收到了卡松地收到了卡松地收到了卡松地收到了卡松地收到了卡松地噶我跟那说的话否否啊我那个跟那说的话否啊我那个啊送牌啊山东顾问西南风狗啊啊送牌否啊我那个跟那说的话否啊我那山东顾问西南风狗啊啊送牌啊山东顾问西南风狗啊二搜嘎结手动狗啊多少呢服务额啊农夫大概\" ]\n" +
                "   }\n" +
                "]";
        err = cataIndexInst.addIdxEntity(index);
//        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=收到了,fuzzy=1," +
                "searchType=SR)");
        System.out.println("ret:" + searchRet);
        assertEquals("", searchRet);
    }

    /**
     * mField=iv(""),mMode=1,mTexts="123"
     */
    public void test_cata_701() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts.remove(0);
        texts.add("55.5");
        segValues.add(new SegValue("", 1, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(15016, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field= ,query=55.5,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        searchRet = cataSearchInst.searchSync("(field=,query=55.5,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * mField=iv("qy3249c7h23jt$J&%Y@$c284ctrwyq34my7#@&(#7YN4TFnq7AGHSIOU23<"),mMode=1,mTexts="123"
     */
    public void test_cata_702() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts.remove(0);
        texts.add("55.5");
        segValues.add(new SegValue("qy3249c7h23jt$J&%Y@$c284ctrwyq34my7#@&(#7YN4TFnq7AGHSIOU23<",
                1, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(15020, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * mField=v("name"),mMode=iv(-1),mTexts="123"
     */
    public void test_cata_703() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts.remove(0);
        texts.add("55.5");
        segValues.add(new SegValue("code", -1, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(15019, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=code,query=55.5,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * mField=v("name"),mMode=iv(100),mTexts="123"
     */
    public void test_cata_704() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts.remove(0);
        texts.add("55.5");
        segValues.add(new SegValue("code", 100, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(15019, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=code,query=55.5,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }


    /**
     * mField=v("name"),mMode=1,mTexts=iv("123","")
     */
    public void test_cata_705() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts = new Vector<>();
        texts.add("58.9");
        texts.add("");
        segValues.add(new SegValue("code", 1, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        //ILog.i(Tag, "addIdxEntity:" + def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=code,query=58.9,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                if ("安徽音乐台".equals(name)) {
                    JSONArray code = j.getJSONArray("code");
                    assertEquals("58.9", (String) code.get(0));
                    assertEquals("", (String) code.get(1));
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * mField=v("name"),mMode=1,mTexts=v("123","456")
     */
    public void test_cata_706() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts = new Vector<>();
        texts.add("55.5");
        texts.add("66.6");
        segValues.add(new SegValue("code", 1, texts));

        System.out.println(def.getSegValuesContent(segValues));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=code,query=55.5,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                if ("安徽音乐台".equals(name)) {
                    JSONArray code = j.getJSONArray("code");
                    assertEquals("55.5", (String) code.get(0));
                    assertEquals("66.6", (String) code.get(1));
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * mField=v("name"),mMode=1,mTexts=iv("","")
     */
    public void test_cata_707() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts = new Vector<>();
        texts.add("");
        texts.add("");
        segValues.add(new SegValue("code", 1, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                if ("安徽音乐台".equals(name)) {
                    JSONArray code = j.getJSONArray("code");
                    assertEquals("", (String) code.get(0));
                    assertEquals("", (String) code.get(1));
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=code,query=,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        searchRet = cataSearchInst.searchSync("(field=code,query= ,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * mField=v("name"),mMode=1,mTexts=v(20个字符串)
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_708() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts = new Vector<>();
        texts.add("1");
        texts.add("2");
        texts.add("3");
        texts.add("4");
        texts.add("5");
        texts.add("6");
        texts.add("7");
        texts.add("8");
        texts.add("9");
        texts.add("10");
        texts.add("11");
        texts.add("12");
        texts.add("13");
        texts.add("14");
        texts.add("15");
        texts.add("16");
        texts.add("17");
        texts.add("18");
        texts.add("19");
        texts.add("20");
        segValues.add(new SegValue("code", 1, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                if ("安徽音乐台".equals(name)) {
                    JSONArray code = j.getJSONArray("code");
                    for (int q = 0; q < 20; q++) {
                        assertEquals("" + (q + 1), (String) code.get(q));
                    }
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * mField=v("name"),mMode=1,mTexts=v("qy3249c7h23jt$J&%Y@$c284ctrwyq34my7#@&
     * (#7YN4TFnq7AGHSIOU23<")
     * <p/>
     * a. 接口全部返回ISS_SUCCESS
     * b. 正常搜索
     */
    public void test_cata_709() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_1, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("安徽音乐台");
        segValues.add(new SegValue("name", 1, texts));

        texts = new Vector<>();
        texts.add("qy3249c7h23jt$J&%Y@$c284ctrwyq34my7#@&(#7YN4TFnq7AGHSIOU23<");
        segValues.add(new SegValue("code", 1, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=交通台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("123456789", code);
            assertEquals("交通台111", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        searchRet = cataSearchInst.searchSync("(field=code,query=c7h23jt$J&%Y@$c2,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);

            assertEquals("qy3249c7h23jt$J&%Y@$c284ctrwyq34my7#@&(#7YN4TFnq7AGHSIOU23<", code);
            assertEquals("安徽音乐台", name);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=iv("test.bin，yellow_page"), cataSearchListener=v
     */
    public void test_cata_801() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin，yellow_page", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=iv("test.bin;yellow_page"), cataSearchListener=v
     */
    public void test_cata_802() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin;yellow_page", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=iv(""), cataSearchListener=v
     */
    public void test_cata_803() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("", def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=iv(null), cataSearchListener=v
     */
    public void test_cata_804() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create(null, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=v("test.bin,test.bin,test.bin"), cataSearchListener=v
     */
    public void test_cata_805() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin,test.bin,test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
                String code = (String) j.getJSONArray("code").get(0);
                assertFalse("FM89.5".equals(code));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=v("test.bin"),cataSearchListener=iv(null)
     */
    public void test_cata_806() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
                String code = (String) j.getJSONArray("code").get(0);
                assertFalse("FM89.5".equals(code));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=iv("test.bin，yellow_page"), cataSearchListener=v
     */
    public void test_cata_901() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin，yellow_page", CataSession.ISS_CATA_LANG_ENGLISH,
                def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=iv("test.bin;yellow_page"), cataSearchListener=v
     */
    public void test_cata_902() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin;yellow_page", CataSession.ISS_CATA_LANG_ENGLISH,
                def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=iv(""), cataSearchListener=v
     */
    public void test_cata_903() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("", CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=iv(null), cataSearchListener=v
     */
    public void test_cata_904() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx(null, CataSession.ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=v("test.bin,test.bin,test.bin"), cataSearchListener=v
     */
    public void test_cata_905() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin,test.bin,test.bin", CataSession
                .ISS_CATA_LANG_ENGLISH, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        ILog.i(Tag, "searchSync ret:" + searchRet, 0);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("Anhui music station".equals(name));
                String code = (String) j.getJSONArray("code").get(0);
                assertFalse("FM89.5".equals(code));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resNames=v("test.bin"),cataSearchListener=iv(null)
     */
    public void test_cata_906() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("Anhui music station", name);
            assertEquals("music", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("Anhui music station".equals(name));
                String code = (String) j.getJSONArray("code").get(0);
                assertFalse("FM89.5".equals(code));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"), langType=v(ISS_CATA_LANG_MANDARIN) , cataSearchListener=v
     *
     * @throws Exception
     */
    public void test_cata_907() throws Exception {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin", CataSession.ISS_CATA_LANG_MANDARIN, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }
        //ILog.i(Tag, "searchSync ret:" + searchRet);

        err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        //ILog.i(Tag, "after del,searchSync ret:" + searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"), langType=iv(-3) , cataSearchListener=v
     *
     * @throws JSONException
     */
    public void test_cata_908() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin,test.bin,test.bin", -3, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        ILog.i(Tag, "searchSync ret:" + searchRet, 0);
//        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * resName=v("test.bin"), langType=iv(30) , cataSearchListener=v
     *
     * @throws JSONException
     */
    public void test_cata_909() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.createEx("test.bin", CataSession.ISS_CATA_LANG_ENGLISH, def
                .iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath_en, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.createEx("test.bin,test.bin,test.bin", 30, def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=music,fuzzy=1," +
                "searchType=SR)");
        ILog.i(Tag, "searchSync ret:" + searchRet, 0);
//        assertEquals("", searchRet);

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * query=iv(null)
     */
    public void test_cata_1001() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        ILog.i(Tag, "after get index", 0);
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();
        ILog.i(Tag, "after get search", 0);

        err = cataIndexInst.create("test.bin", def.iCataListener);
        ILog.i(Tag, "after create index", 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
        ILog.i(Tag, "after add index", 0);

        err = cataIndexInst.endIdxEntity();
        ILog.i(Tag, "after end index", 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", null);
        ILog.i(Tag, "after create search", 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync(null);
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);
    }

    /**
     * query=iv("")
     */
    public void test_cata_1002() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", null);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);
    }

    /**
     * paramtype: 1; paramvalue: 1
     * 使用SR模式搜索
     * 可以搜到同音字
     */
    public void test_cata_1201() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(1, 1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音悦台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音悦台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 6 && score > 5);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: 1; paramvalue: 1->0
     * 使用SR模式搜索
     * 无法搜到同音字
     */
    public void test_cata_1202() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(1, 1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(1, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音悦台,fuzzy=1," +
                "searchType=SR)");
        System.out.println(searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 5);
            assertTrue(score > 4);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音悦台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 5);
            assertTrue(score > 4);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: 1; paramvalue: 1
     * 使用KEYBOARD模式搜索
     * 无法搜到同音字
     */
    public void test_cata_1203() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(1, 1);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=安徽音悦台,fuzzy=1," +
                "searchType=KEYBOARD)");
        assertEquals("", searchRet);

        err = cataSearchInst.searchAsync("(field=name,query=安徽音悦台,fuzzy=1,searchType=KEYBOARD)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;


        assertEquals("", def.asyncSearchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: 2; paramvalue: 8
     * paramtype: 2; paramvalue: 15
     * 使用SR模式搜索
     * 得分为5的结果不会给出
     * 得分为10的结果不会给出
     */
    public void test_cata_1204() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(2, 8);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        assertEquals("", searchRet);

        err = cataSearchInst.searchAsync("(field=name,query=安徽音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score == 10);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }
        def.asyncSearchRet = null;

        err = cataSearchInst.setParam(2, 15);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        assertEquals("", searchRet);

        err = cataSearchInst.searchAsync("(field=name,query=安徽音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        assertEquals("", def.asyncSearchRet);

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(2, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: 2; paramvalue: 8
     * 使用KEYBOARD模式搜索
     *
     * @throws Exception
     */
    public void test_cata_1205() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(2, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=安徽音乐台,fuzzy=0," +
                "searchType=KEYBOARD)");
        System.out.println(searchRet);
        assertFalse("".equals(searchRet));

        err = cataSearchInst.searchAsync("(field=name,query=安徽音乐台,fuzzy=0,searchType=KEYBOARD)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        assertFalse("".equals(def.asyncSearchRet));

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("FM89.5");
        segValues.add(new SegValue("code", 2, texts));
        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(2, 0);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONArray first = ret.getJSONArray("FIRST");

            for (int i = 0; i < first.length(); i++) {
                JSONObject j = first.getJSONObject(i);
                String name = (String) j.getJSONArray("name").get(0);
                assertFalse("安徽音乐台".equals(name));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: 10; paramvalue: 10
     *
     * @throws JSONException
     */
    public void test_cata_1206() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(10, 10);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=安徽音乐台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score == 10);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=安徽音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score == 10);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: -10; paramvalue: -10
     *
     * @throws JSONException
     */
    public void test_cata_1207() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(-10, -10);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=安徽音乐台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score == 10);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=安徽音乐台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score == 10);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: 1; paramvalue: 10
     *
     * @throws JSONException
     */
    public void test_cata_1208() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(1, 10);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音悦台,fuzzy=1," +
                "searchType=SR)");
        System.out.println(searchRet);
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 5);
            assertTrue(score > 4);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音悦台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 5);
            assertTrue(score > 4);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: 1; paramvalue: -10
     *
     * @throws JSONException
     */
    public void test_cata_1209() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(1, -10);
        assertEquals(ISSErrors.ISS_ERROR_INVALID_PARA_VALUE, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音悦台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 5);
            assertTrue(score > 4);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音悦台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 5);
            assertTrue(score > 4);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * paramtype: 2; paramvalue: -10
     *
     * @throws JSONException
     */
    public void test_cata_1210() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.reCreate("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues), 0);
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.setParam(2, -10);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音悦台,fuzzy=1," +
                "searchType=SR)");
        try {
            JSONObject ret = new JSONObject(searchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 5);
            assertTrue(score > 4);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.searchAsync("(field=name,query=音悦台,fuzzy=1,searchType=SR)");
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        while (!def.msgResult) {
            tool.sleep(10);
        }
        def.msgResult = false;

        try {
            JSONObject ret = new JSONObject(def.asyncSearchRet);
            JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

            String code = (String) first.getJSONArray("code").get(0);
            String name = (String) first.getJSONArray("name").get(0);
            String tag = (String) first.getJSONArray("tag").get(0);
            double score = (Double.parseDouble(first.getString("score")));

            assertEquals("FM89.5", code);
            assertEquals("安徽音乐台", name);
            assertEquals("book", tag);
            assertTrue(score < 5);
            assertTrue(score > 4);
        } catch (JSONException e) {
            e.printStackTrace();
            throw e;
        }

        err = cataSearchInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 插入后删除
     */
    public void test_cata_1101() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        Vector<SegValue> segValues = new Vector<>();
        Vector<String> texts = new Vector<>();
        texts.add("234");
        segValues.add(new SegValue("code", 2, texts));

        err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValues));
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 插入后销毁索引
     */
    public void test_cata_1102() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 插入、构建后销毁索引
     */
    public void test_cata_1103() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 插入后销毁索引实例
     */
    public void test_cata_1104() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 插入、构建后销毁索引实例
     */
    public void test_cata_1105() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 创建索引实例后销毁索引
     */
    public void test_cata_1106() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * 创建索引实例后销毁实例
     */
    public void test_cata_1107() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_ERROR_INVALID_HANDLE, err);
    }

    /**
     * 插入后同步搜索
     */
    public void test_cata_1108() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);
    }

    /**
     * 销毁索引后同步搜索
     */
    public void test_cata_1109() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);
    }

    /**
     * 插入、销毁索引实例后同步搜索
     */
    public void test_cata_1110() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                "searchType=SR)");
        //ILog.i(Tag, "searchSync ret:" + searchRet);
        assertEquals("", searchRet);
    }

    /**
     * 重复同步搜索
     */
    public void test_cata_1112() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 100; i++) {
            String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                    "searchType=SR)");
            //ILog.i(Tag, "searchSync ret:" + searchRet);
            try {
                JSONObject ret = new JSONObject(searchRet);
                JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

                String code = (String) first.getJSONArray("code").get(0);
                String name = (String) first.getJSONArray("name").get(0);
                String tag = (String) first.getJSONArray("tag").get(0);
                double score = (Double.parseDouble(first.getString("score")));

                assertEquals("FM89.5", code);
                assertEquals("安徽音乐台", name);
                assertEquals("book", tag);
                assertTrue(score < 6 && score > 5);
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 重复异步搜索
     */
    public void test_cata_1113() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 100; i++) {
            err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            while (!def.msgResult) {
                tool.sleep(10);
            }
            def.msgResult = false;

            try {
                JSONObject ret = new JSONObject(def.asyncSearchRet);
                JSONObject first = ret.getJSONArray("FIRST").getJSONObject(0);

                String code = (String) first.getJSONArray("code").get(0);
                String name = (String) first.getJSONArray("name").get(0);
                String tag = (String) first.getJSONArray("tag").get(0);
                double score = (Double.parseDouble(first.getString("score")));

                assertEquals("FM89.5", code);
                assertEquals("安徽音乐台", name);
                assertEquals("book", tag);
                assertTrue(score < 6 && score > 5);
            } catch (JSONException e) {
                e.printStackTrace();
            }

            def.asyncSearchRet = "";
        }

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 重复创建-销毁搜索实例
     */
    public void test_cata_1114() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        for (int i = 0; i < 100; i++) {
            err = cataSearchInst.create("test.bin", def.iCataListener);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = cataSearchInst.destroy();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
    }

    /**
     * 异步搜索出结果前重复异步搜索
     */
    public void test_cata_1115() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 100; i++) {
            err = cataSearchInst.searchAsync("(field=name,query=音乐台,fuzzy=1,searchType=SR)");
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.destroy();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 重复构建-销毁索引实例
     */
    public void test_cata_1116() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        for (int i = 0; i < 100; i++) {
            err = cataIndexInst.create("test.bin", def.iCataListener);
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = cataIndexInst.destroy();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
    }

    /**
     * 插入后重复构建-销毁索引
     * <p>
     * fail 待定
     */
    public void test_cata_1117() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);
        for (Vector<SegValue> segValues : segValuesList) {
            //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
            err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.endIdxEntity();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 100; i++) {
            err = cataIndexInst.endIdxEntity();
            assertEquals(15008, err);

            err = cataIndexInst.drop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
    }

    /**
     * 重复插入-构建-销毁索引
     */
    public void test_cata_1118() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);

        for (int i = 0; i < 100; i++) {
            for (Vector<SegValue> segValues : segValuesList) {
                //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
                err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
                assertEquals(ISSErrors.ISS_SUCCESS, err);
            }

            err = cataIndexInst.endIdxEntity();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            err = cataIndexInst.drop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
    }

    /**
     * 重复插入-删除记录
     */
    public void test_cata_1119() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);

        for (int i = 0; i < 1000; i++) {
            for (Vector<SegValue> segValues : segValuesList) {
                //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
                err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
                assertEquals(ISSErrors.ISS_SUCCESS, err);

                Vector<SegValue> segValuess = new Vector<>();
                Vector<String> texts = new Vector<>();
                texts.add("89.5");
                segValuess.add(new SegValue("code", 2, texts));
                err = cataIndexInst.delIdxEntity(def.segValues2JSON(segValuess));
                assertEquals(ISSErrors.ISS_SUCCESS, err);
            }

            err = cataIndexInst.endIdxEntity();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }

        err = cataIndexInst.drop();
        assertEquals(ISSErrors.ISS_SUCCESS, err);
    }

    /**
     * 重复插入-构建-同步搜索
     */
    public void test_cata_1120() throws JSONException {
        CataSession cataSession = CataSession.getInstance(getContext(), def.iCataInitListener,
                def.resDir);
        int init_timeout = 0;
        while (!def.msgInitStatus && init_timeout++ < 500) {
            tool.sleep(10);
        }
        def.msgInitStatus = false;

        CataIndexInst cataIndexInst = cataSession.createCataIndexInst();
        CataSearchInst cataSearchInst = cataSession.createCataSearchInst();

        Vector<Vector<SegValue>> segValuesList = def.createSegValuesList(def.segValuePath, 0);

        err = cataIndexInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        err = cataSearchInst.create("test.bin", def.iCataListener);
        assertEquals(ISSErrors.ISS_SUCCESS, err);

        for (int i = 0; i < 100; i++) {
            for (Vector<SegValue> segValues : segValuesList) {
                //ILog.i(Tag, "adding segValues:" + def.getSegValuesContent(segValues));
                err = cataIndexInst.addIdxEntity(def.segValues2JSON(segValues));
                assertEquals(ISSErrors.ISS_SUCCESS, err);
            }

            err = cataIndexInst.endIdxEntity();
            assertEquals(ISSErrors.ISS_SUCCESS, err);

            String searchRet = cataSearchInst.searchSync("(field=name,query=音乐台,fuzzy=1," +
                    "searchType=SR)");
            //ILog.i(Tag, "searchSync ret:" + searchRet);

            err = cataIndexInst.drop();
            assertEquals(ISSErrors.ISS_SUCCESS, err);
        }
    }

    /**
     * 开启log，在多个线程中构建索引
     */
    public void test_cata_mThreads1() {
        TestStabilityCATA t = new TestStabilityCATA(getContext(), 600);
        t.ifWriteToActivity = 0;
        t.test_multiThreading1();
    }

    public void test_cata_rundom() {
        TestStabilityCATA t = new TestStabilityCATA(getContext(), 600);
        t.ifWriteToActivity = 0;
        t.testRandom();
    }
}
