#ifndef TEIN_ADDRESSES_H
#define TEIN_ADDRESSES_H

/*

This file contains the address locations of various things in the TEiN
executable. These are found using the PDB that Tyler has given us.

Note that Ghidra adds in a default relocation at 0x00400000. This IS INCLUDED
in the address. The real entrypoint for example is 0x001ee25d, but we use the
address in Ghidra instead. The entrypoint is used to correct for relocations.

*/

#define TEIN_ADDR_ENTRY 0x005ee25d // WinMainCRTStartup

#define TEIN_ADDR_TOXIC_TIMER_DIE 0x004ead80 // Player.cpp:1422 (43 bytes)
#define TEIN_ADDR_SPIKE_DIE 0x004ea9e9 // Player.cpp:1389 (46 bytes)

#define TEIN_ADDR_CHAINCHOMP_DASH 0x0048ff1a // ChainChomp.cpp:180 (175 bytes)

#define TEIN_ADDR_RESET_DIE_CALL 0x004e9b3e // Player.cpp:1140 (5 bytes)
#define TEIN_ADDR_ENEMY_DIE_CALL 0x004ea31b // Player.cpp:1216 (5 bytes)
#define TEIN_ADDR_OOB_DIE_CALL 0x004ea4c0 // Player.cpp:1322 (5 bytes)
#define TEIN_ADDR_SQUISHX_DIE_CALL 0x004ea4f4 // Player.cpp:1346 (5 bytes)
#define TEIN_ADDR_SQUISHY_DIE_CALL 0x004ea51b // Player.cpp:1349 (5 bytes)
#define TEIN_ADDR_SQUISH_DIE_CALL 0x004ea53f // Player.cpp:1352 (5 bytes)
#define TEIN_ADDR_SPIKE_DIE_CALL 0x004ea9ed // Player.cpp:1390 (5 bytes)
#define TEIN_ADDR_TOXIC_DIE_CALL 0x004ead84 // Player.cpp:1422 (5 bytes)

#define TEIN_ADDR_GROUND_JUMP 0x004e82c1 // Player.cpp:743 (8 bytes)
#define TEIN_ADDR_BONUS_JUMP 0x004e84b0 // Player.cpp:773 (8 bytes)
#define TEIN_ADDR_CORNER_JUMP 0x004e81f6 // Player.cpp:731 (8 bytes)
#define TEIN_ADDR_LONG_JUMP 0x004e8194 // Player.cpp:729 (8 bytes)

#endif
