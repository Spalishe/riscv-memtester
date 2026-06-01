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
