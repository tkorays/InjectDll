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
    LPVOID  pOldAddr;   // 被hook函数地址
    LPVOID  pNewAddr;      // 新函数地址
    JMPCODE stOldCode;      // 被hook函数前5字节代码
    JMPCODE stNewCode;      // 替换的跳转函数代码
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