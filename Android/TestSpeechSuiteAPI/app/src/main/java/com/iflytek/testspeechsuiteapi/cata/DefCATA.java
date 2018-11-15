package com.iflytek.testspeechsuiteapi.cata;


import android.os.Environment;

import com.google.gson.Gson;
import com.iflytek.cata.ICataInitListener;
import com.iflytek.cata.ICataListener;
import com.iflytek.testspeechsuiteapi.Def;
import com.iflytek.testspeechsuiteapi.ILog;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.Vector;

public class DefCATA extends Def {
    public String resDir = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek/res/cata";
    public String resDir_wrong = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/iflytek";

    public String segValuePath = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/cata/cata_add.txt";
    public String segValuePath_en = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/cata/cata_add_en.txt";
    public String segValueDelPath = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/cata/cata_del.txt";
    public String segValuePath_20 = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/cata/cata_add_20.txt";
    public String segValuePath_1 = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/cata/cata_add_1.txt";
    public String segValuePath_temp = Environment.getExternalStorageDirectory().getAbsolutePath()
            + "/TestRes/cata/temp.txt";

    public boolean msgInitStatus = false;
    public boolean msgInitStatus_success = false;
    public boolean msgInitStatus_fail = false;
    public boolean msgResult = false;

    public String asyncSearchRet;

    Gson gson = new Gson();

    public ICataInitListener iCataInitListener = new ICataInitListener() {
        @Override
        public void onCataInited(boolean state, int err) {
            if (state) {
                msgInitStatus_success = true;
                ILog.i("CataSession.onCataInited", "初始化成功", isWriteToActivity);
            } else {
                msgInitStatus_fail = true;
                ILog.e("CataSession.onCataInited", "初始化失败，err=" + err, isWriteToActivity);
            }
            msgInitStatus = true;
        }
    };

    public ICataListener iCataListener = new ICataListener() {
        @Override
        public void onMsgProc(int iMsg, int wParam, String lParam) {
            asyncSearchRet = lParam;
            msgResult = true;
            ILog.i("CataSession.onMsgProc", "iMsg:" + iMsg + ",wParam:" + wParam + ",lParam:" + lParam, isWriteToActivity);
        }
    };

    public Vector<Vector<SegValue>> createSegValuesList(String path, int segValuesNum, int... cutTabNumber) {
        Vector<Vector<SegValue>> segValuesLists;
        if (segValuesNum == 0)
            segValuesLists = new Vector<>();
        else
            segValuesLists = new Vector<>(segValuesNum);

        try {
            BufferedReader bReader = new BufferedReader(new FileReader(path));
            String line;
            while ((line = bReader.readLine()) != null) {
                Vector<SegValue> segValues = new Vector<>();
                String[] parts = null;
                if (cutTabNumber.length > 0) {
                    // 对比一致性时，引擎的构建列表每行最后包含【add XXX.bin】，需去掉
                    parts = Arrays.copyOf(line.split("\t"), line.split("\t").length - cutTabNumber[0]);
                } else {
                    parts = line.split("\t");
                }
                for (String part : parts) {
                    String[] p = part.split("\\|");
//                    ILog.d("createSegValuesList", "p:"+ Arrays.toString(p), isWriteToActivity);

                    String mField = "";
                    Vector<String> texts_name = null;
                    int mMode = 1;
                    if (p.length <= 3) {
                        mField = p[0];
                        texts_name = new Vector<>();
                        for (String t : p[1].split(";")) {
                            texts_name.add(t);
                        }
                        mMode = Integer.valueOf(p[2]);
                    } else {
                        mField = p[0];
                        texts_name = new Vector<>();
                        if (p[p.length - 1].equals("1") && p[p.length - 2].equals("1")) {
                            // 引擎工具把1|1直接适配成|4了。 坑
                            for (int i = 1; i < p.length - 2; i++) {
                                try {
                                    texts_name.add(p[i]);
                                } catch (Exception e) {
                                    ILog.e("createSegValuesList", "segvalue 解析异常", isWriteToActivity);
                                    ILog.e("createSegValuesList", Arrays.toString(p), isWriteToActivity);
                                    System.exit(0);
                                }
                            }
                            mMode = 4;
                        } else {
                            for (int i = 1; i < p.length - 1; i++) {
                                try {
                                    texts_name.add(p[i]);
                                } catch (Exception e) {
                                    ILog.e("createSegValuesList", "segvalue 解析异常", isWriteToActivity);
                                    ILog.e("createSegValuesList", Arrays.toString(p), isWriteToActivity);
                                    System.exit(0);
                                }
                            }
                            mMode = Integer.valueOf(p[p.length - 1]);
                        }
                    }

                    segValues.add(new SegValue(mField, mMode, texts_name));
                }
                segValuesLists.add(segValues);
                segValues = null;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return segValuesLists;
    }

    public String getSegValuesContent(Vector<SegValue> segValues) {
        Vector<Vector<String>> SegValuesContent = new Vector<>();

        for (SegValue segValue : segValues) {
            SegValuesContent.add(getSegValueContent(segValue));
        }

        return Arrays.toString(SegValuesContent.toArray());
    }

    public String segValues2JSON(Vector<SegValue> segValues) {
        return gson.toJson(segValues);
    }

    public void initMsg() {
        msgInitStatus = false;
        msgInitStatus_success = false;
        msgInitStatus_fail = false;
        msgResult = false;

        asyncSearchRet = null;
    }

    private Vector<String> getSegValueContent(SegValue segValue) {
        String mField = segValue.getField();
        int mMode = segValue.getMode();
        Vector<String> mTexts = segValue.getVal();

        Vector<String> content = new Vector<>();
        content.add(mField);
        content.add(mMode + "");
        for (String t : mTexts) {
            content.add(t);
        }

        return content;
    }
}
