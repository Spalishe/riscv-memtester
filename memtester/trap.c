volatile bool trap_triggered = false;
extern volatile bool mem_probing;
extern volatile bool mem_probe_stop;

void printf(const char *s, ...);

void sys_trap_handler(void) {
  unsigned long long mcause, mepc;
  __asm__ __volatile__("csrr %0, mcause" : "=r"(mcause));
  __asm__ __volatile__("csrr %0, mepc" : "=r"(mepc));

  trap_triggered = true;

  if (mcause == 5 || mcause == 7) {
    if (mem_probing)
      mem_probe_stop = true;
  } else {
    printf("\n[FATAL] Exception mcause: %d, mepc: 0x%p\n", (int)mcause,
           (void *)mepc);
    while (1)
      ;
  }

  mepc += 4;
  __asm__ __volatile__("csrw mepc, %0" ::"r"(mepc));
}
