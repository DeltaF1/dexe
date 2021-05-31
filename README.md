# Dexe

Dexe is a [hex editor](https://wiki.xxiivv.com/site/dexe.html), written in [Uxntal](https://wiki.xxiivv.com/site/uxntal.html).

## Build

You must have the [Uxn](https://git.sr.ht/~rabbits/uxn/) assembler and emulator.

```
uxnasm noodle.tal noodle.rom && uxnemu noodle.rom
```

## Controls

- `arrows`, move selection
- `enter`, insert byte
- `backspace`, delete byte
- `left-click`, select bytes
