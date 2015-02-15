#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

const int Nb = 4;

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
    exp
  }
  
}


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
      }
      incrementArray(b);
    }
    if(isConstant) {
      printPerm(perm);
    }
    makeZero(b);
  }
  
  return 0;
}
