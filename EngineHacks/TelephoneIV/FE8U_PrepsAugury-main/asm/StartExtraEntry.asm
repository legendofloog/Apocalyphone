.macro blh to, reg=r3
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
.endm

@ Starts the proc related to the extra entry.
@ Replaces stat screen proc start in 0x96350 switch.
.thumb

blh HealAll


ldr   r0, =0x8096384
mov   r15, r0
GOTO_R2:
bx    r2
