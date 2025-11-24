.thumb
.align


@function prototypes

.global MSSFatigueGetter
.type MSSFatigueGetter, %function

.global MSSEnduranceGetter
.type MSSEnduranceGetter, %function


MSSFatigueGetter: 
@return fatigue value in r0
@r8 = current unit data

push {r4-r7,r14}

@check if this unit can fatigue
mov r0,r8
ldr r1,=CanUnitFatigue
mov r14,r1
.short 0xF800
cmp r0,#0 
beq RetFalse @if not, return -1

@otherwise, get fatigue
mov r0,r8
add r0,#0x3B
ldrb r0,[r0]
b GoBack

RetFalse:
mov r0,#0xFF

GoBack:
pop {r4-r7}
pop {r1}
bx r1

.ltorg
.align

MSSEnduranceGetter:

push {r4-r7,r14}

@check if this unit can fatigue
mov r0,r8
ldr r1,=CanUnitFatigue
mov r14,r1
.short 0xF800
cmp r0,#0 
beq RetFalse @if not, return -1

mov r0,r8
@otherwise, get endurance
ldrb r2,[r0,#0x12] @HP
lsr r2,r2,#1 @halving hp
ldr r3,[r0,#0x04]
ldrb r3,[r3,#0x11] @class con
add r2,r3 @add class con to HP/2
ldr r3,[r0,#0x00]
add r3,#0x13 @char con
mov r7,#0
ldsb r3,[r3,r7]
add r2,r3
ldrb r3,[r0,#0x1A] @bonus con
add r2,r3 @final endurance
mov r0,r2
b GoBack

.ltorg
.align
