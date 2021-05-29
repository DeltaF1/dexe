# Dexe

A minimal hex editor, written in ANSI C.

## Build

You must have the [Uxn](https://git.sr.ht/~rabbits/uxn/) assembler and emulator.

```
uxnasm noodle.usm noodle.rom && uxnemu noodle.rom
```

## Controls

### Generics

- `ctrl+n` New
- `ctrl+r` Reload
- `ctrl+s` Save(.chr)
- `ctrl+h` Toggle Guides

### General

- `0-9a-f` Insert
- `backspace` Erase
- `ctrl+up` Increment
- `ctrl+down` Decrement
- `ctrl+left` Roll Right
- `ctrl+right` Roll Left

### Move

- `up` Move up
- `down` Move down
- `left` Move left
- `right` Move right
