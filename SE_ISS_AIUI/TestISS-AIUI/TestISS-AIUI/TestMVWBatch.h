#ifndef MVWBATCH_H_
#define MVWBATCH_H_

#include "DefMVW.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

class TestMVWBatch {
private:
	ISSErrID err;
	HISSMVW	 phISSMVW;
	DefMVW def;

	int scene;
	string audio_list_path;
	string vw_ret_name;
	string use_path_pre_;

	double getVadEndTime(string);
	void testMVWBatch(string, bool);

public :
	TestMVWBatch();
	void testMVWBatchByScene(int, ...);
	void testMVWBatchByScene(vector<string> &);
	void testMVWBatchByWord();
	int testMVWConsistent();

	void setUsePathPre(std::string use_path_pre ) { use_path_pre_ = use_path_pre; };
};

#endif // !MVWBATCH_H_

