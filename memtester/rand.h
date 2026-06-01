static unsigned long long next_random = 1;

void srand(unsigned int seed) { next_random = seed; }

int rand(void) {
  next_random = next_random * 1103515245ULL + 12345ULL;

  return (int)((next_random / 65536ULL) % 32768ULL);
}

unsigned long long rand64(void) {
  if (next_random == 0) {
    next_random = 1;
  }
  next_random ^= next_random << 13;
  next_random ^= next_random >> 7;
  next_random ^= next_random << 17;
  return next_random;
}
