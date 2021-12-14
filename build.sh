#!/usr/bin/env bash

echo "Cleaning.."
rm -rf bin
mkdir bin

echo "Assembling.."
uxnasm src/main.tal bin/dexe.rom 

echo "Installing.."
if [ -d "$HOME/roms" ] && [ -e ./bin/dexe.rom ]
then
	cp ./bin/dexe.rom $HOME/roms
    echo "Installed in $HOME/roms" 
fi

if [ "${1}" = '--push' ]; 
then
	echo "Pushing.."
	~/Applications/butler push bin/dexe.rom hundredrabbits/dexe:uxn
fi

echo "Running.."
uxnemu bin/dexe.rom example.chr
