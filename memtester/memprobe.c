void printf(const char *s, ...);

extern volatile bool trap_triggered;
volatile bool mem_probing = false;
volatile bool mem_probe_stop = false;
unsigned long probe_memory(unsigned long start_addr, unsigned long start_offs) {
  unsigned char *current_addr = (unsigned char *)(start_addr + start_offs);
  unsigned long mem_size = 0;
  mem_probing = true;
  mem_probe_stop = false;

  printf("Probing memory with step of 1 KB...\n");

  while (1) {
    trap_triggered = 0;

    volatile unsigned int value = *(volatile unsigned int *)current_addr;
    (void)value;

    if (mem_probe_stop) {
      mem_size = (unsigned long)(current_addr -
                                 (unsigned char *)(start_addr + start_offs));
      break;
    }

    current_addr += 1024;
  }

  return mem_size;
}
