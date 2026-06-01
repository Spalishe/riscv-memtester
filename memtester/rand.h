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
