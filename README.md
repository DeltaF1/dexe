# Nasu

A minimal chr editor, written in ANSI C.

## Build

To build nasu, you must have [SDL2](https://wiki.libsdl.org/).

```
cc nasu.c -std=c89 -Os -DNDEBUG -g0 -s -Wall -L/usr/local/lib -lSDL2 -o nasu
```

## I/O

To resume working on a tileset:

```
./nasu example.chr
```

## Controls

### Generics

- `ctrl+n` New
- `ctrl+r` Reload
- `ctrl+s` Save(.chr)
- `ctrl+shift+s` Save(.bmp)
- `ctrl+h` Toggle Guides

### General

- `1234` Colors0-3
- `asdfg` Modes0-4
- `z` Decr. Brush Size
- `x` Incr. Brush Size
- `b` BigPixel Mode

### Paint

- `mouse1` Paint
- `mouse2` Line
- `mouse1+mouse3` Erase
- `ctrl+mouse1` Color Picker

## TODO

- Rotate tile
- Flip tile
- Roll pixels
- Roll colors
