.section .rodata

usage: .asciz "Usage: %s 0x<address>\n"
header: .asciz "  Level   Index  Byte Index\n"
vpns: .asciz "VPN[00%d] = %-5lu(%4lu)\n"
po: .asciz "PO       = %lx\n"

input: .asciz "%lx"

addressVerification: .asciz "This is address: %lx\n" //DELETE

.section .text
.global main

main:
		addi	sp, sp, -80 //Stack allocation
		sd		ra, 0(sp)
		sd		s0, 8(sp)   //long Virtual Address
		sd		s1, 16(sp)  //int argc
		sd		s2, 24(sp)  //long Page Offset
		sd		s3, 32(sp)  //long VPN
		sd		s4, 40(sp)  //long Byte Offset
		sd		s5, 48(sp)  //int i
		sd		s6, 56(sp)	//Name of program
		sd		s7, 64(sp)	//All argv

		//Saving argc and argv[0]
		mv		s1, a0		//int argc
		mv		s7, a1		//char* argv
		ld		s6, 0(s7)   //Name of program argv[0]		

		//argc check
		li		t0, 2		
		beq		t0, s1, 1f  //Checks for argc == 2

		//If argc != 2, print Usage error and return
		la		a0, usage  //NOTE TO SELF: SEG FAULT HERE WHEN USING LD AND NOT LA
		mv		a1, s6  //Derefrences argv[0] for program name.
		call	printf@plt
		j		3f


//Reach this point if argc == 2
1:

		//Saving argv[1]
		ld		a0, 8(s7)
		la		a1, input
		addi	a2, sp, 72 	//Saves address on to stack at next available place. NOTE TO SELF: CANNOT SAVE TO A REGISTER BECAUSE NOT AN ADDRESS (IE A2, S0)
		call	sscanf@plt

		ld		s0, 72(sp)	//Loads from the stack into s0 the address


		la		a0, addressVerification
		mv		a1, s0		//NOTE TO SELF: ERROR HERE WHEN USING LD INSTEAD OF MV
		call	printf@plt

		//Print header
		la		a0, header
		call	printf@plt

		//Initialize s5 (int i)
		li		s5, 2

//Loop has began
2:
		li		t0, 0
		blt		s5,	t0, 2f

		//vpn = (address >> (12 + (i * 9))) & 0x1ff;
		li		t0, 9
		mul		t0, s5, t0	//t0 = i * 9
		addi	t0, t0, 12	//t0 = t0 + 12
		srl		s3, s0, t0  //address >> t0
		li		t1, 0x1ff
		and		s3, s3, t1  //vpn = vpn & 0x1ff

		//byte_offset = vpn * 8
		li		t0, 8
		mul		s4, s3, t0

		//printf
		la		a0, vpns
		mv		a1, s5	//i
		mv		a2, s3	//vpn
		mv		a3, s4	//byte offset
		call	printf@plt

		//Increment I and check condition
		li		t0, 1
		sub		s5, s5, t0 //i--
		j		2b
		
//Loop has ended
2:
		//Produce page offset and print
		li		t0, 0xfff
		and		s2, s0, t0 //NOTE TO SELF: ERROR HERE WHEN USING LITERAL
		la		a0, po
		mv		a1, s2
		call	printf@plt
		

		j		3f
3:
		ld		s7, 64(sp)
		ld		s6, 56(sp)
		ld		s5, 48(sp)
		ld		s4, 40(sp)
		ld		s3, 32(sp)
		ld		s2, 24(sp)
		ld		s1, 16(sp)
		ld		s0, 8(sp)
		ld		ra, 0(sp)
		addi	sp, sp, 80 //Stack deallocation
		ret
