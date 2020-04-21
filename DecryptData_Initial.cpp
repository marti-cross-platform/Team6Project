// DecryptData.cpp
//
// THis file uses the input data and key information to decrypt the input data
//

#include "Main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// code to decrypt the data as specified by the project assignment
int decryptData(char *data, int dataLength)
{
	int resulti = 0;

	gdebug1 = 0;					// a couple of global variables that could be used for debugging
	gdebug2 = 0;					// also can have a breakpoint in C code

	// You can not declare any local variables in C, but should use resulti to indicate any errors
	// Set up the stack frame and assign variables in assembly if you need to do so
	// access the parameters BEFORE setting up your own stack frame
	// Also, you cannot use a lot of global variables - work with registers

	__asm {

		// you will need to reference some of these global variables
		// (gptrPasswordHash or gPasswordHash), (gptrKey or gkey), gNumRounds

		mov esi, gptrPasswordHash // put the address of gPasswordHash into %esi
			xor eax, eax //
			mov al, byte ptr[esi] // store gPassword[0] in %al
			shl ax, 8 // left shift by 8 (multiply by 256)
			xor ecx, ecx // set %ecx = 0
			mov cl, byte ptr[esi + 1] // set %cx to gPassword[1]
			add ax, cx 	// add gPassword[1] to %ax, %ax is now the starting index for key.dat
			// ax = starting_index = gPasswordHash[0] * 256 + gPasswordHash[1]

			xor ebx, ebx // %ebx = control variable (loop)
			xor ecx, ecx
			mov ecx, dataLength // %ecx = length
			cmp ecx, 0 // check that the length is not less than or equal to 0
			sub ecx, 1 // ecx-- (file length is 1 less than previous)
			jbe lbl_EXIT_ZERO_LENGTH //

			mov edi, data // put the address of first byte into %edi
			mov esi, gptrKey // put the address of gKey into %esi

			/* LOOP THROUGH ENTIRE data[] -- BYTE BY BYTE */
		lbl_LOOP :
		mov dl, byte ptr[edi + ebx] // put the data being encrypted into dl
			xor dl, byte ptr[esi + eax] // data[ebx] = data[ebx] xor with keyfile[starting_index]

			// Part D rotate 3 bits right
			ror dl, 3

			// Part B invert bits 0, 3, 6		0xB5 --> 0xFC
			/*
			mov	DWORD PTR[ebp + 8], 73;
			mov	eax, DWORD PTR[data]
			xor	eax, DWORD PTR[ebp + 8]	// XOR with bitMask, 0x49
			*/
			xor dl, 0x49 // flip bits inside edx

			/* Part E, swapping dl with the table value.
			TO BE DONE: ordering each part correctly.*/
			mov dl, gEncodeTable[edx]   // changed ebx to edx as an index

			// Part C swap half nibbles
			mov eax, edx		//load data to be swapped in eax
			lea bl, [eax * 4]	//shift data to the left 2 and save in bl
			and bl, 0xCC		// masking to get indexes we want to swap eg. 1100 1100
			shr al, 2			// shift original data 2 to the right
			and al, 0x33		// mask remaining indexes eg. 0011 0011
			or al, bl			// combine and save in al

			/*
			// #A reverse bit order	- value will be in 'ch'		0xAD --> 0xB5
			mov eax, data
			mov cl, 7
			mov dh, 1	// DH is a 1 which travels in the byte from right to left
			mov dl, 0

		LOOP1: push ax
			   AND al, dh
			   push cx
			   mov cl, dl
			   shr al, cl	// shift right to get the value as 0 or 1
			   pop cx
			   mov bh, al
			   shl bh, cl	// shift into position - '7' for byte 0, '6' for byte 1...
			   OR ch, bh	// OR to the final result
			   DEC cl
			   INC dl
			   shl dh, 1
			   pop ax
			   cmp dl, 8
			   je END
			   jmp LOOP1

		   END :
		   */

		mov byte ptr[edi + ebx], dl // put the now decrypted data back into the array

			add ebx, 1 // increment %ebx by 1
			cmp ebx, ecx // if(ebx > ecx) { end loop }
			ja lbl_EXIT_END //
			jmp lbl_LOOP // else { loop }

		lbl_EXIT_ZERO_LENGTH :
		sub ebx, 1 // decrement ebx to -1 to return failure
			jmp lbl_EXIT //

		lbl_EXIT_END :
		xor ebx, ebx // ebx = 0, correctly executed

		lbl_EXIT :
		mov resulti, ebx

			/*
			// simple example that xors 2nd byte of data with 14th byte in the key file
			lea esi,gkey				// put the ADDRESS of gkey into esi
			mov esi,gptrKey;			// put the ADDRESS of gkey into esi (since *gptrKey = gkey)

			lea	esi,gPasswordHash		// put ADDRESS of gPasswordHash into esi
			mov esi,gptrPasswordHash	// put ADDRESS of gPasswordHash into esi (since unsigned char *gptrPasswordHash = gPasswordHash)

			mov al,byte ptr [esi]				// get first byte of password hash
			mov al,byte ptr [esi+4]				// get 5th byte of password hash
			mov ebx,2
			mov al,byte ptr [esi+ebx]			// get 3rd byte of password hash
			mov al,byte ptr [esi+ebx*2]			// get 5th byte of password hash

			mov ax,word ptr [esi+ebx*2]			// gets 5th and 6th bytes of password hash ( gPasswordHash[4] and gPasswordHash[5] ) into ax
			mov eax,dword ptr [esi+ebx*2]		// gets 4 bytes, as in:  unsigned int X = *( (unsigned int*) &gPasswordHash[4] );

			mov al,byte ptr [gkey+ebx]			// get's 3rd byte of gkey[] data

			mov al,byte ptr [gptrKey+ebx]		// THIS IS INCORRECT - will add the address of the gptrKey global variable (NOT the value that gptrKey holds)

			mov al,byte ptr [esi+0xd];			// access 14th byte in gkey[]: 0, 1, 2 ... d is the 14th byte
			mov edi,data				// Put ADDRESS of first data element into edi
			xor byte ptr [edi+1],al		// Exclusive-or the 2nd byte of data with the 14th element of the keyfile
			// NOTE: Keyfile[14] = 0x21, that value changes the case of a letter and flips the LSB
			// Lowercase "c" = 0x63 becomes capital "B" since 0x63 xor 0x21 = 0x42
			*/
	}

	return resulti;
} // decryptData
