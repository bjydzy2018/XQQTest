

#include"DefSeopt.h"

using namespace VA;
using std::string;

DefSeopt::DefSeopt() {
#if VA_VC
	resDir = "..\\SeoptRes";
#elif VA_ANDROID
	resDir = "/sdcard/iflytek/res/seopt";
#else
	resDir = "SeoptRes";
#endif

}
