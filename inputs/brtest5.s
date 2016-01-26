        # Advanced branch test
	.text

        # J, JR, JAL, JALR, BEQ, BNE, BLEZ, BGTZ, BLTZ, BGEZ, BLTZAL, BGEZAL
        # BLTZAL, BGEZAL
main:
        addiu $v0, $zero, 0xa

        # Set up some comparison values in registers
        addiu $3, $zero, 1
        addiu $4, $zero, -1

        # Checksum register
        addiu $7, $zero, 0x1234

        # Test jump
        j l_1
l_0:
        nop
        addiu $7, $7, 0
        beq   $zero, $zero, l_2
l_1:
        nop
        addiu $7, $7, 7
        jal l_0
        nop
        nop
        j l_8
l_2:    
        nop
        addiu $7, $7, 9
        bne $3, $4, l_4
l_3:
        # Taken
        nop
        addiu $7, $7, 5
        bgez $zero, l_6
l_4:
        # Not taken
        nop
        addiu $7, $7, 11
        blez  $3, l_3
l_5:
        # Taken
        nop
        addiu $7, $7, 99
        bgtz  $3, l_3
l_6:
        # here
        nop
        addiu $7, $7, 111
        jr $ra
        # Should go to l_1, then go to l_8
l_7:
        # Should not get here
        nop
        addiu $7, $7, 200
        
        syscall
l_8:    
        nop
        addiu $7, $7, 215
        jal l_10
l_9:
        # Should not get here
        nop
        addiu $7, $7, 1
        syscall        
l_10:    
        nop
        addu $7, $7, $8
        bltzal $4, l_12
l_11:
        # Should not get here
        nop
        addiu $7, $7, 400
        syscall
l_12:    
        nop
        addu $7, $7, $8
        bgezal $4, l_11
        
l_13:    
        nop
        addiu $7, $7, 0xbeb0063d
	bltz $7, l_14
	andi $7, 0x0
        syscall

l_14:	nop
	syscall
        
        
