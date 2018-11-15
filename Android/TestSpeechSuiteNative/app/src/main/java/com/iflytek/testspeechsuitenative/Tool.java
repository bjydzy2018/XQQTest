package com.iflytek.testspeechsuitenative;

import android.content.Context;
import android.os.Environment;
import android.os.storage.StorageManager;
import android.util.Log;

import com.iflytek.crashcollect.baseinfocollect.HeartbeatInfo;
import com.iflytek.crashcollect.collectcontrol.CrashInfo;
import com.iflytek.crashcollect.crashupload.CrashUpLoadRequest;
import com.iflytek.crashcollect.crashupload.UploadListener;
import com.iflytek.crashcollect.dump.DumpEntity;
import com.iflytek.testspeechsuitenative.mvw.DefMVW;
import com.iflytek.speech.ISSErrors;
import com.iflytek.speech.NativeHandle;
import com.iflytek.speech.libissmvw;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.lang.reflect.Array;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import java.util.Locale;


/**
 * 通用的工具类。
 */
public class Tool {
    private String Tag = "Tool";
    private Date startTime; // 用于计时的变量，初始时间
    private long runTime = -1; // 用于计时的变量，需要计时的时长
    private SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
            .getDefault());
    public int isWriteToActivity = 1;

    public void loadBigPcmAndAppendAudioData(String obj, NativeHandle mNativeHandle, String pcmPath, Def... def) {
        String Tag = this.Tag + ".loadBigPcmAndAppendAudioData";
        File pcmFile = new File(pcmPath);

        // 计算每次调用appendAudioData后需要sleep的时间。如果pcmTime > passTime，则sleep pcmTime - passTime的时间
        Date baseTime = null; // 开始送音频的时间
        long passTime = 0; // 真实过去的时间
        long pcmTime = 0; // 送入音频的时间
        long inputSize = 0; // 送入音频的字节数
        int loadSizeEachTime = 320; // 每次送音频的字节数
        int ret = -1;

        boolean firstLoad = true; // 第一次送音频

        if (!pcmFile.exists()) {
            Log.e(Tag, "pcm文件不存在");
            Log.e(Tag, pcmPath);
            return;
        }

        try {
            DataInputStream data = new DataInputStream(new FileInputStream(pcmPath));
            byte[] byteData = new byte[loadSizeEachTime];
            int byteRead = -1;

            baseTime = dfs.parse(dfs.format(new Date()));

            appendAudioData_label:
            while ((byteRead = data.read(byteData)) > 0) {
                if (firstLoad) {
                    if (byteData[0] == (byte) 'R' && byteData[1] == (byte) 'I'
                            && byteData[2] == (byte) 'F' && byteData[3] == (byte) 'F') {
                        byte[] new_bytes = new byte[loadSizeEachTime - 44];
                        System.arraycopy(byteData, 44, new_bytes, 0, loadSizeEachTime - 44);
                        byteData = new_bytes;
                        Log.i(Tag, "delete wav head");
                    }

                    firstLoad = false;
                }

                ret = libissmvw.appendAudioData(mNativeHandle, byteData, byteData.length);
                switch (ret) {
                    case ISSErrors.ISS_SUCCESS:
//                        Log.i("MvwSession.appendAudio", "ISS_SUCCESS");
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_PARA:
                        Log.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_PARA");
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                        Log.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_HANDLE");
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_CALL:
                        Log.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_CALL");
                        break appendAudioData_label;
                    case ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER:
                        Log.e("MvwSession.appendAudio", "ISS_ERROR_NO_ENOUGH_BUFFER");
                        break;
                    default:
                        Log.e("MvwSession.appendAudio", "UnHandled MSG");
                }

                inputSize += byteData.length;
                pcmTime = inputSize / 32;
                passTime = dfs.parse(dfs.format(new Date())).getTime() - baseTime.getTime();
                if (pcmTime > passTime)
                    sleep(pcmTime - passTime);

                byteData = new byte[loadSizeEachTime];
            }
            if (def.length > 0) {
                DefMVW d = (DefMVW) def[0];
                int i = 0;
                while (!d.msgVwWakeup && i++ < 250) {
                    sleep(10);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 识别、唤醒模块通用送音频接口。
     *
     * @param obj     识别或唤醒的实例
     * @param pcmPath 音频路径
     * @param def     识别或唤醒使用的Def子类实例。在识别中使用该实例进行后端点、ResponseTimeOut、SpeechTimeOut的检测；
     *                唤醒中使用它检测是否唤醒，收到唤醒回调前会等待1.5s
     * @return 一共送了音频的毫秒数，目前只支持识别模块
     */
    public int loadPcmAndAppendAudioData(String obj, NativeHandle mNativeHandle, String pcmPath, Def... def) {
        String Tag = this.Tag + ".loadPcmAndAppendAudioData";
        File pcmFile = new File(pcmPath);

        // 计算每次调用appendAudioData后需要sleep的时间。如果pcmTime > passTime，则sleep pcmTime - passTime的时间
        Date baseTime = null; // 开始送音频的时间
        long passTime = 0; // 真实过去的时间
        long pcmTime = 0; // 送入音频的时间
        long inputSize = 0; // 送入音频的字节数

        if (!pcmFile.exists()) {
            Log.e(Tag, "pcm文件不存在");
            Log.e(Tag, pcmPath);
            return 0;
        }

        try {
            DataInputStream data = new DataInputStream(new FileInputStream(pcmPath));
            byte[] byteData;
            byteData = new byte[data.available()];
            data.read(byteData);
            data.close();

            int location = 0;
            int position = byteData.length;

            // 去掉头44个字节
            if (byteData[0] == (byte) 'R' && byteData[1] == (byte) 'I'
                    && byteData[2] == (byte) 'F' && byteData[3] == (byte) 'F') {
                byte[] new_bytes = new byte[position - 44];
                System.arraycopy(byteData, 44, new_bytes, 0, position - 44);
                byteData = new_bytes;
                Log.i(Tag, "delete wav head");
            }

            if ("mvw".equals(obj)) {
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
                    int ret;
                    if (end > bytesEachTimeSend) {
                        System.arraycopy(byteData, location, mid, 0, bytesEachTimeSend);
                        ret = libissmvw.appendAudioData(mNativeHandle, mid, bytesEachTimeSend);
                    } else {
                        System.arraycopy(byteData, location, mid, 0, end);
                        ret = libissmvw.appendAudioData(mNativeHandle, mid, end);
                    }

                    switch (ret) {
                        case ISSErrors.ISS_SUCCESS:
//                            ILog.i("MvwSession.appendAudio", "ISS_SUCCESS", isWriteToActivity);
                            break;
                        case ISSErrors.ISS_ERROR_INVALID_PARA:
                            Log.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_PARA");
                            break;
                        case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                            Log.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_HANDLE");
                            break;
                        case ISSErrors.ISS_ERROR_INVALID_CALL:
                            Log.e("MvwSession.appendAudio", "ISS_ERROR_INVALID_CALL");
                            break appendAudioData_label;
                        case ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER:
                            Log.e("MvwSession.appendAudio", "ISS_ERROR_NO_ENOUGH_BUFFER");
                            break;
                        default:
                            Log.e("MvwSession.appendAudio", "UnHandled MSG");
                    }
                    location = location + bytesEachTimeSend;
                    inputSize += bytesEachTimeSend;
                    pcmTime = inputSize / 32;
                    passTime = dfs.parse(dfs.format(new Date())).getTime() - baseTime.getTime();
                    if (pcmTime > passTime)
                        sleep(pcmTime - passTime);
                }
                if (def.length > 0) {
                    DefMVW d = (DefMVW) def[0];
                    int i = 0;
                    while (!d.msgVwWakeup && i++ < 250) {
                        sleep(10);
                    }
                }
            }
//            else if ("sr".equals(obj)) {
//                int bytesEachTimeSend = 320; // 每次送的音频数
//                int sleepTimeEachTimeSend = 10; // 每次送音频后的间隔时间，废弃
//                int appendTimes = 0; // 送音频次数
//                int ret_appendAudioData = ISSErrors.ISS_SUCCESS; // appendAudioData接口的返回值
//                byte[] mid;
//
//                if (def.length > 0) {
//                    ((DefSR) def[0]).msgSpeechEnd = false;
//                }
//
//                baseTime = dfs.parse(dfs.format(new Date()));
//                // 当检测到语音后端点、ResponseTimeOut、SpeechTimeOut、appendAudioData返回错误时会停止送录音
//                appendAudioData_label:
//                while (location < byteData.length && location < position && (def.length <= 0 ||
//                        (!((DefSR) def[0]).msgSpeechEnd && !((DefSR) def[0]).msgSpeechTimeOut &&
//                                !((DefSR) def[0]).msgResponseTimeOut))) {
//                    appendTimes++;
//                    int end1 = byteData.length - location;
//                    int end2 = position - location;
//                    int end = 0;
//                    if (end1 >= end2)
//                        end = end2;
//                    else {
//                        end = end1;
//                    }
//
//                    mid = new byte[bytesEachTimeSend];
//                    if (end > bytesEachTimeSend) {
//                        System.arraycopy(byteData, location, mid, 0, bytesEachTimeSend);
//                    } else {
//                        System.arraycopy(byteData, location, mid, 0, end);
//                    }
//                    ret_appendAudioData = ((SrSession) obj).appendAudioData(mid);
//                    switch (ret_appendAudioData) {
//                        case ISSErrors.ISS_SUCCESS:
////                            ILog.i("SrSession.appendAudio", "ISS_SUCCESS", isWriteToActivity);
//                            break;
//                        case ISSErrors.ISS_ERROR_INVALID_PARA:
//                            ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_PARA",
//                                    isWriteToActivity);
//                            break;
//                        case ISSErrors.ISS_ERROR_INVALID_HANDLE:
//                            ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_HANDLE",
//                                    isWriteToActivity);
//                            break;
//                        case ISSErrors.ISS_ERROR_INVALID_CALL:
//                            ILog.e("SrSession.appendAudio", "ISS_ERROR_INVALID_CALL",
//                                    isWriteToActivity);
//                            break appendAudioData_label;
//                        case ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER:
//                            ILog.e("SrSession.appendAudio", "ISS_ERROR_NO_ENOUGH_BUFFER",
//                                    isWriteToActivity);
//                            break;
//                        case ISSErrors.INVALID_SESSION:
//                            ILog.e("SrSession.appendAudio", "INVALID_SESSION", isWriteToActivity);
//                            break;
//                        case ISSErrors.REMOTE_EXCEPTION:
//                            ILog.e("SrSession.appendAudio", "REMOTE_EXCEPTION", isWriteToActivity);
//                            break;
//                        default:
//                            ILog.e("SrSession.appendAudio", "UnHandled MSG", isWriteToActivity);
//                    }
//                    location = location + bytesEachTimeSend;
////                    Thread.sleep(sleepTimeEachTimeSend);
//                    inputSize += bytesEachTimeSend;
//                    pcmTime = inputSize / 32;
//                    passTime = dfs.parse(dfs.format(new Date())).getTime() - baseTime.getTime();
//                    if (pcmTime > passTime)
//                        sleep(pcmTime - passTime);
//                }
//
//                return appendTimes * bytesEachTimeSend / 32;
//            }
            byteData = null;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    public CrashUpLoadRequest crashUpLoadRequest = new CrashUpLoadRequest() {
        @Override
        public void uploadCrash(List<CrashInfo> list, UploadListener uploadListener) {
            Log.i(Tag, "uploadCrash() called.");
            Log.i(Tag, Arrays.toString(list.toArray()));
            try {
                PrintWriter printWriter = new PrintWriter(new FileWriter(new File(Environment
                        .getExternalStorageDirectory().getAbsolutePath() + "/test_crash.txt")));
                for (CrashInfo info :
                        list) {
                    printWriter.println(info.toJson());
                }
                printWriter.close();
            } catch (IOException e) {
                e.printStackTrace();
            }

            uploadListener.onUploadSuccess();
        }

        @Override
        public void uploadDump(List<DumpEntity> list, UploadListener uploadListener) {
            Log.i(Tag, "uploadDump() called.");
        }

        @Override
        public void uploadLog(List<HeartbeatInfo> list, UploadListener uploadListener) {
            Log.i(Tag, "uploadLog() called.");
        }
    };

    public String readFile(String filePath) {
        /**
         * 读取文件内容。
         */
        File fi = null;
        BufferedReader br = null;
        StringBuilder sb;

        try {
            fi = new File(filePath);
            sb = new StringBuilder();
            br = new BufferedReader(new InputStreamReader(new FileInputStream(fi), "utf8"));
            String line = null;
            while ((line = br.readLine()) != null)
                sb.append(line + "\n");

            br.close();
            return sb.toString();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public String[] getFilesListInDir(String path) {
        File file = new File(path);
        String Tag = this.Tag + ".getFilesListInDir";
        String[] fileList = null;

        if (!file.isDirectory()) {
            Log.e(Tag, "not a directory");
        } else {
            fileList = file.list();

            for (int i = 0; i < fileList.length; i++) {
                fileList[i] = path.concat("/".concat(fileList[i]));
            }
        }

        Log.i("getFilesListInDir", Arrays.toString(fileList));
        return fileList;
    }

    public void sleep(long time) {
        try {
            Thread.sleep(time);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 用于计时，需要在需要计时的时候调用一次以设定初始时间和总的时间
     *
     * @param runTime 计时时长，以秒为单位
     */
    public void setRunTime(long runTime) {
        try {
            this.startTime = dfs.parse(dfs.format(new Date()));
            this.runTime = runTime;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 是否计时结束，若未设置计时时间会始终返回false
     *
     * @return 表示是否计时结束的布尔值
     */
    public boolean isTimeUp() {
        if (runTime > 0) {
            Date end = null;

            try {
                end = dfs.parse(dfs.format(new Date()));
                long between = end.getTime() - startTime.getTime();

                if (between >= (runTime * 1000)) {
                    Log.i(Tag, "当前运行" + between / 1000.0 + "s，结束");
                    runTime = -1;
                    return true;
                }
            } catch (Exception e) {
                e.printStackTrace();
                return false;
            }
        }

        return false;
    }

    /**
     * 获取当前java进程占用内存。
     *
     * @return 当前java进程占用内存的字节数。
     */
    public long getUsedMem() {
        Runtime runTime = Runtime.getRuntime();
        return runTime.totalMemory() - runTime.freeMemory();
    }

    /**
     * 计算两个内存值的差。
     *
     * @param memOfSomeTimeStart 一个过程开始时的内存值，以byte为单位。
     * @param menOfSomeTimeEnd   一个过程结束时的内存值，以byte为单位。
     * @return menOfSomeTimeEnd - memOfSomeTimeStart所得的内存差，单位为M。
     */
    public double getMemDiff(long memOfSomeTimeStart, long menOfSomeTimeEnd) {
        return ((double) menOfSomeTimeEnd - memOfSomeTimeStart) / 1000000;
    }

    /**
     * 删除单个文件
     *
     * @param sPath 被删除文件的文件名
     * @return 单个文件删除成功返回true，否则返回false
     */
    public boolean deleteFile(String sPath) {
        boolean flag = false;
        File file = new File(sPath);
        // 路径为文件且不为空则进行删除
        if (file.isFile() && file.exists()) {
            file.delete();
            flag = true;
        }
        return flag;
    }

    /**
     * 删除目录（文件夹）以及目录下的文件
     *
     * @param sPath 被删除目录的文件路径
     * @return 目录删除成功返回true，否则返回false
     */
    public boolean deleteDirectory(String sPath) {
        //如果sPath不以文件分隔符结尾，自动添加文件分隔符
        if (!sPath.endsWith(File.separator)) {
            sPath = sPath + File.separator;
        }
        File dirFile = new File(sPath);
        //如果dir对应的文件不存在，或者不是一个目录，则退出
        if (!dirFile.exists() || !dirFile.isDirectory()) {
            return false;
        }
        boolean flag = true;
        //删除文件夹下的所有文件(包括子目录)
        File[] files = dirFile.listFiles();
        for (int i = 0; i < files.length; i++) {
            //删除子文件
            if (files[i].isFile()) {
                flag = deleteFile(files[i].getAbsolutePath());
                if (!flag) break;
            } //删除子目录
            else {
                flag = deleteDirectory(files[i].getAbsolutePath());
                if (!flag) break;
            }
        }
        if (!flag) return false;
        //删除当前目录
        if (dirFile.delete()) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * 过参数is_removable控制，来获取内部存储和外部存储（内外sd卡）的路径，参数 is_removable为false
     * 时得到的是内置sd卡路径，为true则为外置sd卡路径
     *
     * @param mContext
     * @param is_removale
     * @return
     */
    public static String getStoragePath(Context mContext, boolean is_removale) {

        StorageManager mStorageManager = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);
        Class<?> storageVolumeClazz = null;
        try {
            storageVolumeClazz = Class.forName("android.os.storage.StorageVolume");
            Method getVolumeList = mStorageManager.getClass().getMethod("getVolumeList");
            Method getPath = storageVolumeClazz.getMethod("getPath");
            Method isRemovable = storageVolumeClazz.getMethod("isRemovable");
            Object result = getVolumeList.invoke(mStorageManager);
            final int length = Array.getLength(result);
            for (int i = 0; i < length; i++) {
                Object storageVolumeElement = Array.get(result, i);
                String path = (String) getPath.invoke(storageVolumeElement);
                boolean removable = (Boolean) isRemovable.invoke(storageVolumeElement);
                if (is_removale == removable) {
                    return path;
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        return null;
    }
}


