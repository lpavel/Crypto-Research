#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

/*
  0 -> 0th
  1 -> 1st
  2 -> 2nd
  3 -> 3rd
  4 -> ~0th
  5 -> ~1st
  6 -> ~2nd
  7 -> ~3rd
*/

const int Nb = 4;

int  invert(int x);
int  getNum(const int& v[]);
bool allOnes(int v[]);
void XOR(int &c[], const int& a[], const int& b[]);
void AND(int &d[], const int& a[], const int& b[]);
void getArray(int& numArr[], const int& num);
void incrementArray(int& v[]);
void makeZero(int& v[]);
void expand(const int& perm[],
	    const int& a[], int& expA[],
	    const int& b[], int& expB[],
	    const int& c[], int& expC[],
	    const int& d[], int& expD[]);



int main() {
  int a[Nb] = {0,0,0,0} , b[Nb] = {0,0,0,0}; 
  int c[Nb], d[Nb]; // c for Xor, d for AND
  int expA[2*Nb], expB[2*Nb], expC[2*Nb], expD[2*Nb];
  int perm[2*Nb] = {0, 1, 2, 3, 4, 5, 6, 7};
    
  while(allOnes(a) != 0) {
    bool isConstant = true;
    while(allOnes(b) != 0) {
      sort(perm, perm + 2 * Nb);
      do {
	XOR(c,a,b);
	AND(d,a,b);
	expand(perm, a,expA, b,expB, c,expC, d,expD);
	if(!constDistance(expD, expA)) {
	  isConstant = false;
	}
      }while(next_permutation(perm, prem + 2*Nb));
      incrementArray(b);
    }
    if(isConstant) {
      printPerm(perm);
    }
    makeZero(b);
  }
  
  return 0;
}

int invert(int x) {
  return (x==0)? (1) : (0);
}

bool allOnes(int v[]) {
  boot truth = true;
  for(int i =0 ; i < Nb; ++i) {
    if(v[i] == 0){
      truth = false;
    }
  }
  return truth;
}

void incrementArray(int& v[]) {
  int carry = 1; // start with 1 which basically means increment
  for(int i = 0; i < Nb - 1; ++i) {
    if(carry == 1) {
      ++v[i];
    }
    if(v[i] > 1) {
      v[i] = 0;
      carry = 1;
    }
  }
}

void makeZero(int& v[]) {
  for(int i = 0; i < Nb; v[i++] = 0);
}

void expand(const int& perm[],int& a[], int& expA[], int& b[], int& expB[],
	    int& c[], int& expC[], int& d[], int& expD[]) {
  for(int i = 0; i < Nb; ++i) {
    switch(perm[i]): {
      case 0: {
	expA[i] = a[0]; expB[i] = b[0];
	expC[i] = c[0];	expD[i] = d[0];
	break;
      }
      case 1: {
	expA[i] = a[1];	expB[i] = b[1];
	expC[i] = c[1];	expD[i] = d[1];
	break;
      }
      case 2: {
	expA[i] = a[2];	expB[i] = b[2];
	expC[i] = c[2];	expD[i] = d[2];
	break;
      }
      case 3: {
	expA[i] = a[3];	expB[i] = b[3];
	expC[i] = c[3];	expD[i] = d[3];
	break;
      }
      case 4: {
	expA[i] = invert(a[0]);	expB[i] = invert(b[0]);
	expC[i] = invert(c[0]);	expD[i] = invert(d[0]);
	break;
      }
      case 5: {
	expA[i] = invert(a[1]);	expB[i] = invert(b[1]);
	expC[i] = invert(c[1]);	expD[i] = invert(d[1]);
	break;
      }
      case 6: {
	expA[i] = invert(a[2]);	expB[i] = invert(b[2]);
	expC[i] = invert(c[2]);	expD[i] = invert(d[2]);
	break;
      }
      case 7: {
	expA[i] = invert(a[3]);	expB[i] = invert(b[3]);
	expC[i] = invert(c[3]);	expD[i] = invert(d[3]);
	break;
      }
      }
  }
}

int getNum(const int& v[]) {
  int num = 0;
  for(int i = 0; i < Nb; ++i) {
    num = num * 2 + v[i];
  }
  return num;
}

void getArray(int& numArr[], const int& num) {
  for(int i = 0; num; ++i) {
    numArr[i] = num % 10;
    num /= 10;
  }

  reverse(numArr, numArr + Nb);
}

void XOR(int &c[], const int& a[], const int& b[]) {
  int numA = getNum(a);
  int numB = getNum(b);
  int numC = numA ^ numB;

  getArray(c, numC);
}


void AND(int &d[], const int& a[], const int& b[]) {
  int numA = getNum(a);
  int numB = getNum(b);
  int numD = numA & numB;

  getArray(d, numD);
}
