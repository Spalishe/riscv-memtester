OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY {
 RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 8M
}

SECTIONS
{
  . = ORIGIN(RAM);
  _memory_start = .;

  .text : {
    *(.text.init)
    *(.text .text.*)
  } > RAM

  .rodata : { *(.rodata .rodata.*) } > RAM
  .data   : { *(.data .data.*) } > RAM
  
  .bss : {
    *(.bss .bss.*)
    *(COMMON)
  } > RAM

  . = ALIGN(16);
  .stack (NOLOAD) : ALIGN(16)
  {
    _stack_bottom = .;
        KEEP(*(.stack))
    . += 16K;
    
    _stack_top = .;
  } > RAM
  . = ALIGN(4096);
  _free_memory_start = .;

}
