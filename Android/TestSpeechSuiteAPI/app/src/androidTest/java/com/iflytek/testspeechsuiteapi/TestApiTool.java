package com.iflytek.testspeechsuiteapi;

import com.iflytek.testspeechsuiteapi.cata.DefCATA;
import com.iflytek.testspeechsuiteapi.mvw.DefMVW;
import com.iflytek.testspeechsuiteapi.sr.DefSR;
import com.iflytek.testspeechsuiteapi.tts.DefTTS;
import com.iflytek.mvw.MvwSession;
import com.iflytek.sr.SrSession;
import com.iflytek.tts.TtsSession;

/**
 *
 * 接口测试工具类，封装了一些接口测试需要的对象。
 * 由于JUnit每个测试用例都会创建一个测试类对象，因此使用该类确保一些共用对象的一致性。
 */
class TestApiTool {
    static DefSR defSR = new DefSR();
    static DefMVW defMVW = new DefMVW();
    static DefCATA defCATA = new DefCATA();
    static DefTTS defTTS = new DefTTS();
    static Tool tool = new Tool();

    static SrSession srSession;

    static MvwSession mvwSession;
    static MvwSession mvwSession1;

    static TtsSession ttsSession;
    static TtsSession ttsSession1;

    static {
        tool.isWriteToActivity = 0;
        defSR.isWriteToActivity = 0;
        defMVW.isWriteToActivity = 0;
        defCATA.isWriteToActivity = 0;
        defTTS.isWriteToActivity = 0;
    }
}
