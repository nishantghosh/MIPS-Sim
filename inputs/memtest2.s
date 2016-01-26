	.text

main:
	lui $3, 0x1000
	addiu $5, $zero, 255

	sw $5, 0($3)
	lw $9, 0($3)

	addiu $v0, $zero, 0xa
	syscall