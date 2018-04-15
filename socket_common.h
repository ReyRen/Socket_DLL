/*written by Yuan 04/14/2018*/
/*dll export*/

#ifndef _SOCKET_COMMON_
#define _SOCKET_COMMON_

#ifdef __cplusplus
extern "C" {
#endif

#define _SOCKET_OS_WIN32_ 
#define _SOCKET_OS_LINUX_

#ifdef _SOCKET_OS_WIN32_
#define SOCKET_FUNC_EXPORT(returnType) __declspec(dllexport) \
		returntype
#else
#ifdef _SOCKET_OS_LINUX_
#define SOCKET_FUNC_EXPORT(returnType) __attribute__ \
		((visibility("default"))) returnType
#else
#define SOCKET_FUNC_EXPORT(returnType) returnType

#ifdef __cplusplus
}
#endif

#endif
