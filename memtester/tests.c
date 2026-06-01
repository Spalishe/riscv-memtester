/*
 * memtester version 4
 *
 * Very simple but very effective user-space memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2004-2020 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Copyright (C) 2026 Spalishe <ynororah@gmail.com>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 * See the file LICENSE for details.
 * Code was modified
 */

#include "sizes.h"
void printf(const char *s, ...);
void out_test_end();
void out_test_testing(unsigned int j);
void out_test_setting(unsigned int j);
void out_test_start();
void out_wheel_start();
void out_wheel_advance(unsigned int i);
void out_wheel_end();

int compare_regions(unsigned long volatile *bufa, unsigned long volatile *bufb,
                    unsigned long count) {
  int r = 0;
  unsigned long i;
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;

  for (i = 0; i < count; i++, p1++, p2++) {
    if (*p1 != *p2) {
      printf("FAILURE: 0x%x != 0x%x at offset 0x%x.\n", *p1, *p2,
             (i * sizeof(unsigned long)));
      /* printf("Skipping to next test..."); */
      r = -1;
    }
  }
  return r;
}

int test_stuck_address(unsigned long volatile *bufa, unsigned long count) {
  unsigned long volatile *p1;
  unsigned int j;
  unsigned long i;

  out_test_start();
  for (j = 0; j < 16; j++) {
    p1 = bufa;
    out_test_setting(j);

    unsigned int j_even = (j & 1);

    for (i = 0; i < count; i++) {
      unsigned long val =
          ((i & 1) == j_even) ? (unsigned long)p1 : ~((unsigned long)p1);
      *p1 = val;
      p1++;
    }

    out_test_testing(j);
    p1 = bufa;
    for (i = 0; i < count; i++, p1++) {
      unsigned long expected =
          ((i & 1) == j_even) ? (unsigned long)p1 : ~((unsigned long)p1);
      if (*p1 != expected) {
        printf("FAILURE: possible bad address line at offset 0x%lx.\n",
               i * sizeof(unsigned long));
        printf("Skipping to next test...\n");
        return -1;
      }
    }
  }
  out_test_end();
  return 0;
}

int test_random_value(unsigned long volatile *bufa,
                      unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned long i;

  out_wheel_start();
  for (i = 0; i < count; i++) {
    *p1++ = *p2++ = rand_ul();
    out_wheel_advance(i);
  }
  out_wheel_end();
  return compare_regions(bufa, bufb, count);
}

int test_xor_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned long i;
  unsigned long q = rand_ul();

  for (i = 0; i < count; i++) {
    *p1++ ^= q;
    *p2++ ^= q;
  }
  return compare_regions(bufa, bufb, count);
}

int test_sub_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned long i;
  unsigned long q = rand_ul();

  for (i = 0; i < count; i++) {
    *p1++ -= q;
    *p2++ -= q;
  }
  return compare_regions(bufa, bufb, count);
}

int test_mul_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned long i;
  unsigned long q = rand_ul();

  for (i = 0; i < count; i++) {
    *p1++ *= q;
    *p2++ *= q;
  }
  return compare_regions(bufa, bufb, count);
}

int test_div_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned long i;
  unsigned long q = rand_ul();

  for (i = 0; i < count; i++) {
    if (!q) {
      q++;
    }
    *p1++ /= q;
    *p2++ /= q;
  }
  return compare_regions(bufa, bufb, count);
}

int test_or_comparison(unsigned long volatile *bufa,
                       unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned long i;
  unsigned long q = rand_ul();

  for (i = 0; i < count; i++) {
    *p1++ |= q;
    *p2++ |= q;
  }
  return compare_regions(bufa, bufb, count);
}

int test_and_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned long i;
  unsigned long q = rand_ul();

  for (i = 0; i < count; i++) {
    *p1++ &= q;
    *p2++ &= q;
  }
  return compare_regions(bufa, bufb, count);
}

int test_seqinc_comparison(unsigned long volatile *bufa,
                           unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned long i;
  unsigned long q = rand_ul();

  for (i = 0; i < count; i++) {
    *p1++ = *p2++ = (i + q);
  }
  return compare_regions(bufa, bufb, count);
}

int test_solidbits_comparison(unsigned long volatile *bufa,
                              unsigned long volatile *bufb,
                              unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned int j;
  unsigned long q;
  unsigned long i;

  out_test_start();
  for (j = 0; j < 64; j++) {
    q = (j % 2) == 0 ? UL_ONEBITS : 0;
    out_test_setting(j);
    p1 = (unsigned long volatile *)bufa;
    p2 = (unsigned long volatile *)bufb;
    for (i = 0; i < count; i++) {
      *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
    }
    out_test_testing(j);
    if (compare_regions(bufa, bufb, count)) {
      return -1;
    }
  }
  out_test_end();
  return 0;
}

int test_checkerboard_comparison(unsigned long volatile *bufa,
                                 unsigned long volatile *bufb,
                                 unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned int j;
  unsigned long q;
  unsigned long i;

  out_test_start();
  for (j = 0; j < 64; j++) {
    q = (j % 2) == 0 ? CHECKERBOARD1 : CHECKERBOARD2;
    out_test_setting(j);
    p1 = (unsigned long volatile *)bufa;
    p2 = (unsigned long volatile *)bufb;
    for (i = 0; i < count; i++) {
      *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
    }
    out_test_testing(j);
    if (compare_regions(bufa, bufb, count)) {
      return -1;
    }
  }
  out_test_end();
  return 0;
}

int test_blockseq_comparison(unsigned long volatile *bufa,
                             unsigned long volatile *bufb,
                             unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned int j;
  unsigned long i;

  out_test_start();
  for (j = 0; j < 256; j++) {
    p1 = (unsigned long volatile *)bufa;
    p2 = (unsigned long volatile *)bufb;
    out_test_setting(j);
    for (i = 0; i < count; i++) {
      *p1++ = *p2++ = (unsigned long)UL_BYTE(j);
    }
    out_test_testing(j);
    if (compare_regions(bufa, bufb, count)) {
      return -1;
    }
  }
  out_test_end();
  return 0;
}

int test_walkbits0_comparison(unsigned long volatile *bufa,
                              unsigned long volatile *bufb,
                              unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned int j;
  unsigned long i;

  out_test_start();
  for (j = 0; j < UL_LEN * 2; j++) {
    p1 = (unsigned long volatile *)bufa;
    p2 = (unsigned long volatile *)bufb;
    out_test_setting(j);
    for (i = 0; i < count; i++) {
      if (j < UL_LEN) { /* Walk it up. */
        *p1++ = *p2++ = 1ULL << j;
      } else { /* Walk it back down. */
        *p1++ = *p2++ = 1ULL << (UL_LEN * 2 - j - 1);
      }
    }
    out_test_testing(j);
    if (compare_regions(bufa, bufb, count)) {
      return -1;
    }
  }
  out_test_end();
  return 0;
}

int test_walkbits1_comparison(unsigned long volatile *bufa,
                              unsigned long volatile *bufb,
                              unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned int j;
  unsigned long i;

  out_test_start();
  for (j = 0; j < UL_LEN * 2; j++) {
    p1 = (unsigned long volatile *)bufa;
    p2 = (unsigned long volatile *)bufb;
    out_test_setting(j);
    for (i = 0; i < count; i++) {
      if (j < UL_LEN) { /* Walk it up. */
        *p1++ = *p2++ = UL_ONEBITS ^ (1ULL << j);
      } else { /* Walk it back down. */
        *p1++ = *p2++ = UL_ONEBITS ^ (1ULL << (UL_LEN * 2 - j - 1));
      }
    }
    out_test_testing(j);
    if (compare_regions(bufa, bufb, count)) {
      return -1;
    }
  }
  out_test_end();
  return 0;
}

int test_bitspread_comparison(unsigned long volatile *bufa,
                              unsigned long volatile *bufb,
                              unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned int j;
  unsigned long i;

  out_test_start();
  for (j = 0; j < UL_LEN * 2; j++) {
    p1 = (unsigned long volatile *)bufa;
    p2 = (unsigned long volatile *)bufb;
    out_test_setting(j);
    for (i = 0; i < count; i++) {
      if (j < UL_LEN) { /* Walk it up. */
        *p1++ = *p2++ = (i % 2 == 0)
                            ? (1ULL << j) | (1ULL << (j + 2))
                            : UL_ONEBITS ^ ((1ULL << j) | (1ULL << (j + 2)));
      } else { /* Walk it back down. */
        *p1++ = *p2++ = (i % 2 == 0)
                            ? (1ULL << (UL_LEN * 2 - 1 - j)) |
                                  (1ULL << (UL_LEN * 2 + 1 - j))
                            : UL_ONEBITS ^ (1ULL << (UL_LEN * 2 - 1 - j) |
                                            (1ULL << (UL_LEN * 2 + 1 - j)));
      }
    }
    out_test_testing(j);
    if (compare_regions(bufa, bufb, count)) {
      return -1;
    }
  }
  out_test_end();
  return 0;
}

int test_bitflip_comparison(unsigned long volatile *bufa,
                            unsigned long volatile *bufb, unsigned long count) {
  unsigned long volatile *p1 = bufa;
  unsigned long volatile *p2 = bufb;
  unsigned int j, k;
  unsigned long q;
  unsigned long i;

  out_test_start();
  for (k = 0; k < UL_LEN; k++) {
    q = 1ULL << k;
    for (j = 0; j < 8; j++) {
      q = ~q;
      out_test_setting(k * 8 + j);
      p1 = (unsigned long volatile *)bufa;
      p2 = (unsigned long volatile *)bufb;
      for (i = 0; i < count; i++) {
        *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
      }
      out_test_testing(k * 8 + j);
      if (compare_regions(bufa, bufb, count)) {
        return -1;
      }
    }
  }
  out_test_end();
  return 0;
}
