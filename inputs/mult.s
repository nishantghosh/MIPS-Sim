	.text

main:
	addiu $2, $zero, 0xFFFFF 
	mult $2, $2
	addiu $2, $zero, 1024
	addiu $3, $zero, 64
	div $2, $3
	mfhi $5
	mflo $6
	addiu $2, $zero, 0x4
	addiu $3, $zero, 0xFFFFFFFA
	divu $3, $2
	mfhi $7
	mflo $8
	addiu $2, $zero, 0xA
	syscall
