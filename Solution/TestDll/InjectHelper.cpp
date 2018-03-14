#include "InjectHelper.h"
#include <SDKDDKVer.h>

HookManager::HookManager()
{

}

BOOL HookManager::setup()
{
    hProc = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_CREATE_THREAD |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE,
        FALSE,
        GetCurrentProcessId());

    return (INVALID_HANDLE_VALUE == hProc) ? FALSE : TRUE;
}

VOID HookManager::shutdown()
{
    CloseHandle(hProc);
    return;
}

VOID HookManager::add(LPVOID pOld, LPVOID pNew)
{
    for (size_t i = 0; i < hooks.size(); i++) {
        if (hooks[i].pOldAddr == pOld && hooks[i].pNewAddr == pNew) return;
        else if (hooks[i].pOldAddr == pOld && hooks[i].pNewAddr != pNew) {
            hooks[i].stNewCode.jmp = 0xe9;
            hooks[i].stOldCode.addr = (DWORD)pNew - (DWORD)pOld - 5;
            ::WriteProcessMemory(hProc, (LPVOID)pOld, &hooks[i].stNewCode, sizeof(JMPCODE), NULL);
            return;
        }
    }

    // 没有找到则创建一个新的对应关系
    HookItem item;
    DWORD oldProtect;
    VirtualProtectEx(hProc, pOld, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy_s(&item.stOldCode, sizeof(JMPCODE), pOld, 5);

    item.pOldAddr = pOld;
    item.pNewAddr = pNew;
    item.stNewCode.jmp = 0xe9;
    item.stNewCode.addr = (DWORD)pNew - (DWORD)pOld - 5;
    ::WriteProcessMemory(hProc, pOld, &item.stNewCode, sizeof(JMPCODE), NULL);

    hooks.push_back(item);
    return ;
}

VOID HookManager::hook(LPVOID addr)
{
    HookItem* h = NULL;
    for (size_t i = 0; i < hooks.size(); i++) {
        if (hooks[i].pOldAddr == addr || hooks[i].pNewAddr == addr) {
            h = &hooks[i];
            break;
        }
    }
    if(h) ::WriteProcessMemory(hProc, h->pOldAddr, &h->stNewCode, sizeof(JMPCODE), NULL);
}

VOID HookManager::unhook(LPVOID addr)
{
    HookItem* h = NULL;
    for (size_t i = 0; i < hooks.size(); i++) {
        if (hooks[i].pOldAddr == addr || hooks[i].pNewAddr == addr) {
            h = &hooks[i];
            break;
        }
    }
    if (h) ::WriteProcessMemory(hProc, h->pOldAddr, &h->stOldCode, sizeof(JMPCODE), NULL);
}

LPVOID HookManager::getorg(LPVOID addr)
{
    for (size_t i = 0; i < hooks.size(); i++) {
        if (hooks[i].pNewAddr == addr) {
            return hooks[i].pOldAddr;
        }
    }
    return NULL;
}
