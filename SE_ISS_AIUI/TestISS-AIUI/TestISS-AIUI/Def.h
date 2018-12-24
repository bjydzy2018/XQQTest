#ifndef DEF_H_
#define DEF_H_

#include <string>

class Def {
public:
	virtual ~Def() {};
};

extern std::string testRes_path;

// 定义该宏表示使用套件3.0版本的识别库
//#define ISS_SR_3

// 定义该宏表示执行英文识别用例
//#define ISS_SR_EN

#endif // !DEF_H_

