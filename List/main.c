#include "list2.h"
int main() {
	List L;
  address P;
  infotype nilai;
	CreateList(&L);
  printf("**Circular List**\n");
  printf("Masukkan nilai List : ");
	do {
		scanf("%d",&nilai);
		if (nilai != -999)
			InsVLast(&L,nilai);
	} while(nilai != -999);
  DelVLast(&L,&nilai);
  printf("Hasil delete : %d\n",nilai);
  PrintInfo(L);
  printf("\n");      
	return 0;
}
