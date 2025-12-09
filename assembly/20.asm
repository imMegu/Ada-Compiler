.data
newline: .asciiz "\n"
str_4: .asciiz "Or Else result"
str_3: .asciiz "And Then result"
str_2: .asciiz "false xor false result"
str_1: .asciiz "true xor true result"
str_0: .asciiz "true xor false result"
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 40
    li $t0, 1
    sw $t0, -4($fp)
    lw $t0, -4($fp)
    sw $t0, -8($fp)
    li $t0, 0
    sw $t0, -12($fp)
    lw $t0, -12($fp)
    sw $t0, -16($fp)
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    xor $t2, $t0, $t1
    sw $t2, -20($fp)
    lw $t0, -20($fp)
    sw $t0, -24($fp)
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
    lw $t0, -8($fp)
    lw $t1, -8($fp)
    xor $t2, $t0, $t1
    sw $t2, -28($fp)
    lw $t0, -28($fp)
    sw $t0, -24($fp)
    la $a0, str_1
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
    lw $t0, -16($fp)
    lw $t1, -16($fp)
    xor $t2, $t0, $t1
    sw $t2, -32($fp)
    lw $t0, -32($fp)
    sw $t0, -24($fp)
    la $a0, str_2
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
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    and $t2, $t0, $t1
    sw $t2, -36($fp)
    lw $t0, -36($fp)
    sw $t0, -24($fp)
    la $a0, str_3
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
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    or $t2, $t0, $t1
    sw $t2, -40($fp)
    lw $t0, -40($fp)
    sw $t0, -24($fp)
    la $a0, str_4
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
    li $v0, 10
    syscall
