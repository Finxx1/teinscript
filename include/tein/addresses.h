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

#define TEIN_ADDR_SPIKE_DIE_CALL 0x004ea9ed // Player.cpp:1390 (5 bytes)

#endif
