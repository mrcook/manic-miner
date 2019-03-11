# Manic Miner Video Game

This is a work-in-progress port of Matthew Smith's classic 1983 Sinclair
ZX Spectrum game to the C language.

Manic Miner is Copyright (c) 1983 Matthew Smith.
C/C++ port is Copyright (c) 2016-2019 Michael R. Cook

      Penguins, Man Eating Toilets, Seals, Mutant Telephones, Bugs,
      Falling Skylabs, Kangaroo's, plus many more, join forces
      to stop Willy in his quest to get the treasure

This is my **EXPERIMENTAL** attempt to port the Manic Miner Z80 assembly code
to the C language, allowing it to be played on Windows, macOS, and Linux,
without the need of a ZX Spectrum emulator.

When starting this project I had no previous experience with Z80 assembly, and
only a basic understanding of the C language, therefore this is a _learning
project_, but who knows, perhaps one day the port will be complete.

_The Z80 assembly I used as a starting point was disassembled by Richard Dymond
as part of his Skoolkit project (http://skoolkit.ca)._


_NO EMULATORS were harmed in the making of this game!_


## Controls

- Left, right arrow keys
- Space to jump
- Enter to start
- P to pause
- M to mute sound


## Current State of the Code

The goal of the project is to re-implement Manic Miner in portable C/C++ code,
without changing any of the core game play!

    Q. Does it build, does it run?
    A. Yes!

    Q. Is it playable?
    A. Yes, mostly.


### What works, what doesn't

As the game is playable most things work as expected and there are not actually
that many bugs. Therefore it's easier to list the things that don't work:

- Sound.
- Joystick support.
- Conveyor belts: it's possible to reverse the walking direction.
- GFX glitches: Willy picks up pixels from nearby baddies.
- Game Over screen needs some work.
- Performance: it's not the most performant code at the moment.

As I do my development on macOS, I've only tested on that system.


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
undergraound caverns to the surface and riches.

In order to move to the next chamber, you must collect all
the flashing keys in the room while avoiding nasties like
POISONOUS PANSIES and SPIDERS and SLIME and worst of all,
MANIC MINING ROBOTS. When you have all the keys, you can
enter the portal which will now be flashing. The game ends
when you have been 'got' or fallen heavily three times.
