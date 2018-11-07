/**
 * @file    mvw_log.h
 * @brief   VA项目中log上一些易用的宏
 * 
 *  VA项目中log上一些易用的宏
 *
 * @author  mbqi
 * @version 1.0
 * @date    2016/04/23
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version    <th>Date        <th>Author    <th>Notes</tr>
 *  <tr> <td> 1.0       <td>2016/04/23  <td>mbqi      <td>create</tr>
 * </table>
 */

#ifndef __mvw_log_H__
#define __mvw_log_H__

//#include "iFly_Assert.h"
#include "iFly_Log.h"
namespace VA {
// 日志单体
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, mvw_log);

/**
 * @brief   方便使用的宏
 *  在使用单实例模式的时候，这几个宏可以更简洁的使用日志功能
 */
#ifndef  MY_LOG_DISABLE

    #define mvwlog_inst                           mvw_log::instance()
    #define DEBUG_TRACE                           if ( mvwlog_inst ) mvwlog_inst->log_debug
    #define DEBUG_TRACE_CRIT                      if ( mvwlog_inst ) mvwlog_inst->log_crit
    #define DEBUG_TRACE_ERROR                     if ( mvwlog_inst ) mvwlog_inst->log_error
    #define DEBUG_TRACE_WARN                      if ( mvwlog_inst ) mvwlog_inst->log_warn
    #define DEBUG_TRACE_INFOR                     if ( mvwlog_inst ) mvwlog_inst->log_info
    #define DEBUG_TRACE_VERBOSE                   if ( mvwlog_inst ) mvwlog_inst->log_verbose
    #define DEBUG_TRACE_DEBUG                     if ( mvwlog_inst ) mvwlog_inst->log_debug
    #define DEBUG_TRACE_PERF                      if ( mvwlog_inst ) mvwlog_inst->log_perf
    #define DEBUG_TRACE_TRACE                     if ( mvwlog_inst ) mvwlog_inst->log_trace
    #define DEBUG_TRACE_WMSG                      if ( mvwlog_inst ) mvwlog_inst->write_msg
    #define DEBUG_TRACE_FLUSH                     if ( mvwlog_inst ) mvwlog_inst->flush

	#define DEBUG_TRACE_LEVEL                     if ( mvwlog_inst ) mvwlog_inst->config().level
	#define DEBUG_TRACE_OUTPUT                    if ( mvwlog_inst ) mvwlog_inst->config().output
	#define DEBUG_TRACE_FILENAME                  if ( mvwlog_inst ) mvwlog_inst->config().file_name

    // 能够自动统计性能并打印的
    #define isslog_perf_n(name)              Log_Perf_Helper<Log_Timer, mvw_log>      __ph__(name);
    #define isslog_perf_s(name,fmt)          Log_Perf_Helper<Log_Timer, mvw_log>      __ph__(name); __ph__.log (fmt);
    #define isslog_perf_nsec(name,nsec)      Log_Perf_Helper<Log_Timer, mvw_log>      __ph__(name, nsec);
    #define isslog_perf_stop()                                                       __ph__.stop()

    // 跟踪函数调用的方法
    #define isslog_tracer(sub)               Log_Func_Tracer<mvw_log>                 __lt__(sub);
    #define isslog_tracer_perf(sub)          isslog_perf_n(sub); isslog_tracer(sub);

    // 初始化和逆初始化
    #define mvwlog_open                      mvw_log::open
    #define mvwlog_close                     mvw_log::close
    #define mvwlog_open_ac                   Log_Auto_Close<mvw_log> __lac__; mvw_log::open

#else
    #define isslog_inst
    #define isslog                     
    #define isslog_crit                
    #define isslog_error               
    #define isslog_warn                
    #define isslog_info                
    #define isslog_verbose             
    #define isslog_dbg                 
    #define isslog_perf                
    #define isslog_trace               
    #define isslog_wmsg                
    #define isslog_flush               

    // 能够自动统计性能并打印的
    #define isslog_perf_n(name)        
    #define isslog_perf_s(name,fmt)    
    #define isslog_perf_nsec(name,nsec)
    #define isslog_perf_stop()         

    // 跟踪函数调用的方法
    #define isslog_tracer(sub)         
    #define isslog_tracer_perf(sub)    

    // 初始化和逆初始化
    #define mvwlog_open                
    #define mvwlog_close               
    #define mvwlog_open_ac             

#endif

// 配置类
typedef mvw_log_Cfg                      mvwlog_cfg;


// 全局变量的方式打开
class _LOG_{
public:
    _LOG_(){
        mvwlog_cfg default_cfg("va.log","VAVoiceAssistantEngine Log");
        default_cfg.level(lgl_all);
        default_cfg.output(lgo_file|lgo_console);
        mvwlog_open("va.cfg","mvw_logger",&default_cfg);
    }
    ~_LOG_()
    {
        mvwlog_close();
    }
};

#include "marco_log.inl"

}
#endif /* __mvwlog_H__ */