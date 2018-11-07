/**
 * @file    test_log.h
 * @brief   VA项目中log上一些易用的宏
 * 
 *  VA项目中log上一些易用的宏
 *
 * @author  qqxiong
 * @version 1.0
 * @date    2017/02/15
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version    <th>Date        <th>Author    <th>Notes</tr>
 *  <tr> <td> 1.0       <td>2017/02/15  <td>qqxiong      <td>create</tr>
 * </table>
 */

#ifndef __test_log_H__
#define __test_log_H__

//#include "iFly_Assert.h"
#include "iFly_Log.h"
namespace VA {


// 日志单体
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, test_log);

/**
 * @brief   方便使用的宏
 *  在使用单实例模式的时候，这几个宏可以更简洁的使用日志功能
 */
#define testlog_inst                           test_log::instance()
#define DEBUG_TRACE                           if ( testlog_inst ) testlog_inst->log_debug
#define DEBUG_TRACE_CRIT                      if ( testlog_inst ) testlog_inst->log_crit
#define DEBUG_TRACE_ERROR                     if ( testlog_inst ) testlog_inst->log_error
#define DEBUG_TRACE_WARN                      if ( testlog_inst ) testlog_inst->log_warn
#define DEBUG_TRACE_INFOR                     if ( testlog_inst ) testlog_inst->log_info
#define DEBUG_TRACE_VERBOSE                   if ( testlog_inst ) testlog_inst->log_verbose
#define DEBUG_TRACE_DEBUG                     if ( testlog_inst ) testlog_inst->log_debug
#define DEBUG_TRACE_PERF                      if ( testlog_inst ) testlog_inst->log_perf
#define DEBUG_TRACE_TRACE                     if ( testlog_inst ) testlog_inst->log_trace
#define DEBUG_TRACE_WMSG                      if ( testlog_inst ) testlog_inst->write_msg
#define DEBUG_TRACE_FLUSH                     if ( testlog_inst ) testlog_inst->flush

#define DEBUG_TRACE_LEVEL                     if ( testlog_inst ) testlog_inst->config().level
#define DEBUG_TRACE_OUTPUT                    if ( testlog_inst ) testlog_inst->config().output
#define DEBUG_TRACE_FILENAME                  if ( testlog_inst ) testlog_inst->config().file_name

// 能够自动统计性能并打印的
#define isslog_perf_n(name)              Log_Perf_Helper<Log_Timer, test_log>      __ph__(name);
#define isslog_perf_s(name,fmt)          Log_Perf_Helper<Log_Timer, test_log>      __ph__(name); __ph__.log fmt;
#define isslog_perf_nsec(name,nsec)      Log_Perf_Helper<Log_Timer, test_log>      __ph__(name, nsec);
#define isslog_perf_stop()                                                       __ph__.stop()

// 跟踪函数调用的方法
#define isslog_tracer(sub)               Log_Func_Tracer<test_log>                 __lt__(sub);
#define isslog_tracer_perf(sub)          isslog_perf_n(sub); isslog_tracer(sub);

// 初始化和逆初始化
#define isslog_open                      test_log::open
#define isslog_close                     test_log::close
#define isslog_open_ac                   Log_Auto_Close<test_log> __lac__; test_log::open

// 初始化和逆初始化
#define testlog_open                      test_log::open
#define testlog_close                     test_log::close
#define testlog_open_ac                   Log_Auto_Close<test_log> __lac__; test_log::open

// 配置类
typedef test_log_Cfg                      test_cfg;
typedef test_log_Cfg                      isslog_cfg;


#include "marco_log.inl"


}
#endif /* __test_log_H__ */