package com.iflytek.testspeechsuitenative.mvw;

import android.util.Log;

import com.iflytek.testspeechsuitenative.Tool;
import com.iflytek.speech.NativeHandle;
import com.iflytek.speech.libissmvw;

import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class TestMVWStability {

    /**
     * 一个线程持续送主唤醒词音频，一个线程随机调接口。
     */
    public void testMultiThread() {
        ExecutorService threadPool = Executors.newCachedThreadPool();
        final Random random = new Random();
        Tool tool = new Tool();

        threadPool.execute(new Runnable() {
            @Override
            public void run() {
                NativeHandle mNativeHandle;
                DefMVW def;
                Tool tool;
                String Tag = "testMultiThread.inst1";

                mNativeHandle = new NativeHandle();
                def = new DefMVW();
                tool = new Tool();

                Log.d(Tag, "调用create");
                int errId = libissmvw.create(mNativeHandle, def.resDir, def.iMvwListener);
                Log.d(Tag, "create libissmvw return " + errId);

                Log.d(Tag, "调用start");
                errId = libissmvw.start(mNativeHandle, 1);
                Log.i(Tag, "start libissmvw return " + errId);

                while (true) {
                    tool.loadPcmAndAppendAudioData("mvw", mNativeHandle, def.mvwPcm_global, def);
                    def.initMsg();
                }
            }
        });
//        tool.sleep(1000);

        threadPool.execute(new Runnable() {
            @Override
            public void run() {
                NativeHandle mNativeHandle;
                DefMVW def;
                Tool tool;
                String Tag = "testMultiThread.inst2";

                mNativeHandle = new NativeHandle();
                def = new DefMVW();
                tool = new Tool();

                Log.d(Tag, "调用create");
                int errId = libissmvw.create(mNativeHandle, def.resDir_second, def.iMvwListener);
                Log.d(Tag, "create libissmvw return " + errId);

                while (true) {
                    switch (random.nextInt(9)) {
                        // start
                        case 0:
                            int scene = random.nextInt(25);
                            Log.i(Tag, "调用start接口，scene：" + scene);
                            libissmvw.start(mNativeHandle, scene);
                            break;
                        // addStartScene
                        case 1:
                            scene = random.nextInt(25);
                            Log.i(Tag, "调用addStartScene接口，scene：" + scene);
                            libissmvw.addstartscene(mNativeHandle, scene);
                            break;
                        // stop
                        case 2:
                            if (random.nextInt(5) == 3) {
                                Log.i(Tag, "调用stop接口");
                                libissmvw.stop(mNativeHandle);
                            }
                            break;
                        // stopScene
                        case 3:
                            if (random.nextInt(5) == 3) {
                                scene = random.nextInt(25);
                                Log.i(Tag, "调用stopScene接口，scene：" + scene);
                                libissmvw.stopscene(mNativeHandle, scene);
                            }
                            break;
                        // setThreshold
                        case 4:
                            scene = random.nextInt(25);
                            int id = random.nextInt(8);
                            int threshold = random.nextInt(1000);
                            Log.i(Tag, "调用setThreshold接口，scene：" + scene + "，id：" + id + "，threshold：" +
                                    threshold);
                            libissmvw.setThreshold(mNativeHandle, scene, id, threshold);
                            break;
                        // setMvwKeyWords
                        case 5:
                            scene = random.nextInt(25);
                            String word = "";

                            int word_num = random.nextInt(5);
                            switch (word_num) {
                                case 0:
                                    word = def.mvwWord_KaiYi;
                                    break;
                                case 1:
                                    word = def.mvwWord_LingXi;
                                    break;
                                case 2:
                                    word = def.mvwWord_KaiYi_LingXi;
                                    break;
                                case 3:
                                    word = def.mvwWord_KaiYi_long;
                                    break;
                            }

                            Log.i(Tag, "调用setMvwKeyWords接口，scene：" + scene + ", word: " + word);
                            libissmvw.setMvwKeyWords(mNativeHandle, scene, word);
                            break;
                        // setParam
                        case 6:
                            String param = "";
                            String paramValue = "";
                            switch (random.nextInt(4)) {
                                case 0:
                                    param = "mvw_enable_aec";
                                    break;
                                case 1:
                                    param = "mvw_enable_lsa";
                                    break;
                                case 2:
                                    param = "asdfaw";
                                    break;
                                case 3:
                                    param = "";
                                    break;
                            }
                            switch (random.nextInt(4)) {
                                case 0:
                                    paramValue = "on";
                                    break;
                                case 1:
                                    paramValue = "off";
                                    break;
                                case 2:
                                    paramValue = "asdfaw";
                                    break;
                                case 3:
                                    paramValue = "";
                                    break;
                            }
                            Log.i(Tag, "调用setParam接口，param：" + param + "，paramValue：" + paramValue);
                            libissmvw.setParam(mNativeHandle, param, paramValue);
                            break;
                        // setMvwDefaultKeyWords
                        case 7:
                            scene = random.nextInt(20);
                            Log.i(Tag, "调用setMvwDefaultKeyWords接口，scene：" + scene);
                            libissmvw.setMvwDefaultKeyWords(mNativeHandle, scene);
                            break;
                        case 8:
                            tool.loadPcmAndAppendAudioData("mvw", mNativeHandle, def.mvwPcm_answer, def);
                            break;
                        case 9:
//                            libissmvw.create(mNativeHandle, def.resDir, def.iMvwListener);
                            break;
                        case 10:
//                            switch (random.nextInt(4)) {
//                                case 0:
//                                    break;
//                                case 1:
//                                    break;
//                                case 2:
//                                    break;
//                                case 3:
//                                    libissmvw.destroy(mNativeHandle);
//                            }

                    }
                }
            }
        });
        threadPool.shutdown();
    }
}
