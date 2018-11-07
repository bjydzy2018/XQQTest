#include "DefCATA.h"
#include "VAInc.h"
#include "test_log.h"

#include <vector>
#include "VAString.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using std::vector;
using namespace VA;
using std::string;
using namespace rapidjson;

CataMsgProcInfo DefCATA::msgProcInfo;
int DefCATA::msgSearchResult = 0;
int DefCATA::msgLoadResourceFail = 0;
int DefCATA::msgProc = 0;

void DefCATA::onCataMsgProc(
	const void *        pUsrArg,      /* [In] User defined parameter */
	unsigned int        uMsg,         /* [In] Message identifier */
	unsigned int        wParam,       /* [In] Message additional information */
	const void *        lParam        /* [In] Message additional information */
) {
	msgProcInfo.setpUsrArg(pUsrArg);
	msgProcInfo.setuMsg(uMsg);
	msgProcInfo.setwParam(wParam);
	msgProcInfo.setlParam(lParam);

	switch (uMsg) {
	case ISS_CATA_MSG_OnSearchResult:
		++msgSearchResult;
		break;
	case ISS_CATA_MSG_LoadResourceFail:
		++msgLoadResourceFail;
		break;
	default:
		break;
	}

	++msgProc;
}

void DefCATA::initMsg() {
	msgSearchResult = 0;
	msgLoadResourceFail = 0;
	msgProc = 0;

	msgProcInfo.init();
}

DefCATA::DefCATA() {
//#if VA_VC
//	resDir = "..\\CataRes";
//	resDirW = L"..\\CataRes";
//#elif VA_ANDROID
//	resDir = "/sdcard/iflytek/res/cata";
//#else
//	resDir = "CataRes";
//#endif

//(cata2.0) CataRes的路径必须要传绝对路径
#if VA_VC

	string exePath = GetAppPath();
	string resPath = exePath + "\\..\\CataRes";
	DEBUG_TRACE("Res Path:%s", resPath.c_str());

	const char * resPath_narrow = resPath.c_str();
	resDir = new char[strlen(resPath_narrow) + 1];
	memcpy(resDir, resPath_narrow, strlen(resPath_narrow) + 1);

	const wchar_t * resPath_wide = widen(resPath).c_str();
	resDirW = new wchar_t[wcslen(resPath_wide) + 1];
	memcpy(resDirW, resPath_wide, wcslen(resPath_wide) + 1);
#elif VA_ANDROID
	resDir = "/sdcard/iflytek/res/cata";
#else
	char szExePath[512] = {0};
	if(!getcwd(szExePath, 512)){
		DEBUG_TRACE("getcwd fail!!!");
	}
	string resPath = szExePath;
	resPath += "/CataRes";
	std::cout << resPath << std::endl;

	const char * resPath_narrow = resPath.c_str();
	resDir = new char[strlen(resPath_narrow) + 1];
	memcpy(resDir, resPath_narrow, strlen(resPath_narrow) + 1);

#endif

}

// 将构建索引字符串转换为json格式
string DefCATA::parseSegValues(string segValues_str) {
	Document segValues_j;
	segValues_j.SetArray();
	Document::AllocatorType& allocator = segValues_j.GetAllocator();

	vector<string> segValues_v = split(segValues_str, '\t');

	// 该变量说明见下方'k_v.size() == 2'情况
	string segvalue_temp("");

	for (int i = 0; i < segValues_v.size(); i++) {
		string item(segValues_v[i]);

		// 将类似'add music10000.bin'的字段过滤掉
		//if (item == "add" || contains(item, ".bin"))
		if (item == "add" || (item.find(".bin")!=string::npos))
			continue;

		Value segValue_j(kObjectType);
		vector<string> k_v = split(item, '|');

		if (k_v.size() == 3 || k_v.size() == 4) {
			// 添加field
			Value field_v(kStringType);
			field_v.SetString(k_v[0].c_str(), allocator);
			segValue_j.AddMember("field", field_v, allocator);
		}

		if (k_v.size() == 3) {
			// 正常情况

			// 添加value
			Value vals_j(kArrayType);
			vector<string> vals = split(k_v[1], ';');

			for(size_t i = 0; i < vals.size(); ++i)
			{
				Value val_v(kStringType);
				val_v.SetString(vals[i].c_str(), allocator);

				vals_j.PushBack(val_v, allocator);
			}

			segValue_j.AddMember("val", vals_j, allocator);

			// 添加mode
			if (k_v[2] == "")
				segValue_j.AddMember("mode", 4, allocator);
			else
				segValue_j.AddMember("mode", atoi(k_v[2].c_str()), allocator);
		}
		else if (k_v.size() >= 4) {
			// 添加value
			string val_temp("");
			for (int i = 1; i < k_v.size() - 2; i++)
				val_temp += k_v[i];

			Value vals_j(kArrayType);
			vector<string> vals = split(val_temp, ';');

			for(size_t i = 0; i < vals.size(); ++i)
			{
				Value val_v(kStringType);
				val_v.SetString(vals[i].c_str(), allocator);

				vals_j.PushBack(val_v, allocator);
			}

			segValue_j.AddMember("val", vals_j, allocator);

			if (k_v[k_v.size() - 1] == "1" && k_v[k_v.size() - 2] == "1") {
				// 添加mode，1|1转为4
				segValue_j.AddMember("mode", 4, allocator);
			}
			else {
				// 添加mode
				if (k_v[k_v.size() - 1] == "")
					segValue_j.AddMember("mode", 4, allocator);
				else
					segValue_j.AddMember("mode", atoi(k_v[k_v.size() - 1].c_str()), allocator);
			}
		}
		else if (k_v.size() == 2) {
			// 一个segvalue的字段值中含有tab符，由于每个segvalue都是按tab分隔的因此这样会有冲突
			// 此时相邻的两个segvalue才是一个完整的segvalue, 因此这里要将前后两个segvalue组合起来

			// 若segvalue_temp为空，说明此时的segvalue是一个整segvalue的前半部分，先将其存储起来
			if (segvalue_temp == "") {
				segvalue_temp = item;
				continue;
			}
			else {
				// 若segvalue_temp不为空，说明此时的segvalue是一个整segvalue的后半部分，此时将其组合起来
				item = segvalue_temp + "\t" + item;

				vector<string> k_v = split(item, '|');

				// 添加field
				Value field_v(kStringType);
				field_v.SetString(k_v[0].c_str(), allocator);
				segValue_j.AddMember("field", field_v, allocator);

				// 添加value
				Value vals_j(kArrayType);
				vector<string> vals = split(k_v[1], ';');

				for(size_t i = 0; i < vals.size(); ++i)
				{
					Value val_v(kStringType);
					val_v.SetString(vals[i].c_str(), allocator);

					vals_j.PushBack(val_v, allocator);
				}

				segValue_j.AddMember("val", vals_j, allocator);

				// 添加mode
				if (k_v[2] == "")
					segValue_j.AddMember("mode", 4, allocator);
				else
					segValue_j.AddMember("mode", atoi(k_v[2].c_str()), allocator);

				segvalue_temp = "";
			}
		}

		segValues_j.PushBack(segValue_j, allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	segValues_j.Accept(writer);

	return buffer.GetString();
}

// 从构建索引字符串中提取出要添加到的索引资源名
string DefCATA::getAddBin(std::string segValues_str) {
	vector<string> segValues_v = split(segValues_str, '\t');

	if (segValues_v[segValues_v.size() - 2] == "add") {
		return segValues_v[segValues_v.size() - 1];
	}
	else {
		return "";
	}
}

// 提取搜索的资源名
string DefCATA::getSrhBin(string search_text) {
	// 提取包含资源名的字段
	vector<string> v = split( search_text, '\t');

	// 提取各个资源描述
	vector<string> res_v = split( v[3], '|');

	vector<string> res;
	// 提取出各个资源名，存到res中
	for (int i = 0; i < res_v.size(); i++) {
		vector<string> r = split(res_v[i], ';');

		res.push_back(r[0]);
	}

	string ret("");
	// 根据res构建资源列表字符串
	for (int i = 0; i < res.size(); i++) {
		string res_name("");

		if (res[i] == "cantonese2py.bin")
			res_name = "cantonese2py";
		else if (res[i] == "mandarin2py.bin")
			res_name = "mandarin2py";
		else if (res[i] == "yellow_page.bin")
			res_name = "yellow_page";
		else if (res[i] == "pingyin_fuzzy.bin")
			res_name = "pinyin_fuzzy";
		else if (res[i] == "alias.bin")
			res_name = "alias";
		//else if (contains(res[i], "yellow") && (contains(res[i], "page") || contains(res[i], "Page")) && res[i] != "yellow_page.bin")
		//	res_name = "";
		else
			res_name = res[i];

		if (i > 0 && res_name != "" && ret != "")
			res_name = "," + res_name;

		ret += res_name;
	}

	return ret;
}

// 提取搜索的资源名
string DefCATA::getSrhText(string search_text) {
	vector<string> v = split( search_text, '\t');

	return v[1];
}
