#include <Windows.h>
#include <psapi.h>

#include <stdarg.h>

#include <tein/addresses.h>

#define STB_SPRINTF_IMPLEMENTATION
#include <stb_sprintf.h>

#include <teinmodcore.h>

extern "C" void EXPORT IPrint(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char buf[512] = "";
	int len = stbsp_vsnprintf(buf, 511, fmt, ap);
	va_end(ap);
	
	if (len <= 0) return;
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(out, buf, len, NULL, NULL);
}

static ptrdiff_t IFindLoadOffset(void) {
	MODULEINFO mi;
	BOOL success = K32GetModuleInformation(
		GetCurrentProcess(),
		GetModuleHandleA(NULL),
		&mi,
		sizeof(mi)
	);
	if (!success) return 0xdeadbeef;
	
	IPrint(
		"lpBaseOfDll = %p\nSizeOfImage = %u\nEntryPoint = %p\n",
		mi.lpBaseOfDll,
		mi.SizeOfImage,
		mi.EntryPoint
	);
	
	ptrdiff_t r = (uint8_t*)mi.EntryPoint - (uint8_t*)TEIN_ADDR_ENTRY;
	
	if (r >= 0) {
		IPrint("load offset: %tx\n", r);
	} else {
		IPrint("load offset: -%tx\n", -r); // I think using a - sign looks nicer
	}
	
	return r;
}

static ptrdiff_t ILoadOff = 0xdeadbeef;

extern "C" ptrdiff_t EXPORT IGetLoadOffset(void) {
	return ILoadOff;
}

bool IInit(void) {
	AllocConsole();
	IPrint("teinmodcore.dll loaded\n");
	
	ILoadOff = IFindLoadOffset();
	if (ILoadOff == 0xdeadbeef) {
		IPrint("IGetLoadOffset failed\n");
		return false;
	}
	
	IPatchNOP((void*)TEIN_ADDR_CHAINCHOMP_DASH, 175);
	
	if (!IRegisterHooks()) {
		IPrint("IRegisterHooks failed\n");
		return false;
	}
	
	return true;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID reserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		bool r = IInit();
		if (!r) MessageBoxA(NULL, "TEiN Mod Loader has encountered an error.", "Error", MB_OK);
		return r;
	}
	
	return TRUE;
}