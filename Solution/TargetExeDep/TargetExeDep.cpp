// TargetExeDep.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "TargetExeDep.h"


// 这是导出变量的一个示例
TARGETEXEDEP_API int nTargetExeDep=0;

// 这是导出函数的一个示例。
TARGETEXEDEP_API int fnTargetExeDep(void)
{
    printf("From TargetExeDep!\n");
    return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 TargetExeDep.h
CTargetExeDep::CTargetExeDep()
{
    return;
}
