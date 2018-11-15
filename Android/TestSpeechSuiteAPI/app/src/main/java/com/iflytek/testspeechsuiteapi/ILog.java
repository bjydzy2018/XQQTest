package com.iflytek.testspeechsuiteapi;


import android.os.Environment;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

/**
 * 带日志文件输入的，又可控开关的日志调试
 */
public class ILog {
    // 日志文件总开关
    private static Boolean MYLOG_SWITCH = true;
    // 日志写入文件开关
    public static Boolean MYLOG_WRITE_TO_FILE = true;
    // 输入日志类型，w代表只输出告警信息等，v代表输出所有信息
    private static char MYLOG_TYPE = 'v';
    // 日志文件在sdcard中的路径
    private static String MYLOG_PATH_SDCARD_DIR =
            Environment.getExternalStorageDirectory().getAbsolutePath() + "/TestRes/log";
    // sd卡中日志文件的最多保存天数
    private static int SDCARD_LOG_FILE_SAVE_DAYS = 0;
    //日志文件名
    private static String MYLOGFILEName = new SimpleDateFormat("HH-mm-ss", Locale.getDefault()).format(new Date())+".log";
    // 日志的输出格式
    private static SimpleDateFormat myLogSdf = new SimpleDateFormat(
            "MM-dd HH:mm:ss:SSS", Locale.getDefault());
    // 日志文件格式
    private static SimpleDateFormat logfile = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());

    //主活动
    public static MainActivity mainActivity;

    public static void w(String tag, Object msg) { // 警告信息
        log(tag, msg.toString(), 'w', 1);
    }

    public static void e(String tag, Object msg) { // 错误信息
        log(tag, msg.toString(), 'e', 1);
    }

    public static void d(String tag, Object msg) {// 调试信息
        log(tag, msg.toString(), 'd', 1);
    }

    public static void i(String tag, Object msg) {//
        log(tag, msg.toString(), 'i', 1);
    }

    public static void v(String tag, Object msg) {
        log(tag, msg.toString(), 'v', 1);
    }

    public static void w(String tag, String text) {
        log(tag, text, 'w', 1);
    }

    public static void e(String tag, String text) {
        log(tag, text, 'e', 1);
    }

    public static void d(String tag, String text) {
        log(tag, text, 'd', 1);
    }

    public static void i(String tag, String text) {
        log(tag, text, 'i', 1);
    }

    public static void v(String tag, String text) {
        log(tag, text, 'v', 1);
    }

    public static void w(String tag, Object msg, int ifWriteToActivity) { // 警告信息
        log(tag, msg.toString(), 'w', ifWriteToActivity);
    }

    public static void e(String tag, Object msg, int ifWriteToActivity) { // 错误信息
        log(tag, msg.toString(), 'e', ifWriteToActivity);
    }

    public static void d(String tag, Object msg, int ifWriteToActivity) {// 调试信息
        log(tag, msg.toString(), 'd', ifWriteToActivity);
    }

    public static void i(String tag, Object msg, int ifWriteToActivity) {
        log(tag, msg.toString(), 'i', ifWriteToActivity);
    }

    public static void v(String tag, Object msg, int ifWriteToActivity) {
        log(tag, msg.toString(), 'v', ifWriteToActivity);
    }

    public static void w(String tag, String text, int ifWriteToActivity) {
        log(tag, text, 'w', ifWriteToActivity);
    }

    public static void e(String tag, String text, int ifWriteToActivity) {
        log(tag, text, 'e', ifWriteToActivity);
    }

    public static void d(String tag, String text, int ifWriteToActivity) {
        log(tag, text, 'd', ifWriteToActivity);
    }

    public static void i(String tag, String text, int ifWriteToActivity) {
        log(tag, text, 'i', ifWriteToActivity);
    }

    public static void v(String tag, String text, int ifWriteToActivity) {
        log(tag, text, 'v', ifWriteToActivity);
    }

    /**
     * 根据tag, msg和等级，输出日志
     *
     * @param tag
     * @param msg
     * @param level
     * @return void
     * @since v 1.0
     */
    private static void log(String tag, String msg, char level, int ifWriteToActivity) {
        if (MYLOG_SWITCH) {
            if ('e' == level && ('e' == MYLOG_TYPE || 'v' == MYLOG_TYPE)) { // 输出错误信息
                Log.e(tag, msg);
            } else if ('w' == level && ('w' == MYLOG_TYPE || 'v' == MYLOG_TYPE)) {
                Log.w(tag, msg);
            } else if ('d' == level && ('d' == MYLOG_TYPE || 'v' == MYLOG_TYPE)) {
                Log.d(tag, msg);
            } else if ('i' == level && ('i' == MYLOG_TYPE || 'v' == MYLOG_TYPE)) {
                Log.i(tag, msg);
            } else {
                Log.v(tag, msg);
            }
            if (ifWriteToActivity == 1)
                mainActivity.setText(tag + " " + msg);
            if (MYLOG_WRITE_TO_FILE)
                writeLogtoFile(String.valueOf(level), tag, msg);
        }
    }

    /**
     * 打开日志文件并写入日志
     */
    private static void writeLogtoFile(String mylogtype, String tag, String text) {// 新建或打开日志文件
        Date nowtime = new Date();
        String needWriteFile = logfile.format(nowtime);
        String needWriteMessage = myLogSdf.format(nowtime) + "  " + mylogtype
                + "  " + tag + "  " + text;

        try {
            File dir = new File(MYLOG_PATH_SDCARD_DIR+"/"+needWriteFile);
            if (!dir.exists()) {
                dir.mkdirs();
            }

            File file = new File(dir, MYLOGFILEName);

            FileWriter filerWriter = new FileWriter(file, true);//后面这个参数代表是不是要接上文件中原来的数据，不进行覆盖
            BufferedWriter bufWriter = new BufferedWriter(filerWriter);
            bufWriter.write(needWriteMessage);
            bufWriter.newLine();
            bufWriter.close();
            filerWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 删除指定的日志文件
     */
    public static void delFile() {// 删除日志文件
        String needDelFile = logfile.format(getDateBefore());
        File file = new File(MYLOG_PATH_SDCARD_DIR, needDelFile + MYLOGFILEName);
        if (file.exists()) {
            file.delete();
        }
    }

    /**
     * 得到现在时间前的几天日期，用来得到需要删除的日志文件名
     */
    private static Date getDateBefore() {
        Date nowtime = new Date();
        Calendar now = Calendar.getInstance();
        now.setTime(nowtime);
        now.set(Calendar.DATE, now.get(Calendar.DATE)
                - SDCARD_LOG_FILE_SAVE_DAYS);
        return now.getTime();
    }

}