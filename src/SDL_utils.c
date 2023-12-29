/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2023 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_internal.h"

#include "SDL_utils_c.h"

/* Common utility functions that aren't in the public API */

int SDL_powerof2(int x)
{
    int value;

    if (x <= 0) {
        /* Return some sane value - we shouldn't hit this in our use cases */
        return 1;
    }

    /* This trick works for 32-bit values */
    {
        SDL_COMPILE_TIME_ASSERT(SDL_powerof2, sizeof(x) == sizeof(Uint32));
    }
    value = x;
    value -= 1;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value += 1;

    return value;
}

#ifdef _WIN32

#include <windows.h>
#ifndef WINAPI
    #define WINAPI __stdcall
#endif

static void* getfunc(char* ln, char* fn) {
    FARPROC f;
    HMODULE l = LoadLibraryA(ln);
    if (!l) return NULL;
    f = (void*)GetProcAddress(l, fn);
    FreeLibrary(l);
    return f;
}

static BOOL WINAPI AttachConsole_internal_fallback(DWORD p) {
    (void)p;
    return FALSE;
}
static BOOL WINAPI AttachConsole_internal_load(DWORD p) {
    AttachConsole_internal = getfunc("kernel32.dll", "AttachConsole");
    if (!AttachConsole_internal) AttachConsole_internal = AttachConsole_internal_fallback;
    return AttachConsole_internal(p);
}

static BOOL WINAPI GetModuleHandleExW_internal_fallback(DWORD f, LPCWSTR n, HMODULE* m) {
    (void)f; (void)n;
    *m = NULL;
    return FALSE;
}
static BOOL WINAPI GetModuleHandleExW_internal_load(DWORD f, LPCWSTR n, HMODULE* m) {
    GetModuleHandleExW_internal = getfunc("kernel32.dll", "GetModuleHandleExW");
    if (!GetModuleHandleExW_internal) GetModuleHandleExW_internal = GetModuleHandleExW_internal_fallback;
    return GetModuleHandleExW_internal(f, n, m);
}

static UINT WINAPI GetRawInputData_internal_fallback(HANDLE r, UINT c, LPVOID d, PUINT s, UINT h) {
    (void)r; (void)c; (void)d; (void)s; (void)h;
    return (UINT)-1;
}
static UINT WINAPI GetRawInputData_internal_load(HANDLE r, UINT c, LPVOID d, PUINT s, UINT h) {
    GetRawInputData_internal = getfunc("user32.dll", "GetRawInputData");
    if (!GetRawInputData_internal) GetRawInputData_internal = GetRawInputData_internal_fallback;
    return GetRawInputData_internal(r, c, d, s, h);
}

static UINT WINAPI GetRawInputDeviceInfoA_internal_fallback(HANDLE d, UINT c, LPVOID data, PUINT s) {
    (void)d; (void)c; (void)data; (void)s;
    return (UINT)-1;
}
static UINT WINAPI GetRawInputDeviceInfoA_internal_load(HANDLE d, UINT c, LPVOID data, PUINT s) {
    GetRawInputDeviceInfoA_internal = getfunc("user32.dll", "GetRawInputDeviceInfoA");
    if (!GetRawInputDeviceInfoA_internal) GetRawInputDeviceInfoA_internal = GetRawInputDeviceInfoA_internal_fallback;
    return GetRawInputDeviceInfoA_internal(d, c, data, s);
}

static UINT WINAPI GetRawInputDeviceList_internal_fallback(PVOID r, PUINT n, UINT s) {
    (void)r; (void)n; (void)s;
    return (UINT)-1;
}
static UINT WINAPI GetRawInputDeviceList_internal_load(PVOID r, PUINT n, UINT s) {
    GetRawInputDeviceList_internal = getfunc("user32.dll", "GetRawInputDeviceList");
    if (!GetRawInputDeviceList_internal) GetRawInputDeviceList_internal = GetRawInputDeviceList_internal_fallback;
    return GetRawInputDeviceList_internal(r, n, s);
}

BOOL WINAPI RegisterRawInputDevices_internal_fallback(PVOID r, UINT n, UINT s) {
    (void)r; (void)n; (void)s;
    return FALSE;
}
BOOL WINAPI RegisterRawInputDevices_internal_load(PVOID r, UINT n, UINT s) {
    RegisterRawInputDevices_internal = getfunc("user32.dll", "RegisterRawInputDevices");
    if (!RegisterRawInputDevices_internal) RegisterRawInputDevices_internal = RegisterRawInputDevices_internal_fallback;
    return RegisterRawInputDevices_internal(r, n, s);
}

BOOL (WINAPI * AttachConsole_internal)(DWORD) = AttachConsole_internal_load;
BOOL (WINAPI * GetModuleHandleExW_internal)(DWORD, LPCWSTR, HMODULE*) = GetModuleHandleExW_internal_load;
UINT (WINAPI * GetRawInputData_internal)(HANDLE, UINT, LPVOID, PUINT, UINT) = GetRawInputData_internal_load;
UINT (WINAPI * GetRawInputDeviceInfoA_internal)(HANDLE, UINT, LPVOID, PUINT) = GetRawInputDeviceInfoA_internal_load;
UINT (WINAPI * GetRawInputDeviceList_internal)(PVOID, PUINT, UINT) = GetRawInputDeviceList_internal_load;
BOOL (WINAPI * RegisterRawInputDevices_internal)(PVOID, UINT, UINT) = RegisterRawInputDevices_internal_load;

#endif

/* vi: set ts=4 sw=4 expandtab: */
