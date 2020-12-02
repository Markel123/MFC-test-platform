#include "stdafx.h"
#include"JINOBase.h"


/////////////////////静态成员的定义 暂时放在这儿/////////////////////
#pragma region 静态成员的定义
CMutex CJINOBase::mutex;
CJINOBase* CJINOBase::Singleton = NULL;
#pragma endregion
