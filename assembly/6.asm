.data
newline: .asciiz "\n"
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 56
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $v0, 5
    syscall
    sw $v0, -8($fp)
    li $t0, 0
    sw $t0, -12($fp)
    lw $t0, -4($fp)
    lw $t1, -12($fp)
    sgt $t2, $t0, $t1
    sw $t2, -16($fp)
    lw $t0, -16($fp)
    beqz $t0, L0
    li $t0, 0
    sw $t0, -24($fp)
    lw $t0, -8($fp)
    lw $t1, -24($fp)
    sgt $t2, $t0, $t1
    sw $t2, -28($fp)
    lw $t0, -28($fp)
    beqz $t0, L2
    lw $t0, -4($fp)
    lw $t1, -8($fp)
    add $t2, $t0, $t1
    sw $t2, -36($fp)
    lw $t0, -36($fp)
    sw $t0, -40($fp)
    j L3
L2:
    lw $t0, -4($fp)
    lw $t1, -8($fp)
    sub $t2, $t0, $t1
    sw $t2, -48($fp)
    lw $t0, -48($fp)
    sw $t0, -40($fp)
L3:
    j L1
L0:
    li $t0, 0
    sw $t0, -56($fp)
    lw $t0, -56($fp)
    sw $t0, -40($fp)
L1:
    lw $a0, -40($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 10
    syscall
