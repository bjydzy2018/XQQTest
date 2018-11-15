package com.iflytek.testspeechsuiteapi.sr;


import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.sr.SrSession;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;


public class ToolSR {
    int err = -1;
    Tool tool = new Tool();
    private SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
            .getDefault());

    public void start(SrSession srSession, DefSR def, String path, String scene, int mode, String
            szCmd) {
        err = srSession.start(scene, mode, szCmd);
        HandleRet.handlStartRet(err);

        try {
            Date startTime = dfs.parse(dfs.format(new Date()));
            //appendAudioData
            tool.loadPcmAndAppendAudioData(srSession, path, def);

            srSession.endAudioData();
            while (!def.msgResult) {
                tool.sleep(10);
            }

            ILog.i("识别响应时间", dfs.parse(dfs.format(new Date())).getTime() - def.speechend_time -
                    startTime.getTime(), 0);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        srSession.stop();
        def.initMsg();
    }
}
