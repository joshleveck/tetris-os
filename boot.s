.section ".text.boot"

.global _start

_start:
    // Check PID is 0, or hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // Not main core, infinite loop
1:  wfe
    b       1b
2:  // Main core
    ldr     x1, =_start
    mov     sp, x1

    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b

4:  bl      main
    b       1b
