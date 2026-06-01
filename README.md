Very simple but very effective user-space memory tester.
Ported [memtester version 4](https://pyropus.ca./software/memtester/) to RISC-V Bare-metal.
It assumes you have UART8250 located on 0x10000000

# Building
There's not much essential, the only what you want to have is riscv64-linux-gnu toolchain(which can be easily changed in Makefile)
Output is located in memtester/memtester
