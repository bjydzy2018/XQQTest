

// ============================================================================
// 方便的宏组合
// ============================================================================

#define isslog_tracer_func isslog_tracer(__FUNCTION__)
#define isslog_tracer_perf_func isslog_tracer_perf(__FUNCTION__)

// ASSERT然后打印日志
#define isslog_error_assert(exp, msg)    if (!(exp)) { DEBUG_TRACE_ERROR msg; SP_ASSERTS(exp, msg); }
#define isslog_error_assert_return(exp, msg, ret)    \
	if (!(exp)) { DEBUG_TRACE_ERROR msg; SP_ASSERTS(exp, msg); return ret; }
#define isslog_error_assert_return_no(exp, msg)  \
	if (!(exp)) { DEBUG_TRACE_ERROR msg; SP_ASSERTS(exp, msg); return ; }
#define isslog_warn_assert(exp, msg)     if (!(exp)) { DEBUG_TRACE_WARN msg;  SP_ASSERTS(exp, msg); }
#define isslog_warn_assert_return(exp, msg, ret) \
	if (!(exp)) { DEBUG_TRACE_WARN msg; SP_ASSERTS(exp, msg); return ret; }
#define isslog_warn_assert_return_no(exp, msg)   \
	if (!(exp)) { DEBUG_TRACE_WARN msg; SP_ASSERTS(exp, msg); return ; }

#define isslog_error_assert_throw(exp, msg, ret)	\
	if (!(exp)) { DEBUG_TRACE_ERROR msg; SP_ASSERTS(exp, msg); throw (ISS_EXCEPTION)ret; }

#define isslog_error_noassert(exp, msg)  if (!(exp)) { DEBUG_TRACE_ERROR msg; }
#define isslog_error_return(exp, msg, ret)   \
	if (!(exp)) { DEBUG_TRACE_ERROR msg; return ret; }
#define isslog_error_return_no(exp, msg)     \
	if (!(exp)) { DEBUG_TRACE_ERROR msg; return ; }
#define isslog_warn_noassert(exp, msg)   if (!(exp)) { DEBUG_TRACE_WARN msg; }
#define isslog_warn_return(exp, msg, ret)    \
	if (!(exp)) { DEBUG_TRACE_WARN msg; return ret; }
#define isslog_warn_return_no(exp, msg)  \
	if (!(exp)) { DEBUG_TRACE_WARN msg; return ; }

#define isslog_error_throw(exp, msg, ret)	\
	if (!(exp)) { DEBUG_TRACE_ERROR msg; throw (ISS_EXCEPTION)ret; }

// 根据返回值打印相同的信息
#define isslog_func(ret, msg)        if ( ret != 0 ) { DEBUG_TRACE_ERROR msg; } else { DEBUG_TRACE_VERBOSE msg; }
#define isslog_func_return(ret, msg)        if ( ret != 0 ) { DEBUG_TRACE_ERROR msg; return ret;} else { DEBUG_TRACE_VERBOSE msg; }
#define isslog_func_return_num(ret, msg, rc)        if ( ret != 0 ) { DEBUG_TRACE_ERROR msg; return rc;} else { DEBUG_TRACE_VERBOSE msg; }

//#define isslog_func_info(ret, msg)   if ( ret != 0 ) { DEBUG_TRACE_ERROR msg; } else { DEBUG_TRACE_INFOR msg; }

// 根据函数返回值打印不同的信息
#define isslog_func_verb(ret, succ_msg, fail_msg)    \
	if ( ret == 0 ) { DEBUG_TRACE_VERBOSE succ_msg; } else { DEBUG_TRACE_ERROR fail_msg; }
#define isslog_func_info(ret, succ_msg, fail_msg)    \
	if ( ret == 0 ) { DEBUG_TRACE_INFOR succ_msg; } else { DEBUG_TRACE_ERROR fail_msg; }
#define isslog_func_verb_return(ret, succ_msg, fail_msg) \
	if ( ret == 0 ) { DEBUG_TRACE_VERBOSE succ_msg; } else { DEBUG_TRACE_ERROR fail_msg; return ret; }
#define isslog_func_info_return(ret, succ_msg, fail_msg) \
	if ( ret == 0 ) { DEBUG_TRACE_INFOR succ_msg; } else { DEBUG_TRACE_ERROR fail_msg; return ret; }

// 判断表达式，打印不同的信息
#define isslog_func_expr(expr, succ_msg, fail_msg)   \
	if ( expr ) { DEBUG_TRACE_INFOR succ_msg; } else { DEBUG_TRACE_ERROR fail_msg; }
#define isslog_func_expr_return(expr, succ_msg, fail_msg, ret)   \
	if ( expr ) { DEBUG_TRACE_INFOR succ_msg; } else { DEBUG_TRACE_ERROR fail_msg; return ret; }

// 校验句柄，并打印日志
#define isslog_verify_inst(func, inst)               \
	if ( !(inst) ) { DEBUG_TRACE_ERROR("%s | %s handle is NULL. ret ISS_ERROR_INVALID_HANDLE", func, #inst); return ISS_ERROR_INVALID_HANDLE; }
#define isslog_verify_para(func, para)               \
	if ( !(para) ) { DEBUG_TRACE_ERROR("%s | para %s is NULL. ret ISS_ERROR_INVALID_PARA", func, #para); return ISS_ERROR_INVALID_PARA; }

#define isslog_verify_inst_func(inst)               \
	if ( !(inst) ) { DEBUG_TRACE_ERROR("%s | %s handle is NULL. ret ISS_ERROR_INVALID_HANDLE", __FUNCTION__, #inst); return ISS_ERROR_INVALID_HANDLE; }
#define isslog_verify_para_func(para)               \
	if ( !(para) ) { DEBUG_TRACE_ERROR("%s | para %s is NULL. ret ISS_ERROR_INVALID_PARA", __FUNCTION__, #para); return ISS_ERROR_INVALID_PARA; }

#define isslog_verify_inst_return(func, inst, ret)               \
	if ( !(inst) ) { DEBUG_TRACE_ERROR("%s | %s handle is NULL. ret %s", func, #inst, #ret); return ret; }
#define isslog_verify_para_return(func, para, ret)               \
	if ( !(para) ) { DEBUG_TRACE_ERROR("%s | para %s is NULL. ret %s", func, #para, #ret); return ret; }

#define isslog_verify_inst_return_func(inst, ret)               \
	if ( !(inst) ) { DEBUG_TRACE_ERROR("%s | %s handle is NULL. ret %s", __FUNCTION__, #inst, #ret); return ret; }
#define isslog_verify_para_return_func(para, ret)               \
	if ( !(para) ) { DEBUG_TRACE_ERROR("%s | para %s is NULL. ret %s", __FUNCTION__, #para, #ret); return ret; }


#define isslog_verify_return(func, inst, ret, msg)   \
	if ( !(inst) ) { DEBUG_TRACE_ERROR("%s | %s is NULL, %s. ret %s", func, #inst, msg, #ret); return ret; }

#define isslog_verify_return_func(inst, ret, msg)   \
	if ( !(inst) ) { DEBUG_TRACE_ERROR("%s | %s is NULL, %s. ret %s", __FUNCTION__, #inst, msg, #ret); return ret; }

#define isslog_verify_rec(func)          isslog_verify_inst(func, rec)
#define isslog_verify_ep(func)           isslog_verify_inst(func, ep)
