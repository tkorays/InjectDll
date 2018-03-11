#include <stdio.h>
#include <Windows.h>

#define __EXIT(ret, errinfo, ...) do {printf("%s\n", errinfo); __VA_ARGS__; return ret;} while(0)

int main(int argc, char* argv[]) {
    if (argc <= 2) __EXIT(-1, "help: injectdll.exe pid /path/to/a.dll\n");

    DWORD dwProcId = atoi(argv[1]);
    CHAR* sDllPath = (CHAR*)argv[2];

    if (!dwProcId || !sDllPath) __EXIT(-1, "bad params!");

    // �ж�dll�Ƿ����
    DWORD dwAttr = GetFileAttributesA(sDllPath);
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
    LPVOID pDllPath = VirtualAllocEx(hRmtProc, NULL, strlen(sDllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (NULL == pDllPath) __EXIT(-1, "fail to alloc virtual memory.", {
        CloseHandle(hRmtProc);
    });
    if (TRUE != WriteProcessMemory(hRmtProc, pDllPath, sDllPath, strlen(sDllPath)+1, NULL)) __EXIT(
        -1,
        "fail to write memory", {
            VirtualFreeEx(hRmtProc, pDllPath, 0, MEM_RELEASE);
            CloseHandle(hRmtProc);
        }
    );

    // ��ȡLoadLibraryW��ַ��ʵ�⣬ֱ��ʹ��LoadLibraryW�����Ϳ�����
    PTHREAD_START_ROUTINE pfnLoadLib = (PTHREAD_START_ROUTINE)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")), "LoadLibraryA");
    if (!pfnLoadLib) __EXIT(-1, "fail to get LoadLibrary from kernel32.dll", {
        VirtualFreeEx(hRmtProc, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hRmtProc);
    });

    // ��Ŀ����̴���Զ���̣߳���ں���ΪLoadLibraryW������Ϊdll��ַ
    // ����������Ŀ���߳���ִ��dll�е���ں���
    HANDLE hNewThread = CreateRemoteThread(hRmtProc, NULL, 0, pfnLoadLib, pDllPath, 0, NULL);
    if (NULL == hNewThread) __EXIT(-1, "fail to create remote thread", {
        VirtualFreeEx(hRmtProc, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hRmtProc);
    });

    if (WAIT_OBJECT_0 != WaitForSingleObject(hNewThread, INFINITE)) __EXIT(-1, "fail to wait");

    printf("inject success!\n");
    VirtualFreeEx(hRmtProc, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hNewThread);
    CloseHandle(hRmtProc);

    return 0;
}
