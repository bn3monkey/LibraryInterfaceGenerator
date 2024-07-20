#ifndef CSHARPTYPECONVERTER_LIBRARY
#define CSHARPTYPECONVERTER_LIBRARY

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#if defined(_WIN32)
	#ifdef CSHARPTYPELIBRARY_EXPORTS
		#define CSHARPTYPELIBRARYEXPORT __declspec(dllexport)
	#else
		#define CSHARPTYPELIBRARYEXPORT __declspec(dllimport)
	#endif
#else
	#define CSHARPTYPELIBRARYEXPORT
#endif 

#ifdef __cplusplus
extern "C" {
#endif
	struct CSharpTypeLibrary_Context;

	CSHARPTYPELIBRARYEXPORT CSharpTypeLibrary_Context* __stdcall CSharpTypeLibrary_initialize();

	CSHARPTYPELIBRARYEXPORT void __stdcall CSharpTypeLibrary_release(CSharpTypeLibrary_Context* context);

	CSHARPTYPELIBRARYEXPORT void __stdcall CSharpTypeLibrary_registerCallback(CSharpTypeLibrary_Context* context, void (*callback)(int, float));

	CSHARPTYPELIBRARYEXPORT void __stdcall CSharpTypeLibrary_unregisterCallback(CSharpTypeLibrary_Context* context);

	CSHARPTYPELIBRARYEXPORT void __stdcall CSharpTypeLibrary_runCallback(CSharpTypeLibrary_Context* context, int param1, float param2);


#ifdef __cplusplus
}
#endif

#endif // CSHARPTYPECONVERTER_LIBRARY