

#include "TestApiSeopt.h"
#include "CommonFun.h"

#include <string>
#include <climits>

using namespace VA;
using std::string;

// 无效参数
class SeoptInvalidPara : public ::testing::Test {
protected:
	ISSErrID err;
	ISSSeoptHandle	 phISSSeopt;

	DefSeopt def;

	virtual void SetUp() {
		err = -1;
		phISSSeopt = NULL;
	}

	virtual void TearDown() {
		phISSSeopt = NULL;
	}
};

// 无效参数
// 生成资源路径参数
class SeoptInvalidParaGenResDir : public ::testing::TestWithParam<const char *> {
protected:
	ISSErrID err;
	ISSSeoptHandle	 phISSSeopt;

	DefSeopt def;

	virtual void SetUp() {
		err = -1;
		phISSSeopt = NULL;
	}

	virtual void TearDown() {
		phISSSeopt = NULL;
	}
};
INSTANTIATE_TEST_CASE_P(ResDir, SeoptInvalidParaGenResDir, ::testing::Values("1122aabb", "123", "asdf"));

// SEOPT_API_INVALID_PARA_0001
// create句柄传空
TEST_F(SeoptInvalidPara, CreateNULL) {

	err = ISSSeoptCreate(NULL, def.resDir);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}

}

// SEOPT_API_INVALID_PARA_0002
// create资源路径传空
TEST_F(SeoptInvalidPara, CreateResDirNULL) {

	err = ISSSeoptCreate(&phISSSeopt, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}

}

// SEOPT_API_INVALID_PARA_0003~0004
// create资源路径无效
TEST_P(SeoptInvalidParaGenResDir, CreateResDirInvalid) {
	err = ISSSeoptCreate(&phISSSeopt, GetParam());
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}

}

// SEOPT_API_INVALID_PARA_0005
// create资源路径传空字符串
TEST_F(SeoptInvalidPara, CreateResDirNULLStr) {
	err = ISSSeoptCreate(&phISSSeopt, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}
}
