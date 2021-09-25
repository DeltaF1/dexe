# Dexe

[Dexe](https://wiki.xxiivv.com/site/dexe.html) is a simple hex editor, written in [Uxntal](https://wiki.xxiivv.com/site/uxntal.html).

## Build

You must have the [Uxn](https://git.sr.ht/~rabbits/uxn/) assembler and emulator.

```
uxnasm noodle.tal noodle.rom && uxnemu noodle.rom
```

## Controls

### File

- `ctrl+r` edit filenname
- `ctrl+R` erase filenname
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
