package com.iflytek.testspeechsuiteapi.cata;

import java.util.Vector;

/**
 *
 * 原始版本CATA使用内部的SegValue作为索引的一个记录，版本更新后使用JSON作为索引，SegValue废弃。
 * 为减小测试工具的改动，现在实现一个SegValue保证对之前代码的兼容，然后由一个翻译函数将SegValue转
 * 为JSON。
 */
public class SegValue {
    // 索引字段名，如"id"、 "name"、 "phone_num"或 "category"
    public String field = null;

    // 字段的值
    public Vector<String> val;

    /**
     * mode参数预定义宏
     * #define  ONLY_RAW			           0x0
     * #define  RAW_AND_PINYIN		           0x01
     * #define  TAG					           0x02   //用作标识字段, 只有该模式支持删除
     * #define	RAW_AND_TJIU	               0x03
     * #define	RAW_AND_PINYIN_AND_TJIU	       0x04
     */
    public static final int ONLY_RAW = 0;
    public static final int RAW_AND_PINYIN = 1;
    public static final int TAG = 2;
    public static final int RAW_AND_TJIU = 2;
    public static final int RAW_AND_PINYIN_AND_TJIU = 2;
    public int mode = 0;

    public SegValue(String field, int mode, Vector<String> texts) {
        this.field = field;
        this.mode = mode;
        val = texts;
    }

    public String getField() {
        return field;
    }

    public Vector<String> getVal() {
        return val;
    }

    public int getMode() {
        return mode;
    }
}

