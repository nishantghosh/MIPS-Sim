	.text
main:

	addiu	$2, $zero, 0x10
	addiu 	$3, $zero, 0xFFFF
	sll	$4, $3, 0x10
	sra	$5, $4, 0x10
	srl 	$6, $4, 0x10
	sllv 	$7, $3, $2
	srav    $8, $7, $2
	srlv 	$9, $7, $2
	addiu	$2, $zero, 0xA
	syscall