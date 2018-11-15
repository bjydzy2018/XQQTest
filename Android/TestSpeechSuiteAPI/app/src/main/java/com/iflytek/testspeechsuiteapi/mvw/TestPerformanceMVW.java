package com.iflytek.testspeechsuiteapi.mvw;

import android.content.Context;

import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.mvw.MvwSession;
import com.iflytek.speech.ISSErrors;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;


public class TestPerformanceMVW {
    DefMVW def = new DefMVW();
    Tool tool = new Tool();
    MvwSession mvwSession;
    String Tag = "TestPerformanceMVW";
    int isWriteToActivity = 0;

    public TestPerformanceMVW(Context context) {
        mvwSession = new MvwSession(context, def.iMvwListener, def.resDir);
        ILog.i(Tag, "开始初始化", isWriteToActivity);

        while (!def.msgInitStatus) {
            tool.sleep(10);
        }
    }

    public void test() {
        tool.setRunTime(1800);

        Random random = new Random();
        String pcmPath = null;

        mvwSession.start(15);
        int location = 0;
        int bytesEachTimeSend = 320;
        Date baseTime = null; // 开始送音频的时间
        long passTime = 0; // 真实过去的时间
        long pcmTime = 0; // 送入音频的时间
        long inputSize = 0; // 送入音频的字节数
        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());
        try {
            baseTime = dfs.parse(dfs.format(new Date()));
        } catch (ParseException e) {
            e.printStackTrace();
        }

//        while (!tool.isTimeUp()) {
        while (true) {
//            int r = random.nextInt(5);
//            if (r == 3) {
//                // 送入唤醒音频
//                r = random.nextInt(5);
//                switch (r) {
//                    case 0:
//                        pcmPath = def.mvwPcm_global;
//                        break;
//                    case 1:
//                        pcmPath = def.mvwPcm_confirm;
//                        break;
//                    case 2:
//                        pcmPath = def.mvwPcm_first;
//                        break;
//                    case 3:
//                        pcmPath = def.mvwPcm_third;
//                        break;
//                    case 4:
//                        pcmPath = def.mvwPcm_answer;
//                        break;
//                }
//
//                ILog.i(Tag, "送入唤醒音频：" + pcmPath, isWriteToActivity);
//            } else
            {
                // 送入无效音频
                pcmPath = def.mvwPcm_KaiYi;

//                ILog.i(Tag, "送入非唤醒音频", isWriteToActivity);
            }

//            appendAudio(pcmPath);
            byte[] byteData;
            byteData = new byte[320];

            mvwSession.appendAudioData(byteData);

            location = location + bytesEachTimeSend;
            inputSize += bytesEachTimeSend;
            pcmTime = inputSize / 32;
            try {
                passTime = dfs.parse(dfs.format(new Date())).getTime() - baseTime.getTime();
            } catch (ParseException e) {
                e.printStackTrace();
            }
            if (pcmTime > passTime) {
                tool.sleep(pcmTime - passTime);
            }

//            int i = 0;
//            while (!def.msgVwWakeup && i++ < 100) {
//                tool.sleep(10);
//            }
//            def.msgVwWakeup = false;
        }
    }

    private void appendAudio(String pcmPath) {
        // 计算每次调用appendAudioData后需要sleep的时间。如果pcmTime > passTime，则sleep pcmTime - passTime的时间
        Date baseTime = null; // 开始送音频的时间
        long passTime = 0; // 真实过去的时间
        long pcmTime = 0; // 送入音频的时间
        long inputSize = 0; // 送入音频的字节数
        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());

        try {
            DataInputStream data = new DataInputStream(new FileInputStream(pcmPath));
            byte[] byteData;
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            int location = 0;
            int position = byteData.length;

            // 去掉头44个字节
//            if (byteData[0] == (byte) 'R' && byteData[1] == (byte) 'I'
//                    && byteData[2] == (byte) 'F' && byteData[3] == (byte) 'F') {
//                byte[] new_bytes = new byte[position - 44];
//                System.arraycopy(byteData, 44, new_bytes, 0, position - 44);
//                byteData = new_bytes;
//                ILog.i(Tag, "delete wav head", isWriteToActivity);
//            }

            int bytesEachTimeSend = 320;

            baseTime = dfs.parse(dfs.format(new Date()));
            appendAudioData_label:
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
                int ret = mvwSession.appendAudioData(mid);
                switch (ret) {
                    case ISSErrors.ISS_SUCCESS:
//                            ILog.i("MvwSession.appendAudio", "ISS_SUCCESS", isWriteToActivity);
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_PARA:
                        ILog.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_PARA",
                                isWriteToActivity);
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                        ILog.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_HANDLE",
                                isWriteToActivity);
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_CALL:
                        ILog.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_CALL",
                                isWriteToActivity);
                        break appendAudioData_label;
                    case ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER:
                        ILog.e("MvwSession.appendAudio", "ISS_ERROR_NO_ENOUGH_BUFFER",
                                isWriteToActivity);
                        break;
                    default:
                        ILog.e("MvwSession.appendAudio", "UnHandled MSG", isWriteToActivity);
                }
                location = location + bytesEachTimeSend;
                inputSize += bytesEachTimeSend;
                pcmTime = inputSize / 32;
                passTime = dfs.parse(dfs.format(new Date())).getTime() - baseTime.getTime();
                if (pcmTime > passTime) {
                    tool.sleep(pcmTime - passTime);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

