#include "DefMVW.h"

using std::string;

WakeupInfo DefMVW::wakeupInfo;
MvwMsgProcInfo DefMVW::msgProcInfo;
int DefMVW::msgInitStatus = 0;
int DefMVW::msgVolumeLevel = 0;
int DefMVW::msgError = 0;
int DefMVW::msgResult = 0;
int DefMVW::msgTimeout = 0;
int DefMVW::msgWakeup = 0;
int DefMVW::msgProc = 0;
double DefMVW::vw_time = 0;

void DefMVW::onWakeup(
  void*			 pUsrArg,	 /* [In] 用户自定义参数 */
  unsigned int	 nMvwScene,	 /* [In] 多唤醒场景 */
  unsigned int	 nMvwId,	 /* [In] 唤醒词ID */
  signed int	 nMvwScore,  /* [In] 唤醒的置信度得分 */
  const char*	 lParam
  ) {
	wakeupInfo.setpUsrArg(pUsrArg);
	wakeupInfo.setnMvwScene(nMvwScene);
	wakeupInfo.setnMvwId(nMvwId);
	wakeupInfo.setnMvwScore(nMvwScore);
	wakeupInfo.setlParam(lParam);

	++msgWakeup;
}

void DefMVW::onMsgProc(
  const void*     pUsrArg,    /* [In] User defined parameter */
  unsigned int    uMsg,       /* [In] The message identifier */
  unsigned int    wParam,     /* [In] Message additional information */
  const void*     lParam      /* [In] Message additional information */
  ) {
	msgProcInfo.setpUsrArg(pUsrArg);
	msgProcInfo.setuMsg(uMsg);
	msgProcInfo.setwParam(wParam);
	msgProcInfo.setlParam(lParam);

	switch (uMsg) {
	case ISS_MVW_MSG_InitStatus:
		++msgInitStatus;
		break;
	case ISS_MVW_MSG_VolumeLevel:
		++msgVolumeLevel;
		break;
	case ISS_MVW_MSG_Error:
		++msgError;
		break;
	case ISS_MVW_MSG_Result:
		++msgResult;
		break;
	case ISS_MVW_MSG_Timeout:
		++msgTimeout;
		break;
	default:
		break;
	}

	++msgProc;
}

void DefMVW::initMsg() {
	msgInitStatus	= 0;
	msgVolumeLevel	= 0;
	msgError		= 0;
	msgResult		= 0;
	msgTimeout		= 0;
	msgWakeup		= 0;
	msgProc			= 0;

	wakeupInfo.init();
	msgProcInfo.init();
}

DefMVW::DefMVW() {
#if VA_VC
	resDir	= "..\\MVWRes\\FirstRes";
	resDirW = L"..\\MVWRes\\FirstRes";
#elif VA_ANDROID
	resDir = "/sdcard/iflytek/res/mvw/FirstRes";
#else
	resDir = "MVWRes/FirstRes";
#endif

	mvwPcm_global				= testRes_path + "pcm_src/mvw/mvwword.pcm";
	mvwPcm_confirm				= testRes_path + "pcm_src/mvw/confirm.pcm";
	mvwPcm_cancel				= testRes_path + "pcm_src/mvw/cancel.pcm";
	mvwPcm_select				= testRes_path + "pcm_src/mvw/select.pcm";
	mvwPcm_answer				= testRes_path + "pcm_src/mvw/answer.wav";
	mvwPcm_hangup				= testRes_path + "pcm_src/mvw/Hungup.pcm";
	mvwPcm_first				= testRes_path + "pcm_src/mvw/first.pcm";
	mvwPcm_second				= testRes_path + "pcm_src/mvw/second.pcm";
	mvwPcm_third				= testRes_path + "pcm_src/mvw/third.pcm";
	mvwPcm_fourth				= testRes_path + "pcm_src/mvw/forth.pcm";
	mvwPcm_fifth				= testRes_path + "pcm_src/mvw/fifth.pcm";
	mvwPcm_sixth				= testRes_path + "pcm_src/mvw/sixth.pcm";
	mvwPcm_last					= testRes_path + "pcm_src/mvw/last.pcm";

	mvwPcm_first_en				= testRes_path + "pcm_src/mvw/first_en.pcm";
	mvwPcm_confirm_en			= testRes_path + "pcm_src/mvw/confirm_en.pcm";

	mvwPcm_KaiYi				= testRes_path + "pcm_src/mvw/KaiYiNiHao.pcm";
	mvwPcm_KaiYi_long			= testRes_path + "pcm_src/mvw/KaiYiNiHao_long.wav";
	mvwPcm_ZhiDou				= testRes_path + "pcm_src/mvw/ZhiDouNiHao.pcm";
	mvwPcm_ChenSheng			= testRes_path + "pcm_src/sr/ChenSheng.pcm";
	mvwPcm_DaLianJiPai			= testRes_path + "pcm_src/sr/DaLianJiPai.pcm";
	mvwPcm_HuShiCiShen			= testRes_path + "pcm_src/sr/HuShiCiShen.pcm";
	mvwPcm_ChongQingXiaoMian	= testRes_path + "pcm_src/sr/ChongQingXiaoMian.pcm";
	mvwPcm_NavigateIflytek		= testRes_path + "pcm_src/sr/NavigateIflytek.pcm";
	mvwPcm_iflytek				= testRes_path + "pcm_src/sr/iflytek.pcm";
	mvwPcm_ShenTong				= testRes_path + "pcm_src/sr/ShenTong.pcm";
	mvwPcm_ZhengKai				= testRes_path + "pcm_src/sr/ZhengKai.pcm";
	mvwPcm_PhoneZhangsan		= testRes_path + "pcm_src/sr/PhoneZhangsan.wav";
	mvwPcm_PhoneBaiYawei		= testRes_path + "pcm_src/sr/PhoneBaiYawei.wav";
	mvwPcm_ZhiDou_long			= testRes_path + "pcm_src/mvw/ZhiDouNiHao_long.wav";
	mvwPcm_LingXi				= testRes_path + "pcm_src/mvw/LingXiLingXi.pcm";
	mvwPcm_LingXi_long			= testRes_path + "pcm_src/mvw/LingXiLingXi_long.wav";
	mvwPcm_123					= testRes_path + "pcm_src/mvw/123.pcm";
	mvwPcm_HelloHello			= testRes_path + "pcm_src/mvw/HelloHello.wav";

	mvwKeyWord_KaiYiNiHao = "{\"Keywords\":[{\"KeyWordId\":0,\"KeyWord\":\"凯翼你好\"}]}";
	mvwKeyWord_KaiYi_LingXi = "{\"Keywords\":[{\"KeyWordId\":0,\"KeyWord\":\"凯翼你好\"},{\"KeyWordId\":1,\"KeyWord\":\"灵犀灵犀\"}]}";
	mvwKeyWord_123 = "{\"Keywords\":[{\"KeyWordId\":0,\"KeyWord\":\"一二三\"}]}";
}
