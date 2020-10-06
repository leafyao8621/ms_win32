#ifndef _CORE_H_
#define _CORE_H_

#define ONGOING 0
#define WIN 1
#define LOSS 2

#define MINE 0x80
#define CHECK 0x40
#define MARK 0x20
#define CNT 0xf

#include <stdio.h>

extern unsigned char board[64];
extern char state, mines;

void core_initialize(void);
void core_check(unsigned char r, unsigned char c);
void core_mark(unsigned char r, unsigned char c);

#endif
