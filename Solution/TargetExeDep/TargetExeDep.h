// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TARGETEXEDEP_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TARGETEXEDEP_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef TARGETEXEDEP_EXPORTS
#define TARGETEXEDEP_API __declspec(dllexport)
#else
#define TARGETEXEDEP_API __declspec(dllimport)
#endif

// �����Ǵ� TargetExeDep.dll ������
class TARGETEXEDEP_API CTargetExeDep {
public:
	CTargetExeDep(void);
	// TODO:  �ڴ�������ķ�����
};

extern TARGETEXEDEP_API int nTargetExeDep;

#ifdef __cplusplus
extern "C" {
#endif
    TARGETEXEDEP_API int fnTargetExeDep(void);
#ifdef __cplusplus
    }
#endif
