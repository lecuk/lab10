#ifndef COLOR_SUPPORT_H
#define COLOR_SUPPORT_H

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif 

#include <stdio.h>
#include <windows.h>
#include <fileapi.h>

BOOL EnableColorSupport(BOOL enabled);
BOOL ColorSupportEnabled();
BOOL HaveColorSupport();
BOOL IsRedirectedToFile(DWORD stdHandle);
BOOL EnableColorSupport(BOOL enabled);
LPSTR formatOkKo(LPSTR buffer, BOOL cond, LPCSTR ok, LPCSTR ko);

#endif