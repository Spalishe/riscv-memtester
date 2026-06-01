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
