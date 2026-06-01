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

.section .text.init
.global _start
.global sys_trap_handler

_start:
	csrw mie, zero
	csrw satp, zero

	# We need only hart 0
	csrr x31, mhartid
	bnez x31, _loop

	la sp, _stack_top

	la t0, trap_vector
    csrw mtvec, t0

	call main

_loop:
	wfi
	j _loop

.align 4
trap_vector:
    call sys_trap_handler
    mret
