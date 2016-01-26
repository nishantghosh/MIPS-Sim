	.text

main:
	lui    $3, 0x1000

	addiu  $5, $zero, 0xcafe
	addiu  $6, $zero, 0xfeca
	addiu  $7, $zero, 0xbeef
	addiu  $8, $zero, 0xefbe
	
	sh     $5, 0($3)
	sh     $6, 2($3)
	sh     $7, 4($3)
	sh     $8, 6($3)
	
	lh     $9,  0($3)
	lh     $10, 2($3)
	lh     $11, 4($3)
	lh     $12, 6($3)
	

	addiu  $v0, $zero, 0xa
	syscall