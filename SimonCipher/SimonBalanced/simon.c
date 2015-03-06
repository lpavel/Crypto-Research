#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>
#include"simon.h"

const int BYTESIZE = 8;
const int HALFWORD = sizeof(word) * BYTESIZE / 2;
const int blockSize = 2;
const int firstHalf = 0;
const int secondHalf = 1;
const word ONE_ZERO = 2;
const word ZERO_ONE = 1;

#define TRUE  1
#define FALSE 0


#if (WORDSIZE == 32)
const int keySize = 4;
const int T = 44;
const int m = 4;
const int j = 3; // in this particular setup
#elif (WORDSIZE == 64)
const int keySize = 2;
const int T = 68;
const int m = 2;
const int j = 2; // in this particular setup
#endif


const uint32_t z[5][62] = {
  {1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,
   1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0},
  {1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,
   1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0},
  {1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,
   0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1},
  {1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,
   0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1},
  {1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,
   0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1}
};

word S(word w, int pos) {
  int numbits = sizeof(w) * BYTESIZE;
  if(pos >= 0) {
    return (w << pos) | (w >> (numbits - pos));
  }
  else {
    return (w >> (-pos)) | (w << (numbits - (-pos)));
  }
}

/* // TODO: needs to be updated */
/* void keyExpansion(word key[keySize * T][2]) { */
/*   for(int i = m; i < T; ++i) { */
/*     word tmp = S(key[i-1],-3); */
/*     if(m==4) { */
/*       tmp ^= key[i-3]; */
/*     } */
/*     tmp ^= S(tmp, -1); */
/*     key[i] = ~key[i-m] ^ tmp ^ z[j][(i-m) % 62] ^ 3; */
/*   } */
/*   /\*  for(int i = 0; i < T; ++i) { */
/*     printf("%x \n", key[i]); */
/*     }*\/ */
/* } */

/*
// TODO: needs to be updated
void encrypt(word *xp, word *yp, word key[]) {
  word x = *xp, y = *yp;
  for(int i = 0; i < T; ++i) {
    word tmp = x;
    x = y ^ (S(x,1) & S(x,8)) ^ S(x,2) ^ key[i];
    y = tmp;
  }
  *xp = x; *yp = y;
}

// TODO: needs to be updated
void decrypt(word *xp, word *yp, word key[]){
  word x = *xp, y = *yp;
  for(int i = T - 1; i >= 0 ; --i) {
    word tmp = x;
    x = y;
    y = tmp ^ key[i] ^ S(y,2) ^ (S(y,1) & S(y,8));
  }
  *xp = x;
  *yp = y;
}
*/
void readKeyBlock32(word key[][2], word block[][2]) {

  key[3][firstHalf] = 0x1b1a1918;
  key[2][firstHalf] = 0x13121110;
  key[1][firstHalf] = 0x0b0a0908;
  key[0][firstHalf] = 0x03020100;

  block[0][firstHalf] = 0x656b696c;
  block[1][firstHalf] = 0x20646e75;
}

void readKeyBlock64(word key[keySize * T][2], word block[blockSize][2]) {
  
  key[1][firstHalf] = 0x0f0e0d0c0b0a0908;
  key[0][firstHalf] = 0x0706050403020100;
  
  block[0][firstHalf] = 0x6c61766975716520;
  block[1][firstHalf] = 0x7469206564616d20;
}

void printBlockHex(word block[blockSize][2], char* status) {
  printf("%s: ", status);
  for(int i = 0; i < blockSize; ++i) {
    printf("%llx ", block[i][firstHalf]);
  }
  printf("\n");
}

void printWordBits(word w, int isspace) {
  word one = 1; // again very important for casting
  for(int i = 0; i < sizeof(word) * BYTESIZE; ++i) {
    int bit = !!(w & (one << i));
    if(isspace) {
      printf("%d ", bit);
    }
    else {
      printf("%d", bit);
    }
  }
}

void printBlockInitialBits(word block[blockSize][2], char* status) {
  for(int j = 0; j < blockSize; ++j) {
    printf("block[%d]:",j);
    printWordBits(block[j][firstHalf], TRUE);
    printf("\n");
  }
}

void printBlockDoubleBits(word block[blockSize][2], char* status) {
  for(int j = 0; j < blockSize; ++j) {
    printf("block[%d]:",j);
    printWordBits(block[j][firstHalf], FALSE);
    printf(" ");
    printWordBits(block[j][secondHalf], FALSE);
    printf("\n");
  }
}


//stick to the definition:
// 1 -> 10
// 0 -> 01
word expandEncoding(word w) {
  word newWord = 0; 
  word one = 1; // very important for auto casting
  word two = 2;
  for(int i = 0; i < HALFWORD; ++i) {
    //    printf("i: %d     bit: %d -> ", i, !!(w & (one << i)));
    if(!!(w & (one << i))) {
      word append = (ONE_ZERO << (two*i)); 
      //      printWordBits(append, FALSE);
      newWord |= append;
    }
    else{
      word append = (ZERO_ONE << (two*i));
      //      printWordBits(append, FALSE);
      newWord |= append;
    }
    //    printf("\n");
  }
  return newWord;
}

// asumes regular input and creates balanced encoding
void transformKeyBlock(word key[keySize * T][2], word block[blockSize][2]) {
  // first transform key
  for(int i = 0; i < keySize; ++i) {
    word auxKey = key[i][firstHalf];
    key[i][firstHalf]  = expandEncoding(auxKey);
    key[i][secondHalf] = expandEncoding(auxKey >> HALFWORD);
  }
  // now transform block
  for(int i = 0; i < blockSize; ++i) {
    word auxBlock = block[i][firstHalf];
    block[i][firstHalf]  = expandEncoding(auxBlock);
    block[i][secondHalf] = expandEncoding(auxBlock >> HALFWORD);
  }
}
