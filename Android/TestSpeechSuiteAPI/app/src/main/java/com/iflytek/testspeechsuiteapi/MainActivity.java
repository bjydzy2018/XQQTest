package com.iflytek.testspeechsuiteapi;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.method.ScrollingMovementMethod;
import android.widget.ScrollView;
import android.widget.TextView;

import com.iflytek.testspeechsuiteapi.cata.TestStabilityCATA;
import com.iflytek.testspeechsuiteapi.mvw.TestStabilityMVW;
import com.iflytek.testspeechsuiteapi.sr.TestSR;
import com.iflytek.testspeechsuiteapi.sr.TestStabilitySR;
import com.iflytek.testspeechsuiteapi.tts.TestStabilityTTS;

public class MainActivity extends AppCompatActivity {
    private TextView textView;
    private ScrollView scrollView;
    private Context context = this;
    private MainActivity mainActivity = this;
    private Tool tool = new Tool();

    private TestStabilitySR testStabilitySR;
    private TestStabilityMVW testStabilityMVW;
    private TestStabilityTTS testStabilityTTS;
    private TestStabilityCATA testStabilityCATA;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = (TextView) findViewById(R.id.textView);
        textView.setMovementMethod(ScrollingMovementMethod.getInstance());

        scrollView = (ScrollView) findViewById(R.id.scrollView);

        ILog.mainActivity = this;

//        testStabilitySR = new TestStabilitySR(context, mainActivity, 3600 * 20);
//        testStabilityTTS = new TestStabilityTTS(context, mainActivity, 3600 * 13);
//        testStabilityMVW = new TestStabilityMVW(context, mainActivity, 3600 * 13);
//        testStabilityCATA = new TestStabilityCATA(context, mainActivity, 3600*13);

//        CrashCollector.setAppReportDelay(100);
//        CrashCollector.setMaxDatasByWifi(50000);
//        CrashCollector.setMaxTrafficsByWifi(50000);
//        CrashCollector.setMaxTimesByWifi(50000);
//        CrashCollector.triggerUpload();
//
//        CrashCollector.setDebugable(true);
//        CrashCollector.init(context, "557esddd", tool.crashUpLoadRequest);

        new Thread(new Runnable() {
            @Override
            public void run() {
//                System.out.println("进入线程");
//                tool.sleep(10000);
//                test();

                new TestSR(mainActivity, context).test_temp();
//                new TestMVW(context).test_temp();
//                new TestTTS(context, mainActivity).test_temp();
//                new TestCATA(context).test_temp();
//                new TestCATA(context).smokeTest();
//========================================识别======================================================
                // 获取云端语义
//                new TestSR(mainActivity, context).test_mspSearch(
//                        "合肥未来三天的天气",
//                        null);
                // 获取本地语义
//                new TestSR(mainActivity, context).test_localNli(
//                        "合肥未来三天的天气",
//                        "ISS_SR_SCENE_ALL");
                // 获取本地语义，上传词典文件
//                new TestSR(mainActivity, context).test_localNli(
//                        "打个电话给舒正昇",
//                        "ISS_SR_SCENE_ALL",
//                        "/test.hcliu/dict/dict_list_nlp.txt",
//                        0);
                // 上传词典响应时间
//                new TestSR(mainActivity, context).test_upLoadDictTime(0,
//                        "/test.hcliu/dict/dict_M-500.txt",
//                        "/test.hcliu/dict/dict_M-1000.txt",
//                        "/test.hcliu/dict/dict_M-1500.txt",
//                        "/test.hcliu/dict/dict_M-2000.txt",
//                        "/test.hcliu/dict/dict_M-2500.txt",
//                        "/test.hcliu/dict/dict_M-3000.txt",
//                        "/test.hcliu/dict/dict_M-15000.txt");
//                new TestSR(mainActivity, context).test_upLoadDictTime(0,
//                        "/test.hcliu/dict/dict_M-500.txt");
                // 识别一条音频，不上传词典
//                new TestSR(mainActivity, context).test_sr(
//                        "test.temp/vad_log_2016-07-05 Tue 08-34-37.348952_in.pcm",
//                        SrSession.ISS_SR_SCENE_ALL,
//                        SrSession.ISS_SR_MODE_MIX_REC,
//                        null);
                // 识别一条音频，不上传词典，传入本地命令词
//                new TestSR(mainActivity, context).test_sr(
//                        "test.hcliu/pcm_src/sr/park.wav",
//                        "all",
//                        SrSession.ISS_SR_MODE_LOCAL_REC,
//                        "{\"list_name\": \"poi\",\"select_list\": [{\"name\": \"科大讯飞语音产业基地\"}, {\"name\": \"科大讯飞停车场\"}]}");
                // 识别一条音频，上传词典文件
//                new TestSR(mainActivity, context).test_sr(
//                        "test.hcliu/pcm_src/sr/CallZhang.wav",
//                        SrSession.ISS_SR_SCENE_ALL,
//                        SrSession.ISS_SR_MODE_LOCAL_REC,
//                        null,
//                        "/ret.json",
//                        0);
                // 识别一条音频，上传词典字符串
//                new TestSR(mainActivity, context).test_sr(
//                        "test.hcliu/pcm_src/sr/2.pcm",
//                        SrSession.ISS_SR_SCENE_ALL,
//                        SrSession.ISS_SR_MODE_LOCAL_REC,
//                        null,
//                        0,
//                        "{" +
//                        "    \"grm\": [" +
//                        "        {" +
//                        "            \"dictname\": \"contact\"," +
//                        "            \"dictcontant\": [" +
//                        "                {" +
//                        "                    \"name\": \"Jack李\"," +
//                        "                    \"id\": 2" +
//                        "                }" +
//                        "            ]" +
//                        "        }" +
//                        "    ]" +
//                        "}");
                // 批量获取语义，不上传词典
//                new TestSR(mainActivity, context).test_manyNli(
//                        0,
//                        null,
//                        "/test.hcliu/sr/nli/label_en_2017.3.16_nodict.txt",
//                        "/test.hcliu/sr/nli/cloud_ret.txt");
                // 批量获取语义，上传词典文件 测试前将ServicesSearchContactsByCata关掉,开语义模糊
//                new TestSR(mainActivity, context).test_manyNli(
//                        0,
//                        null,
//                        "/test.hcliu/sr/nli/label.txt",
//                        "/test.hcliu/sr/nli/ret_cloud.txt",
//                        "/test.hcliu/dict/dict_list_nlp.txt",
//                        0);
                // 接口随机调用
//                new TestStabilitySR(context, 60000).testRandom();
                // 接口循环调用
//                new TestStabilitySR(context, 120).testRollApi();
                // 批量识别
//                new TestStabilitySR(context).testManySR(SrSession.ISS_SR_SCENE_ALL,
//                        SrSession.ISS_SR_MODE_MIX_REC,
//                        "/sdcard/issTest/pcmList/isr_list_ch中文.txt");
                // 多线程测试
//                new TestStabilitySR(context).test_multiThreading1();
                // 随机中断
//                testStabilitySR.testRandomExit(0);
                // 测试可见即可说
//                new TestSTKS(context).testFunction();
//========================================唤醒======================================================
                // 唤醒一条音频
//                new TestMVW(context).test_mvw(
//                        8,
//                        "/MVWTestSuite/MVWTestEffect_WithTime/SceneId-8/接听/接听郊区开窗/M004_接听_004.wav");
                // 唤醒响应时间
//                new TestMVW(context).testVwTime(
//                        1,
//                        "/test.hcliu/pcm_src/mvw/mvwword.pcm");
//                new TestStabilityMVW(context).test_mvwTime(true, false, 1,2,4,8);
                // 接口随机调用
//                new TestStabilityMVW(context, 3600*12).testRandom();
                // 接口循环调用
//                new TestStabilityMVW(context).testRollApi();
                // 唤醒一致性 注意在何时start
//                new TestStabilityMVW(context).testManyMVWByWord(true, false);
//                new TestStabilityMVW(context).testManyMVWByScene(false, false, 1, 2, 4, 8);
                // 多线程测试
//                new TestStabilityMVW(context).test_multiThreading1();
//                new TestStabilityMVW(context).test_mvwTime();
                // 随机中断
//                testStabilityMVW.testRandomExit(0);
//========================================合成======================================================
                // 合成一个文本
//                new TestTTS(context).test_tts("交给我吧");
                // 首次合成响应时间
//                new TestTTS(context).test_ttsTime("/test.hcliu/tts/pro.txt");
                // 批量合成文本
//                new TestStabilityTTS(context).testManyTts();
                // 接口随机调用
//                new TestStabilityTTS(context, 3600*12).testRandom();
                // 接口循环调用
//                new TestStabilityTTS(context, 3600*12).testRollApi();
                // 多线程测试
//                new TestStabilityTTS(context).test_multiThreading1();
                // 随机中断
//                testStabilityTTS.testRandomExit(0);
//========================================搜索======================================================
                // 测试一次搜索
//                new TestCATA(context).test_search(
//                        "test.bin,alias,pinyin_fuzzy",
//                        "(field=song,query=12-TEMPEST_Millhouse_FghtCmplte01 - 谁最猛？谁最猛？呼~没错，我们最猛！,fuzzy=1,searchType=SR)");
                // 测试一致性
//                new TestCATA(context).test_consistent(false, false);
                // 稳定性测试
//                new TestStabilityCATA(context).test();
//                new TestStabilityCATA(context).test1();
                // 接口随机调用
//                new TestStabilityCATA(context, 30000).testRandom();
                // 接口循环调用
//                new TestStabilityCATA(context, 180).testRollApi();
                // 多线程测试
//                new TestStabilityCATA(context).test_multiThreading1();
            }
        }).start();
    }

//    @Override
//    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
//        switch (requestCode) {
//            case 0:
//                testStabilitySR.testRandomExit(1);
//                break;
//            case 1:
//                testStabilityTTS.testRandomExit(1);
//                break;
//            case 2:
//                testStabilityMVW.testRandomExit(1);
//                break;
//        }
//    }

    public void setText(String text) {
        new UpdateUITask().execute(text);
    }

    // 用于在工作线程更新UI
    private class UpdateUITask extends AsyncTask<String, Void, String> {
        protected String doInBackground(String... text) {
            return text[0];
        }

        protected void onPostExecute(String result) {
            int lines = textView.getLineCount();
            if (lines >= 300) {
                textView.setText(result);
            } else {
                textView.append(result + "\n");
            }
            scrollView.fullScroll(ScrollView.FOCUS_DOWN);
        }
    }
}
