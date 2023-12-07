  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #


.data
.text

.global hexasc
hexasc:	
	addi $v0, $a0, 48
	blt $a0, 10, hop
	nop
	addi $v0, $v0, 7
hop:
	jr $ra
	nop

.global time2string
time2string:

	
	andi $t2, $a1, 0x0ff
	beqz $t2, ding
	
	andi $t1, $a1, 0x0f000
	srl $t1, $t1, 12
	PUSH $ra
	PUSH $a0
	addi $a0, $t1, 0
	jal hexasc
	nop
	POP $a0
	POP $ra
	sb $v0, 0($a0)
	
	andi $t1, $a1, 0x0f00
	srl $t1, $t1, 8
	PUSH $ra
	PUSH $a0
	addi $a0, $t1, 0
	jal hexasc
	nop
	POP $a0
	POP $ra
	sb $v0, 1($a0)
	
	
	
	addi $v0, $0, 0x03A
	sb $v0, 2($a0)
	
	andi $t1, $a1, 0x0f0
	srl $t1, $t1, 4
	PUSH $ra
	PUSH $a0
	addi $a0, $t1, 0
	jal hexasc
	nop
	POP $a0
	POP $ra
	sb $v0, 3($a0)
	
	andi $t1, $a1, 0x0f
	srl $t1, $t1, 0
	PUSH $ra
	PUSH $a0
	addi $a0, $t1, 0
	jal hexasc
	nop
	POP $a0
	POP $ra
	sb $v0, 4($a0)
	
	addi $v0, $0, 0
	sb $v0, 5($a0)
	j finish
ding:
	addi $v0, $0, 0x44 #D
	sb $v0, 0($a0)
	addi $v0, $0, 0x49 #I
	sb $v0, 1($a0)
	addi $v0, $0, 0x4E #N
	sb $v0, 2($a0)
	addi $v0, $0, 0x47 #G
	sb $v0, 3($a0)
	addi $v0, $0, 0x0 #NULL
	sb $v0, 4($a0)

finish:

.global delay	
delay:
	PUSH $t1
	PUSH $t0
	PUSH $s0

	move $t1, $a0
	blez $t1, exit
	nop

	li $t0, 7500
otherloop:

	addi $t1, $t1, -1
	li $s1, 0
loop:	
	addi $s1, $s1, 1
	
	bne $t0, $s1, loop
	nop
	
	bnez $t1, otherloop
	nop
exit:
	POP $s0
	POP $t0
	POP $t1
	
	jr $ra
	nop

