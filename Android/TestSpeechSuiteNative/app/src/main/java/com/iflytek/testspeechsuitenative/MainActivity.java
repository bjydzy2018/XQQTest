package com.iflytek.testspeechsuitenative;


import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.method.ScrollingMovementMethod;
import android.widget.ScrollView;
import android.widget.TextView;
import com.iflytek.crashcollect.CrashCollector;
import com.iflytek.testspeechsuitenative.cata.TestCATABatch;
import com.iflytek.testspeechsuitenative.mvw.TestMVW;
import com.iflytek.testspeechsuitenative.mvw.TestMVWBatch;
import com.iflytek.testspeechsuitenative.mvw.TestMVWStability;
import com.iflytek.testspeechsuitenative.mvw.TestMultiInstPerf;
import com.iflytek.testspeechsuitenative.sr.TestSRBatch;
import com.iflytek.testspeechsuitenative.tts.TestTTSBatchTime;

public class MainActivity extends AppCompatActivity {
    private TextView textView;
    private ScrollView scrollView;
    private Tool tool;

    private MainActivity mainActivity = this;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = (TextView) findViewById(R.id.textView);
        textView.setMovementMethod(ScrollingMovementMethod.getInstance());

        scrollView = (ScrollView) findViewById(R.id.scrollView);

        tool = new Tool();
    }

    @Override
    protected void onStart() {
        super.onStart();

//        CrashCollector.setMaxDatasByWifi(50000);
//        CrashCollector.setMaxTrafficsByWifi(50000);
//        CrashCollector.setMaxTimesByWifi(50000);
//        CrashCollector.triggerUpload();
//
//        CrashCollector.setDebugable(true);
//        CrashCollector.init(this, "557esddd", tool.crashUpLoadRequest);
//
//        tool.sleep(10000);

        new Thread(new Runnable() {
            @Override
            public void run() {
//                new TestSRBatch().test_upLoadDictTime(0,
//                        "/TestRes/dict/dict_M-500.txt",
//                        "/TestRes/dict/dict_M-1000.txt",
//                        "/TestRes/dict/dict_M-1500.txt",
//                        "/TestRes/dict/dict_M-2000.txt",
//                        "/TestRes/dict/dict_M-2500.txt",
//                        "/TestRes/dict/dict_M-3000.txt",
//                        "/TestRes/dict/dict_M-15000.txt");

//                new TestTTSBatchTime().test();

                new TestMVWBatch(mainActivity).testMVWBatchByWord(true);
//                new TestMVWBatch(mainActivity).testDNConsistent();
//                new TestMVWStability().testMultiThread();
//                new TestMVW().test();

//                new TestCATABatch().test_time();
            }
        }).start();
    }

    public void setText(String text) {
        new UpdateUITask().execute(text);
    }

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