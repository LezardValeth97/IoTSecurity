#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aes.c"

typedef unsigned char BYTE;
void aes_encrypt(BYTE* cipher, BYTE* message, BYTE* key);
void aes_decrypt(BYTE* message, BYTE* cipher, BYTE *key);

int main(int argc, char const *argv[]){
  if(argc != 3){
    printf("Enter input file and output file!\n");
    return -1;
  }

  // Enter the byte
  BYTE K[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
  BYTE *content = 0;
  BYTE cbc[16] = {0};
  BYTE temp[16];
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

  if (content){
    int i, j, block;

    block = length/16;
    for(i=0; i<block; i++){
      memcpy(temp, content, 16);
      aes_decrypt(content, content, K);
      for(j=0; j<16; j++){
        content[j] ^= cbc[j];
      }
      memcpy(cbc, temp, 16);
      content += 16;
    }

    content -= length;

    printf("Decrypted!\n");
    printf("Ready to unpadding!\n");
    padding = content[length-1];
    final = length - padding;
    printf("Padding Lenght = %d\n", padding);

    FILE *output = fopen(argv[2], "wb");
    if(output){
      printf("Writing to %s\n", argv[2]);
      fwrite(content, 1, final, output);
      fclose(output);
    }
    printf("Exit!\n");
    return 0;
  }
}

