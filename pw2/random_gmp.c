#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gmp.h"

int main(int argc, char* argv[])
{
  	// Big integer declaration
	mpz_t	z_p;
	mpz_t	z_q;

  	// Big integer initialisation
	mpz_init(z_p);
	mpz_init(z_q);
	
  	// Random generator declaration
	gmp_randstate_t	prng;	
	
  	// Random generator initialisation
	gmp_randinit_default(prng);

  	// Seed the random generator
  	gmp_randseed_ui(prng, time(NULL));
  
  	// Pick at random an integer between 0 and 2^200-1
	mpz_urandomb(z_p, prng, 200);
  
  	// Pick at random an integer between 0 and z_p
	mpz_urandomm(z_q, prng, z_p);
  
  	// Print that integer
  	printf("\n");
	gmp_printf("z_p = %Zu\n", z_p);	
	gmp_printf("z_q = %Zu\n", z_q);	
  	printf("\n");	
	
	gmp_randclear(prng);
	
	mpz_clear(z_p);
	mpz_clear(z_q);
	
	return 0;
}












//	gmp_randseed_ui(prng, time(NULL));
