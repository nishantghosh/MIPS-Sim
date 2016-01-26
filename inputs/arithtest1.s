	.text
main:
	addiu 	$3, $zero, 0xFFFFFFFE
	addiu 	$4, $zero, 0x10
	addiu   $2, $zero, 0x0000FFFF

	# slt tests
	
	slt	$5, $3, $4    # $5 should be set to 1
	slt 	$6, $4, $3    # $6 should be set to 0

	sltu 	$9, $3, $4    # $9 should be set to 0
	sltu 	$10, $4, $3   # $10 should be set to 1
	
	slti 	$7, $3, 0xFFFFFFFF    # $7 should be set to 1
	slti 	$8, $4, 0xFFFFFFFF    # $8 should be set to 0

	sltiu	$11, $3, 0xFFFFFFFF   # $11 should be set to 1
	sltiu	$12, $4, 0xFFFFFFFF   # $12 should be set to 1

	slt     $14, $2, $3
	slt 	$15, $2, $4

	addiu   $2, $zero, 0xA	

	syscall
