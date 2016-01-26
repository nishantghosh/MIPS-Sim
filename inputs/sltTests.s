	.text
main:
	addiu 	$3, $zero, 0xFFFFFFFE
	addiu 	$4, $zero, 6

	# slt tests
	slt	$5, $3, $4    # $5 should be set to 1
	slt 	$6, $4, $3    # $6 should be set to 0

	# sltu tests
	sltu	$9, $3, $4	# $9 should be set to 1
	sltu	$10, $4, $3	# $10 should be set to 0

	
	# slti
	slti 	$7, $3, 6     # $7 should be set to 1
	slti 	$8, $4, 5     # $8 should be set to 0

	# sltiu
	sltiu 	$11, $3, 6	# $11 should be set to 1
	sltiu	$12, $4, 6	# $12 should be set to 0
	
	addiu 	$v0, $zero, 0xa

	syscall