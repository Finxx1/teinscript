#include <Windows.h>

#include <teinmodcore.h>

extern "C" bool EXPORT IWriteProtected(void* dst, void* src, size_t bytes) {
	DWORD oldprot;
	BOOL success = VirtualProtect(dst, bytes, PAGE_READWRITE, &oldprot);
	if (!success) return false;
	
	memcpy(dst, src, bytes);
	
	DWORD prot;
	success = VirtualProtect(dst, bytes, oldprot, &prot);
	if (!success) return false;
	return true;
}

// Replaces code with NOPs
extern "C" bool EXPORT IPatchNOP(void* addr, size_t bytes) {
	uint8_t* byteaddr = (uint8_t*)addr + IGetLoadOffset();

	uint8_t* buf = (uint8_t*)malloc(bytes);
	if (!buf) return false;
	
	uint8_t nop = 0x90;
	memset(buf, nop, bytes);
	
	bool r = IWriteProtected(byteaddr, buf, bytes);
	free(buf);
	return r;
}

// Replaces a function call with code that pretends the function returned zero
extern "C" bool EXPORT IPatchCallZero(void* addr, size_t bytes) {
	if (bytes < 2) return false;
	uint8_t* byteaddr = (uint8_t*)addr + IGetLoadOffset();
	
	uint8_t* buf = (uint8_t*)malloc(bytes);
	if (!buf) return false;
	
	buf[0] = 0x33; // XOR EAX,EAX
	buf[1] = 0xc0;
	
	uint8_t nop = 0x90;
	if (bytes > 2) memset(buf + 2, nop, bytes - 2);
	
	bool r = IWriteProtected(byteaddr, buf, bytes);
	free(buf);
	return r;
}

// Replaces a function call with code that pretends the function returned one
extern "C" bool EXPORT IPatchCallOne(void* addr, size_t bytes) {
	if (bytes < 3) return false;
	uint8_t* byteaddr = (uint8_t*)addr + IGetLoadOffset();
	
	uint8_t* buf = (uint8_t*)malloc(bytes);
	if (!buf) return false;
	
	buf[0] = 0x33; // XOR EAX,EAX
	buf[1] = 0xc0;
	buf[2] = 0x40; // INC
	
	uint8_t nop = 0x90;
	if (bytes > 3) memset(buf + 3, nop, bytes - 3);
	
	bool r = IWriteProtected(byteaddr, buf, bytes);
	free(buf);
	return r;
}
