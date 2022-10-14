#pragma once
#include "../main.h"
#include "../debug.h"

extern CDebug *pDebug;

uintptr_t FindLibrary(const char* library);
void cp1251_to_utf8(char *out, const char *in, unsigned int len = 0);

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

void Log(const char *fmt, ...);
uint32_t GetTickCount();
