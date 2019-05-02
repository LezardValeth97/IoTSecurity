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
		mpz_t z_p, z_q, z_dp, z_dq, z_Ip, z_n, z_s, z_h, z_mp, z_mq, z_temp;
        mpz_inits(z_p, z_q, z_dp, z_dq, z_Ip, z_n, z_s, z_h, z_mp, z_mq, z_temp, NULL);

		FILE *input = fopen(argv[2], "rb");
		if(input){
			printf("Reading from %s\n", argv[2]);
			while(fscanf(input, "%m[^=]=%ms\n", &key, &value) == 2){
				switch(key[0]){
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
        mpz_powm(z_mp, z_h, z_dp, z_p);
        mpz_powm(z_mq, z_h, z_dq, z_q);
        mpz_sub(z_temp, z_mq, z_mp);   // temp = mq - mp;
        mpz_mul(z_temp, z_temp, z_Ip); // temp *= Ip;
        mpz_mod(z_temp, z_temp, z_q);  // temp %= q;
        mpz_mul(z_temp, z_temp, z_p);  // temp *= p;
        mpz_add(z_s, z_mp, z_temp);    // m = mp + temp
		gmp_printf("\n MD5 Signature: %Zx\n", z_s);
		
		return 0;
	}
}