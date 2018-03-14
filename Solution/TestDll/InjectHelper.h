#pragma once
#include <windows.h>
#include <vector>

#pragma pack(1)
typedef struct {
    BYTE jmp;
    DWORD addr;
} JMPCODE;
#pragma pack()

typedef struct {
    LPVOID  pOldAddr;   // ��hook������ַ
    LPVOID  pNewAddr;      // �º�����ַ
    JMPCODE stOldCode;      // ��hook����ǰ5�ֽڴ���
    JMPCODE stNewCode;      // �滻����ת��������
} HookItem;

class HookManager {
public:
    std::vector<HookItem> hooks;
    HANDLE hProc;
    HookManager();

    BOOL setup();
    VOID shutdown();

    VOID add(LPVOID dwOld, LPVOID dwNew);
    VOID hook(LPVOID addr);
    VOID unhook(LPVOID addr);
    LPVOID getorg(LPVOID addr);
} ;