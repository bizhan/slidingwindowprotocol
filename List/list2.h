/*LIST CIRCULAR WITH TWO POINTER 
Changes due to sliding window ass :
	1. infotype
	2. void Search()
	3.  
*/
#ifndef list2_H
#define list2_H
#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"
#include "../header.h"
/* *** Konstanta *** */
#define Nil NULL

/* *** Definisi Type List *** */
typedef RESPL *infotype;
typedef struct tElmtList *address;
typedef struct tElmtList {
	infotype info;
	address prev;
	address next; 
}ElmtList;

typedef struct {
	address First;
	address Last;
}List;

/*selektor*/
#define Info(P) (P)->info
#define Prev(P) (P)->prev
#define Next(P) (P)->next
#define First(L) ((L).First)
#define Last(L) ((L).Last)

/* ***************** PRIMITIF-PRIMITIF LIST ***************** */

/* *** TEST LIST KOSONG *** */

boolean IsListEmpty (List L); 
/* Mengirim true jika list kosong: First(L) = Nil dan Last(L) = Nil */

/* *** PEMBUATAN LIST KOSONG *** */

void CreateList (List *L);
/* I.S. Sembarang */

/* *** Manajemen Memori *** */

address Alokasi (infotype X);
/* Mengirimkan address hasil alokasi sebuah elemen */
/* Jika alokasi berhasil, maka address tidak Nil, dan misalnya menghasilkan P, maka
Info(P) = X, Next(P) = Nil */
/* Jika alokasi gagal, mengirimkan Nil */

void Dealokasi (address *P);
/* I.S. P terdefinisi */
/* F.S. P dikembalikan ke sistem */
/* Melakukan dealokasi/pengembalian address P */

/* *** PENCARIAN SEBUAH ELEMEN LIST *** */

boolean FSearch (List L,address P);
/* Mencari apakah ada elemen list yang beralamat P */
/* Mengirimkan true jika ada, false jika tidak ada */

address Search (List L,Byte X); 
/* Mencari apakah ada elemen list dengan Info(P).msgno == X */
/* Jika ada, mengirimkan address elemen tersebut */
/* Jika tidak ada, mengirimkan Nil */

/* ***************** PRIMITIF BERDASARKAN NILAI ******************/

/* *** PENAMBAHAN ELEMEN *** */

void InsVFirst (List *L,infotype X);
/* I.S. L mungkin kosong */
/* F.S. Melakukan alokasi sebuah elemen dan menambahkan elemen pertama dengan nilai
X jika alokasi berhasil */

void InsVLast (List *L,infotype X);
/* I.S. L mungkin kosong */
/* F.S. Melakukan alokasi sebuah elemen dan menambahkan elemen list di sebelum
elemen akhir (elemen sebelum elemen dummy); bernilai X
jika alokasi berhasil. */
/*Jika alokasi gagal: I.S. = F.S. */

/* *** PENGHAPUSAN ELEMEN *** */

void DelVFirst (List *L,infotype *X);
/* I.S. List L tidak kosong */
/* F.S. Elemen pertama list dihapusnilai info disimpan pada X */
/*dan alamat elemen pertama didealokasi */

void DelVLast (List *L,infotype *X);
/* I.S. List tidak kosong */
/* F.S. Elemen sebelum dummy dihapusnilai info disimpan pada X */
/* dan alamat elemen terakhir sebelum dummy di-dealokasi */

/* ***************** PRIMITIF BERDASARKAN ALAMAT ***************** */

/* *** PENAMBAHAN ELEMEN BERDASARKAN ALAMAT *** */

void InsertFirst (List *L,address P);
/* I.S. Sembarang, P sudah dialokasi */
/* F.S. Menambahkan elemen ber-address P sebagai elemen pertama */

void InsertAfter (List *L,address P,address Prec);
/* I.S. Prec pastilah elemen list dan bukan elemen terakhir, */
/*P sudah dialokasi */
/* F.S. Insert P sebagai elemen sesudah elemen beralamat Prec */

void InsertLast (List *L,address P);
/* I.S. Sembarang, P sudah dialokasi */
/* F.S. P ditambahkan sebagai elemen terakhir yang baru, */
/*yaitu menjadi elemen sebelum dummy */

/* *** PENGHAPUSAN SEBUAH ELEMEN *** */

void DelFirst (List *L,address *P);
/* I.S. List tidak kosong */
/* F.S. P adalah alamat elemen pertama list sebelum penghapusan */
/*
Elemen list berkurang satu (mungkin menjadi kosong); */
/* First element yg baru adalah suksesor elemen pertama yang lama */
void DelLast (List *L,address  *P);
/* I.S. List tidak kosong */
/* F.S. P adalah alamat elemen terakhir sebelum dummy pada list sebelum
penghapusan */
/*
Elemen list berkurang satu (mungkin menjadi kosong); */
void DelAfter ( List *L,address *Pdel,address Prec);
/* I.S. List tidak kosong. Prec adalah anggota list. */
/* F.S. Menghapus Next(Prec);Pdel adalah alamat elemen list yang dihapus */
/* ***************** PROSES SEMUA ELEMEN LIST ***************** */
void PrintInfo (List L);
/* I.S. List mungkin kosong */
/* F.S. Jika list tidak kosong, */
/* Semua info.msgno yg disimpan pada elemen list diprint */
/* Jika list kosong, hanya menuliskan "list kosong" */


#endif