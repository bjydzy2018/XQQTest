package com.iflytek.testspeechsuitenative;
import java.text.SimpleDateFormat;
import java.util.Locale;
/**
 *
 * 公共定义类。
 */

public interface Def {
    SimpleDateFormat dfs = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale
            .getDefault());

    void initMsg();
}
