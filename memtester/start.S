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
