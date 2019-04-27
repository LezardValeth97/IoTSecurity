#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int mod(int &a, int &b){
	if(b<0){
		return mod(a, -b);
	}

	int ret = a  % b;
	if(ret = 0){
		ret+=b;
	}
	return ret;
}

int main(){
	a = 123456;
	b = 435959;
	c = mod(a, b);
	return 0;
}