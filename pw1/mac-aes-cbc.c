#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aes.c"

typedef unsigned char BYTE;
void aes_encrypt(BYTE* cipher, BYTE* message, BYTE* key);
void aes_decrypt(BYTE* message, BYTE* cipher, BYTE *key);

int main(int argc, char const *argv[]){
  if(argc != 2){
    printf("Enter input file!\n");
    return -1;
  }

  // Enter the byte
  BYTE K[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
  BYTE *content = 0;
  BYTE cbc[16] = {0};
  long length;
  char padding;
  long total;

  FILE *input = fopen(argv[1], "rb");
  if(input){
    printf("Reading from %s\n", argv[1]);
    fseek(input, 0, SEEK_END); // sets the file position of input 
    length = ftell(input); // obtains the current value  of  the  file
    //printf("%ld\n", length);
    padding = (length % 16) ? (16 - length % 16) : 16; 
    total = length + padding;
    //printf("%ld\n", total);

    fseek(input, 0, SEEK_SET);
    printf("Allocating memory for input...\n");
    content = malloc(total);
    if(content){
      // reads data from input into the array pointed to content
      fread(content, 1, length, input);
    }
    fclose(input);
  }

  if (content){
    printf("File size: %li\n", length);
    printf("Padding Size: %d\n", padding);
    int i, j, block;
    for(i=0; i<padding; i++){
      content[length+i] = padding;
    }

    block = total/16;
    for(i=0; i<block; i++){
      for(j=0; j<16; j++){
        cbc[j] ^= content[j];
      }
      aes_encrypt(cbc, cbc, K);
      memcpy(content, cbc, 16); // copy 16 bytes from cbc to content
      content += 16;
    }

    printf("Encrypted!\n");
    printf("MAC: ");
    for (int i = 0; i < 16; ++i){
      printf("%x", *(content - 16 + i));
    }
  }
  printf("\nExit\n");
  return 0;
}