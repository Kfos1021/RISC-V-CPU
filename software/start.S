    .section .text
    .globl _start

_start:
    # Initialize the stack pointer.
    la sp, _stack_top

    # Call main().
    call main

# If main ever returns, loop forever.
hang:
    j hang