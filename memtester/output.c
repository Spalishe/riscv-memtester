void uart_putc(char c);
void printf(const char *s, ...);

static int wheel_pos;

void out_test_start() { printf("           "); }

void out_test_setting(unsigned int j) {
  printf("\b\b\b\b\b\b\b\b\b\b\b");
  printf("setting %3u", j);
}

void out_test_testing(unsigned int j) {
  printf("\b\b\b\b\b\b\b\b\b\b\b");
  printf("testing %3u", j);
}

void out_test_end() {
  printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
}

void out_wheel_start() {
  uart_putc(' ');
  wheel_pos = 0;
}

void out_wheel_advance(unsigned int i) {
  static const unsigned int wheel_often = 2500;
  static const unsigned int n_chars = 4;
  char wheel_char[4] = {'-', '\\', '|', '/'};

  if (!(i % wheel_often)) {
    uart_putc('\b');
    uart_putc(wheel_char[++wheel_pos % n_chars]);
  }
}

void out_wheel_end() { printf("\b \b"); }
