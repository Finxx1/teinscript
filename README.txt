teinscript - a mod loader (and optional scripting language) for The End is Nigh

Getting Started

Teinscript doesn't actually modify your game files, so
you launch mods from teinscript directly. It will automatically
detect any mods downloaded from the workshop or placed
in your mod directory. Mods cannot themselves inject code
for security, so code mods have to be installed separately.

Contributing

NOTE: Teinscript does _NOT_ accept any contributions containing
AI-generated code.

Teinscript is written in C++ but is very much written in a C-like
style. This is intentional, and contributions should reflect
the present style. Templates and lambdas should not be used,
nor should the STL.

Teinscript consists of three parts: the loader and the injected DLL
that hooks into the game, and the Teinscript code mod. The loader finds
game mods and code mods and then launches the game with the injection
DLL. The injection DLL installs detours in various places, with code
mods registering themselves with the DLL. The Teinscript code mod
implements Teinscript, a basic language for extending functionality in
levels.