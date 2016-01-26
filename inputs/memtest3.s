	.text

main:
	lui    $3, 0x1000

	addiu  $5, $zero, 0xcafe
	addiu  $6, $zero, 0xfeca
	addiu  $7, $zero, 0xbeef
	addiu  $8, $zero, 0xefbe
	
	sb     $5, 0($3)
	sb     $6, 1($3)
	sb     $7, 6($3)
	sb     $8, 7($3)
	
	lb     $9,  0($3)
	lb     $10, 1($3)
	lb     $11, 6($3)
	lb     $12, 7($3)
	

	addiu  $v0, $zero, 0xa
	syscall