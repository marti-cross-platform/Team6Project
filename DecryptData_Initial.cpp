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
			mov esi, gptrPasswordHash;	put the address of gPasswordHash into esi
			xor eax, eax
			mov al, byte ptr[esi];		store gPassword[0] in al
			shl ax, 8;					left shift by 8 (multiply by 256)
			xor ecx, ecx;				set ecx = 0
			mov cl, byte ptr[esi + 1];	set cx to gPassword[1]

			// ax = starting_index = gPasswordHash[0] * 256 + gPasswordHash[1]
			add ax, cx;					add gPassword[1] to ax, ax is now the starting index for the keyFile

			xor ebx, ebx;				ebx = control variable(loop)
			xor ecx, ecx
			mov ecx, dataLength;		ecx = length
			cmp ecx, 0;					check that the length is not less than or equal to 0
			sub ecx, 1;					ecx-- (file length is 1 less than previous)
			jbe lbl_EXIT_ZERO_LENGTH

			mov edi, data;				put the address of first byte of data in edi
			mov esi, gptrKey;			put the address of gKey into esi
	
			//
			// LOOP THROUGH ENTIRE data[] BYTE BY BYTE
			// At each step, data is loaded from the dl register and that step is performed on the data
			// The modified data is then loaded back into dl
		lbl_LOOP:
				// Get the current data
				mov dl, byte ptr[edi + ebx];	load the next byte from input file into dl

				// Free up the registers for use in Steps A - E
				push eax;	save keyFile index(0x917)
				push ebx;	save loop counter
				push ecx;	save dataLength

				// Part A reverse bit order
				xor ebx, ebx
				mov cl, 0x08;	set counter to proper size
				mov ax, dx
			LOOP1:
				rcr ax, 1;		shift to the right, moving lsb to carry flag
				rcl bx, 1;		shift to the left, inserting from carry flag to lsb
				dec cl
				cmp cl, 0
				je END
				jmp LOOP1
			END :
				mov edx, ebx;	reversed bits in ebx, copy to edx
				//*/
				
				// Part C swap half nibbles
				mov eax, edx;		load data to be swapped in eax
				lea bl, [eax * 4];	shift data to the left 2 and save in bl
				and bl, 0xCC;		masking to get indexes we want to swap eg. 1100 1100
				shr al, 2;			shift original data 2 to the right
				and al, 0x33;		mask remaining indexes eg. 0011 0011
				or al, bl;			combine and save in al
				mov dl, al
				//*/

				// Part E, swapping dl with the table value.
				mov dl, gDecodeTable[edx]
				//*/

				// Part B invert bits 0, 3, 6
				xor	dl, 0x49; bitMask = 0x49
				//*/

				//Part D rotate 3 bits left
				rol dl, 3
				//*/

				// Restore registers back to their previous states
				pop ecx;						restore dataLength
				pop ebx;						restore loop counter
				pop eax;						restore keyFile index(0x917)
				
				// XOR with the keyFile
				xor dl, byte ptr[esi + eax];	data[ebx] = data[ebx] xor with keyfile[starting_index]
				//*/
				
				// Save the modified data
				mov byte ptr[edi + ebx], dl;	replace the data in the array with the decrypted data
				
				// LOOP control
				add ebx, 1;			increment loop counter by 1
				cmp ebx, ecx;		if dataLength of the input file > loop counter, exit to lbl_EXIT
				ja lbl_EXIT_END
				jmp lbl_LOOP;		if dataLength < loop counter, jump back to lbl_LOOP

			lbl_EXIT_ZERO_LENGTH :
				sub ebx, 1;		decrement ebx to - 1 to return failure
				jmp lbl_EXIT

			lbl_EXIT_END :
				xor ebx, ebx;	ebx = 0, correctly executed

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

