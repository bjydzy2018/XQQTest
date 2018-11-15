package com.iflytek.testspeechsuiteapi.sr;




import android.content.Context;
import android.os.Environment;
import android.util.Log;

import com.iflytek.testspeechsuiteapi.Def;
import com.iflytek.testspeechsuiteapi.ILog;
import com.iflytek.testspeechsuiteapi.Tool;
import com.iflytek.sr.SrSession;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;

/**
 * 测试可见即可说功能、稳定性。
 */

public class TestSTKS {
    SrSession srSession;
    DefSR def = new DefSR();
    Tool tool = new Tool();
    String Tag = "TestSTKS";
    int err;

    public TestSTKS(Context context) {
        srSession = SrSession.getInstance(context, def.isrListener, def.resDir, "asodhfoaweufh");
        int i = 0;
        while (!def.msgInitStatus && i++ < 1200) {
            tool.sleep(10);
        }

        tool.isWriteToActivity = 0;
        def.isWriteToActivity = 0;
    }

    public void testFunction() {
        String pcmPath = "/issTest/pcmList/isr_list_time.txt";
        String anotherPcmPath = "/issTest/pcmList/isr_list-冒烟800-android.txt";

        String p = null;
        List<String> pcmPaths = new ArrayList<>();
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream
                    (Environment.getExternalStorageDirectory().getAbsolutePath() + pcmPath), "utf-8"));

            while ((p = br.readLine()) != null) {
                pcmPaths.add(p);
            }
            br.close();

            br = new BufferedReader(new InputStreamReader(new FileInputStream
                    (Environment.getExternalStorageDirectory().getAbsolutePath() + anotherPcmPath), "utf-8"));

            while ((p = br.readLine()) != null) {
                pcmPaths.add(p);
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        for (int j = 0; j < 100; j++) {
            for (String pcm : pcmPaths) {
                err = srSession.start(SrSession.ISS_SR_SCENE_ALL, SrSession.ISS_SR_MODE_CLOUD_REC, null);
                if (err != 0) {
                    Log.e(Tag, "start返回："+err);
                    continue;
                }

                tool.loadPcmAndAppendAudioData(srSession, pcm, def);
                srSession.endAudioData();

                int i = 0;
                while (!def.msgResult && i++ < 3000) {
                    tool.sleep(10);
                }

//                ILog.i(Tag, "第一次识别结果：" + def.srResult, 0);
                System.out.println("第一次识别结果：" + def.srResult);

                String hotword = parsePOIRet(def.srResult);
//                ILog.i(Tag, "热词结果：" + hotword, 0);
                System.out.println("热词结果：" + hotword);
                def.initMsg();

                if (hotword != null) {
                    err = srSession.start("selectlist_poi", SrSession.ISS_SR_MODE_LOCAL_REC, hotword);
                    if (err != 0)
                        break;

                    tool.loadPcmAndAppendAudioData(srSession, pcm, def);
                    srSession.endAudioData();

                    i = 0;
                    while (!def.msgResult && i++ < 3000) {
                        tool.sleep(10);
                    }
//                    ILog.i(Tag, "第二次识别结果：" + def.srResult, 0);
                    System.out.println("第二次识别结果：" + def.srResult);
                }
                def.initMsg();
            }
        }
    }

    private String parsePOIRet(String ret) {
        try {
            JSONObject json = new JSONObject(ret);
            JSONArray poiend = json.getJSONObject("data").getJSONObject("result").getJSONArray("poiend");

            List<String> pois = new ArrayList<>();
            for (int i = 0; i < poiend.length(); i++) {
                JSONObject poi = poiend.getJSONObject(i);

                String address = poi.getString("address");
                String name = poi.getString("name");
                pois.add(address);
                pois.add(name);
            }

            String poiCmdListJson = poiNameArray2JSON(pois);
            pois = null;

            return poiCmdListJson;
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return null;
    }

    private String poiNameArray2JSON(List<String> pois) {
        JSONObject json = new JSONObject();
        try {
            json.put("list_name", "poi");

            JSONArray select_list = new JSONArray();
            for (int i = 0; i < pois.size(); i++) {
                JSONObject poi = new JSONObject();
                poi.put("name", pois.get(i));

                select_list.put(i, poi);
            }

            json.put("select_list", select_list);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        return json.toString();
    }
}

