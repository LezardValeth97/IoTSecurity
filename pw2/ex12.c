#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "gmp.h"
#include <fcntl.h>

#define TEXT 60

void encrpyt_rsa(mpz_t c, const mpz_t m, const mpz_t n, const mpz_t e){
	printf("Encrypting!\n");
	mpz_powm(c, m, e, n); // c = m^e mod n
}

void decrypt_rsa(mpz_t m, const mpz_t c, const mpz_t n, const mpz_t d){
	printf("Decrypting!\n");
	mpz_powm(m, c, d, n); // m = c^d mod n
}

int main(int argc, char const *argv[]){
	if(argc != 2){
		printf("No small text can be found!!!\n");
		return -1;
	}

	else{
		char *key, *value;
		mpz_t z_d, z_e, z_n, z_m, cipher, decrypt; 
		mpz_inits(cipher, decrypt, z_d, z_e, z_n, z_m,NULL); 
		/* assign to the message variable an int value
			represented by a string of characters */
		mpz_set_str(z_m, argv[1], TEXT); 

		FILE *input = fopen("key.txt", "rb"); 
		if(input){
			// read formatted input (key and value) from the file
			while(fscanf(input, "%m[^=]=%ms\n", &key, &value) == 2){ 
				switch(key[0]){
					case 'e':
						mpz_set_str(z_e, value, 0);
						break;
					case 'n':
						mpz_set_str(z_n, value, 0);
						break;
					case 'd':
						mpz_set_str(z_d, value, 0);
						break;
				}
				// Deallocate allocated memory
				free(key);
				free(value);
			}

			fclose(input); // close the file
		}

		encrpyt_rsa(cipher, z_m, z_n, z_e);
		printf("Ciphertext: %s\n", mpz_get_str(NULL, TEXT, cipher));
		decrypt_rsa(decrypt, cipher, z_n, z_d);
		printf("Decrypted: %s\n", mpz_get_str(NULL, TEXT, decrypt));

		return 0;
	}
}