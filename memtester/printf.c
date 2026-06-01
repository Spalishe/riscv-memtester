#include <stdarg.h>

void uart_putc(char c);

static int my_strlen(const char *s) {
  int len = 0;
  while (s[len])
    len++;
  return len;
}

static int get_num_len(unsigned long long num, int base, int is_signed) {
  int len = 0;
  if (is_signed && (long long)num < 0) {
    len++;
    num = -(long long)num;
  }
  do {
    len++;
    num /= base;
  } while (num > 0);
  return len;
}

static void print_number(unsigned long long num, int base, int is_signed) {
  char buf[32];
  int i = 0;

  if (is_signed && (long long)num < 0) {
    uart_putc('-');
    num = -(long long)num;
  }

  do {
    unsigned long long rem = num % base;
    buf[i++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'a');
    num /= base;
  } while (num > 0 && i < 32);

  while (i > 0) {
    uart_putc(buf[--i]);
  }
}

int printf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  for (const char *p = format; *p != '\0'; p++) {
    if (*p != '%') {
      if (*p == '\n') {
        uart_putc('\r');
      }
      uart_putc(*p);
      continue;
    }

    p++;

    if (*p == '\0') {
      break;
    }

    int left_justify = 0;
    if (*p == '-') {
      left_justify = 1;
      p++;
    }

    int width = 0;
    while (*p >= '0' && *p <= '9') {
      width = width * 10 + (*p - '0');
      p++;
    }

    switch (*p) {
    case 'c': {
      char c = (char)va_arg(args, int);
      int padding = width - 1;
      if (!left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      uart_putc(c);
      if (left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      break;
    }
    case 's': {
      char *s = va_arg(args, char *);
      if (!s)
        s = "(null)";

      int len = my_strlen(s);
      int padding = width - len;

      if (!left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      while (*s) {
        if (*s == '\n')
          uart_putc('\r');
        uart_putc(*s++);
      }
      if (left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      break;
    }
    case 'd': {
      int val = va_arg(args, int);
      int len = get_num_len((unsigned long long)val, 10, 1);
      int padding = width - len;

      if (!left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      print_number((unsigned long long)val, 10, 1);
      if (left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      break;
    }
    case 'u': {
      unsigned int val = va_arg(args, unsigned int);
      int len = get_num_len((unsigned long long)val, 10, 0);
      int padding = width - len;

      if (!left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      print_number((unsigned long long)val, 10, 0);
      if (left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      break;
    }
    case 'x': {
      unsigned int val = va_arg(args, unsigned int);
      int len = get_num_len((unsigned long long)val, 16, 0);
      int padding = width - len;

      if (!left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      print_number((unsigned long long)val, 16, 0);
      if (left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      break;
    }
    case 'p': {
      void *ptr = va_arg(args, void *);
      int len = get_num_len((unsigned long long)ptr, 16, 0) + 2;
      int padding = width - len;

      if (!left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      uart_putc('0');
      uart_putc('x');
      print_number((unsigned long long)ptr, 16, 0);
      if (left_justify) {
        while (padding-- > 0)
          uart_putc(' ');
      }
      break;
    }
    case '%': {
      uart_putc('%');
      break;
    }
    default:
      uart_putc('%');
      uart_putc(*p);
      break;
    }
  }

  va_end(args);
  return 0;
}
