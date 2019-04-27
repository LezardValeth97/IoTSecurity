#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "gmp.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[]){
	if(argc != 3){
		printf("Enter k-bit and public exponent!\n");
		return -1;
	}

	else{
		/* create file key.txt if not exist ! don't want to read |
				if the file already exist -> clean everything */
		int keyfile = open("key.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		if(keyfile){
			dup2(keyfile, 1);
		}
	
		int k = atoi(argv[1]), e = atoi(argv[2]);

		mpz_t	z_p, z_q, z_p_1, z_q_1, z_phi, z_exponent, z_e, z_d, z_n, gcd;	// Big interger declaration	
		mpz_inits(z_p, z_q, z_p_1, z_q_1, z_phi, z_exponent, z_e, z_d, z_n, gcd, NULL);	// Big integer initialisation
		gmp_randstate_t prng;	// Random generator declaration
		gmp_randinit_default(prng);	// Random generator initialisation
		gmp_randseed_ui(prng, time(NULL));	// Seed the random generator
		mpz_set_ui(z_e, e); // Assign the value e to z_e
		mpz_ui_pow_ui(z_exponent, 2, k-1); // Assign the value z_exponent = 2^k-1

		int isPrime = 0;
		while(!isPrime){
			mpz_urandomb(z_p, prng, k/2-1);
			mpz_add(z_p, z_p, z_exponent);
			isPrime = mpz_probab_prime_p(z_p, 5); // check prime number p
		}

		isPrime = 0;
		int invertible = 0;
		while(!isPrime || mpz_cmp(z_p, z_q) == 0 || !invertible){
			// check e mod phi(n) exist
			mpz_urandomb(z_q, prng, k/2-1);
			mpz_add(z_q, z_q, z_exponent);
			isPrime = mpz_probab_prime_p(z_q, 5); // check prime number q

			mpz_sub_ui(z_p_1, z_p, 1); // Assign p = p-1
			mpz_sub_ui(z_q_1, z_q, 1); // Assign q = q-1

			mpz_gcd(gcd, z_e, z_p_1);
			if(mpz_cmp_ui(gcd, 1) != 0) 
				continue;
			mpz_gcd(gcd, z_e, z_q_1);
			if(mpz_cmp_ui(gcd, 1) != 0)
				continue;

			mpz_mul(z_phi, z_q_1,z_p_1); // phi = (p-1)*(q-1)
			// Assign computed inversion of z_e modulo z_phi
			invertible = mpz_invert(z_d, z_e, z_phi); 
		}
		mpz_mul(z_n, z_p, z_q);
	  
	  	// Print the integer
		gmp_printf("e = 0x%ZX\n", z_e);	
		gmp_printf("d = 0x%ZX\n", z_d);	
		gmp_printf("n = 0x%ZX", z_n);
	  	printf("\n");	
		
		gmp_randclear(prng);
		mpz_clears(z_p, z_q, z_exponent, z_phi, z_d, z_e, z_n, NULL);

		if(keyfile){
			//to ensure the result wrote to a file is written out on disk
			fflush(stdout); 
			close(keyfile);
		}
		return 0;
	}
}