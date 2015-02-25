#include<iostream>
#include<vector>
#include<algorithm>
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
bool isEqual(int a[], int b[], const int& Nb);
bool allOnes(int v[]);
void incrementArray(int v[]);
void makeZero(int v[]);
void initializePerm(int perm[]);
void expand(int permA[], int permB[],
	    int a[], int expA[],
	    int b[], int expB[]);
void printExp(int perm[]);
void printReg(int arr[]);

int countDistance(int exp1[], int exp2[]) {
  int dist = 0;
  for(int i = 0; i < 2*Nb; ++i) {
    if(exp1[i] != exp2[i]) {
      ++dist;
    }
  }
  return dist;
}

int main() {
  int a[Nb] = {0,0,0,0} , b[Nb] = {0,0,0,0}; 
  int c[Nb], d[Nb]; // c for Xor, d for AND
  int expA[2*Nb], expB[2*Nb];
  int permA[2*Nb] = {0, 1, 2, 3, 4, 5, 6, 7};
  int permB[2*Nb] = {0, 1, 2, 3, 4, 5, 6, 7};  

  do {
    bool isConstant = true;
    int hammingDistance = -1;
    
    do {
      if(isEqual(permA, permB, 2*Nb)) {
	continue;
      }
      bool conditionA = true;
      bool conditionB = true;
      while(conditionA) {
	while(conditionB) {
	  expand(permA, permB, a, expA, b, expB);
	  if(hammingDistance == -1) {
	    hammingDistance = countDistance(expA, expB);
	  }
	  else {
	    if(hammingDistance != countDistance(expA, expB)) {
	      isConstant = false;
	      break;
	    }
	  }	
	  conditionB = (!allOnes(b));
	  incrementArray(b);
	}
	makeZero(b);
	if(isConstant == false) {
	  break;
	}
	conditionA = (!allOnes(a));
	incrementArray(a);
      }
      makeZero(a);
      if(isConstant){
	cout << "!!!SOLUTION " << "\n permA:";
	printExp(permA);
	cout << "permb:";
	printExp(permB);
	cout << "HD:" << hammingDistance << "\n------\n";
      }
    }while(next_permutation(permA, permA + 2*Nb));
    initializePerm(permA);
  }while(next_permutation(permB, permB + 2*Nb));
  return 0;
}

void printReg(int arr[]) {
  for(int i = 0; i < Nb; ++i) {
    cout << arr[i];
  }
  cout << "\n";
}

void printExp(int perm[]) {
  for(int i = 0; i < (2*Nb); ++i) {
    //    cout << "i:" << i << '\n';
    cout << perm[i];
  }
  cout << "\n";
}

int invert(int x) {
  return (x==0)? (1) : (0);
}

bool allOnes(int v[]) {
  bool truth = true;
  for(int i =0 ; i < Nb; ++i) {
    if(v[i] == 0){
      truth = false;
    }
  }
  return truth;
}

void incrementArray(int v[]) {
  int carry = 1; // start with 1 which basically means increment
  for(int i = 0; i < Nb - 1; ++i) {
    if(carry == 1) {
      ++v[i];
    }
    if(v[i] > 1) {
      v[i] = 0;
      carry = 1;
    }
    else {
      carry = 0;
    }
  }
  if(carry == 1) {
    v[Nb - 1] = 1;
  }
}

void initializePerm(int perm[]) {
  for(int i = 0; i < 2*Nb; ++i) {
    perm[i] = i;
  }
}

void makeZero(int v[]) {
  for(int i = 0; i < Nb; v[i++] = 0);
}

void expand(int permA[], int permB[],
	    int a[], int expA[],
	    int b[], int expB[]) {
  for(int i = 0; i < 2*Nb; ++i) {
    switch(permA[i]) {
    case 0: 
      expA[i] = a[0];
      break;
    case 1:
      expA[i] = a[1];
      break;
    case 2: 
      expA[i] = a[2];
      break;
    case 3: 
      expA[i] = a[3];
      break;
    case 4: 
      expA[i] = invert(a[0]);
      break;
    case 5: 
      expA[i] = invert(a[1]);
      break;
    case 6: 
      expA[i] = invert(a[2]);
      break;
    case 7:
      expA[i] = invert(a[3]);
      break;
    }
    switch(permB[i]) {
    case 0: 
      expB[i] = b[0];
      break;
    case 1: 
      expB[i] = b[1];
      break;
    case 2: 
      expB[i] = b[2];
      break;
    case 3: 
      expB[i] = b[3];
      break;
    case 4: 
      expB[i] = invert(b[0]);
      break;
    case 5: 
      expB[i] = invert(b[1]);
      break;
    case 6: 
      expB[i] = invert(b[2]);
      break;
    case 7:
      expB[i] = invert(b[3]);
      break;
    }
  }
}

bool isEqual(int a[], int b[], const int& Nb) {
  bool truth = true;
  for(int i = 0; i < Nb; ++i) {
    if(a[i] != b[i]) {
      truth = false;
    }
  }
  return truth;
}
