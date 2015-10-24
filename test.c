#include "cksum.h"

int main(int argc, char *argv[]){
	char a[] = "aaaa";

	printf("0x%04x\n", a);

	Byte hasil = cksum(a, 4);

	printf("0x%04x :: %d\n", hasil, hasil);
	return 0;
}