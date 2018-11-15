package com.iflytek.testspeechsuitenative.tts;

import android.util.Log;

import com.iflytek.speech.ISSErrors;
import com.iflytek.speech.libisstts;

import java.util.concurrent.TimeUnit;

/**
 * <p>
 * 测试内存泄漏。
 */

public class TestMemoryLeak extends TestTTS {
    private String Tag = "TTS.TestMemoryLeak";

    public void test() {
        while (true) {
            libisstts.start(mNativeHandle, "123123");
            Log.i(Tag, "start errCode: " + mNativeHandle.err_ret);

            while (!def.msgOnDataReady) {
                try {
                    TimeUnit.MICROSECONDS.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            def.initMsg();

            while (errCode != ISSErrors.ISS_ERROR_TTS_COMPLETED) {
                libisstts.getAudioData(mNativeHandle, buffer, ttsBytes, bufferSize);
                errCode = mNativeHandle.err_ret;
//                Log.i(Tag, "getAudioData errCode: " + errCode);
//                Log.i(Tag, "getAudioData size: " + bufferSize[0]);
                try {
                    TimeUnit.MILLISECONDS.sleep(3);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Log.i(Tag, "TTS complete.");
            errCode = -1;
        }
    }
}

