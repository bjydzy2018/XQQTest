package com.iflytek.srbatchfornative;

/**
 * Created by qqxiong on 2018/11/15.
 */


import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.iflytek.srbatchfornative.util.RecordTime;
import com.iflytek.speech.ISSErrors;
import com.iflytek.speech.libisssr;
import com.iflytek.speech.sr.ISRListener;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Environment;
import android.os.IBinder;
import android.os.SystemClock;
import android.util.Log;


public class SpeechService extends Service {

    private final String TAG = "SpeechService";
    private myService binder = new myService();
    private ISRListener isrListener = null;
    private WorkThread workThread = new WorkThread("workThread");

    public static int mode = 0;
    public static int scene = 0;
    public static int sleepInterval = 10;
    private String pcmFile;
    private String sceneRec;

    public class myService extends Binder {
        public Date initDate = null;

        void setListener(ISRListener listener) {
            isrListener = listener;
        }

        void upLoadDict(String dictFile) {
            String text = readFile(dictFile);
            libisssr.uploadDict(text, 0);
        }

        void ReCreate(String lang) {
            initDate = ReCreateService(lang);
        }

        void batchRec(int mode, int scene, String pcmFile) {
            SpeechService.this.mode = mode;
            SpeechService.this.scene = scene;
            SpeechService.this.pcmFile = pcmFile;
            quitOnce = false;
            switch (scene) {
                case 0:
                    sceneRec = "all";
                    break;
                case 1:
                    sceneRec = "poi";
                    break;
                case 2:
                    sceneRec = "contacts";
                    break;
                case 3:
                    sceneRec = "select";
                    break;
                case 4:
                    sceneRec = "confirm";
                    break;
            }
            workThread.workQueue.add(MsgType.START);
        }

        void stopRec() {
            quitOnce = true;
        }

        public void setSleepInterval(int sleepIntervaltemp) {
            sleepInterval = sleepIntervaltemp;
        }
    }

    private String readFile(String file) {
        File fi = null;
        FileReader fr = null;
        try {
            fi = new File(file);
            fr = new FileReader(fi);
            char buf[] = new char[(int) fi.length()];
            fr.read(buf, 0, (int) fi.length());
            String text = new String(buf);
            fr.close();
            return text;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }


        return null;
    }

    private enum MsgType {
        START
    }

    public Date ReCreateService(String lang) {
        Date initDate = null;
        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());

        libisssr.destroy();
        if ("english".equals(lang)) {
            System.out.println("创建英文句柄");
            try {
                initDate = dfs.parse(dfs.format(new Date()));
            } catch (ParseException e) {
                e.printStackTrace();
            }
            libisssr.setMachineCode("test_speechsuite_machinecode");
            libisssr.createEx(1, Environment.getExternalStorageDirectory()
                    .getAbsolutePath() + "/iflytek/res/sr/", listener);
        } else if ("chinese".equals(lang)) {
            System.out.println("创建中文句柄");
            try {
                initDate = dfs.parse(dfs.format(new Date()));
            } catch (ParseException e) {
                e.printStackTrace();
            }
            libisssr.setMachineCode("test_speechsuite_machinecode");
            libisssr.createEx(0, Environment.getExternalStorageDirectory()
                    .getAbsolutePath() + "/iflytek/res/sr/", listener);
        } else {
            System.out.println("创建粤语句柄");
            try {
                initDate = dfs.parse(dfs.format(new Date()));
            } catch (ParseException e) {
                e.printStackTrace();
            }
            libisssr.setMachineCode("test_speechsuite_machinecode");
            libisssr.createEx(2, Environment.getExternalStorageDirectory()
                    .getAbsolutePath() + "/iflytek/res/sr/", listener);
        }

        return initDate;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        System.out.println("onCreate");
        workThread.start();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        System.out.println("onDestroy");
        libisssr.destroy();
    }

    @Override
    public void onRebind(Intent intent) {
        super.onRebind(intent);
        System.out.println("onRebind");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        System.out.println("onStartCommand");
        isNeedCallback = true;
        return super.onStartCommand(intent, flags, startId);

    }

    private boolean isNeedCallback = false;

    @Override
    public boolean onUnbind(Intent intent) {
        System.out.println("onUnbind");
        isNeedCallback = false;
        return super.onUnbind(intent);
    }

    @Override
    public IBinder onBind(Intent intent) {
        System.out.println("onbind");
        isNeedCallback = true;
        return binder;
    }

    private ISRListener listener = new ISRListener() {

        @Override
        public void onSRMsgProc_(long uMsg, long wParam, String lParam) {
            recordTime.error_code = wParam;
            switch ((int) uMsg) {
                case 20000: { //ISS_SR_MSG_InitStatus
                    if (wParam == ISSErrors.ISS_SUCCESS) {
                        System.out.println("成功创建识别会话.");

                    } else {
                        if (wParam == ISSErrors.ISS_ERROR_FAIL)
                            System.out.println("创建识别会话失败");
                        if (wParam == ISSErrors.ISS_ERROR_OUT_OF_MEMORY)
                            System.out.println("创建识别会话失败,内存不足。");
                    }
                    break;
                }
                case 20004: { // ISS_SR_MSG_ResponseTimeout
                    recordTime.ResponseTimeoutTime = System.currentTimeMillis();
                    recordTime.result_status = "ResponseTimeout";
                    logEffect.append("\n");
                    System.out.println("响应超时,没有在预定的时间内检测到语音,ISS_SR_MSG_ResponseTimeout");
                    break;
                }
                case 20005: { // ISS_SR_MSG_SpeechStart
                    recordTime.SpeechStartTime = System.currentTimeMillis();
                    recordTime.vadSpeechStartMs = wParam;
                    System.out.println("检测到语音开始_SpeechStart:");
                    break;
                }
                case 20006: {
                    recordTime.SpeechTimeOut = System.currentTimeMillis();
                    System.out.println("语音超时，正在进行识别处理,不需要再写入数据_ISS_SR_MSG_SpeechTimeOut");
                    break;
                }
                case 20007: {
                    recordTime.SpeechEndTime = System.currentTimeMillis();
                    recordTime.vadSpeechEndMs = wParam;
                    System.out.println("检测到语音结束点，正在进行识别处理,不需要再写入数据_SpeechEnd:");
                    break;
                }
                case 20008: {
                    recordTime.ErrorTime = System.currentTimeMillis();
                    recordTime.result_status = "MSG_Error";
                    logEffect.append("\n");
                    System.out.println("出现其它错误，识别引擎停止  MSG_Error:" + "wParam"
                            + wParam + "\n");
                    break;
                }
                case 20009: {
                    recordTime.ResultTime = System.currentTimeMillis();
                    recordTime.result_status = "MSG_Result";
                    System.out.println("---识别结果--" + "---\n" + lParam
                            + "\n---识别结束------\n");
                    if (lParam.contains("bislocalresult")) {
                        recordTime.IfLocalResult = "本地";
                    } else {
                        recordTime.IfLocalResult = "云端";
                    }
                    if (lParam.contains("<rawtext><![CDATA[")) {
                        int a = lParam.lastIndexOf("<rawtext><![CDATA[") + 18;
                        int b = lParam.lastIndexOf("]]></rawtext>");
                        recordTime.ISR_Result = lParam.substring(a, b);

                        if (lParam.contains("<nlocalconfidencescore>")) {
                            a = lParam.lastIndexOf("<nlocalconfidencescore>") + 23;
                            b = lParam.lastIndexOf("</nlocalconfidencescore>");
                            recordTime.Confidence_score = lParam.substring(a, b);
                        } else {
                            recordTime.Confidence_score = "none";
                        }
                    } else if (lParam.contains("<rawtext>")) {
                        int a = lParam.lastIndexOf("<rawtext>") + 9;
                        int b = lParam.lastIndexOf("</rawtext>");
                        recordTime.ISR_Result = lParam.substring(a, b);


                        if (lParam.contains("<nlocalconfidencescore>")) {
                            a = lParam.lastIndexOf("<nlocalconfidencescore>") + 23;
                            b = lParam.lastIndexOf("</nlocalconfidencescore>");
                            recordTime.Confidence_score = lParam.substring(a, b);
                        } else {
                            recordTime.Confidence_score = "none";
                        }
                    } else if (lParam.contains("\"text\":\"")) {
                        int a = lParam.lastIndexOf("\"text\":\"") + 8;
                        int b = lParam.substring(a).indexOf("\"") + a;
                        recordTime.ISR_Result = lParam.substring(a, b);

                        if (lParam.contains("\"nlocalconfidencescore\":\"")) {
                            a = lParam.lastIndexOf("\"nlocalconfidencescore\":\"") + 25;
                            b = lParam.substring(a).indexOf("\"") + a;
                            recordTime.Confidence_score = lParam.substring(a, b);
                        } else {
                            recordTime.Confidence_score = "none";
                        }
                    }

                    logEffect.append(recordTime.ISR_Result + "\n");
                    recordTime.Result = lParam;
                    break;
                }
                default:
            }
            if (isNeedCallback && isrListener != null) {
                isrListener.onSRMsgProc_(uMsg, wParam, lParam);
            }
        }
    };

    public class WorkThread extends Thread {
        BlockingQueue<MsgType> workQueue = new LinkedBlockingQueue<MsgType>();

        WorkThread(String name) {
            super(name);
        }

        @Override
        public void run() {
            super.run();
            while (true) {
                MsgType msg = null;
                try {
                    msg = workQueue.take();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                switch (msg) {
                    case START:
                        for (int i = 0; i < 15; i++)
                            startRec();
                        break;
                }
            }
        }

    }

    private String csvName;
    private String logEffectName;
    private StringBuffer logEffect;
    private StringBuffer log;
    private RecordTime recordTime = new RecordTime();

    public static boolean quitOnce = true;

    private byte[] bytes;

    private void startRec() {
        csvName = "/" + System.currentTimeMillis() + ".csv";
        logEffectName = "/" + System.currentTimeMillis() + ".log";
        try {
            BufferedReader br;
            br = new BufferedReader(new InputStreamReader(new FileInputStream(pcmFile), "utf-8"));
            String line = "";
            int line_num = 1;
            // 列表循环次数
            while (((line = br.readLine()) != null) && quitOnce == false) {
                System.out.println("test num: " + line_num++);
                bytes = new byte[320];
                recordTime.ReSet();
                logEffect = new StringBuffer();
                log = new StringBuffer();

                Date baseTime = null;
                long passTime = 0;
                long pcmTime = 0;
                long inputSize = 0;
                SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                        .getDefault());

                int err = libisssr.start(sceneRec, mode, "");
                if (err == 0) {
                    System.out.println("Testing: " + line);
                    logEffect.append("\"" + line + "\"\n");
                    File file = new File(line.trim());
                    if (!file.exists()) {
                        if (isNeedCallback && isrListener != null) {
                            isrListener.onSRMsgProc_(1111111111, 111, "111");
                        }
                        break;
                    }
                    System.out.println("Path: " + Environment.getExternalStorageDirectory()
                            .getAbsolutePath() + line.trim());
                    FileInputStream is = new FileInputStream(
                            line.trim());
                    int size = (int) file.length();
                    System.out.println("pcm size: " + size);
                    boolean bFirstRead = true;    //首次取数据标志

                    recordTime.SessionStartSucessTime = System
                            .currentTimeMillis();
//                    log.append(file.getAbsolutePath() + ",");
                    log.append(line + ",");
                    System.out
                            .println("---sessionStart成功---");
                    int a = 0;
                    long result = 0;
                    long startAppendAudioDataTime = 0;
                    Pattern pattern = Pattern.compile(".*/\\w+_\\w\\d_(.*)_\\d+.*\\.pcm");

                    baseTime = dfs.parse(dfs.format(new Date()));
                    while ((a = is.read(bytes)) > 0) {
//                        System.out.println("read from file bytes number: " + a);

                        // 如果是wav格式的音频则做去头处理
                        if (bFirstRead) {
                            bFirstRead = false;
                            System.out.println("line end 4 bytes string: " + line.substring(line.length()
                                    - 4, line.length()));
                            if (line.substring(line.length() - 4, line.length()).equals(".wav")) {
                                System.out.println("file name end with .wav");

                                if (bytes[0] == (byte) 'R' && bytes[1] == (byte) 'I'
                                        && bytes[2] == (byte) 'F' && bytes[3] == (byte) 'F') {
                                    System.out.println("wav file");
                                    System.out.println("bytes.length: " + bytes.length);
                                    byte[] new_bytes = new byte[bytes.length - 44];
                                    //Arrays.copyOfRange(bytes, 44, bytes.length);
                                    System.out.println(bytes.length);
                                    System.arraycopy(bytes, 44, new_bytes, 0, bytes.length - 44);
                                    bytes = new_bytes;
                                } else {
                                    Log.e(TAG, ".wav but not wav format file");
                                }
                            }
                        }

                        if (recordTime.ResponseTimeoutTime != 0
                                || recordTime.SpeechEndTime != 0
                                || recordTime.SpeechTimeOut != 0
                                || recordTime.ResultTime != 0
                                || recordTime.ErrorTime != 0) {
                            break;
                        }
                        if (startAppendAudioDataTime == 0) {
                            startAppendAudioDataTime = System.currentTimeMillis();
                        }
                        result = libisssr.appendAudioData(bytes, 320);
                        if (result != ISSErrors.ISS_SUCCESS) {
                            System.out.println("appendAudioData返回的结果码：" + result);
                            break;
                        }
                        try {
                            inputSize += 320;
                            pcmTime = inputSize / 32;
                            passTime = dfs.parse(dfs.format(new Date())).getTime() - baseTime
                                    .getTime();
                            if (pcmTime > passTime) {
                                Thread.sleep(pcmTime - passTime);
                            }

                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        bytes = new byte[320];
                    }

                    // 如果未检测到后端点，则调用endAudioData结束录音
                    if (recordTime.SpeechEndTime == 0 && recordTime.ResponseTimeoutTime == 0) {
                        int endAudio = libisssr.endAudioData();
                        recordTime.EndAudioDataEndTime = System
                                .currentTimeMillis();
                        if (endAudio == ISSErrors.ISS_ERROR_NO_SPEECH) {
                            recordTime.NoSpeechTime = recordTime.EndAudioDataEndTime;
                            /*
                             * recordTime.time_Result =
							 * recordTime.NoSpeechTime;
							 */
                            recordTime.result_status = "NO_SPEECH";
                            logEffect.append("\n");
                        }
                        System.out
                                .println("调用endAudioData后");
                    }

                    while (recordTime.ResponseTimeoutTime == 0
                            && recordTime.NoSpeechTime == 0
                            && recordTime.ResultTime == 0
                            && recordTime.ErrorTime == 0) {
                        try {
                            Thread.sleep((long) (50));
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }

                    if (recordTime.ResultTime != 0)
                        recordTime.time_Result2 = System.currentTimeMillis();
                    System.out.println("循环后");
                    SystemClock.sleep(200);

                    long t0 = 0;

                    // 计算识别响应时间
                    Matcher matcher = pattern.matcher(line);
                    boolean isUseVadLabel = true; // 是否使用检测标注VAD的方法计算响应时间

                    System.out.println(line);
                    if (isUseVadLabel && matcher.find()) {
                        // 音频名带前后端点标注
                        double pcm_length = file.length() * 1.0 / 1600 * 50;
                        double start = getPointTime(matcher.group(1))[0];
                        int end = (int) (pcm_length - getPointTime(matcher.group(1))[1]);
                        System.out.println(Arrays.toString(getPointTime(matcher.group(1))));
                        System.out.println(pcm_length + ", " + start + ", " + end);
                        t0 = startAppendAudioDataTime + end;
                        recordTime.speechEndVadType = "vad_label";
                        recordTime.SpeechEndTime = recordTime.SpeechEndTime - t0;
                    } else {
                        // 音频名不带前后端点标注
                        if (recordTime.EndAudioDataEndTime != 0) {
                            t0 = recordTime.EndAudioDataEndTime;
                            recordTime.speechEndVadType = "endAudioData";
                        } else if (recordTime.SpeechEndTime != 0) {
                            t0 = recordTime.SpeechEndTime;
//                            t0 = startAppendAudioDataTime + recordTime.vadSpeechEndMs;
                            recordTime.speechEndVadType = "vad_speechEnd";
                        }
                    }
                    log.append(recordTime.SpeechStartTime + ",");
                    log.append(recordTime.SpeechEndTime + ",");
                    log.append(recordTime.EndAudioDataEndTime + ",");
                    log.append(recordTime.time_Result2 + ",");
                    log.append(recordTime.result_status + ",");

                    log.append(recordTime.time_Result2 != 0 ? recordTime.time_Result2 - t0 : 0);
                    log.append(",");
                    log.append(recordTime.IfLocalResult);
                    log.append(",");
                    log.append(recordTime.error_code);
                    log.append(",");

                    String tmp = "";
                    tmp = "\"";
                    tmp += recordTime.ISR_Result;
                    tmp += "\"";
                    log.append(tmp);
                    log.append(",");
                    log.append(recordTime.Confidence_score);
                    log.append(",");


                    tmp = "\"";
                    for (int i = 0; i < recordTime.Result.length(); ++i) {
                        tmp += recordTime.Result.charAt(i);
                        if (recordTime.Result.charAt(i) == '\"') {
                            tmp += '\"';
                        }
                    }
                    tmp += "\"";
                    log.append(tmp);
                    log.append(",");

                    log.append(recordTime.vadSpeechStartMs);
                    log.append(",");

                    log.append(recordTime.vadSpeechEndMs);
                    log.append(",");

                    log.append(recordTime.speechEndVadType);
                    log.append("\n");

                    is.close();
                    file = null;
                } else {
                    recordTime.SessionStartFailTime = System
                            .currentTimeMillis();
                    System.out.println("sessionStart失败.");
                    System.out.println("start ret: " + err);
                }
                logEffect.append(".");
                logEffect.append("\n");
                appendMethod(MainActivity.PcmIndexPath + csvName, log.toString());
                appendMethod2(MainActivity.PcmIndexPath + logEffectName,
                        logEffect.toString());
                logEffect = null;
                log = null;
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }

    // 对标注了前后端点的音频名提取出前端点和尾部静音时间
    public double[] getPointTime(String s) {
        double start = 0;
        double end = 0;

        String[] temp_xp = s.split("_");

        for (int i = 0; i < temp_xp.length; i++) {
            String t = temp_xp[i];
            if (t.endsWith("s")) {
                if (start == 0) {
                    start += Double.parseDouble(t.split("s")[0]) * 1000;
                } else if (end == 0) {
                    end += Double.parseDouble(t.split("s")[0]) * 1000;
                }
            }
        }

        return new double[]{start, end};
    }

    public static void appendMethod(String fileName, String content) {
        try {
            File f = new File(fileName);
            if (!f.exists()) {
                f.createNewFile();
                FileOutputStream fis = new FileOutputStream(fileName, true);
                OutputStreamWriter osw = new OutputStreamWriter(fis, "GBK");
                osw.write("录音,time_SpeechStart,time_SpeechEnd,time_EndAudioData,time_Result," +
                        "result_status,time_resume,结果来源,error_code,ISR_Result,Confidence_score," +
                        "Result,vadSpeechStart,vadSpeechEnd,endVadType\n");
                osw.flush();
                osw.close();
            }
            FileOutputStream fis = new FileOutputStream(fileName, true);
            OutputStreamWriter osw = new OutputStreamWriter(fis, "GBK");
            osw.write(content);
            osw.flush();
            osw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void appendMethod2(String fileName, String content) {
        try {
            File f = new File(fileName);
            if (!f.exists()) {
                f.createNewFile();
                FileOutputStream fis = new FileOutputStream(fileName, true);
                OutputStreamWriter osw = new OutputStreamWriter(fis, "GBK");
                osw.write("#!MLF!#\n");
                osw.flush();
                osw.close();
            }
            FileOutputStream fis = new FileOutputStream(fileName, true);
            OutputStreamWriter osw = new OutputStreamWriter(fis, "GBK");
            osw.write(content);
            osw.flush();
            osw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
