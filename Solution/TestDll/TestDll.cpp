// TestDll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "TestDll.h"


// ���ǵ���������һ��ʾ��
TESTDLL_API int nTestDll=0;

// ���ǵ���������һ��ʾ����
TESTDLL_API int fnTestDll(void)
{
    return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� TestDll.h
CTestDll::CTestDll()
{
    return;
}
