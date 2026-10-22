#include "printing.h"
#include "vga.h"

struct {
  dword x, y, w, h;
  struct {
    dword x, y;
  } cursor;
} window;

// A WINDOW_DO(statements) MACRO?????????
void window_clear() {
  window.cursor.x = 0;
  window.cursor.y = 0;
  
  for (dword x = 0; x < window.w; x++) {
    for (dword y = 0; y < window.h; y++) {
      dword loc_x = window.x + x;
      dword loc_y = window.y + y;
      
      VGA_CHAR_AT(loc_x, loc_y) = ' ';
    }
  }
}

void window_init(dword x, dword y, dword w, dword h) {
  window.x = x;
  window.y = y;
  window.w = w;
  window.h = h;
  
  window.cursor.x = 0;
  window.cursor.y = 0;
  
  for (dword x = 0; x < w; x++) {
    for (dword y = 0; y < h; y++) {
      dword loc_x = window.x + x;
      dword loc_y = window.y + y;
      
      vga_set_bg_col(BLACK, loc_x, loc_y);
      vga_set_fg_col(WHITE, loc_x, loc_y);
      VGA_CHAR_AT(loc_x, loc_y) = (byte)(x ^ y);
    }
  }
  window_clear();
}

void window_feed() {
  for (dword x = 0; x < window.w; x++) {
    for (dword y = 0; y < window.h - 1; y++) {
      dword loc_x = window.x + x;
      dword loc_y = window.y + y;
      VGA_CHAR_AT(loc_x, loc_y) = VGA_CHAR_AT(loc_x, loc_y + 1);
    }
  }
}


void putchar(char c) {
  // this could be a.... MACCRO !!!
  VGA_CHAR_AT(window.x + window.cursor.x, window.y + window.cursor.y) = c;

  if (++window.cursor.x == window.w) {
    window.cursor.x = 0;
    if (++window.cursor.y == window.h) {
      window_feed();
      window.cursor.y--;
    }
  }
}

// (can handle escape sequences)
void printchar(char c) {
    switch (c) {
    case '\n':
      if (++window.cursor.y == window.h) {
	window_feed();
	window.cursor.y--;
      }
      break;
      
    case '\r':
      window.cursor.x = 0;
      break;
      
    case '\b':
      if (--window.cursor.x < 0) {
	window.cursor.x++; // but i could also try to go previous maybe ???
      }
      break;

      /*
    case '\t': // uhh
      if (++window.cursor.x == window.w) {
	window.cursor.x--;
      }
      break;
      */

    default:
      putchar(c);
      break; 
    }
    
  
    if (window.cursor.y >= window.h) {
      window_feed();
      window.cursor.y--;
    }
}

void print(const char *string) {
  for (; *string != '\0'; string++) {
    printchar(*string);
  }
}

void printnum(dword number, dword base) {
  char digits[] = "0123456789ANCDEFGHIJKLMOPQRSTUVWXYZ";

  /*
  for (; number > 0; number /= base) {
    putchar(digits[number % base]);
  }
  */
  dword number_number = number;

  for (dword counting_number = number;
       counting_number > 0;
       counting_number /= base) {
    putchar(' ');
  }
  
    if (++window.cursor.x == window.w) {
      window.cursor.x = 0;
      if (++window.cursor.y == window.h) {
	window_feed();
	window.cursor.y--;
      }
    }
  
  for (; number > 0; number /= base) {
    printchar('\b');
    printchar('\b');
    putchar(digits[number % base]);
  }
  
  for (dword counting_number = number_number;
       counting_number > 0;
       counting_number /= base) {
    
    if (++window.cursor.x == window.w) {
      window.cursor.x = 0;
      if (++window.cursor.y == window.h) {
	window_feed();
	window.cursor.y--;
      }
    }
  }
}

void printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  for (; *fmt != '\0'; fmt++) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
      case 'd':
	printnum(va_arg(args, dword), 10);
        break;
      case 'x':
	printnum(va_arg(args, dword), 0x10);
        break;
      case 'c':
	printchar((char)va_arg(args, dword));
        break;
      case 's':
	print(va_arg(args, const char*));
        break;
      default:
        // lol
        // PANIC("unknown format specifier"); // : %c [string]
        printchar(*fmt);
      }
    } else {
      printchar(*fmt);
    }
  }
  
  va_end(args);
}
