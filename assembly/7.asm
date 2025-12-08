.data
newline: .asciiz "\n"
str_16: .asciiz "Done!"
str_15: .asciiz "Countdown:"
str_14: .asciiz "Complex result:"
str_13: .asciiz "Final value:"
str_12: .asciiz "Positive number entered"
str_11: .asciiz "Zero entered"
str_10: .asciiz "Negative number entered"
str_9: .asciiz "Enter a number:"
str_8: .asciiz "Numbers are different"
str_7: .asciiz "num1 equals 15"
str_6: .asciiz "num2 is greater or equal"
str_5: .asciiz "num1 is greater"
str_4: .asciiz "Number is positive"
str_3: .asciiz "Remainder:"
str_2: .asciiz "Division:"
str_1: .asciiz "Product:"
str_0: .asciiz "Sum:"
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 256
    li $t0, 15
    sw $t0, -4($fp)
    lw $t0, -4($fp)
    sw $t0, -8($fp)
    li $t0, 4
    sw $t0, -12($fp)
    lw $t0, -12($fp)
    sw $t0, -16($fp)
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    add $t2, $t0, $t1
    sw $t2, -20($fp)
    lw $t0, -20($fp)
    sw $t0, -24($fp)
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    mul $t2, $t0, $t1
    sw $t2, -28($fp)
    lw $t0, -28($fp)
    sw $t0, -32($fp)
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
    lw $a0, -32($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    div $t2, $t0, $t1
    sw $t2, -36($fp)
    lw $t0, -36($fp)
    sw $t0, -40($fp)
    la $a0, str_2
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -40($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    rem $t2, $t0, $t1
    sw $t2, -44($fp)
    lw $t0, -44($fp)
    sw $t0, -40($fp)
    la $a0, str_3
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -40($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $t0, 1
    sw $t0, -48($fp)
    lw $t0, -48($fp)
    sw $t0, -52($fp)
    lw $t0, -52($fp)
    beqz $t0, L0
    la $a0, str_4
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
L0:
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    sgt $t2, $t0, $t1
    sw $t2, -60($fp)
    lw $t0, -60($fp)
    beqz $t0, L1
    la $a0, str_5
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    j L2
L1:
    la $a0, str_6
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
L2:
    li $t0, 15
    sw $t0, -72($fp)
    lw $t0, -8($fp)
    lw $t1, -72($fp)
    seq $t2, $t0, $t1
    sw $t2, -76($fp)
    lw $t0, -76($fp)
    beqz $t0, L3
    la $a0, str_7
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
L3:
    lw $t0, -8($fp)
    lw $t1, -16($fp)
    sne $t2, $t0, $t1
    sw $t2, -84($fp)
    lw $t0, -84($fp)
    beqz $t0, L4
    la $a0, str_8
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
L4:
    li $t0, 1
    sw $t0, -92($fp)
    lw $t0, -92($fp)
    sw $t0, -96($fp)
    li $t0, 10
    sw $t0, -100($fp)
    lw $t0, -100($fp)
    sw $t0, -104($fp)
L5:
    lw $t0, -96($fp)
    lw $t1, -104($fp)
    sle $t2, $t0, $t1
    sw $t2, -112($fp)
    lw $t0, -112($fp)
    beqz $t0, L6
    li $t0, 0
    sw $t0, -120($fp)
    lw $t0, -120($fp)
    sw $t0, -124($fp)
    li $t0, 2
    sw $t0, -128($fp)
    lw $t0, -96($fp)
    lw $t1, -128($fp)
    rem $t2, $t0, $t1
    sw $t2, -132($fp)
    lw $t0, -132($fp)
    sw $t0, -40($fp)
    li $t0, 0
    sw $t0, -136($fp)
    lw $t0, -40($fp)
    lw $t1, -136($fp)
    seq $t2, $t0, $t1
    sw $t2, -140($fp)
    lw $t0, -140($fp)
    beqz $t0, L7
    li $t0, 1
    sw $t0, -148($fp)
    lw $t0, -148($fp)
    sw $t0, -124($fp)
L7:
    lw $t0, -124($fp)
    beqz $t0, L8
    lw $a0, -96($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
L8:
    li $t0, 1
    sw $t0, -156($fp)
    lw $t0, -96($fp)
    lw $t1, -156($fp)
    add $t2, $t0, $t1
    sw $t2, -160($fp)
    lw $t0, -160($fp)
    sw 