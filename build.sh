#!/bin/bash

# format code
clang-format -i exed.c

# remove old
rm ./exed

# debug(slow)
cc -std=c89 -DDEBUG -Wall -Wno-unknown-pragmas -Wpedantic -Wshadow -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og -fsanitize=address -fsanitize=undefined exed.c -L/usr/local/lib -lSDL2 -o exed

# build(fast)
# cc exed.c -std=c89 -Os -DNDEBUG -g0 -s -Wall -Wno-unknown-pragmas -L/usr/local/lib -lSDL2 -o exed

# Size
echo "Size: $(du -sk ./exed)"

# Install
if [ -d "$HOME/bin" ] && [ -e ./exed ]
then
	cp ./exed $HOME/bin
    echo "Installed: $HOME/bin" 
fi

# run
./exed example.chr
# ./exed example.bmp
