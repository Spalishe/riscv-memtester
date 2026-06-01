#include <stddef.h>

int test_stuck_address(unsigned long volatile *bufa, unsigned long count);
int test_random_value(unsigned long volatile *bufa,
                      unsigned long volatile *bufb, unsigned long count);
int test_xor_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count);
int test_sub_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count);
int test_mul_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count);
int test_div_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count);
int test_or_comparison(unsigned long volatile *bufa,
                       unsigned long volatile *bufb, unsigned long count);
int test_and_comparison(unsigned long volatile *bufa,
                        unsigned long volatile *bufb, unsigned long count);
int test_seqinc_comparison(unsigned long volatile *bufa,
                           unsigned long volatile *bufb, unsigned long count);
int test_solidbits_comparison(unsigned long volatile *bufa,
                              unsigned long volatile *bufb,
                              unsigned long count);
int test_checkerboard_comparison(unsigned long volatile *bufa,
                                 unsigned long volatile *bufb,
                                 unsigned long count);
int test_blockseq_comparison(unsigned long volatile *bufa,
                             unsigned long volatile *bufb, unsigned long count);
int test_walkbits0_comparison(unsigned long volatile *bufa,
                              unsigned long volatile *bufb,
                              unsigned long count);
int test_walkbits1_comparison(unsigned long volatile *bufa,
                              unsigned long volatile *bufb,
                              unsigned long count);
int test_bitspread_comparison(unsigned long volatile *bufa,
                              unsigned long volatile *bufb,
                              unsigned long count);
int test_bitflip_comparison(unsigned long volatile *bufa,
                            unsigned long volatile *bufb, unsigned long count);

struct test {
  char *name;
  int (*fp)(unsigned long volatile *, unsigned long volatile *, unsigned long);
};

struct test tests[] = {{"Random Value", test_random_value},
                       {"Compare XOR", test_xor_comparison},
                       {"Compare SUB", test_sub_comparison},
                       {"Compare MUL", test_mul_comparison},
                       {"Compare DIV", test_div_comparison},
                       {"Compare OR", test_or_comparison},
                       {"Compare AND", test_and_comparison},
                       {"Sequential Increment", test_seqinc_comparison},
                       {"Solid Bits", test_solidbits_comparison},
                       {"Block Sequential", test_blockseq_comparison},
                       {"Checkerboard", test_checkerboard_comparison},
                       {"Bit Spread", test_bitspread_comparison},
                       {"Bit Flip", test_bitflip_comparison},
                       {"Walking Ones", test_walkbits1_comparison},
                       {"Walking Zeroes", test_walkbits0_comparison},
                       {NULL, NULL}};
