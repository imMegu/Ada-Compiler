.data
newline: .asciiz "\n"
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 32
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $v0, 5
    syscall
    sw $v0, -8($fp)
    li $v0, 5
    syscall
    sw $v0, -12($fp)
    lw $t0, -4($fp)
    lw $t1, -8($fp)
    add $t2, $t0, $t1
    sw $t2, -16($fp)
    lw $t0, -16($fp)
    lw $t1, -12($fp)
    mul $t2, $t0, $t1
    sw $t2, -20($fp)
    li $t0, 10
    sw $t0, -24($fp)
    lw $t0, -20($fp)
    lw $t1, -24($fp)
    sub $t2, $t0, $t1
    sw $t2, -28($fp)
    lw $t0, -28($fp)
    sw $t0, -32($fp)
    lw $a0, -32($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 10
    syscall
