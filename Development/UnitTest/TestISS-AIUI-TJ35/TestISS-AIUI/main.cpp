#include "JobCtrl.h"
#include "VAInc.h"
#include "Def.h"
#include "VAFileSystem.h"
#include <string>
#include <vector>
#include "test_log.h"

using namespace VA;
using std::string;
using std::vector;

void test() {}

#ifdef ISS_SR_3

#else
#pragma comment(lib, "../../../../Project/07.Release/Windows_x86_32/lib/libSearchForNli.lib")
#endif

string testRes_path = "TestRes/";

int main(int argc, char * argv[]) {

	JobCtrl jobCtrl(argc, argv);
	jobCtrl.work();
	//test();

}