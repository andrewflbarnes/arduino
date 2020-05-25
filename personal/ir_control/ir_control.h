#ifndef _IR_H
#define _IR_H

#define IR_POWER (0xFFA25D)
#define IR_FUNC_STOP (0xFFE21D)
#define IR_VOL_ADD (0xFF629D)
#define IR_REWIND (0xFF22DD)
#define IR_PLAY_PAUSE (0xFF02FD)
#define IR_FAST_FORWARD (0xFFC23D)
#define IR_DOWN (0xFFE01F)
#define IR_VOL_DE (0xFFA857)
#define IR_UP (0xFF906F)
#define IR_EQ (0xFF9867)
#define IR_ST_REPT (0xFFB04F)
#define IR_0 (0xFF6897)
#define IR_1 (0xFF30CF)
#define IR_2 (0xFF18E7)
#define IR_3 (0xFF7A85)
#define IR_4 (0xFF10EF)
#define IR_5 (0xFF38C7)
#define IR_6 (0xFF5AA5)
#define IR_7 (0xFF42BD)
#define IR_8 (0xFF4AB5)
#define IR_9 (0xFF52AD)
#define IR_REPEAT (0xFFFFFFFF)

const int IR_COUNT = 22;
const unsigned long IR_VALS[IR_COUNT] = {
  IR_POWER,
  IR_FUNC_STOP,
  IR_VOL_ADD,
  IR_REWIND,
  IR_PLAY_PAUSE,
  IR_FAST_FORWARD,
  IR_DOWN,
  IR_VOL_DE,
  IR_UP,
  IR_EQ,
  IR_ST_REPT,
  IR_0,
  IR_1,
  IR_2,
  IR_3,
  IR_4,
  IR_5,
  IR_6,
  IR_7,
  IR_8,
  IR_9,
  IR_REPEAT
};

const String IR_DESCS[IR_COUNT] = {
  "POWER",
  "FUNC/STOP",
  "VOL+",
  "REWIND",
  "PLAY/PAUSE",
  "FAST FORWARD",
  "DOWN",
  "VOL-",
  "UP",
  "EQ",
  "ST/REPT",
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "_REPEAT"
};
#endif
