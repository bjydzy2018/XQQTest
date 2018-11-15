package com.iflytek.testspeechsuiteapi;

import junit.framework.Test;
import junit.framework.TestSuite;
import org.junit.runners.Suite;

/**
 * Created by qqxiong on 2018/10/23.
 */

public class AllTestSuite extends TestSuite {
    public static Test suite(){
        TestSuite suite = new TestSuite();
        suite.addTestSuite(TestDemo.class);
        //suite.addTestSuite(TestApiSR.class);
        //suite.addTestSuite(TestApiMVW.class);
        //suite.addTestSuite(TestApiTTS.class);
        //suite.addTestSuite(TestApiCATA.class);
        return suite;
    }
}

