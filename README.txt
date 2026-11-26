teinscript - a mod loader (and optional scripting language) for The End is Nigh

Getting Started

Teinscript requires a modified TheEndIsNigh.exe to load teinmodcore.dll. For
now this must be done manually by adding the DLL to the import table.

Contributing

NOTE: Teinscript does _NOT_ accept any contributions containing
AI-generated code.

Teinscript is written in C++ but is very much written in a C-like
style. This is intentional, and contributions should reflect
the present style. Templates and lambdas should not be used,
nor should the STL.

Teinscript consists of two parts: the DLL that hooks into the game and the
Teinscript code mod. The game is modified to load teinmodcore.dll which sets
up its own hooks and then loads the list of code mods in the `TEINCODEMODS`
environment variable, which is separated by semicolons. Teinscript is itself a
mod distributed with the mod loader.