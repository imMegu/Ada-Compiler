.data
newline: .asciiz "\n"
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 136
    li $t0, 1
    sw $t0, -4($fp)
    lw $t0, -4($fp)
    sw $t0, -8($fp)
    li $t0, 0
    sw $t0, -12($fp)
    lw $t0, -12($fp)
    sw $t0, -16($fp)
    lw $t0, -8($fp)
    xori $t2, $t0, 1
    sw $t2, -20($fp)
    lw $t0, -20($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -16($fp)
    xori $t2, $t0, 1
    sw $t2, -28($fp)
    lw $t0, -28($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    and $t2, $t0, $t1
    sw $t2, -32($fp)
    lw $t0, -32($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -8($fp)
    lw $t1, -8($fp)
    and $t2, $t0, $t1
    sw $t2, -36($fp)
    lw $t0, -36($fp)
    sw $t0, -24($fp)
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
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -16($fp)
    lw $t1, -16($fp)
    or $t2, $t0, $t1
    sw $t2, -44($fp)
    lw $t0, -44($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    xor $t2, $t0, $t1
    sw $t2, -48($fp)
    lw $t0, -48($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -8($fp)
    lw $t1, -8($fp)
    xor $t2, $t0, $t1
    sw $t2, -52($fp)
    lw $t0, -52($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $t0, 10
    sw $t0, -56($fp)
    lw $t0, -56($fp)
    sw $t0, -60($fp)
    li $t0, 20
    sw $t0, -64($fp)
    lw $t0, -64($fp)
    sw $t0, -68($fp)
    lw $t0, -60($fp)
    lw $t1, -68($fp)
    slt $t2, $t0, $t1
    sw $t2, -72($fp)
    li $t0, 5
    sw $t0, -76($fp)
    lw $t0, -68($fp)
    lw $t1, -76($fp)
    sgt $t2, $t0, $t1
    sw $t2, -80($fp)
    lw $t0, -72($fp)
    lw $t1, -80($fp)
    and $t2, $t0, $t1
    sw $t2, -84($fp)
    lw $t0, -84($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -60($fp)
    lw $t1, -68($fp)
    sgt $t2, $t0, $t1
    sw $t2, -88($fp)
    li $t0, 15
    sw $t0, -92($fp)
    lw $t0, -60($fp)
    lw $t1, -92($fp)
    slt $t2, $t0, $t1
    sw $t2, -96($fp)
    lw $t0, -88($fp)
    lw $t1, -96($fp)
    or $t2, $t0, $t1
    sw $t2, -100($fp)
    lw $t0, -100($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -60($fp)
    lw $t1, -68($fp)
    seq $t2, $t0, $t1
    sw $t2, -104($fp)
    lw $t0, -104($fp)
    xori $t2, $t0, 1
    sw $t2, -108($fp)
    lw $t0, -108($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -60($fp)
    lw $t1, -68($fp)
    slt $t2, $t0, $t1
    sw $t2, -112($fp)
    lw $t0, -16($fp)
    xori $t2, $t0, 1
    sw $t2, -116($fp)
    lw $t0, -112($fp)
    lw $t1, -116($fp)
    and $t2, $t0, $t1
    sw $t2, -120($fp)
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    xor $t2, $t0, $t1
    sw $t2, -124($fp)
    lw $t0, -120($fp)
    lw $t1, -124($fp)
    or $t2, $t0, $t1
    sw $t2, -128($fp)
    lw $t0, -128($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    and $t2, $t0, $t1
    sw $t2, -132($fp)
    lw $t0, -132($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    or $t2, $t0, $t1
    sw $t2, -136($fp)
    lw $t0, -136($fp)
    sw $t0, -24($fp)
    lw $a0, -24($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 10
    syscall
