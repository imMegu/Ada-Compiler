.data
newline: .asciiz "\n"
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 100
    li $t0, 10
    sw $t0, -4($fp)
    li $t0, 5
    sw $t0, -8($fp)
    lw $t0, -4($fp)
    lw $t1, -8($fp)
    add $t2, $t0, $t1
    sw $t2, -12($fp)
    lw $t0, -12($fp)
    sw $t0, -16($fp)
    li $t0, 20
    sw $t0, -20($fp)
    li $t0, 8
    sw $t0, -24($fp)
    lw $t0, -20($fp)
    lw $t1, -24($fp)
    sub $t2, $t0, $t1
    sw $t2, -28($fp)
    lw $t0, -28($fp)
    sw $t0, -32($fp)
    li $t0, 7
    sw $t0, -36($fp)
    lw $t0, -32($fp)
    lw $t1, -36($fp)
    mul $t2, $t0, $t1
    sw $t2, -40($fp)
    lw $t0, -40($fp)
    sw $t0, -44($fp)
    li $t0, 100
    sw $t0, -48($fp)
    li $t0, 4
    sw $t0, -52($fp)
    lw $t0, -48($fp)
    lw $t1, -52($fp)
    div $t2, $t0, $t1
    sw $t2, -56($fp)
    lw $t0, -56($fp)
    sw $t0, -60($fp)
    li $t0, 17
    sw $t0, -64($fp)
    li $t0, 3
    sw $t0, -68($fp)
    lw $t0, -64($fp)
    lw $t1, -68($fp)
    rem $t2, $t0, $t1
    sw $t2, -72($fp)
    lw $t0, -72($fp)
    sw $t0, -76($fp)
    li $t0, 999
    sw $t0, -80($fp)
    lw $t0, -80($fp)
    sw $t0, -84($fp)
    li $t0, 2
    sw $t0, -88($fp)
    lw $t0, -16($fp)
    lw $t1, -88($fp)
    mul $t2, $t0, $t1
    sw $t2, -92($fp)
    lw $t0, -92($fp)
    sw $t0, -96($fp)
    lw $t0, -16($fp)
    lw $t1, -32($fp)
    add $t2, $t0, $t1
    sw $t2, -100($fp)
    lw $a0, -100($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -32($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -44($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -60($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -76($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -84($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -96($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 10
    syscall
