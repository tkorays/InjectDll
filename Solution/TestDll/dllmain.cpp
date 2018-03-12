// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

// ���������Ҫ�滻�ĺ�����Ŀ����Ҫ������������Լ��ĺ������� 
//int fnTargetExeDep(void);
int fnTargetExeDepReplace() {
    printf("From TestDll, inject success!\n");
    return 0;
}

uintptr_t* p2;
// ���ڸı亯����ַ
void __ReplaceFunc() {
    printf("start replace\n");
    HMODULE hTargetExeDepDll = GetModuleHandle(TEXT("TargetExeDep"));
    if (!hTargetExeDepDll) {
        printf("fail to get TargetExeDep.dll HANDLE\n");
        return;
    }

    uintptr_t* p1 = (uintptr_t*)GetProcAddress(hTargetExeDepDll, "fnTargetExeDep");
    if (!p1) {
        printf("fail to get fnTargetExeDep's address in module %p\n", hTargetExeDepDll);
        return;
    }
    p2 = (uintptr_t*)fnTargetExeDepReplace;
    uintptr_t** p3 = &p2;

    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_CREATE_THREAD |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE,
        FALSE,
        GetCurrentProcessId());
    if (INVALID_HANDLE_VALUE == hProc) {
        printf("can not get cur proc\n");
        return;
    }
    DWORD oldProtect;
    VirtualProtectEx(hProc, p1, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

    // jmp to new address
    *(unsigned char*)p1 = 0xff;
    *(((unsigned char*)p1) + 1) = 0x25;
    memcpy_s(((unsigned char*)p1) + 2, 4, (unsigned char*)&p3, sizeof(uintptr_t));
    VirtualProtectEx(hProc, p1, 6, oldProtect, NULL);
    printf("replace success!");
    CloseHandle(hProc);
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
        MessageBox(NULL, TEXT("process attach"), TEXT("hack"), MB_OK);
    }
	break;
	}
	return TRUE;
}

