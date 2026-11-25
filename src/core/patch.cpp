#include <Windows.h>

#include <teinmodcore.h>

extern "C" bool EXPORT IWriteProtected(void* dst, void* src, size_t bytes) {
	if (dst == NULL) return false;
	if (src == NULL) return false;
	if (bytes == 0) return false;
	
	DWORD oldprot;
	BOOL success = VirtualProtect(dst, bytes, PAGE_READWRITE, &oldprot);
	if (!success) return false;
	
	memcpy(dst, src, bytes);
	
	DWORD prot;
	success = VirtualProtect(dst, bytes, oldprot, &prot);
	if (!success) return false;
	return true;
}

extern "C" void EXPORT * IAllocProtected(size_t bytes) {
	if (bytes == 0) return false;
	void* r = VirtualAlloc(NULL, bytes, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READ);
	return r;
}

extern "C" void EXPORT IFreeProtected(void* addr) {
	VirtualFree(addr, 0, MEM_RELEASE);
}

// Replaces code with NOPs
extern "C" bool EXPORT IPatchNOP(void* addr, size_t bytes) {
	if (addr == NULL) return false;
	if (bytes == 0) return false;
	
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
	if (addr == NULL) return false;
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
	if (addr == NULL) return false;
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

// Replaces a function call with a call to a stub that calls a detour followed
// by the original function. Ensure detourfn is cdecl!!!
extern "C" bool EXPORT IPatchDetour(void* addr, IDetourFn detourfn, bool classfunc) {
	if (addr == NULL) return false;
	if (detourfn == NULL) return false;
	
	uint8_t* byteaddr = (uint8_t*)addr + IGetLoadOffset();
	if (byteaddr[0] != 0xe8) return false; // Not CALL rel32
	int32_t reladdr = *(int32_t*)(byteaddr + 1);
	uint8_t* targetaddr = byteaddr + 5 + reladdr;
	
	uint8_t* stub = (uint8_t*)IAllocProtected(12);
	if (stub == NULL) return false;
	
	uint8_t buf[] = {
		0x51, // PUSH ECX
		0xe8, 0x00, 0x00, 0x00, 0x00, // CALL rel32
		0x59, // POP ECX
		0xe9, 0x00, 0x00, 0x00, 0x00, // JMP rel32
	};
	uint8_t* reldetourfn = (uint8_t*)((uint8_t*)detourfn - (stub + 6));
	memcpy(buf + 2, &reldetourfn, sizeof(uint8_t*));
	uint8_t* reltargetfn = (uint8_t*)((uint8_t*)targetaddr - (stub + 12));
	memcpy(buf + 8, &reltargetfn, sizeof(uint8_t*));
	
	// Don't push/pop ECX if not a thiscall
	if (!classfunc) {
		buf[0] = 0x90; // NOP
		buf[6] = 0x90;
	}
	
	if (!IWriteProtected(stub, buf, 12)) {
		IFreeProtected(stub);
		return false;
	}
	
	uint8_t* relstub = (uint8_t*)((uint8_t*)stub - (byteaddr + 5));
	if (!IWriteProtected(byteaddr + 1, &relstub, sizeof(uint8_t*))) {
		IFreeProtected(stub);
		return false;
	}
	
	IPrint("Created detour at %p from %p to %p with stub %p\n", byteaddr, targetaddr, detourfn, stub);
	
	return true;
}
