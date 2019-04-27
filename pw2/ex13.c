#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include <unistd.h>
#include <ctype.h>

int main(int argc, char * argv[]){
	if(argc != 3 ){
		printf("Please enter filename and RSA key: ");
		return -1;
	}else{
		char *key, *value, *string;
		char md5[32]; 
		mpz_t z_s, z_h, z_d, z_n;
		mpz_inits(z_s, z_h, z_d, z_n, NULL);

		FILE *input = fopen(argv[2], "rb");
		if(input){
			printf("Reading from %s\n", argv[2]);
			while(fscanf(input, "%m[^=]=%ms\n", &key, &value) == 2){
				switch(key[0]){
					case 'n':
						mpz_set_str(z_n, value, 0);
						break;
					case 'd':
						mpz_set_str(z_d, value, 0);
						break;
				}
				free(key);
				free(value);
			}
			fclose(input);
		}

		string = malloc(sizeof("md5sum | cut -d\" \" -f 1") + sizeof(argv[1]));
		sprintf(string, "md5sum %s| cut -d\" \" -f 1", argv[1]);

	 	
		FILE *cipherhash = popen(string, "r");
		if(cipherhash){
			fgets(md5, 32, cipherhash);
			pclose(cipherhash);
		}

		printf("MD5: %s\n", md5);
		mpz_set_str(z_h, md5, 16);
		mpz_powm(z_s, z_h, z_d, z_n);
		gmp_printf("\n MD5 Signature: %Zx\n", z_s);
		
		return 0;
	}
}