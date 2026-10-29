#include <stdarg.h>
// ew

#include "general.h"


void window_clear();

void window_init(dword x, dword y, dword w, dword h);

void window_feed();


void putchar(char c);

void printchar(char c);

void print(const char *string);

void printnum(dword number, dword base);

void printf(const char* fmt, ...);
