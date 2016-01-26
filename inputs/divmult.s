	.text

main:
	addiu 	$2, $zero, 5
	addiu	$3, $zero, 4
	mult 	$2, $3  # $4 should hold 20

	mfhi 	$5  # should be 0
	mflo	$6  # should be 20

	addiu	$7, $zero, 20
	div	$7, $2

	mfhi 	$8 # should be 4
	mflo 	$9 # should be 0
	
	addiu $v0, $zero, 0xa
	syscall	