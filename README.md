# Exed

A minimal hex editor, written in ANSI C.

## Build

To build exed, you must have [SDL2](https://wiki.libsdl.org/).

```
cc exed.c -std=c89 -Os -DNDEBUG -g0 -s -Wall -L/usr/local/lib -lSDL2 -o exed
```

## I/O

To resume working on a tileset:

```
./exed example.chr
```

## Controls

### Generics

- `ctrl+n` New
- `ctrl+r` Reload
- `ctrl+s` Save(.chr)
- `ctrl+h` Toggle Guides

### General

- `up` Move up
- `down` Move down
- `left` Move left
- `right` Move right
- `ctrl+up` Increment
- `ctrl+down` Decrement
- `ctrl+left` Roll Right
- `ctrl+right` Roll Left
