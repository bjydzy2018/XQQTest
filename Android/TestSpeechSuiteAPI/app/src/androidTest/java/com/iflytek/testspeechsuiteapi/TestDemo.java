package com.iflytek.testspeechsuiteapi;

import android.test.AndroidTestCase;
import android.util.Log;

public class TestDemo extends AndroidTestCase {

    private static final String TAG = "TestDemo";

    @Override
    protected void setUp() throws Exception {
        super.setUp();
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void test1() throws Exception{
        assertEquals(1,4);
        Log.d(TAG, "test1: finish");
    }
    public void test2() throws Exception{
        assertEquals(1,1);
        Log.d(TAG, "test2: finish");
    }

    public void test3() throws Exception{
        assertEquals(1,5);
        Log.d(TAG, "test3: finish");
    }
    public void test4() throws Exception{
        assertEquals(1,6);
        Log.d(TAG, "test4: finish");
    }


}

