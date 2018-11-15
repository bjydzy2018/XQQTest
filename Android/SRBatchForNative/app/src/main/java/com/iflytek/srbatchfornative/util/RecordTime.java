package com.iflytek.srbatchfornative.util;

/**
 * Created by qqxiong on 2018/11/15.
 */

public class RecordTime {
    public long SessionStartSucessTime = 0;
    public long SessionStartFailTime = 0;

    public long ResponseTimeoutTime = 0;

    public long SpeechStartTime = 0;
    public long SpeechEndTime = 0;
    public long SpeechTimeOut = 0;

    public long EndAudioDataEndTime = 0;
    public long NoSpeechTime = 0;

    public long ResultTime = 0;
    public long ErrorTime = 0;
    public long time_Result = 0;
    public long time_Result2 = 0;
    public String IfLocalResult = "";
    public String result_status = "";
    // public long IfCloudResult = 0;

    public long error_code = 0;
    public String ISR_Result = "";
    public String Confidence_score = "";
    public String Result = "";

    public long vadSpeechStartMs = 0;
    public long vadSpeechEndMs = 0;
    public String speechEndVadType = "";

    public void ReSet() {
        SessionStartSucessTime = 0;
        SessionStartFailTime = 0;

        ResponseTimeoutTime = 0;

        SpeechStartTime = 0;
        SpeechEndTime = 0;
        SpeechTimeOut = 0;

        EndAudioDataEndTime = 0;
        NoSpeechTime = 0;

        ResultTime = 0;
        ErrorTime = 0;

        IfLocalResult = "";
        time_Result = 0;
        time_Result2 = 0;
        result_status = "";
        //IfCloudResult = 0;

        error_code = 0;
        ISR_Result = "";
        Confidence_score = "";
        Result = "";

        vadSpeechStartMs = 0;
        vadSpeechEndMs = 0;
        speechEndVadType = "";
    }
}
