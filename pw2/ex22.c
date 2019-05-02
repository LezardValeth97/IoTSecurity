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

void decrypt_rsa(mpz_t m, const mpz_t c, const mpz_t n, const mpz_t p, const mpz_t q, const mpz_t dp, const mpz_t dq, const mpz_t Ip){
	printf("Decrypting!\n");
    mpz_t mp, mq, temp;
    mpz_inits(mp, mq, temp, NULL);
    mpz_powm(mp, c, dp, p);
    mpz_powm(mq, c, dq, q);		
    mpz_sub(temp, mq, mp);		// temp = mq - mp;
    mpz_mul(temp, temp, Ip);	// temp *= Ip;
    mpz_mod(temp, temp, q);		// temp %= q;
    mpz_mul(temp, temp, p);		// temp *= p;
    mpz_add(m, mp, temp);		// m = mp + temp;
}

int main(int argc, char const *argv[]){
	if(argc != 2){
		printf("No small text can be found!!!\n");
		return -1;
	}

	else{
		char *key, *value;
		mpz_t z_p, z_q, z_dp, z_dq, z_Ip, z_e, z_n, z_m, cipher, decrypt;
		mpz_inits(z_p, z_q, z_dp, z_dq, z_Ip, z_e, z_n, z_m, cipher, decrypt, NULL);
		/* assign to the message variable an int value
			represented by a string of characters */
		mpz_set_str(z_m, argv[1], TEXT); 

		FILE *input = fopen("keycrt.txt", "rb"); 
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
					case 'p':
					    mpz_set_str(z_p, value, 0);
					    break;
					case 'q':
					    mpz_set_str(z_q, value, 0);
					    break;

					case 'd':
					    switch (key[1])
					    {
					    case 'p':
					        mpz_set_str(z_dp, value, 0);
					        break;
					    case 'q':
					        mpz_set_str(z_dq, value, 0);
					        break;
					    }
					    break;

					case 'I':
					    switch (key[1])
					    {
					    case 'p':
					        mpz_set_str(z_Ip, value, 0);
					        break;
					    }
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
		decrypt_rsa(decrypt, cipher, z_n, z_p, z_q, z_dp, z_dq, z_Ip);
		printf("Decrypted: %s\n", mpz_get_str(NULL, TEXT, decrypt));

		return 0;
	}
}