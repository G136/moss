#include "general.h"
#include "vga.h"

void vga_set_fg_col(color16 col, dword x, dword y) {
  byte attr = VGA_ATTR_AT(x, y);

  attr &= 0xF0;
  attr |= col * 0x01;
  
  VGA_ATTR_AT(x, y) = attr;
}

// note how the last attr bit can be a blink bit actually
void vga_set_bg_col(color16 col, dword x, dword y) {
  byte attr = VGA_ATTR_AT(x, y);

  attr &= 0x0F;
  attr |= col * 0x10;
  
  VGA_ATTR_AT(x, y) = attr;
}

/*
//#include "movinit.h"  
void vga_clear_screen() {
  //memzero((void*) VGA_BUFFER_ADDRESS, VGA_TEXT_WIDTH * 2 * VGA_TEXT_HEIGHT);
  for (dword x = 0; x < VGA_TEXT_WIDTH; x++) {
    for (dword y = 0; y < VGA_TEXT_HEIGHT; y++) {
      VGA_CHAR_AT(x, y) = ' ';
    }
  }
}
*/
