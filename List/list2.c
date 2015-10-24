#include "list2.h"
/* ***************** PRIMITIF-PRIMITIF LIST ***************** */

/* *** TEST LIST KOSONG *** */

boolean IsListEmpty (List L) 
/* Mengirim true jika list kosong: First(L) = dummy@ dan Last(L) = dummy@ */
{ return (First(L) == Nil && Last(L) == Nil); }

/* *** PEMBUATAN LIST KOSONG *** */

void CreateList (List *L)
/* I.S. Sembarang */
/* F.S. Terbentuk list L kosong, dengan satu elemen dummy */
{
	First(*L) = Nil; 
	Last(*L) = Nil;
}

/* *** Manajemen Memori *** */

address Alokasi (infotype X)
/* Mengirimkan address hasil alokasi sebuah elemen */
/* Jika alokasi berhasil, maka address tidak Nil, dan misalnya menghasilkan P, maka
Info(P) = X, Next(P) = Nil */
/* Jika alokasi gagal, mengirimkan Nil */
{
	address P;
	P = (address)malloc(sizeof(infotype));
	if (P != Nil)
	{
		Info(P) = X;
		Next(P) = Nil;
		Prev(P) = Nil;
	}
	return P;
}
void Dealokasi (address *P)
/* I.S. P terdefinisi */
/* F.S. P dikembalikan ke sistem */
/* Melakukan dealokasi/pengembalian address P */
{	free(*P);}

/* *** PENCARIAN SEBUAH ELEMEN LIST *** */

boolean FSearch (List L,address P) {
/* Mencari apakah ada elemen list yang beralamat P */
/* Mengirimkan true jika ada, false jika tidak ada */
  address Pt;
/*algoritma*/
  if(IsListEmpty(L)) return false;
  else {
  	Pt = First(L);
  	do {
  		if(Pt==P) return true;
  		Pt = Next(Pt);
  	} while(Pt!=First(L));
  	return false;
  }
}

address Search (List L,infotype X) {
/* Mencari apakah ada elemen list dengan Info(P) = X */
/* Jika ada, mengirimkan address elemen tersebut */
/* Jika tidak ada, mengirimkan Nil */
	address P = Nil;
	/*algoritma*/
	if(IsListEmpty(L)) return P;
	else {
		P = First(L);
		do {
			if(Info(P) == X) return P;
			P = Next(P);
		} while(P!=First(L));
		return P;
	}
}

/* ***************** PRIMITIF BERDASARKAN NILAI ******************/

/* *** PENAMBAHAN ELEMEN *** */

void InsVFirst (List *L,infotype X) {
/* I.S. L mungkin kosong */
/* F.S. Melakukan alokasi sebuah elemen dan menambahkan elemen pertama dengan nilai
X jika alokasi berhasil */
	address P = Alokasi(X);
	if(P!=Nil) 
		InsertFirst(L,P);
	else
		printf("Allocation failed. \n");
}

void InsVLast(List *L,infotype X) {
/* I.S. L mungkin kosong 
   F.S. Melakukan alokasi sebuah elemen dan menambahkan elemen list di sebelum 
   elemen akhir (elemen sebelum elemen dummy) bernilai X jika alokasi berhasil.
   Jika alokasi gagal: I.S. = F.S. */
	address P = Alokasi(X);
	if(P!=Nil)
		InsertLast(L,P);
	else
		printf("Allocation failed. \n");
}

/* *** PENGHAPUSAN ELEMEN *** */

void DelVFirst(List *L,infotype *X) { 
/* I.S. List L tidak kosong */
/* F.S. Elemen pertama list dihapus. nilai info disimpan pada X */
/*dan alamat elemen pertama didealokasi */
	address P;
	DelFirst(L,&P);
	(*X) = Info(P);
	Dealokasi(&P);
}

void DelVLast(List *L,infotype *X) {
/* I.S. List tidak kosong */
/* F.S. Elemen sebelum dummy dihapus nilai info disimpan pada X */
/*dan alamat elemen terakhir sebelum dummy di-dealokasi */
	address P;
	DelLast(L,&P);
	(*X) = Info(P);
	Dealokasi(&P);
}


/* ***************** PRIMITIF BERDASARKAN ALAMAT ***************** */

/* *** PENAMBAHAN ELEMEN BERDASARKAN ALAMAT *** */

void InsertFirst (List *L,address P)
/* I.S. Sembarang, P sudah dialokasi */
/* F.S. Menambahkan elemen ber-address P sebagai elemen pertama */
{ 
	/* Kamus Lokal */
	address Pt;
	/* Algoritma */
	if (IsListEmpty(*L)) {
		Next(P) = P;
		Prev(P) = P;
		Last(*L) = P;
	}
	else {
		Pt = First(*L);
		Prev(Pt) = P;
		Next(P) = Pt;
		Pt = Last(*L);
		Next(Pt) = P;
		Prev(P) = Pt;
	}	
	First(*L) = P;
}

void InsertAfter (List *L,address P,address Prec) {
/*	I.S. Prec pastilah elemen list dan bukan elemen terakhir, 
	P sudah dialokasi  
	F.S. Insert P sebagai elemen sesudah elemen beralamat Prec */
	if(FSearch(*L,P))
		printf("Address tidak boleh element dari List yang sudah ada.\n");
	else {
		if(Prec==Last(*L)) InsertLast(L,P);
		else {	
			address Pt = Next(Prec);
			Next(Prec) = P;
			Prev(P) = Prec;
			Next(P) = Pt;
			Prev(Pt) = P;
		}
	}
}

void InsertLast (List *L,address P) {
/* I.S. Sembarang, P sudah dialokasi */
/* F.S. P ditambahkan sebagai elemen terakhir yang baru, */
/*yaitu menjadi elemen sebelum dummy */
	//Kamus Lokal
	address Pt;
	//Algoritma
	if (IsListEmpty(*L)) 
		InsertFirst(L,P);
	else {
		Pt = First(*L);
		Prev(Pt) = P;
		Next(P) = Pt;
		Pt = Last(*L);
		Next(Pt) = P;
		Prev(P) = Pt;
		Last(*L) = P;
	}
}


/* *** PENGHAPUSAN SEBUAH ELEMEN *** */

void DelFirst (List *L,address *P) {
/*	I.S. List tidak kosong 
	F.S. P adalah alamat elemen pertama list sebelum penghapusan 
	Elemen list berkurang satu (mungkin menjadi kosong) 
	First element yg baru adalah suksesor elemen pertama yang lama */
//Kamus Lokal
	address Pt;	
//Algoritma
	(*P) = First(*L);
	if ((*P) == Last(*L)) {
		First(*L) = Nil;
		Last(*L) = Nil;
	}
	else {
		Pt = Next(*P);
		Prev(Pt) = Last(*L);
		First(*L) = Pt;
		Pt = Last(*L);
		Next(Pt) = First(*L);
	}
}

void DelLast (List *L,address  *P) {
/* I.S. List tidak kosong */
/* F.S. P adalah alamat elemen terakhir sebelum dummy pada list sebelum
penghapusan */
/*Elemen list berkurang satu (mungkin menjadi kosong) */
//Kamus Lokal
	address Pt;
//Algoritma
	(*P) = Last(*L);
	if((*P) == First(*L))
		DelFirst(L,P);
	else {
		Pt = Prev(*P);
		Next(Pt) = First(*L);
		Last(*L) = Pt;
		Pt = First(*L);
		Prev(Pt) = Last(*L);
	}
}
void DelAfter(List *L,address *Pdel,address Prec) {
/*	I.S. List tidak kosong. Prec adalah anggota list.
	F.S. Menghapus Next(Prec).
	Pdel adalah alamat elemen list yang dihapus */
//Kamus Lokal
	address Pt;
//Algoritma 
	if(Prec==Last(*L)) DelFirst(L,Pdel);
	else if(Prec==Prev(First(*L))) DelLast(L,Pdel);
	else {
		(*Pdel) = Next(Prec);
		Pt = Next(*Pdel);
		Next(Prec) = Pt;
		Prev(Pt) = Prec;
	}
}
/* ***************** PROSES SEMUA ELEMEN LIST ***************** */

void PrintInfo (List L) {
/* I.S. List mungkin kosong */
/* F.S. Jika list tidak kosong, */
/* Semua info yg disimpan pada elemen list (kecuali dummy) diprint */
/* Jika list kosong, hanya menuliskan "list kosong" */
/* Kamus Lokal */
	address P;
/* Algoritma */
	if (IsListEmpty(L)) printf("List Kosong \n"); 
	else {
		P = First(L);
		printf("List : ");
		do {
			printf("%d ", Info(P));
			P = Next(P);
		} while (P != First(L));
		printf("\n");
	}	
}