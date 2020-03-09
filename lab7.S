//Autumn Henderson
//Lab7
//March 1st, 2019
//CS 130
//This program takes two operands and an operator and performs the
//given operation

.section .rodata

scan_integer: .asciz "%d"
scan_char: .asciz "%c"
result: .asciz "Result: %d %c %d = %d\n"
unknown: .asciz "Unknown operator '%c'\n"
not_enough: .asciz "Not enough arguments.\n"
too_many: .asciz "Too many arguments.\n"
div_zero: .asciz "Error: dividing by 0.\n"

add:
		lw		a0, 0(a0) //dereferences pointer a0
		lw		a1, 0(a1) //dereferences pointer a1
		add		a0, a0, a1 //result is now in a0
		ret
sub:	
		lw		a0, 0(a0) //a0 = a0[0]
		lw		a1, 0(a1) //a1 = a1[0]
		sub		a0, a0, a1 //result is now in a0
		ret
mul:
		lw		a0, 0(a0) //a0 = a0[0]
		lw		a1, 0(a1) //a1 = a1[0]
		mul		a0, a0, a1 //result is now in a0
		ret
div:
		lw		a0, 0(a0) //a0 = a0[0]
		lw		a1, 0(a1) //a1 = a1[0]
		div		a0, a0, a1 //result is now in a0
		ret
rem:
		lw		a0, 0(a0) //a0 = a0[0]
		lw		a1, 0(a1) //a1 = a1[0]
		rem		a0, a0, a1 //result is now in a0
		ret

.section .text
.global main
main:
		addi	sp, sp, -56
		sd		ra, 0(sp)
		sd		s0, 8(sp)
		sd		s1, 16(sp)
		sd		s2, 24(sp)
		
		//Step 1: Check argc
		li		t0, 4
		blt		a0, t0, not_enough_arguments
		bgt		a0, t0, too_many_arguments

		//Step 2: Load in values into saved registers
		ld		s0, 8(a1) //s0 = argv[1]
		ld		s1, 16(a2) //s1 = argv[2]
		ld		s2, 24(a3) //s2 = argv[3]

		//Step 3: Convert s0 and s2 to integers using sscanf
		mv		a0, s0 //a0 = argv[1]
		la		a1, scan_integer 
		addi	a2, sp, 32 
		call	sscanf //&left is at sp + 32

		mv		a0, s1 //a0 = argv[2]
		la		a1, scan_char
		addi	a2, sp, 40
		call	sscanf //&operation is at sp + 40

		mv		a0, s2 //a0 = argv[3]
		la		a1, scan_integer
		addi	a2, sp, 48
		call	sscanf //&right is at sp + 48

		lb		s1, 40(sp) //s1 = argv[2]

		//Step 4: Check character value to determine function call
		li		t0, '+'
		beq		s1, t0, 1f
		lw		a0, 32(sp)
		lw		a1, 48(sp)
		call	add //a0 is now equal to the sum of s0 and s2
		j		exit

1:
		li		t0, '-'
		beq		s1, t0, 2f
		lw		a0, 32(sp)
		lw		a1, 48(sp)
		call	sub //a0 is now equal to the difference of s0 and s2
		j		exit
2:
		li		t0, 'x'
		beq		s1, t0, 3f
		lw		a0, 32(sp)
		lw		a1, 48(sp)
		call	mul //a0 is now equal to the product of s0 and s2
		j		exit
3:
		li		t0, '/'
		lw		t1, 48(sp)
		li		t2, 0
		beq		s1, t0, 4f
		blt		t1, t2, div_zero //making sure not dividing by 0
		lw		a0, 32(sp)
		lw		a1, 48(sp)
		call	div //a0 is now equal to the quotient of s0 and s2
		j		exit
4:
		li		t0, '%'
		lw		t1, 48(sp)
		li		t2, 0
		beq		s1, t0, unknown_op //s1 is not equal to any operations
		blt		t1, t2, div_zero //making sure not dividing by 0
		lw		a0, 32(sp)
		lw		a1, 48(sp)
		call	rem //a0 is now equal to the remainder of s0 and s2
		j		exit


/*5:
		//Step 5: Call printf for result
		lw		t1, 0(a0)
		la		a0, result
		lw		a1, 32(sp)
		lb		a2, 40(sp)
		lw		a3, 48(sp)
		lw		a4, 0(t1)		
		call	printf
		j		exit			
*/
not_enough_arguments:
		la		a0, not_enough
		call	printf
		li		a0, -1
		j		exit

too_many_arguments:
		la		a0, too_many
		call	printf
		li		a0, -1
		j		exit

unknown_op:
		la		a0, unknown
		ld		a1, 16(sp)
		call	printf
		li		a0, -1
		j		exit

dividing_by_zero:
		la		a0, div_zero
		call	printf
		li		a0, -1
		j		exit

exit:
		ld		s2, 24(sp)
		ld		s1, 16(sp)
		ld		s0, 8(sp)
		ld		ra, 0(sp)
		addi	sp, sp, 56
		ret

