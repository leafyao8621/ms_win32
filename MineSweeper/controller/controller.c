#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include "../core/core.h"
#include "controller.h"

void controller_render(char reveal, HWND * hwnd) {
	unsigned char *bp = board;
	HWND *wp = hwnd;
	char buf[10];
	for (char i = 0; i < 8; ++i) {
		for (char j = 0; j < 8; ++j, ++bp, ++wp) {
			if (reveal) {
				if (*bp & MINE) {
					SetWindowText(*wp, "*");
				} else {
					snprintf(buf, 9, "%d", *bp & CNT);
					SetWindowText(*wp, buf);
				}
			} else {
				if (*bp & CHECK) {
					Button_SetState(*wp, 1);
					if (*bp & MINE) {
						SetWindowText(*wp, "*");
					} else {
						snprintf(buf, 9, "%d", *bp & CNT);
						SetWindowText(*wp, *bp & CNT ? buf : "");
					}
				} else if (*bp & MARK) {
					SetWindowText(*wp, "F");
				} else {
					Button_SetState(*wp, 0);
					SetWindowText(*wp, "");
				}
			}
		}
		putchar(10);
	}
}
