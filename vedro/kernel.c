#include "general.h"
#include "vga.h"
#include "alloc.h"
#include "printing.h"

//#define DEBUG
#include "panic-and-assert.h"


color16 palette[] = {
  BLUE, LBLUE, CYAN, LCYAN, GREEN, LGREEN, MAGENTA, LMAGENTA
};

void draw_wallpaper() {
  for (dword x = 0; x < VGA_TEXT_WIDTH; x++) {
    for (dword y = 0; y < VGA_TEXT_HEIGHT; y++) {
      dword seed = y / 2 + ((y ^ 17) % 2) + (x % 2);
      vga_set_bg_col(palette[seed % 8], x, y);
      VGA_CHAR_AT(x, y) = ' ';
      
      if ((x ^ y) % 3 == 0) {
	vga_set_bg_col(palette[(seed + 7) % 8], x, y);
        vga_set_fg_col(palette[(seed + 1) % 8], x, y);
	VGA_CHAR_AT(x, y) = 0xB1;
      }
    }
  }
}

void kernel_entry() {
  draw_wallpaper();

  window_init(VGA_TEXT_WIDTH / 4,
	      VGA_TEXT_HEIGHT / 4,
	      VGA_TEXT_WIDTH / 2,
	      VGA_TEXT_HEIGHT / 2);

  printf("Helo\blo \n\r World! %c %s\n\r %x, %d", '\2', "MOM\nMA", 0xDEAD, 12345);
  //asm("hlt");
  
  for (dword i = 1;; i++) {
    dword size      = i * 100;
    dword alignment = i * 2;
      
    void* obj = alloc_forever(size, alignment);

    printf("OBJ \n\r"
	   " - at %x\n\r"
	   " - of size %d\n\r"
	   " - aliged to %d\n\n\r",
	   obj, size, alignment);

    // HOW DO I SLOW THIS BIRD DOWN
  }
    
  asm("hlt");
}
