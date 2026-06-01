#define UART_BASE 0x10000000

#define UART_THR ((volatile unsigned char *)(UART_BASE + 0))
#define UART_RBR ((volatile unsigned char *)(UART_BASE + 0))
#define UART_LSR ((volatile unsigned char *)(UART_BASE + 5))

#define LSR_TX_IDLE (1 << 5)
#define LSR_RX_READY (1 << 0)

void uart_init() {
  volatile unsigned char *lcr = (volatile unsigned char *)(UART_BASE + 3);
  *lcr = 0x03;
}

void uart_putc(char c) {
  // Wait until UART will ready to receive char
  while ((*UART_LSR & LSR_TX_IDLE) == 0)
    ;

  *UART_THR = c;
}

void uart_puts(const char *s) {
  while (*s) {
    if (*s == '\n') {
      uart_putc('\r');
    }
    uart_putc(*s++);
  }
}

char uart_getc(void) {
  // Wait until we receive byte
  while ((*UART_LSR & LSR_RX_READY) == 0)
    ;
  return *UART_RBR;
}

void uart_gets(char *s, unsigned int size) {
  if (size == 0)
    return;

  unsigned int i = 0;
  while (i < (size - 1)) {
    char c = uart_getc();
    if (c == '\r' || c == '\n') {
      break;
    }
    if (c == 127 && i > 0) {
      s[i] = 0x32;
      uart_putc('\b');
      uart_putc(32);
      uart_putc('\b');
      i--;
      continue;
    }
    uart_putc(c);

    s[i] = c;
    i++;
  }

  s[i] = '\0';
  uart_putc('\r');
  uart_putc('\n');
}
