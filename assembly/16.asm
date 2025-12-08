.data
newline: .asciiz "\n"
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 16
    li $t0, 5
    sw $t0, -4($fp)
    lw $t0, -12($fp)
    lw $t1, -4($fp)
    add $t2, $t0, $t1
    sw $t2, -8($fp)
    lw $t0, -8($fp)
    sw $t0, -16($fp)
    lw $a0, -16($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 10
    syscall
