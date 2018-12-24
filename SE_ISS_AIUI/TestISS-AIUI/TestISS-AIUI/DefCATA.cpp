#include "DefCATA.h"
#include "CommonFun.h"

#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using std::vector;
using std::string;
using namespace rapidjson;
using namespace boost::algorithm;
using boost::lexical_cast;

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
#if VA_VC
	resDir = "..\\CataRes";
	resDirW = L"..\\CataRes";
#elif VA_ANDROID
	resDir = "/sdcard/iflytek/res/cata";
#else
	resDir = "CataRes";
#endif
}

// �����������ַ���ת��Ϊjson��ʽ
string DefCATA::parseSegValues(string segValues_str) {
	Document segValues_j;
	segValues_j.SetArray();
	Document::AllocatorType& allocator = segValues_j.GetAllocator();

	vector<string> segValues_v;
	split(segValues_v, segValues_str, is_any_of("\t"));

	// �ñ���˵�����·�'k_v.size() == 2'���
	string segvalue_temp("");

	for (int i = 0; i < segValues_v.size(); i++) {
		string item(segValues_v[i]);

		// ������'add music10000.bin'���ֶι��˵�
		if (item == "add" || contains(item, ".bin"))
			continue;

		Value segValue_j(kObjectType);
		vector<string> k_v;
		split(k_v, item, is_any_of("|"));

		if (k_v.size() == 3 || k_v.size() == 4) {
			// ���field
			Value field_v(kStringType);
			field_v.SetString(k_v[0].c_str(), allocator);
			segValue_j.AddMember("field", field_v, allocator);
		}

		if (k_v.size() == 3) {
			// �������

			// ���value
			Value vals_j(kArrayType);
			vector<string> vals;
			split(vals, k_v[1], is_any_of(";"));
			BOOST_FOREACH(string val, vals) {
				Value val_v(kStringType);
				val_v.SetString(val.c_str(), allocator);

				vals_j.PushBack(val_v, allocator);
			}
			segValue_j.AddMember("val", vals_j, allocator);

			// ���mode
			if (k_v[2] == "")
				segValue_j.AddMember("mode", 4, allocator);
			else
				segValue_j.AddMember("mode", lexical_cast<int>(k_v[2]), allocator);
		}
		else if (k_v.size() >= 4) {
			// ���value
			string val_temp("");
			for (int i = 1; i < k_v.size() - 2; i++)
				val_temp += k_v[i];

			Value vals_j(kArrayType);
			vector<string> vals;
			split(vals, val_temp, is_any_of(";"));

			BOOST_FOREACH(string val, vals) {
				Value val_v(kStringType);
				val_v.SetString(val.c_str(), allocator);

				vals_j.PushBack(val_v, allocator);
			}
			segValue_j.AddMember("val", vals_j, allocator);

			if (k_v[k_v.size() - 1] == "1" && k_v[k_v.size() - 2] == "1") {
				// ���mode��1|1תΪ4
				segValue_j.AddMember("mode", 4, allocator);
			}
			else {
				// ���mode
				if (k_v[k_v.size() - 1] == "")
					segValue_j.AddMember("mode", 4, allocator);
				else
					segValue_j.AddMember("mode", lexical_cast<int>(k_v[k_v.size() - 1]), allocator);
			}
		}
		else if (k_v.size() == 2) {
			// һ��segvalue���ֶ�ֵ�к���tab��������ÿ��segvalue���ǰ�tab�ָ�������������г�ͻ
			// ��ʱ���ڵ�����segvalue����һ��������segvalue, �������Ҫ��ǰ������segvalue�������

			// ��segvalue_tempΪ�գ�˵����ʱ��segvalue��һ����segvalue��ǰ�벿�֣��Ƚ���洢����
			if (segvalue_temp == "") {
				segvalue_temp = item;
				continue;
			}
			else {
				// ��segvalue_temp��Ϊ�գ�˵����ʱ��segvalue��һ����segvalue�ĺ�벿�֣���ʱ�����������
				item = segvalue_temp + "\t" + item;

				vector<string> k_v;
				split(k_v, item, is_any_of("|"));

				// ���field
				Value field_v(kStringType);
				field_v.SetString(k_v[0].c_str(), allocator);
				segValue_j.AddMember("field", field_v, allocator);

				// ���value
				Value vals_j(kArrayType);
				vector<string> vals;
				split(vals, k_v[1], is_any_of(";"));
				BOOST_FOREACH(string val, vals) {
					Value val_v(kStringType);
					val_v.SetString(val.c_str(), allocator);

					vals_j.PushBack(val_v, allocator);
				}
				segValue_j.AddMember("val", vals_j, allocator);

				// ���mode
				if (k_v[2] == "")
					segValue_j.AddMember("mode", 4, allocator);
				else
					segValue_j.AddMember("mode", lexical_cast<int>(k_v[2]), allocator);

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

// �ӹ��������ַ�������ȡ��Ҫ��ӵ���������Դ��
string DefCATA::getAddBin(std::string segValues_str) {
	vector<string> segValues_v;
	split(segValues_v, segValues_str, is_any_of("\t"));

	if (segValues_v[segValues_v.size() - 2] == "add") {
		return segValues_v[segValues_v.size() - 1];
	}
	else {
		return "";
	}
}

// ��ȡ��������Դ��
string DefCATA::getSrhBin(string search_text) {
	// ��ȡ������Դ�����ֶ�
	vector<string> v;
	split(v, search_text, is_any_of("\t"));

	// ��ȡ������Դ����
	vector<string> res_v;
	split(res_v, v[3], is_any_of("|"));

	vector<string> res;
	// ��ȡ��������Դ�����浽res��
	for (int i = 0; i < res_v.size(); i++) {
		vector<string> r;
		split(r, res_v[i], is_any_of(";"));

		res.push_back(r[0]);
	}

	string ret("");
	// ����res������Դ�б��ַ���
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
		else if (contains(res[i], "yellow") && (contains(res[i], "page") || contains(res[i], "Page")) && res[i] != "yellow_page.bin")
			res_name = "";
		else
			res_name = res[i];

		if (i > 0 && res_name != "" && ret != "")
			res_name = "," + res_name;

		ret += res_name;
	}

	return ret;
}

// ��ȡ�������ı�
string DefCATA::getSrhText(string search_text) {
	vector<string> v;
	split(v, search_text, is_any_of("\t"));

	return v[1];
}
