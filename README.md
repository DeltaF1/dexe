# Dexe

[Dexe](https://wiki.xxiivv.com/site/dexe.html) is a simple hex editor, written in [Uxntal](https://wiki.xxiivv.com/site/uxntal.html).

## Build

You must have the [Uxn](https://git.sr.ht/~rabbits/uxn/) assembler and emulator.

```
uxnasm src/main.tal bin/dexe.rom && uxnemu bin/dexe.rom
```

## Controls

### File

- `ctrl+r` edit filename
- `ctrl+R` erase filename
- `ctrl+o` open file
- `ctrl+s` save file

### Edit

- `ctrl+c` copy selection
- `ctrl+C` copy selection(text mode)
- `ctrl+v` paste selection

### General

- `arrows`, move selection
- `shift-arrows`, move selection
- `enter`, insert byte
- `backspace`, delete byte
