        # Basic branch test
	.text

main:
        addiu $v0, $zero, 0xa
l_0:    
        addiu $5, $zero, 1
        addiu $4, $zero, 1
	bgezal $4, l_2
        syscall        
l_2:
        addiu $6, $zero, 0x1337
	syscall

        
         
        
