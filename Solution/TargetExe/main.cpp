#include <stdio.h>
#include <Windows.h>

// TargetExeDep��dll������õĺ���
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllimport) int fnTargetExeDep(void);
#ifdef __cplusplus
}
#endif

int main() {
    
    while (1) {
        fnTargetExeDep();
        Sleep(1000);
    }
    return 0;
}