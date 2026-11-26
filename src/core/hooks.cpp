#include <teinmodcore.h>

#include <tein/addresses.h>
#include <tein/Player.h>

static glaiel::Player* ICurrentPlayer = NULL;

void ICreatePlayerHook(uintptr_t self, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("New player %p\n", self);
	
	ICurrentPlayer = (glaiel::Player*)self;
}

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

void IMotherDeathHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to mother\n");
}

void IAgonDieHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to agon\n");
}

void IFinalSteveDieHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player died to final steve\n");
}

void IEnemyBounceHook(void) {
	IPrint("Player enemy bounced\n");
}

void IMushBounceHook(void) {
	IPrint("Player mush bounced\n");
}

void IHighBouceHook(void) {
	IPrint("Player high bounced\n");
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

void IPlayerUpdateEnd(void) {
	if (ICurrentPlayer->MoveX < 0.0) {
		//IPrint("Player moved left\n");
	} else if (ICurrentPlayer->MoveX > 0.0) {
		//IPrint("Player moved right\n");
	}
}

void ILedgeGrabHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Player grabbed ledge\n");
}

void INPCHook(void) {
	IPrint("NPC activated\n");
}

void IOrbExplodeHook(uintptr_t, uintptr_t, uintptr_t, uintptr_t) {
	IPrint("Orb exploded\n");
}

bool IRegisterHooks(void) {
	if (!IPatchDetour((void*)TEIN_ADDR_PLAYER_INIT_CALL1, ICreatePlayerHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_PLAYER_INIT_CALL2, ICreatePlayerHook, true)) return false;
	
	if (!IPatchDetour((void*)TEIN_ADDR_SPIKE_DIE_CALL, ISpikeDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_RESET_DIE_CALL, IResetDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_ENEMY_DIE_CALL, IEnemyDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_OOB_DIE_CALL, IOobDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_SQUISHX_DIE_CALL, ISquishXDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_SQUISHY_DIE_CALL, ISquishYDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_SQUISH_DIE_CALL, ISquishDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_TOXIC_DIE_CALL, IToxicDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_MOTHER_DIE_CALL, IMotherDeathHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_AGON_DIE_CALL, IAgonDieHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_FINALSTEVE_DIE_CALL1, IFinalSteveDieHook, true)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_FINALSTEVE_DIE_CALL2, IFinalSteveDieHook, true)) return false;
	
	if (!IPatchDetour((void*)TEIN_ADDR_LEDGE_GRAB_SOUND_CALL, ILedgeGrabHook, true)) return false;
	
	if (!IPatchInject((void*)TEIN_ADDR_ENEMY_BOUNCE, IEnemyBounceHook, 8)) return false;
	if (!IPatchInject((void*)TEIN_ADDR_MUSH_BOUNCE, IMushBounceHook, 8)) return false;
	if (!IPatchInject((void*)TEIN_ADDR_HIGH_BOUNCE, IHighBouceHook, 8)) return false;
	
	if (!IPatchInject((void*)TEIN_ADDR_GROUND_JUMP, IGroundJumpHook, 8)) return false;
	if (!IPatchInject((void*)TEIN_ADDR_BONUS_JUMP, IBonusJumpHook, 8)) return false;
	if (!IPatchInject((void*)TEIN_ADDR_CORNER_JUMP, ICornerJumpHook, 8)) return false;
	if (!IPatchInject((void*)TEIN_ADDR_LONG_JUMP, ILongJumpHook, 8)) return false;
	
	if (!IPatchInject((void*)TEIN_ADDR_PLAYER_UPDATE_END, IPlayerUpdateEnd, 5)) return false;
	
	if (!IPatchInject((void*)TEIN_ADDR_NPC_INTERACT, INPCHook, 7)) return false;
	if (!IPatchDetour((void*)TEIN_ADDR_ORB_EXPLODE, IOrbExplodeHook, true)) return false;
	
	return true;
}
