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
