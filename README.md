# Dexe

A minimal hex editor, written in ANSI C.

## Build

You must have the [Uxn](https://git.sr.ht/~rabbits/uxn/) assembler and emulator.

```
uxnasm noodle.usm noodle.rom && uxnemu noodle.rom
```

## Controls

- `arrows`, move selection
- `enter`, insert byte
- `backspace`, delete byte
- `left-click`, select bytes
