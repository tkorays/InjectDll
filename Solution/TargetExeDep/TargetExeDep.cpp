// TargetExeDep.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "TargetExeDep.h"


// ���ǵ���������һ��ʾ��
TARGETEXEDEP_API int nTargetExeDep=0;

// ���ǵ���������һ��ʾ����
TARGETEXEDEP_API int fnTargetExeDep(void)
{
    printf("From TargetExeDep!\n");
    return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� TargetExeDep.h
CTargetExeDep::CTargetExeDep()
{
    return;
}
