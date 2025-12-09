.data
newline: .asciiz "\n"
str_31: .asciiz "== All tests complete =="
str_30: .asciiz "Steps taken:"
str_29: .asciiz "Sequence:"
str_28: .asciiz "Enter starting number:"
str_27: .asciiz "== Collatz Sequence =="
str_26: .asciiz "Power result:"
str_25: .asciiz "Enter exponent:"
str_24: .asciiz "Enter base:"
str_23: .asciiz "== Power =="
str_22: .asciiz "Number of primes:"
str_21: .asciiz "Enter upper limit:"
str_20: .asciiz "== Count Primes =="
str_19: .asciiz "Reversed number:"
str_18: .asciiz "Enter a number to reverse:"
str_17: .asciiz "== Reverse Number =="
str_16: .asciiz "Sum of digits:"
str_15: .asciiz "Enter a number:"
str_14: .asciiz "== Sum of Digits =="
str_13: .asciiz "Number is not prime"
str_12: .asciiz "Number is prime"
str_11: .asciiz "Enter a number to check if prime:"
str_10: .asciiz "== Prime Checker =="
str_9: .asciiz "GCD result:"
str_8: .asciiz "Enter second number:"
str_7: .asciiz "Enter first number:"
str_6: .asciiz "== GCD =="
str_5: .asciiz "Fibonacci sequence:"
str_4: .asciiz "Enter how many Fibonacci numbers:"
str_3: .asciiz "== Fibonacci =="
str_2: .asciiz "Factorial result:"
str_1: .asciiz "Enter a number for factorial:"
str_0: .asciiz "== Factorial =="
.text
.globl main
main:
    move $fp, $sp
    subu $sp, $sp, 524
    la $a0, str_0
    li $v0, 4
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
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $t0, 1
    sw $t0, -8($fp)
    lw $t0, -8($fp)
    sw $t0, -12($fp)
    li $t0, 1
    sw $t0, -16($fp)
    lw $t0, -16($fp)
    sw $t0, -20($fp)
L0:
    lw $t0, -20($fp)
    lw $t1, -4($fp)
    sle $t2, $t0, $t1
    sw $t2, -28($fp)
    lw $t0, -28($fp)
    beqz $t0, L1
    lw $t0, -12($fp)
    lw $t1, -20($fp)
    mul $t2, $t0, $t1
    sw $t2, -36($fp)
    lw $t0, -36($fp)
    sw $t0, -12($fp)
    li $t0, 1
    sw $t0, -40($fp)
    lw $t0, -20($fp)
    lw $t1, -40($fp)
    add $t2, $t0, $t1
    sw $t2, -44($fp)
    lw $t0, -44($fp)
    sw $t0, -20($fp)
    j L0
L1:
    la $a0, str_2
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -12($fp)
    li $v0, 1
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
    la $a0, str_4
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $t0, 0
    sw $t0, -48($fp)
    lw $t0, -48($fp)
    sw $t0, -52($fp)
    li $t0, 1
    sw $t0, -56($fp)
    lw $t0, -56($fp)
    sw $t0, -60($fp)
    li $t0, 0
    sw $t0, -64($fp)
    lw $t0, -64($fp)
    sw $t0, -20($fp)
    la $a0, str_5
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
L2:
    lw $t0, -20($fp)
    lw $t1, -4($fp)
    slt $t2, $t0, $t1
    sw $t2, -72($fp)
    lw $t0, -72($fp)
    beqz $t0, L3
    lw $a0, -52($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $t0, -52($fp)
    lw $t1, -60($fp)
    add $t2, $t0, $t1
    sw $t2, -80($fp)
    lw $t0, -80($fp)
    sw $t0, -84($fp)
    lw $t0, -60($fp)
    sw $t0, -52($fp)
    lw $t0, -84($fp)
    sw $t0, -60($fp)
    li $t0, 1
    sw $t0, -88($fp)
    lw $t0, -20($fp)
    lw $t1, -88($fp)
    add $t2, $t0, $t1
    sw $t2, -92($fp)
    lw $t0, -92($fp)
    sw $t0, -20($fp)
    j L2
L3:
    la $a0, str_6
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_7
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -96($fp)
    la $a0, str_8
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -100($fp)
L4:
    li $t0, 0
    sw $t0, -108($fp)
    lw $t0, -100($fp)
    lw $t1, -108($fp)
    sne $t2, $t0, $t1
    sw $t2, -112($fp)
    lw $t0, -112($fp)
    beqz $t0, L5
    lw $t0, -100($fp)
    sw $t0, -84($fp)
    lw $t0, -96($fp)
    lw $t1, -100($fp)
    rem $t2, $t0, $t1
    sw $t2, -120($fp)
    lw $t0, -120($fp)
    sw $t0, -100($fp)
    lw $t0, -84($fp)
    sw $t0, -96($fp)
    j L4
L5:
    lw $t0, -96($fp)
    sw $t0, -124($fp)
    la $a0, str_9
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -124($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_10
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_11
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $t0, 0
    sw $t0, -128($fp)
    lw $t0, -128($fp)
    sw $t0, -132($fp)
    li $t0, 2
    sw $t0, -136($fp)
    lw $t0, -4($fp)
    lw $t1, -136($fp)
    slt $t2, $t0, $t1
    sw $t2, -140($fp)
    lw $t0, -140($fp)
    beqz $t0, L6
    li $t0, 1
    sw $t0, -148($fp)
    lw $t0, -148($fp)
    sw $t0, -132($fp)
    j L7
L6:
    li $t0, 2
    sw $t0, -156($fp)
    lw $t0, -156($fp)
    sw $t0, -160($fp)
L8:
    lw $t0, -160($fp)
    lw $t1, -160($fp)
    mul $t2, $t0, $t1
    sw $t2, -168($fp)
    lw $t0, -168($fp)
    lw $t1, -4($fp)
    sle $t2, $t0, $t1
    sw $t2, -172($fp)
    lw $t0, -172($fp)
    beqz $t0, L9
    lw $t0, -4($fp)
    lw $t1, -160($fp)
    rem $t2, $t0, $t1
    sw $t2, -180($fp)
    li $t0, 0
    sw $t0, -184($fp)
    lw $t0, -180($fp)
    lw $t1, -184($fp)
    seq $t2, $t0, $t1
    sw $t2, -188($fp)
    lw $t0, -188($fp)
    beqz $t0, L10
    li $t0, 1
    sw $t0, -196($fp)
    lw $t0, -132($fp)
    lw $t1, -196($fp)
    add $t2, $t0, $t1
    sw $t2, -200($fp)
    lw $t0, -200($fp)
    sw $t0, -132($fp)
L10:
    li $t0, 1
    sw $t0, -204($fp)
    lw $t0, -160($fp)
    lw $t1, -204($fp)
    add $t2, $t0, $t1
    sw $t2, -208($fp)
    lw $t0, -208($fp)
    sw $t0, -160($fp)
    j L8
L9:
L7:
    li $t0, 0
    sw $t0, -212($fp)
    lw $t0, -132($fp)
    lw $t1, -212($fp)
    seq $t2, $t0, $t1
    sw $t2, -216($fp)
    lw $t0, -216($fp)
    beqz $t0, L11
    la $a0, str_12
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    j L12
L11:
    la $a0, str_13
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
L12:
    la $a0, str_14
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_15
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $t0, 0
    sw $t0, -228($fp)
    lw $t0, -228($fp)
    sw $t0, -12($fp)
L13:
    li $t0, 0
    sw $t0, -236($fp)
    lw $t0, -4($fp)
    lw $t1, -236($fp)
    sgt $t2, $t0, $t1
    sw $t2, -240($fp)
    lw $t0, -240($fp)
    beqz $t0, L14
    li $t0, 10
    sw $t0, -248($fp)
    lw $t0, -4($fp)
    lw $t1, -248($fp)
    rem $t2, $t0, $t1
    sw $t2, -252($fp)
    lw $t0, -12($fp)
    lw $t1, -252($fp)
    add $t2, $t0, $t1
    sw $t2, -256($fp)
    lw $t0, -256($fp)
    sw $t0, -12($fp)
    li $t0, 10
    sw $t0, -260($fp)
    lw $t0, -4($fp)
    lw $t1, -260($fp)
    div $t2, $t0, $t1
    sw $t2, -264($fp)
    lw $t0, -264($fp)
    sw $t0, -4($fp)
    j L13
L14:
    la $a0, str_16
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -12($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_17
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_18
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $t0, 0
    sw $t0, -268($fp)
    lw $t0, -268($fp)
    sw $t0, -12($fp)
L15:
    li $t0, 0
    sw $t0, -276($fp)
    lw $t0, -4($fp)
    lw $t1, -276($fp)
    sgt $t2, $t0, $t1
    sw $t2, -280($fp)
    lw $t0, -280($fp)
    beqz $t0, L16
    li $t0, 10
    sw $t0, -288($fp)
    lw $t0, -12($fp)
    lw $t1, -288($fp)
    mul $t2, $t0, $t1
    sw $t2, -292($fp)
    li $t0, 10
    sw $t0, -296($fp)
    lw $t0, -4($fp)
    lw $t1, -296($fp)
    rem $t2, $t0, $t1
    sw $t2, -300($fp)
    lw $t0, -292($fp)
    lw $t1, -300($fp)
    add $t2, $t0, $t1
    sw $t2, -304($fp)
    lw $t0, -304($fp)
    sw $t0, -12($fp)
    li $t0, 10
    sw $t0, -308($fp)
    lw $t0, -4($fp)
    lw $t1, -308($fp)
    div $t2, $t0, $t1
    sw $t2, -312($fp)
    lw $t0, -312($fp)
    sw $t0, -4($fp)
    j L15
L16:
    la $a0, str_19
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -12($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_20
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_21
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $t0, 0
    sw $t0, -316($fp)
    lw $t0, -316($fp)
    sw $t0, -132($fp)
    li $t0, 2
    sw $t0, -320($fp)
    lw $t0, -320($fp)
    sw $t0, -96($fp)
L17:
    lw $t0, -96($fp)
    lw $t1, -4($fp)
    sle $t2, $t0, $t1
    sw $t2, -328($fp)
    lw $t0, -328($fp)
    beqz $t0, L18
    li $t0, 0
    sw $t0, -336($fp)
    lw $t0, -336($fp)
    sw $t0, -84($fp)
    li $t0, 2
    sw $t0, -340($fp)
    lw $t0, -340($fp)
    sw $t0, -160($fp)
L19:
    lw $t0, -160($fp)
    lw $t1, -160($fp)
    mul $t2, $t0, $t1
    sw $t2, -348($fp)
    lw $t0, -348($fp)
    lw $t1, -96($fp)
    sle $t2, $t0, $t1
    sw $t2, -352($fp)
    lw $t0, -352($fp)
    beqz $t0, L20
    lw $t0, -96($fp)
    lw $t1, -160($fp)
    rem $t2, $t0, $t1
    sw $t2, -360($fp)
    li $t0, 0
    sw $t0, -364($fp)
    lw $t0, -360($fp)
    lw $t1, -364($fp)
    seq $t2, $t0, $t1
    sw $t2, -368($fp)
    lw $t0, -368($fp)
    beqz $t0, L21
    li $t0, 1
    sw $t0, -376($fp)
    lw $t0, -84($fp)
    lw $t1, -376($fp)
    add $t2, $t0, $t1
    sw $t2, -380($fp)
    lw $t0, -380($fp)
    sw $t0, -84($fp)
L21:
    li $t0, 1
    sw $t0, -384($fp)
    lw $t0, -160($fp)
    lw $t1, -384($fp)
    add $t2, $t0, $t1
    sw $t2, -388($fp)
    lw $t0, -388($fp)
    sw $t0, -160($fp)
    j L19
L20:
    li $t0, 0
    sw $t0, -392($fp)
    lw $t0, -84($fp)
    lw $t1, -392($fp)
    seq $t2, $t0, $t1
    sw $t2, -396($fp)
    lw $t0, -396($fp)
    beqz $t0, L22
    li $t0, 1
    sw $t0, -404($fp)
    lw $t0, -132($fp)
    lw $t1, -404($fp)
    add $t2, $t0, $t1
    sw $t2, -408($fp)
    lw $t0, -408($fp)
    sw $t0, -132($fp)
L22:
    li $t0, 1
    sw $t0, -412($fp)
    lw $t0, -96($fp)
    lw $t1, -412($fp)
    add $t2, $t0, $t1
    sw $t2, -416($fp)
    lw $t0, -416($fp)
    sw $t0, -96($fp)
    j L17
L18:
    la $a0, str_22
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -132($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_23
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_24
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -96($fp)
    la $a0, str_25
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -100($fp)
    li $t0, 1
    sw $t0, -420($fp)
    lw $t0, -420($fp)
    sw $t0, -12($fp)
    li $t0, 0
    sw $t0, -424($fp)
    lw $t0, -424($fp)
    sw $t0, -20($fp)
L23:
    lw $t0, -20($fp)
    lw $t1, -100($fp)
    slt $t2, $t0, $t1
    sw $t2, -432($fp)
    lw $t0, -432($fp)
    beqz $t0, L24
    lw $t0, -12($fp)
    lw $t1, -96($fp)
    mul $t2, $t0, $t1
    sw $t2, -440($fp)
    lw $t0, -440($fp)
    sw $t0, -12($fp)
    li $t0, 1
    sw $t0, -444($fp)
    lw $t0, -20($fp)
    lw $t1, -444($fp)
    add $t2, $t0, $t1
    sw $t2, -448($fp)
    lw $t0, -448($fp)
    sw $t0, -20($fp)
    j L23
L24:
    la $a0, str_26
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -12($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_27
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_28
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 5
    syscall
    sw $v0, -4($fp)
    li $t0, 0
    sw $t0, -452($fp)
    lw $t0, -452($fp)
    sw $t0, -132($fp)
    la $a0, str_29
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
L25:
    li $t0, 1
    sw $t0, -460($fp)
    lw $t0, -4($fp)
    lw $t1, -460($fp)
    sne $t2, $t0, $t1
    sw $t2, -464($fp)
    lw $t0, -464($fp)
    beqz $t0, L26
    lw $a0, -4($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $t0, 2
    sw $t0, -472($fp)
    lw $t0, -4($fp)
    lw $t1, -472($fp)
    rem $t2, $t0, $t1
    sw $t2, -476($fp)
    li $t0, 0
    sw $t0, -480($fp)
    lw $t0, -476($fp)
    lw $t1, -480($fp)
    seq $t2, $t0, $t1
    sw $t2, -484($fp)
    lw $t0, -484($fp)
    beqz $t0, L27
    li $t0, 2
    sw $t0, -492($fp)
    lw $t0, -4($fp)
    lw $t1, -492($fp)
    div $t2, $t0, $t1
    sw $t2, -496($fp)
    lw $t0, -496($fp)
    sw $t0, -4($fp)
    j L28
L27:
    li $t0, 3
    sw $t0, -504($fp)
    lw $t0, -504($fp)
    lw $t1, -4($fp)
    mul $t2, $t0, $t1
    sw $t2, -508($fp)
    li $t0, 1
    sw $t0, -512($fp)
    lw $t0, -508($fp)
    lw $t1, -512($fp)
    add $t2, $t0, $t1
    sw $t2, -516($fp)
    lw $t0, -516($fp)
    sw $t0, -4($fp)
L28:
    li $t0, 1
    sw $t0, -520($fp)
    lw $t0, -132($fp)
    lw $t1, -520($fp)
    add $t2, $t0, $t1
    sw $t2, -524($fp)
    lw $t0, -524($fp)
    sw $t0, -132($fp)
    j L25
L26:
    lw $a0, -4($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_30
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    lw $a0, -132($fp)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    la $a0, str_31
    li $v0, 4
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    li $v0, 10
    syscall
