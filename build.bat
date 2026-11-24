@echo off

pushd .
cd src/core
nmake
copy /Y teinmodcore.dll "C:\Program Files (x86)\Steam\steamapps\common\theendisnigh\teinmodcore.dll"
popd
