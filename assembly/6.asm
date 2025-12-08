.data
newline: .asciiz "\n"
str_3: .asciiz "=>This is the largest we can compute due to (signed) integer limits."
str_2: .asciiz "Fib(47) is 2971215073"
str_1: .asciiz "2^31 is    2147483648"
str_0: .asciiz "Should be 1836311903"
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 104
    li $t0, 46
    sw $t0, -4($fp)
    lw $t0, -4($fp)
    sw $t0, -8($fp)
    li $t0, 0
    sw $t0, -12($fp)
    lw $t0, -12($fp)
    sw $t0, -16($fp)
    li $t0, 1
    sw $t0, -20($fp)
    lw $t0, -20($fp)
    sw $t0, -24($fp)
    li $t0, 2
    sw $t0, -28($fp)
    lw $t0, -28($fp)
    sw $t0, -32($fp)
    li $t0, 0
    sw $t0, -36($fp)
    lw $t0, -36($fp)
    sw $t0, -40($fp)
    li $t0, 1
    sw $t0, -44($fp)
    lw $t0, -8($fp)
    lw $t1, -44($fp)
    sgt $t2, $t0, $t1
    sw $t2, -48($fp)
    lw $t0, -48($fp)
    beqz $t0, L0
L2:
    lw $t0, -32($fp)
    lw $t1, -8($fp)
    sle $t2, $t0, $t1
    sw $t2, -60($fp)
    lw $t0, -60($fp)
    beqz $t0, L3
    lw $t0, -16($fp)
    lw $t1, -24($fp)
    add $t2, $t0, $t1
    sw $t2, -68($fp)
    lw $t0, -68($fp)
    sw $t0, -40($fp)
    lw $t0, -24($fp)
    sw $t0, -16($fp)
    lw $t0, -40($fp)
    sw $t0, -24($fp)
    li $t0, 1
    sw $t0, -72($fp)
    lw $t0, -32($fp)
    lw $t1, -72($fp)
    add $t2, $t0, $t1
    sw $t2, -76($fp)
    lw $t0, -76($fp)
    sw $t0, -32($fp)
    j L2
L3:
    j L1
L0:
    li $t0, 1
    sw $t0, -84($fp)
    lw $t0, -8($fp)
    lw $t1, -84($fp)
    seq $t2, $t0, $t1
    sw $t2, -88($fp)
    lw $t0, -88($fp)
    beqz $t0, L4
    li $t0, 1
    sw $t0, -96($fp)
    lw $t0, -96($fp)
    sw $t0, -24($fp)
    j L5
L4:
    li $t0, 0
    sw $t0, -104($fp)
    lw $t0, -104($fp)
    sw $t0, -24($fp)
L5:
L1:
    la $a0, str_0
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_1
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_2
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_3
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 10
    syscall
