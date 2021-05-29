#!/usr/bin/env bash

# format code
# clang-format -i dexe.c

# remove old
# rm ./dexe

# debug(slow)
# cc -std=c89 -DDEBUG -Wall -Wno-unknown-pragmas -Wpedantic -Wshadow -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og -fsanitize=address -fsanitize=undefined dexe.c -L/usr/local/lib -lSDL2 -o dexe

# build(fast)
# cc dexe.c -std=c89 -Os -DNDEBUG -g0 -s -Wall -Wno-unknown-pragmas -L/usr/local/lib -lSDL2 -o dexe

# Size
# echo "Size: $(du -sk ./dexe)"

# Install
# if [ -d "$HOME/bin" ] && [ -e ./dexe ]
# then
	# cp ./dexe $HOME/bin
    # echo "Installed: $HOME/bin" 
# fi

# run
# ./dexe example.chr
# ./dexe example.bmp


echo "Cleaning.."
rm -rf bin
mkdir bin

echo "Assembling.."
uxnasm dexe.usm bin/dexe.rom 

echo "Installing.."
if [ -d "$HOME/roms" ] && [ -e ./bin/dexe.rom ]
then
	cp ./bin/dexe.rom $HOME/roms
    echo "Installed in $HOME/roms" 
fi

echo "Running.."
uxnemu bin/dexe.rom
