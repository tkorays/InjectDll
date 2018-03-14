// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "InjectHelper.h"

HookManager hm;


// 这个是我们要替换的函数，目的是要把它变成我们自己的函数调用 
typedef int (*FnTargetExeDep)(void);
int fnTargetExeDepReplace() {
    
    printf("From TestDll, inject success!\n");

    hm.unhook(fnTargetExeDepReplace);
    ((FnTargetExeDep)hm.getorg(fnTargetExeDepReplace))();
    hm.hook(fnTargetExeDepReplace);
    return 0;
}
// 用于改变函数地址
void __ReplaceFunc() {
    HMODULE hTargetExeDepDll = GetModuleHandle(TEXT("TargetExeDep"));
    if (!hTargetExeDepDll) {
        printf("fail to get TargetExeDep.dll HANDLE\n");
        return;
    }

    LPVOID p1 = GetProcAddress(hTargetExeDepDll, "fnTargetExeDep");
    if (!p1) {
        printf("fail to get fnTargetExeDep's address in module %p\n", hTargetExeDepDll);
        return;
    }
    BOOL ret = hm.setup();
    hm.add(p1, fnTargetExeDepReplace);
    printf("%u\n", sizeof(hm.hooks[0].stOldCode));
}


BOOL APIENTRY DllMain( HMODULE  ,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
    {
        __ReplaceFunc();
        MessageBox(NULL, TEXT("process attach"), TEXT("hack"), MB_OK);
    }
    break;
	case DLL_THREAD_ATTACH:
    {
        MessageBox(NULL, TEXT("thread attach"), TEXT("hack"), MB_OK);
    }
    break;
	case DLL_THREAD_DETACH:
    {
        MessageBox(NULL, TEXT("thread deattach"), TEXT("hack"), MB_OK);
    }
    break;
	case DLL_PROCESS_DETACH:
    {
        MessageBox(NULL, TEXT("process deattach"), TEXT("hack"), MB_OK);
    }
	break;
	}
	return TRUE;
}

