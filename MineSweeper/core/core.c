#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "core.h"

unsigned char board[64];
static unsigned char stack[64];
static unsigned char *sp, *bp;

static unsigned char rem;
char state, mines;

void core_initialize(void) {
    memset(board, 0, 64);
    sp = stack;
    rem = 54;
    mines = 0;
    state = ONGOING;
    srand(time(0));
    char idx = 0;
    for (; mines < 10; ++mines) {
        for (idx = rand() & 0x3f; board[idx]; idx = rand() & 0x3f);
        board[idx] |= MINE;
    }
    bp = board;
    for (char i = 0; i < 8; ++i) {
        for (char j = 0; j < 8; ++j, ++bp) {
            if (!(*bp & MINE)) {
                for (signed char k = -1; k < 2; ++k) {
                    for (signed char l = -1; l < 2; ++l) {
                        if (i + k >= 0 && i + k < 8 &&
                            j + l >= 0 && j + l < 8) {
                            if (bp[(k << 3) + l] & MINE) {
                                ++*bp;
                            }
                        }
                    }
                }
            }
        }
    }
}

void core_mark(unsigned char r, unsigned char c) {
    unsigned char idx = (r << 3) + c;
    if (!(board[idx] & CHECK)) {
        mines += !(board[idx] & MARK) ? -1 : 1;
        board[idx] ^= MARK;
    }
}

void core_check(unsigned char r, unsigned char c) {
    if (state) {
        return;
    }
    unsigned char idx = (r << 3) + c;
    unsigned char cell = board[idx];
    unsigned char *cur = board + idx;
    if ((cell & MARK) || (cell & CHECK)) {
        return;
    }
    if (cell & MINE) {
        state = LOSS;
        return;
    }
    sp = stack;
    *cur |= CHECK;
    --rem;
    if (!(cell & CNT)) {
        *(sp++) = (r << 3) + c;
    }
    for (; sp != stack;) {
        idx = *(--sp);
        r = idx >> 3;
        c = idx & 0x7;
        cur = board + idx;
        cell = board[idx];
        if (!(cell & CNT)) {
            for (signed char i = -1; i < 2; ++i) {
                for (signed char j = -1; j < 2; ++j) {
                    if (r + i >= 0 && r + i < 8 &&
                        c + j >= 0 && c + j < 8 &&
                        !(cur[(i << 3) + j] & CHECK)) {
                        cur[(i << 3) + j] |= CHECK;
                        --rem;
                        if (!(cur[(i << 3) + j] & CNT)) {
                            *(sp++) = idx + (i << 3) + j;
                        }
                    }
                }
            }
        }
    }
    if (!rem) {
        state = WIN;
    }
}
