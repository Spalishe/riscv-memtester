/*Copyright (C) 2026  Spalishe
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
