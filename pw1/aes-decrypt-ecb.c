#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.c"

typedef unsigned char BYTE;
void aes_encrypt(BYTE* cipher, BYTE* message, BYTE* key);
void aes_decrypt(BYTE* message, BYTE* cipher, BYTE *key);

int main(int argc, char const *argv[]){
  	if(argc != 3){
    	printf("Specify input and output file!\n");
	    return -1;
  	}

  	// Enter the byte
	BYTE K[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

  	BYTE *content = 0;
  	BYTE *fullOutput;
  	long length;
  	char padding;
  	long final;

  	FILE *input = fopen(argv[1], "rb");
  	if(input){
    	printf("Reading from %s\n", argv[1]);
    	fseek(input, 0, SEEK_END); // sets the file position of input 
    	length = ftell(input); // obtains the current value  of  the  file
    	//printf("%ld\n", length);
    	padding = (length % 16) ? (16 - length % 16) : 16; 
    	//total = length + padding;
    	//printf("%ld\n", total);

    	fseek(input, 0, SEEK_SET);
    	printf("Allocating memory for input...\n");
    	content = malloc(length);
    	if(content){
      	// reads data from input into the array pointed to content
      	fread(content, 1, length, input);
    	}
    	fclose(input);
  	}

	if(content){
		int i;
		fullOutput = malloc(length);
		for(i=0; i<length; i+=16){
			aes_decrypt(fullOutput+i, content+i, K);
		}
		printf("Decrypted!\n");
		printf("Ready to unpadding!\n");
		padding = fullOutput[length-1];
		final = length - padding;
		printf("Padding Lenght = %d\n", padding);

		FILE *output = fopen(argv[2], "wb");
		if(output){
			printf("Writing to file %s\n", argv[2]);
			fwrite(fullOutput, 1, final, output);
			fclose(output);
		}
}

	printf("Exit!\n");
	return 0;
}