package com.iflytek.testspeechsuitenative.sr;

import android.os.Environment;
import android.util.Log;

import com.iflytek.speech.ISSErrors;
import com.iflytek.speech.libisssr;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;


public class TestSTKS {
    DefSR def = new DefSR();
    String Tag = "TestSTKS";

    public TestSTKS() {
        libisssr.setMachineCode("test_speechsuite_machinecode");

        libisssr.create(def.resDir, def.mSRListener);
        while (!def.msgInitSuccess) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        libisssr.setParam("longitude", "117.17");
        libisssr.setParam("latitude", "31.837463");
    }

    public void test() {
        String pcmPath = "/issTest/pcmList/isr_list_time.txt";

        String p = null;
        List<String> pcmPaths = new ArrayList<>();
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream
                    (Environment.getExternalStorageDirectory().getAbsolutePath() + pcmPath), "utf-8"));

            while ((p = br.readLine()) != null) {
                pcmPaths.add(p);
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        for (int j = 0; j < 100; j++) {
            for (String pcm : pcmPaths) {
                int err = libisssr.start("all", 0, null);
                if (err != 0) {
                    Log.e(Tag, "start返回：" + err);
                    continue;
                }

                loadPcmAndAppendAudioData(pcm);
                libisssr.endAudioData();

                int i = 0;
                while (!def.msgResult && i++ < 3000) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }

                Log.i(Tag, "第一次识别结果：" + def.srResult);

                String hotword = parsePOIRet(def.srResult);
                Log.i(Tag, "热词结果：" + hotword);
                def.initMsg();

                if (hotword != null) {
                    err = libisssr.start("selectlist_poi", 1, hotword);
                    if (err != 0)
                        break;

                    loadPcmAndAppendAudioData(pcm);
                    libisssr.endAudioData();

                    i = 0;
                    while (!def.msgResult && i++ < 3000) {
                        try {
                            Thread.sleep(10);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                    Log.i(Tag, "第二次识别结果：" + def.srResult);
                }
                def.initMsg();
            }
        }
    }

    /**
     * 不跑云端，使用固定cmdlist只跑本地分词识别。
     */
    public void test1() {
        for (int i = 0; i < 10000; i++) {
            int err = libisssr.start("selectlist_poi", 1, "{\"list_name\":\"poi\",\"select_list\":[{\"name\":\"20路;20路区间;27路;28路;47路;48路;181路;209路...\"},{\"name\":\"胡家庙(公交站)\"},{\"name\":\"3号线\"},{\"name\":\"胡家庙(地铁站)\"},{\"name\":\"淳化县\"},{\"name\":\"胡家庙镇\"},{\"name\":\"淳化县\"},{\"name\":\"胡家庙村\"},{\"name\":\"北碚区\"},{\"name\":\"胡家庙\"},{\"name\":\"涪陵区\"},{\"name\":\"胡家庙\"},{\"name\":\"912路\"},{\"name\":\"胡家庙(招呼站)(公交站)\"},{\"name\":\"723路\"},{\"name\":\"胡家庙(地铁口)(公交站)\"},{\"name\":\"622路\\/K622路\"},{\"name\":\"胡家庙(东二环路)(公交站)\"},{\"name\":\"东街御泉矿泉水胡家庙直销点\"},{\"name\":\"胡家庙中学\"},{\"name\":\"211国道附近\"},{\"name\":\"淳化县胡家庙镇政府\"},{\"name\":\"G69银百高速与G69银百高速出口交叉口西北100米\"},{\"name\":\"胡家庙出口(G69银百高速出口西北向)\"},{\"name\":\"荣昌区\"},{\"name\":\"胡家庙\"},{\"name\":\"丰都县\"},{\"name\":\"胡家庙\"},{\"name\":\"涪陵区\"},{\"name\":\"胡家庙\"}]}");
            if (err != 0) {
                Log.e(Tag, "start返回：" + err);
                continue;
            }

            loadPcmAndAppendAudioData(Environment.getExternalStorageDirectory().getAbsolutePath() + "/pcm/400/shiqu/LSA/C3/hight_c3_1s_5s/hight_c3_1s_5s_7_encode.pcm");
            libisssr.endAudioData();

            i = 0;
            while (!def.msgResult && i++ < 1000) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            def.initMsg();

            err = libisssr.start("selectlist_poi", 1, "{\"list_name\":\"poi\",\"select_list\":[{\"name\":\"皇姑区\"},{\"name\":\"天山路与虹桥路交叉口\"},{\"name\":\"皇姑区\"},{\"name\":\"天山路与珠江桥交叉口\"},{\"name\":\"福山区\"},{\"name\":\"天山路与长江路交叉口\"},{\"name\":\"福山区\"},{\"name\":\"天山路与长江路交叉口\"},{\"name\":\"裕华区\"},{\"name\":\"天山大街与长江大道交叉口\"},{\"name\":\"天山大街与珠江大道交叉口南50米\"},{\"name\":\"天山南大街与珠江大道交叉口\"},{\"name\":\"皇姑区\"},{\"name\":\"兴工北街与天山路交叉口\"},{\"name\":\"皇姑区\"},{\"name\":\"天山路与新安江街交叉口\"},{\"name\":\"皇姑区\"},{\"name\":\"天山路与黄浦江街交叉口\"},{\"name\":\"长江路与天山路交叉口北\"},{\"name\":\"刁彦华内科诊所\"},{\"name\":\"长江路与天山路交叉口北200米\"},{\"name\":\"中国福利彩票\"},{\"name\":\"长江路与天山路交叉口北200米\"},{\"name\":\"伊之美发型设计\"},{\"name\":\"长江大道26号天山海世界对面长江大道与天山大街交叉口西行50米路南\"},{\"name\":\"英立口腔\"},{\"name\":\"长江大道与天山大街交叉口西行100米路北(北国超市旁边)\"},{\"name\":\"绝味鸭脖(长江店)\"},{\"name\":\"裕兴街道裕华路与天山大街交叉口天山海世界三楼\"},{\"name\":\"茶桔便(天山海世界广场店)\"}]}");
            if (err != 0) {
                Log.e(Tag, "start返回：" + err);
                continue;
            }

            loadPcmAndAppendAudioData(Environment.getExternalStorageDirectory().getAbsolutePath() + "/pcm/400/jiaoqu/C3/LSA/low_c3_2.5s_3.5s/low_c3_2.5s_3.5s_167_encode.pcm");
            libisssr.endAudioData();

            i = 0;
            while (!def.msgResult && i++ < 1000) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            def.initMsg();
        }
    }

    /**
     * 只跑云端。
     */
    public void test2() {
        for (int j = 0; j < 10000; j++) {
            int err = libisssr.start("all", 0, null);
            if (err != 0) {
                Log.e(Tag, "start返回：" + err);
                continue;
            }

            loadPcmAndAppendAudioData(Environment.getExternalStorageDirectory().getAbsolutePath() + "/pcm/400/shiqu/LSA/C3/hight_c3_1s_5s/hight_c3_1s_5s_7_encode.pcm");
            libisssr.endAudioData();

            int i = 0;
            while (!def.msgResult && i++ < 3000) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            String hotword = parsePOIRet(def.srResult);
            Log.i(Tag, "热词结果：" + hotword);

            def.initMsg();
        }
    }

    /**
     * 固定音频跑本地和云端
     */
    public void test3() {
        for (int q = 0; q < 10000; q++) {
            int err = libisssr.start("all", 0, null);
            if (err != 0) {
                Log.e(Tag, "start返回：" + err);
                continue;
            }

            loadPcmAndAppendAudioData(Environment.getExternalStorageDirectory().getAbsolutePath() + "/pcm/400/shiqu/LSA/C3/hight_c3_1s_5s/hight_c3_1s_5s_7_encode.pcm");
            libisssr.endAudioData();

            int i = 0;
            while (!def.msgResult && i++ < 3000) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            String hotword = parsePOIRet(def.srResult);
            Log.i(Tag, "热词结果：" + hotword);

            def.initMsg();

            if (hotword != null) {
                err = libisssr.start("selectlist_poi", 1, hotword);
                if (err != 0) {
                    Log.e(Tag, "start返回：" + err);
                    continue;
                }

                loadPcmAndAppendAudioData(Environment.getExternalStorageDirectory().getAbsolutePath() + "/pcm/400/shiqu/LSA/C3/hight_c3_1s_5s/hight_c3_1s_5s_7_encode.pcm");
                libisssr.endAudioData();

                i = 0;
                while (!def.msgResult && i++ < 3000) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                def.initMsg();
            }

            // 第二次
            err = libisssr.start("all", 0, null);
            if (err != 0) {
                Log.e(Tag, "start返回：" + err);
                continue;
            }

            loadPcmAndAppendAudioData(Environment.getExternalStorageDirectory().getAbsolutePath() + "/pcm/400/jiaoqu/C3/LSA/low_c3_2.5s_3.5s/low_c3_2.5s_3.5s_167_encode.pcm");
            libisssr.endAudioData();

            i = 0;
            while (!def.msgResult && i++ < 3000) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            hotword = parsePOIRet(def.srResult);
            Log.i(Tag, "热词结果：" + hotword);

            def.initMsg();

            if (hotword != null) {
                err = libisssr.start("selectlist_poi", 1, hotword);
                if (err != 0) {
                    Log.e(Tag, "start返回：" + err);
                    continue;
                }

                loadPcmAndAppendAudioData(Environment.getExternalStorageDirectory().getAbsolutePath() + "/pcm/400/jiaoqu/C3/LSA/low_c3_2.5s_3.5s/low_c3_2.5s_3.5s_167_encode.pcm");
                libisssr.endAudioData();

                i = 0;
                while (!def.msgResult && i++ < 3000) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                def.initMsg();
            }
        }
    }

    /**
     * 循环音频跑本地
     */
    public void test4() {
        String pcmPath = "/issTest/pcmList/isr_list_time.txt";

        String p = null;
        List<String> pcmPaths = new ArrayList<>();
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream
                    (Environment.getExternalStorageDirectory().getAbsolutePath() + pcmPath), "utf-8"));

            while ((p = br.readLine()) != null) {
                pcmPaths.add(p);
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        int err;
        for (int j = 0; j < 1000; j++) {
            int num = 0;

            for (String pcm : pcmPaths) {
                if (num % 2 == 0)
                    err = libisssr.start("selectlist_poi", 1, "{\"list_name\":\"poi\",\"select_list\":[{\"name\":\"皇姑区\"},{\"name\":\"天山路与虹桥路交叉口\"},{\"name\":\"皇姑区\"},{\"name\":\"天山路与珠江桥交叉口\"},{\"name\":\"福山区\"},{\"name\":\"天山路与长江路交叉口\"},{\"name\":\"福山区\"},{\"name\":\"天山路与长江路交叉口\"},{\"name\":\"裕华区\"},{\"name\":\"天山大街与长江大道交叉口\"},{\"name\":\"天山大街与珠江大道交叉口南50米\"},{\"name\":\"天山南大街与珠江大道交叉口\"},{\"name\":\"皇姑区\"},{\"name\":\"兴工北街与天山路交叉口\"},{\"name\":\"皇姑区\"},{\"name\":\"天山路与新安江街交叉口\"},{\"name\":\"皇姑区\"},{\"name\":\"天山路与黄浦江街交叉口\"},{\"name\":\"长江路与天山路交叉口北\"},{\"name\":\"刁彦华内科诊所\"},{\"name\":\"长江路与天山路交叉口北200米\"},{\"name\":\"中国福利彩票\"},{\"name\":\"长江路与天山路交叉口北200米\"},{\"name\":\"伊之美发型设计\"},{\"name\":\"长江大道26号天山海世界对面长江大道与天山大街交叉口西行50米路南\"},{\"name\":\"英立口腔\"},{\"name\":\"长江大道与天山大街交叉口西行100米路北(北国超市旁边)\"},{\"name\":\"绝味鸭脖(长江店)\"},{\"name\":\"裕兴街道裕华路与天山大街交叉口天山海世界三楼\"},{\"name\":\"茶桔便(天山海世界广场店)\"}]}");
                else
                    err = libisssr.start("selectlist_poi", 1, "{\"list_name\":\"poi\",\"select_list\":[{\"name\":\"北京市东城区\"},{\"name\":\"南剪子巷北口站自行车租赁点\"},{\"name\":\"中剪子胡同17\"},{\"name\":\"北京市交通执法总队第一执法大队(中剪子巷)\"},{\"name\":\"将台路商业街4号楼(和睦家医院斜对面)\"},{\"name\":\"巷子口(将台路)\"},{\"name\":\"北岗子巷52号楼\"},{\"name\":\"北岗子巷52号院\"},{\"name\":\"建外街道建外soho西区11号楼底商1108(恒惠路西侧)建外soho西区\"},{\"name\":\"巷子口(CBD商圈)\"}]}");
                if (err != 0) {
                    Log.e(Tag, "start返回：" + err);
                    continue;
                }

                loadPcmAndAppendAudioData(pcm);
                libisssr.endAudioData();

                int timeout = 0;
                while (!def.msgResult && timeout++ < 1000) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }

                num++;
                def.initMsg();
            }
        }
    }

    /**
     * 循环音频跑云端
     */
    public void test5() {
        String pcmPath = "/issTest/pcmList/isr_list_time.txt";

        String p = null;
        List<String> pcmPaths = new ArrayList<>();
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream
                    (Environment.getExternalStorageDirectory().getAbsolutePath() + pcmPath), "utf-8"));

            while ((p = br.readLine()) != null) {
                pcmPaths.add(p);
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        int err;
        for (int j = 0; j < 1000; j++) {
            for (String pcm : pcmPaths) {
                err = libisssr.start("all", 0, "null");
                if (err != 0) {
                    Log.e(Tag, "start返回：" + err);
                    continue;
                }

                loadPcmAndAppendAudioData(pcm);
                libisssr.endAudioData();

                int timeout = 0;
                while (!def.msgResult && timeout++ < 1000) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
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

    private int loadPcmAndAppendAudioData(String pcmPath) {
        String Tag = "loadPcmAndAppendAudioData";
        File pcmFile = new File(pcmPath);
        SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
                .getDefault());

        // 计算每次调用appendAudioData后需要sleep的时间。如果pcmTime > passTime，则sleep pcmTime - passTime的时间
        Date baseTime = null; // 开始送音频的时间
        long passTime = 0; // 真实过去的时间
        long pcmTime = 0; // 送入音频的时间
        long inputSize = 0; // 送入音频的字节数

        if (!pcmFile.exists()) {
            Log.e(Tag, pcmPath + "不存在");
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
//            if (byteData[0] == (byte) 'R' && byteData[1] == (byte) 'I'
//                    && byteData[2] == (byte) 'F' && byteData[3] == (byte) 'F') {
//                byte[] new_bytes = new byte[position - 44];
//                System.arraycopy(byteData, 44, new_bytes, 0, position - 44);
//                byteData = new_bytes;
//                Log.i(Tag, "删除wav头");
//            }

            int bytesEachTimeSend = 320; // 每次送的音频数
            int appendTimes = 0; // 送音频次数
            int ret_appendAudioData = ISSErrors.ISS_SUCCESS; // appendAudioData接口的返回值
            byte[] mid;

            baseTime = dfs.parse(dfs.format(new Date()));

            // 当检测到语音后端点、ResponseTimeOut、SpeechTimeOut、appendAudioData返回错误时会停止送录音
            appendAudioData_label:
            while (location < byteData.length && location < position && !def.msgSpeechEnd) {
                appendTimes++;
                int end1 = byteData.length - location;
                int end2 = position - location;
                int end = 0;
                if (end1 >= end2)
                    end = end2;
                else {
                    end = end1;
                }

                mid = new byte[bytesEachTimeSend];
                if (end > bytesEachTimeSend) {
                    System.arraycopy(byteData, location, mid, 0, bytesEachTimeSend);
                } else {
                    System.arraycopy(byteData, location, mid, 0, end);
                }
                ret_appendAudioData = libisssr.appendAudioData(mid, bytesEachTimeSend);
                mid = null;
                switch (ret_appendAudioData) {
                    case ISSErrors.ISS_SUCCESS:
//                            ILog.i("SrSession.appendAudio", "ISS_SUCCESS", isWriteToActivity);
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_PARA:
                        Log.e("SrSession.appendAudio", "ISS_ERROR_INVALID_PARA");
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_HANDLE:
                        Log.e("SrSession.appendAudio", "ISS_ERROR_INVALID_HANDLE");
                        break;
                    case ISSErrors.ISS_ERROR_INVALID_CALL:
                        Log.e("SrSession.appendAudio", "ISS_ERROR_INVALID_CALL");
                        break appendAudioData_label;
                    case ISSErrors.ISS_ERROR_NO_ENOUGH_BUFFER:
                        Log.e("SrSession.appendAudio", "ISS_ERROR_NO_ENOUGH_BUFFER");
                        break;
                    case ISSErrors.INVALID_SESSION:
                        Log.e("SrSession.appendAudio", "INVALID_SESSION");
                        break;
                    case ISSErrors.REMOTE_EXCEPTION:
                        Log.e("SrSession.appendAudio", "REMOTE_EXCEPTION");
                        break;
                    default:
                        Log.e("SrSession.appendAudio", "UnHandled MSG");
                }
                location = location + bytesEachTimeSend;
                inputSize += bytesEachTimeSend;
                pcmTime = inputSize / 32;
                passTime = dfs.parse(dfs.format(new Date())).getTime() - baseTime.getTime();
                if (pcmTime > passTime)
                    Thread.sleep(pcmTime - passTime);
            }
            byteData = null;
            return appendTimes * bytesEachTimeSend / 32;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }
}

