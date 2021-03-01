# Manic Miner: Retro!

A work-in-progress port of Matthew Smith's classic 1983 Sinclair ZX Spectrum
game to the C++ language and using SDL.

      Penguins, Man Eating Toilets, Seals, Mutant Telephones, Bugs,
      Falling Skylabs, Kangaroo's, plus many more, join forces
      to stop Willy in his quest to get the treasure

This is my **very experimental** attempt at porting the Manic Miner Z80 assembly
to the C++ language, allowing for it to be played on Windows, macOS, and Linux,
without the need for a ZX Spectrum emulator.

One of my goals was to retain the pixel perfect nature of the game. To achieve
this the original memory reading/writing as done on the ZX Spectrum has been
retained. The full memory map is provided as a simple array:

```c++
// src/speccy.h
uint8_t memory[TOTAL_MEMORY]; // 64KB
```

All memory read/write address values of the original assembly (e.g. `LD (16384),A`)
were retained in the C code without alteration. It does however mean that on each
game frame the screen memory has to be converted to something that SDL can work
with. This comes at a performance cost, however, this should not a problem with
any computer from the last couple of decades.

It's a neat trick that saved a huge amount of time and allowed for a pixel perfect conversion.


_The Z80 assembly I used as a starting point was disassembled by Richard Dymond
as part of his Skoolkit project (https://github.com/skoolkid/manicminer)._


_NO EMULATORS were harmed in the making of this game!_


## Controls

- Left, right arrow keys
- Space to jump
- Enter to start
- P to pause
- M to mute sound
- Q to quit


## Current State of the Code

The goal of the project is to re-implement Manic Miner in portable C/C++ code,
without changing any of the core game play!

    Q. Does it build, does it run?
    A. Yes and Yes!

    Q. Is it playable?
    A. Yes, mostly.


### What works, what doesn't

As the game is playable most things work as expected and there are actually not
that many bugs. Therefore it's easier to list the things that don't work:

- GFX glitches: baddie sprites can 'pick up' pixels from Willy when you _get too close_ to them.
- No Sound.
- No Joystick support.
- Game Over screen needs some work.
- Performance: it's not the most performant code at the moment.

I've only tested on Linux and macOS.


## The Game : Introduction

Miner Willy, while prospecting down Surbiton way, stumbles
upon an ancient, long forgotten mine-shaft. On further
exploration, he finds evidence of a lost civilisation far
superior to our own, which used automatons to dig beep into
the Earth's core to supply the essential raw materials for
their advanced industry. After centuries of peace and
prosperity, the civilisation was torn apart by war, and
lapsed into a long dark age, abandoning their industry and
machines. Nobody, however, thought to tell the mine robots
to stop working, and through countless aeons they had
steadily accumulated a hugh stockpile of valuable metals
and minerals, and Miner Willy realises that he now has the
opportunity to make his fortune by finding the underground
store.

Can YOU take the challenge and guide Willy through the
underground caverns to the surface and riches.

In order to move to the next chamber, you must collect all
the flashing keys in the room while avoiding nasties like
POISONOUS PANSIES and SPIDERS and SLIME and worst of all,
MANIC MINING ROBOTS. When you have all the keys, you can
enter the portal which will now be flashing. The game ends
when you have been 'got' or fallen heavily three times.


# LICENSE

Manic Miner Copyright (c) 1983 Matthew Smith.

Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
