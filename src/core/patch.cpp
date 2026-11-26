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

// Effectively injects code in the middle of a function. Does this by
// overwriting insbytes with a jmp instruction to a stub that runs injectfn
// followed by the overwritten code. Special care must be taken to ensure no
// overwritten instructions use IP-relative addressing, and ensure injectfn is
// cdecl.
extern "C" bool EXPORT IPatchInject(void* addr, IInjectFn injectfn, size_t insbytes) {
	if (addr == NULL) return false;
	if (injectfn == NULL) return false;
	if (insbytes < 5) return false;
	
	uint8_t* byteaddr = (uint8_t*)addr + IGetLoadOffset();
	
	uint8_t buf[] = {
		0x60, // PUSHAD
		0x81, 0xEC, 0x80, 0x00, 0x00, 0x00, // sub         esp,80h
		0xF3, 0x0F, 0x7F, 0x04, 0x24,       // movdqu      xmmword ptr [esp],xmm0
		0xF3, 0x0F, 0x7F, 0x4C, 0x24, 0x10, // movdqu      xmmword ptr [esp+10h],xmm1
		0xF3, 0x0F, 0x7F, 0x54, 0x24, 0x20, // movdqu      xmmword ptr [esp+20h],xmm2
		0xF3, 0x0F, 0x7F, 0x5C, 0x24, 0x30, // movdqu      xmmword ptr [esp+30h],xmm3
		0xF3, 0x0F, 0x7F, 0x64, 0x24, 0x40, // movdqu      xmmword ptr [esp+40h],xmm4
		0xF3, 0x0F, 0x7F, 0x6C, 0x24, 0x50, // movdqu      xmmword ptr [esp+50h],xmm5
		0xF3, 0x0F, 0x7F, 0x74, 0x24, 0x60, // movdqu      xmmword ptr [esp+60h],xmm6
		0xF3, 0x0F, 0x7F, 0x7C, 0x24, 0x70, // movdqu      xmmword ptr [esp+70h],xmm7
		0xe8, 0x00, 0x00, 0x00, 0x00, // CALL rel32
		0xF3, 0x0F, 0x6F, 0x04, 0x24,       // movdqu      xmm0,xmmword ptr [esp]
		0xF3, 0x0F, 0x6F, 0x4C, 0x24, 0x10, // movdqu      xmm1,xmmword ptr [esp+10h]
		0xF3, 0x0F, 0x6F, 0x54, 0x24, 0x20, // movdqu      xmm2,xmmword ptr [esp+20h]
		0xF3, 0x0F, 0x6F, 0x5C, 0x24, 0x30, // movdqu      xmm3,xmmword ptr [esp+30h]
		0xF3, 0x0F, 0x6F, 0x64, 0x24, 0x40, // movdqu      xmm4,xmmword ptr [esp+40h]
		0xF3, 0x0F, 0x6F, 0x6C, 0x24, 0x50, // movdqu      xmm5,xmmword ptr [esp+50h]
		0xF3, 0x0F, 0x6F, 0x74, 0x24, 0x60, // movdqu      xmm6,xmmword ptr [esp+60h]
		0xF3, 0x0F, 0x6F, 0x7C, 0x24, 0x70, // movdqu      xmm7,xmmword ptr [esp+70h]
		0x81, 0xC4, 0x80, 0x00, 0x00, 0x00, // add         esp,80h
		0x61, // POPAD
	};
	uint8_t* stub = (uint8_t*)IAllocProtected(insbytes + sizeof(buf) + 5);
	uint8_t* relinjectfn = (uint8_t*)((uint8_t*)injectfn - (stub + 59));
	memcpy(buf + 55, &relinjectfn, sizeof(uint8_t*));
	
	if (!IWriteProtected(stub, buf, sizeof(buf))) {
		IFreeProtected(stub);
		return false;
	}
	
	if (!IWriteProtected(stub + sizeof(buf), byteaddr, insbytes)) {
		IFreeProtected(stub);
		return false;
	}
	
	buf[0] = 0xe9; // JMP rel32
	uint8_t* relreturn = (uint8_t*)(byteaddr + insbytes - (stub + sizeof(buf) + insbytes + 5));
	memcpy(buf + 1, &relreturn, sizeof(uint8_t*));
	
	if (!IWriteProtected(stub + sizeof(buf) + insbytes, buf, 5)) {
		IFreeProtected(stub);
		return false;
	}
	
	buf[0] = 0xe9; // JMP rel32
	uint8_t* relstub = (uint8_t*)((uint8_t*)stub - (byteaddr + 5));
	memcpy(buf + 1, &relstub, sizeof(uint8_t*));
	
	if (!IWriteProtected(byteaddr, buf, 5)) {
		IFreeProtected(stub);
		return false;
	}
	
	for (size_t i = 0; i < insbytes - 5; i++) {
		uint8_t nop = 0x90;
		if (!IWriteProtected(byteaddr + 5 + i, &nop, 1)) {
			IFreeProtected(stub);
			return false;
		}
	}
	
	IPrint("Created injection at %p with %p with stub %p\n", byteaddr, injectfn, stub);
	
	return true;
}
