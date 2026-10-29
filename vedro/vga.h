#define VGA_BUFFER_ADDRESS 0xB8000
#define VGA_TEXT_WIDTH   80
#define VGA_TEXT_HEIGHT  25

#define VGA__POS_TO_ADDR(x, y) \
  VGA_BUFFER_ADDRESS + ((x) + VGA_TEXT_WIDTH * (y)) * 2

#define VGA_CHAR_AT(x, y) *((byte*) VGA__POS_TO_ADDR((x), (y)))
#define VGA_ATTR_AT(x, y) *((byte*) VGA__POS_TO_ADDR((x), (y)) + 1)


#pragma once

typedef enum {
  BLACK,  BLUE,  GREEN,  CYAN,  RED,  MAGENTA,  BROWN, LGRAY,
  DGRAY, LBLUE, LGREEN, LCYAN, LRED, LMAGENTA, YELLOW, WHITE
} color16;
// something something the bright 8
// https://en.wikipedia.org/wiki/VGA_text_mode
// https://wiki.osdev.org/Text_UI


#include "general.h"

void vga_set_fg_col(color16 col, dword x, dword y);
void vga_set_bg_col(color16 col, dword x, dword y);
