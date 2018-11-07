#include "TestSPWordConsistent.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <vector>

#ifdef ISS_SR_3

using namespace VA;
using namespace rapidjson;
using std::vector;
using std::string;

TestSPWordConsistent::TestSPWordConsistent():err(-1), phISSSR(NULL), lang_(0) {}

void TestSPWordConsistent::test() {
	err = ISSSetMachineCode(def.machineCode.c_str());
	DEBUG_TRACE("ISSSetMachineCode return %d", err);
	if (err != ISS_SUCCESS)
		return;

	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang_), def.resDir, DefSR::onMsgProc, NULL);
	DEBUG_TRACE("ISSSRCreate return %d", err);
	if (err != ISS_SUCCESS)
		return;

	vector<string> lines = readFileByLine(test_text_list_path_, false);
	// 构建poi列表json串
	// 根
	Document poi_names;
	Document::AllocatorType& allocator = poi_names.GetAllocator();
	poi_names.SetObject();

	// list
	Value list(kArrayType);
	int poi_name_id = 1;
	for(size_t i = 0; i < lines.size(); ++i){
		string line = lines[i];
		Value list_item(kObjectType);

		// dimension
		Value dimension(kArrayType);
		Value poi_name(kObjectType);
		Value field;
		field.SetString("poiName", allocator);
		poi_name.AddMember("field", field, allocator);

		Value spword(true);
		poi_name.AddMember("spword", spword, allocator);

		Value val;
		val.SetString(line.c_str(), allocator);
		poi_name.AddMember("val", val, allocator);

		dimension.PushBack(poi_name, allocator);

		// id
		Value id(poi_name_id++);

		// list_item
		list_item.AddMember("dimension", dimension, allocator);
		list_item.AddMember("id", id, allocator);

		list.PushBack(list_item, allocator);
	}
	poi_names.AddMember("list", list, allocator);

	Value nliFieldSearch(kArrayType);
	Value temp_str1;
	temp_str1.SetString("semantic.slots.poiName", allocator);
	nliFieldSearch.PushBack(temp_str1, allocator);

	Value temp_str2;
	temp_str2.SetString("semantic.slots.poiName2", allocator);
	nliFieldSearch.PushBack(temp_str2, allocator);
	poi_names.AddMember("nliFieldSearch", nliFieldSearch, allocator);

	Value type;
	type.SetString("poi", allocator);
	poi_names.AddMember("type", type, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	poi_names.Accept(writer);

	const char * poi_names_json = buffer.GetString();

	ISSSRStart(phISSSR, ISS_SR_SCENE_STKS, ISS_SR_MODE_LOCAL_REC, poi_names_json);
	DEBUG_TRACE("ISSSRStart return %d", err);

	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE("ISSSRDestroy return %d", err);
}

#endif // ISS_SR_3