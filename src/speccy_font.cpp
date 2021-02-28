// Speccy library Copyright (c) 2016-2021 Michael R. Cook

#include "speccy.h"

// The ZX Spectrum ASCII font graphics
uint8_t SpeccyDisplay_Font[96][8] = {
        {0,  0,   0,   0,   0,   0,   0,   0},   // $20 - character: ' '   CHR$(32)
        {0,  16,  16,  16,  16,  0,   16,  0},   // $21 - character: '!'   CHR$(33)
        {0,  36,  36,  0,   0,   0,   0,   0},   // $22 - character: '"'   CHR$(34)
        {0,  36,  126, 36,  36,  126, 36,  0},   // $23 - character: '#'   CHR$(35)
        {0,  8,   62,  40,  62,  10,  62,  8},   // $24 - character: '$'   CHR$(36)
        {0,  98,  100, 8,   16,  38,  70,  0},   // $25 - character: ''    CHR$(37)
        {0,  16,  40,  16,  42,  68,  58,  0},   // $26 - character: '&'   CHR$(38)
        {0,  8,   16,  0,   0,   0,   0,   0},   // $27 - character: '''   CHR$(39)
        {0,  4,   8,   8,   8,   8,   4,   0},   // $28 - character: '('   CHR$(40)
        {0,  32,  16,  16,  16,  16,  32,  0},   // $29 - character: ')'   CHR$(41)
        {0,  0,   20,  8,   62,  8,   20,  0},   // $2A - character: '*'   CHR$(42)
        {0,  0,   8,   8,   62,  8,   8,   0},   // $2B - character: '+'   CHR$(43)
        {0,  0,   0,   0,   0,   8,   8,   16},  // $2C - character: ','   CHR$(44)
        {0,  0,   0,   0,   62,  0,   0,   0},   // $2D - character: '-'   CHR$(45)
        {0,  0,   0,   0,   0,   24,  24,  0},   // $2E - character: '.'   CHR$(46)
        {0,  0,   2,   4,   8,   16,  32,  0},   // $2F - character: '/'   CHR$(47)
        {0,  60,  70,  74,  82,  98,  60,  0},   // $30 - character: '0'   CHR$(48)
        {0,  24,  40,  8,   8,   8,   62,  0},   // $31 - character: '1'   CHR$(49)
        {0,  60,  66,  2,   60,  64,  126, 0},   // $32 - character: '2'   CHR$(50)
        {0,  60,  66,  12,  2,   66,  60,  0},   // $33 - character: '3'   CHR$(51)
        {0,  8,   24,  40,  72,  126, 8,   0},   // $34 - character: '4'   CHR$(52)
        {0,  126, 64,  124, 2,   66,  60,  0},   // $35 - character: '5'   CHR$(53)
        {0,  60,  64,  124, 66,  66,  60,  0},   // $36 - character: '6'   CHR$(54)
        {0,  126, 2,   4,   8,   16,  16,  0},   // $37 - character: '7'   CHR$(55)
        {0,  60,  66,  60,  66,  66,  60,  0},   // $38 - character: '8'   CHR$(56)
        {0,  60,  66,  66,  62,  2,   60,  0},   // $39 - character: '9'   CHR$(57)
        {0,  0,   0,   16,  0,   0,   16,  0},   // $3A - character: ':'   CHR$(58)
        {0,  0,   16,  0,   0,   16,  16,  32},  // $3B - character: '//'  CHR$(59)
        {0,  0,   4,   8,   16,  8,   4,   0},   // $3C - character: '<'   CHR$(60)
        {0,  0,   0,   62,  0,   62,  0,   0},   // $3D - character: '='   CHR$(61)
        {0,  0,   16,  8,   4,   8,   16,  0},   // $3E - character: '>'   CHR$(62)
        {0,  60,  66,  4,   8,   0,   8,   0},   // $3F - character: '?'   CHR$(63)
        {0,  60,  74,  86,  94,  64,  60,  0},   // $40 - character: '@'   CHR$(64)
        {0,  60,  66,  66,  126, 66,  66,  0},   // $41 - character: 'A'   CHR$(65)
        {0,  124, 66,  124, 66,  66,  124, 0},   // $42 - character: 'B'   CHR$(66)
        {0,  60,  66,  64,  64,  66,  60,  0},   // $43 - character: 'C'   CHR$(67)
        {0,  120, 68,  66,  66,  68,  120, 0},   // $44 - character: 'D'   CHR$(68)
        {0,  126, 64,  124, 64,  64,  126, 0},   // $45 - character: 'E'   CHR$(69)
        {0,  126, 64,  124, 64,  64,  64,  0},   // $46 - character: 'F'   CHR$(70)
        {0,  60,  66,  64,  78,  66,  60,  0},   // $47 - character: 'G'   CHR$(71)
        {0,  66,  66,  126, 66,  66,  66,  0},   // $48 - character: 'H'   CHR$(72)
        {0,  62,  8,   8,   8,   8,   62,  0},   // $49 - character: 'I'   CHR$(73)
        {0,  2,   2,   2,   66,  66,  60,  0},   // $4A - character: 'J'   CHR$(74)
        {0,  68,  72,  112, 72,  68,  66,  0},   // $4B - character: 'K'   CHR$(75)
        {0,  64,  64,  64,  64,  64,  126, 0},   // $4C - character: 'L'   CHR$(76)
        {0,  66,  102, 90,  66,  66,  66,  0},   // $4D - character: 'M'   CHR$(77)
        {0,  66,  98,  82,  74,  70,  66,  0},   // $4E - character: 'N'   CHR$(78)
        {0,  60,  66,  66,  66,  66,  60,  0},   // $4F - character: 'O'   CHR$(79)
        {0,  124, 66,  66,  124, 64,  64,  0},   // $50 - character: 'P'   CHR$(80)
        {0,  60,  66,  66,  82,  74,  60,  0},   // $51 - character: 'Q'   CHR$(81)
        {0,  124, 66,  66,  124, 68,  66,  0},   // $52 - character: 'R'   CHR$(82)
        {0,  60,  64,  60,  2,   66,  60,  0},   // $53 - character: 'S'   CHR$(83)
        {0,  254, 16,  16,  16,  16,  16,  0},   // $54 - character: 'T'   CHR$(84)
        {0,  66,  66,  66,  66,  66,  60,  0},   // $55 - character: 'U'   CHR$(85)
        {0,  66,  66,  66,  66,  36,  24,  0},   // $56 - character: 'V'   CHR$(86)
        {0,  66,  66,  66,  66,  90,  36,  0},   // $57 - character: 'W'   CHR$(87)
        {0,  66,  36,  24,  24,  36,  66,  0},   // $58 - character: 'X'   CHR$(88)
        {0,  130, 68,  40,  16,  16,  16,  0},   // $59 - character: 'Y'   CHR$(89)
        {0,  126, 4,   8,   16,  32,  126, 0},   // $5A - character: 'Z'   CHR$(90)
        {0,  14,  8,   8,   8,   8,   14,  0},   // $5B - character: '['   CHR$(91)
        {0,  0,   64,  32,  16,  8,   4,   0},   // $5C - character: '\'   CHR$(92)
        {0,  112, 16,  16,  16,  16,  112, 0},   // $5D - character: ']'   CHR$(93)
        {0,  16,  56,  84,  16,  16,  16,  0},   // $5E - character: '^'   CHR$(94)
        {0,  0,   0,   0,   0,   0,   0,   255}, // $5F - character: '_'   CHR$(95)
        {0,  28,  34,  120, 32,  32,  126, 0},   // $60 - character: ' £ ' CHR$(96)
        {0,  0,   56,  4,   60,  68,  60,  0},   // $61 - character: 'a'   CHR$(97)
        {0,  32,  32,  60,  34,  34,  60,  0},   // $62 - character: 'b'   CHR$(98)
        {0,  0,   28,  32,  32,  32,  28,  0},   // $63 - character: 'c'   CHR$(99)
        {0,  4,   4,   60,  68,  68,  60,  0},   // $64 - character: 'd'   CHR$(100)
        {0,  0,   56,  68,  120, 64,  60,  0},   // $65 - character: 'e'   CHR$(101)
        {0,  12,  16,  24,  16,  16,  16,  0},   // $66 - character: 'f'   CHR$(102)
        {0,  0,   60,  68,  68,  60,  4,   56},  // $67 - character: 'g'   CHR$(103)
        {0,  64,  64,  120, 68,  68,  68,  0},   // $68 - character: 'h'   CHR$(104)
        {0,  16,  0,   48,  16,  16,  56,  0},   // $69 - character: 'i'   CHR$(105)
        {0,  4,   0,   4,   4,   4,   36,  24},  // $6A - character: 'j'   CHR$(106)
        {0,  32,  40,  48,  48,  40,  36,  0},   // $6B - character: 'k'   CHR$(107)
        {0,  16,  16,  16,  16,  16,  12,  0},   // $6C - character: 'l'   CHR$(108)
        {0,  0,   104, 84,  84,  84,  84,  0},   // $6D - character: 'm'   CHR$(109)
        {0,  0,   120, 68,  68,  68,  68,  0},   // $6E - character: 'n'   CHR$(110)
        {0,  0,   56,  68,  68,  68,  56,  0},   // $6F - character: 'o'   CHR$(111)
        {0,  0,   120, 68,  68,  120, 64,  64},  // $70 - character: 'p'   CHR$(112)
        {0,  0,   60,  68,  68,  60,  4,   6},   // $71 - character: 'q'   CHR$(113)
        {0,  0,   28,  32,  32,  32,  32,  0},   // $72 - character: 'r'   CHR$(114)
        {0,  0,   56,  64,  56,  4,   120, 0},   // $73 - character: 's'   CHR$(115)
        {0,  16,  56,  16,  16,  16,  12,  0},   // $74 - character: 't'   CHR$(116)
        {0,  0,   68,  68,  68,  68,  56,  0},   // $75 - character: 'u'   CHR$(117)
        {0,  0,   68,  68,  40,  40,  16,  0},   // $76 - character: 'v'   CHR$(118)
        {0,  0,   68,  84,  84,  84,  40,  0},   // $77 - character: 'w'   CHR$(119)
        {0,  0,   68,  40,  16,  40,  68,  0},   // $78 - character: 'x'   CHR$(120)
        {0,  0,   68,  68,  68,  60,  4,   56},  // $79 - character: 'y'   CHR$(121)
        {0,  0,   124, 8,   16,  32,  124, 0},   // $7A - character: 'z'   CHR$(122)
        {0,  14,  8,   48,  8,   8,   14,  0},   // $7B - character: '{'   CHR$(123)
        {0,  8,   8,   8,   8,   8,   8,   0},   // $7C - character: '|'   CHR$(124)
        {0,  112, 16,  12,  16,  16,  112, 0},   // $7D - character: '}'   CHR$(125)
        {0,  20,  40,  0,   0,   0,   0,   0},   // $7E - character: '~'   CHR$(126)
        {60, 66,  153, 161, 161, 153, 66,  60},  // $7F - character: '©'   CHR$(127)
};
