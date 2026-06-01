

void uart_init();
void uart_gets(char *s, unsigned int size);
void printf(const char *s, ...);
unsigned long probe_memory(unsigned long start_addr, unsigned long start_offs);

#include "tests.h"

extern unsigned char _free_memory_start;
extern unsigned char _memory_start;

void *memset(void *dest, int c, unsigned long count) {
  unsigned char *ptr = (unsigned char *)dest;
  while (count--) {
    *ptr++ = (unsigned char)c;
  }
  return dest;
}

int stoi(const char *s) {
  int res = 0;
  int sign = 1;
  while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') {
    s++;
  }

  if (*s == '-') {
    sign = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  while (*s >= '0' && *s <= '9') {
    res = res * 10 + (*s - '0');
    s++;
  }

  return res * sign;
}

int main() {
  int exit_code = 0;
  uart_init();

  unsigned long free_mem = (unsigned long)&_free_memory_start;
  unsigned long mem_start = (unsigned long)&_memory_start;
  printf("Memtester\n");
  printf("Region start: 0x%x\n", (void *)free_mem);

  printf("Region offset: 0x%x\n", free_mem - mem_start);
  unsigned long size = probe_memory(mem_start, free_mem - mem_start);
  printf("Region size: 0x%x\n", size);

  char str[11];
  printf("Enter loop count: ");
  uart_gets(str, 11);

  int loops = stoi(str);
  if (loops == 0) {
    printf("setting loops count to 2\n");
    loops = 2;
  }
  for (int i = 1; i <= loops; i++) {
    printf("Loop %d:\n", i);
    printf("  %-20s: ", "Stuck Address");
    if (!test_stuck_address((unsigned long *)free_mem,
                            size / sizeof(unsigned long))) {
      printf("ok\n");
    } else {
      exit_code |= 0x2; // EXIT_FAIL_ADDRESSLINES
    }

    unsigned long halflen = size / 2;
    unsigned long count = halflen / sizeof(unsigned long);
    unsigned long volatile *bufa = (unsigned long volatile *)free_mem;
    unsigned long volatile *bufb =
        (unsigned long volatile *)((unsigned long)free_mem + halflen);

    for (i = 0;; i++) {
      if (!tests[i].name)
        break;
      printf("  %-20s: ", tests[i].name);
      if (!tests[i].fp(bufa, bufb, count)) {
        printf("ok\n");
      } else {
        exit_code |= 0x4; // EXIT_FAIL_OTHERTEST
      }
      memset((void *)free_mem, 255, size);
    }
    printf("\n");
  }
  return exit_code;
}
