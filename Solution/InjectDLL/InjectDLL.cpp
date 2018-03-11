#include <stdio.h>
#include <Windows.h>

#define __EXIT(ret, errinfo, ...) do {printf("%s\n", errinfo); __VA_ARGS__; return ret;} while(0)

int main(int argc, char* argv[]) {
    if (argc <= 2) __EXIT(-1, "help: injectdll.exe pid /path/to/a.dll\n");

    DWORD dwProcId = atoi(argv[1]);
    WCHAR* sDllPath = (WCHAR*)argv[2];

    if (!dwProcId || !sDllPath) __EXIT(-1, "bad params!");

    // �ж�dll�Ƿ����
    DWORD dwAttr = GetFileAttributesW(sDllPath);
    if (dwAttr == INVALID_FILE_ATTRIBUTES) __EXIT(-1, "fail to get dll file atrributes");

    // ���Ѿ����ڵ�Ŀ����̣����豾���̲���Ŀ����̵�Ȩ��
    HANDLE hRmtProc = OpenProcess(
        PROCESS_QUERY_INFORMATION |
        PROCESS_CREATE_THREAD |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE,
        FALSE,
        dwProcId);
    if (NULL == hRmtProc) {
        __EXIT(-1, "fail to open remote process!");
    }

    // ��Ŀ����̴��������ڴ�ռ䣬����dll·�����ƽ�ȥ
    LPVOID pDllPath = VirtualAllocEx(hRmtProc, NULL, 000, MEM_COMMIT, PAGE_READWRITE);
    if (NULL == pDllPath) __EXIT(-1, "fail to alloc virtual memory.", {
        CloseHandle(hRmtProc);
    });
    if (TRUE != WriteProcessMemory(hRmtProc, pDllPath, sDllPath, lstrlenW(sDllPath), NULL)) __EXIT(
        -1,
        "fail to write memory", {
            //VirtualFreeEx(hRmtProc, pDllPath, )
            CloseHandle(hRmtProc);
        }
    );

    // ��ȡLoadLibraryW��ַ��ʵ�⣬ֱ��ʹ��LoadLibraryW�����Ϳ�����
    PTHREAD_START_ROUTINE pfnLoadLibW = (PTHREAD_START_ROUTINE)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")), "LoadLibraryW");
    if (!pfnLoadLibW) __EXIT(-1, "fail to get LoadLibraryW from kernel32.dll");

    // ��Ŀ����̴���Զ���̣߳���ں���ΪLoadLibraryW������Ϊdll��ַ
    // ����������Ŀ���߳���ִ��dll�е���ں���
    HANDLE hNewThread = CreateRemoteThread(hRmtProc, NULL, 0, pfnLoadLibW, pDllPath, 0, NULL);
    if (NULL == hNewThread) __EXIT(-1, "fail to create remote thread");

    if (WAIT_OBJECT_0 != WaitForSingleObject(hNewThread, INFINITE)) __EXIT(-1, "fail to wait");

    VirtualFreeEx(hRmtProc, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hNewThread);
    CloseHandle(hRmtProc);

    return 0;
}