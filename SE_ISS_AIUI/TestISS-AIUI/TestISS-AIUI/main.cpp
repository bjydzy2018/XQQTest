#include "JobCtrl.h"
#include "VAInc.h"
#include "Def.h"
#include "gtest.h"
#include <string>
#include "VAFileSystem.h"
#include <cstdio>

using namespace VA;
using std::string;

#ifdef ISS_SR_3
#pragma comment(lib, "../../../../Project/07.Release/win32/lib/libisssr_3.lib")
#else
#pragma comment(lib, "../../../../Project/07.Release/win32/lib/libisssr.lib")
#pragma comment(lib, "../../../../Project/07.Release/win32/lib/libSearchForNli.lib")
#endif

string testRes_path = "TestRes/";

#if defined AiuiToolMainFuncMacro || defined AiuiToolMainFuncCallJavaNativeApiMacro
int AiuiToolMainFunc(int argc, char * argv[]) {
#else
int main(int argc, char * argv[]) {
#endif

	::testing::InitGoogleTest(&argc, argv);
	JobCtrl jobCtrl(argc, argv);
	jobCtrl.work();

#if VA_VC
	//system("pause");
#endif
	return 0;
}