#include <teinmodcore.h>

#include <tein/addresses.h>

void ISpikeDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to tile\n");
}

void IResetDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died by reset\n");
}

void IEnemyDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to enemy\n");
}

void IOobDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to out of bounds\n");
}

void ISquishXDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to x squish\n");
}

void ISquishYDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to y squish\n");
}

void ISquishDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to squish\n");
}

void IToxicDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to toxic\n");
}

void IGroundJumpHook(void) {
	IPrint("Player ground jumped\n");
}

void IBonusJumpHook(void) {
	IPrint("Player bonus jumped\n");
}

void ICornerJumpHook(void) {
	IPrint("Player corner jumped\n");
}

void ILongJumpHook(void) {
	IPrint("Player long jumped\n");
}

bool IRegisterHooks(void) {
	if (!IPatchDetour((void*)TEIN_ADDR_SPIKE_DIE_CALL, ISpikeDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_RESET_DIE_CALL, IResetDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_ENEMY_DIE_CALL, IEnemyDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_OOB_DIE_CALL, IOobDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_SQUISHX_DIE_CALL, ISquishXDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_SQUISHY_DIE_CALL, ISquishYDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_SQUISH_DIE_CALL, ISquishDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_TOXIC_DIE_CALL, IToxicDeathHook, true)) return false;
	
	if (!IPatchInject((void*)TEIN_ADDR_GROUND_JUMP, IGroundJumpHook, 8)) return false;
	if (!IPatchInject((void*)TEIN_ADDR_BONUS_JUMP, IBonusJumpHook, 8)) return false;
	if (!IPatchInject((void*)TEIN_ADDR_CORNER_JUMP, ICornerJumpHook, 8)) return false;
	if (!IPatchInject((void*)TEIN_ADDR_LONG_JUMP, ILongJumpHook, 8)) return false;
	
	return true;
}
