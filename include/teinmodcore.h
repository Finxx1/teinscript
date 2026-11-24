#ifndef TEINMODCORE_H_
#define TEINMODCORE_H_

#include <stdint.h>
#include <stddef.h>

#ifdef EXPORT
#undef EXPORT
#endif
#define EXPORT __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

// main.cpp
void EXPORT IPrint(const char* fmt, ...);
ptrdiff_t EXPORT IGetLoadOffset(void);

// patch.cpp
bool EXPORT IWriteProtected(void* dst, void* src, size_t bytes);
bool EXPORT IPatchNOP(void* addr, size_t bytes);
bool EXPORT IPatchCallZero(void* addr, size_t bytes);
bool EXPORT IPatchCallOne(void* addr, size_t bytes);

#ifdef __cplusplus
}
#endif

#endif
