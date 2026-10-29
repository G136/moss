#include "printing.h"
#include "vga.h"

struct {
  dword x, y, w, h;
  struct {
    dword x, y;
  } cursor;
} window;

#define WINDOW_DO(STATEMENTS)			\
  for (window.cursor.x = 0;			\
       window.cursor.x < window.w;		\
       window.cursor.x++) {			\
    for (window.cursor.y = 0;			\
	 window.cursor.y < window.h;		\
	 window.cursor.y++) { STATEMENTS }	\
  }

#define WINDOW_CURSOR_VGA_TEXT_X window.x + window.cursor.x
#define WINDOW_CURSOR_VGA_TEXT_Y window.y + window.cursor.y

#define WINDOW_CHAR_AT_CURSOR \
  VGA_CHAR_AT(WINDOW_CURSOR_VGA_TEXT_X, WINDOW_CURSOR_VGA_TEXT_Y)

void window_clear() {
  WINDOW_DO(WINDOW_CHAR_AT_CURSOR = ' ';)
  window.cursor.x = 0;
  window.cursor.y = 0;
}

void window_init(dword x, dword y, dword w, dword h) {
  window.x = x;
  window.y = y;
  window.w = w;
  window.h = h;

  WINDOW_DO(vga_set_bg_col(BLACK,
			   WINDOW_CURSOR_VGA_TEXT_X,
			   WINDOW_CURSOR_VGA_TEXT_Y);
	    vga_set_fg_col(WHITE,
			   WINDOW_CURSOR_VGA_TEXT_X,
			   WINDOW_CURSOR_VGA_TEXT_Y);
	    WINDOW_CHAR_AT_CURSOR =
	    (byte)(WINDOW_CURSOR_VGA_TEXT_X ^ WINDOW_CURSOR_VGA_TEXT_Y);)
  window_clear();
}

void window_feed() {
  WINDOW_DO(
      if (window.cursor.y == window.h - 1) {
	WINDOW_CHAR_AT_CURSOR = ' ';
      } else {
	VGA_CHAR_AT(WINDOW_CURSOR_VGA_TEXT_X, WINDOW_CURSOR_VGA_TEXT_Y) =
	  VGA_CHAR_AT(WINDOW_CURSOR_VGA_TEXT_X, WINDOW_CURSOR_VGA_TEXT_Y + 1);
      })

  window.cursor.x = 0;
}


void putchar(char c) {
  WINDOW_CHAR_AT_CURSOR = c;

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
