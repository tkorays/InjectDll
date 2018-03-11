// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TARGETEXEDEP_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TARGETEXEDEP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef TARGETEXEDEP_EXPORTS
#define TARGETEXEDEP_API __declspec(dllexport)
#else
#define TARGETEXEDEP_API __declspec(dllimport)
#endif

// 此类是从 TargetExeDep.dll 导出的
class TARGETEXEDEP_API CTargetExeDep {
public:
	CTargetExeDep(void);
	// TODO:  在此添加您的方法。
};

extern TARGETEXEDEP_API int nTargetExeDep;

#ifdef __cplusplus
extern "C" {
#endif
    TARGETEXEDEP_API int fnTargetExeDep(void);
#ifdef __cplusplus
    }
#endif
