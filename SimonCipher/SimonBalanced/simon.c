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
const word ONE = 1;
const word ZERO = 0;

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

word balancedZ[5][62][2];

void createZ() {
  for(int i = 0; i < 5; ++i) {
    for(int j = 0; j < 62; ++j) {
      balancedZ[i][j][firstHalf]  = expandEncoding(0);
      balancedZ[i][i][secondHalf] = expandEncoding(1);
    }
  }
}

void S(word w[2], word dest[2], int pos) {
  word numbits = sizeof(word) * BYTESIZE;
  if(pos >= 0) {
    word carry1 = w[firstHalf]  >> (numbits - pos);
    word carry2 = w[secondHalf] >> (numbits - pos);
    dest[firstHalf]  = (w[firstHalf] << pos) | carry2; 
    dest[secondHalf] = (w[secondHalf] << pos) | carry1;
  }
  else {
    word carry1 = w[firstHalf] << (numbits - (-pos));
    word carry2 = w[secondHalf] << (numbits - (-pos));
    dest[firstHalf] = (w[firstHalf] >> (-pos)) | carry2;
    dest[secondHalf] = (w[secondHalf] >> (-pos)) | carry1;
  }
}

// TODO: needs to be updated
void keyExpansion(word key[keySize * T][2]) {
  word three[2];
  three[firstHalf] = expandEncoding(3);
  three[secondHalf] = expandEncoding(0);
  for(int i = m; i < T; ++i) {
    word tmp[2];
    S(key[i-1], tmp,-3);
    if(m==4) {
      xor(tmp, key[i-3], tmp);
    }
    word retS[2],var1[2], var2[2], notKey[2];
    S(tmp, retS, -1);
    xor(tmp, retS , tmp);
    not(key[i-m], notKey);    
    xor(notKey, tmp, var1);
    xor(balancedZ[j][(i-m) % 62], three, var2); // still need to do this modulo shit
    xor(var1, var2, key[i]);
  }
  /*  for(int i = 0; i < T; ++i) {
    printf("%x \n", key[i]);
    }*/
}

/*
// TODO: needs to be updated
void encrypt(word *xp[], word *yp[], word key[]) {
  word x[] = *xp, y[] = *yp;
  for(int i = 0; i < T; ++i) {
    word tmp = x;
    x = y ^ (S(x,1) & S(x,8)) ^ S(x,2) ^ key[i];
    y = tmp;
  }
  *xp = x; *yp = y;
}
*/
/*
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
  for(int i = 0; i < sizeof(word) * BYTESIZE; ++i) {
    int bit = !!(w & (ONE << i));
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
  word two = 2;
  for(int i = 0; i < HALFWORD; ++i) {
    //    printf("i: %d     bit: %d -> ", i, !!(w & (one << i)));
    if(!!(w & (ONE << i))) {
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

word getBitAt(word x, word i) {
  return x & (ONE << i);
}

word andWord(word x, word y) {
  word newWord = 0;
  word two = 2;
  for(int i = 0; i < HALFWORD; ++i) {
    word mid = getBitAt(x, 2*i) & getBitAt(y, 2*i);
    if(mid != 0) {
      word append = (ONE_ZERO << (two*i)); 
      newWord |= append;
    }
    else{
      word append = (ZERO_ONE << (two*i));
      newWord |= append;
    }
  }
  return newWord;
}

word xorWord(word x, word y) {
  word newWord = 0;
  word two = 2;
  for(int i = 0; i < HALFWORD; ++i) {
    word mid = getBitAt(x,2*i) ^ getBitAt(y, 2*i);
    if(mid & 1) {
      word append = (ONE_ZERO << (two*i)); 
      newWord |= append;
    }
    else{
      word append = (ZERO_ONE << (two*i));
      newWord |= append;
    }
  }
  return newWord;  
}

word notWord(word w) {
  uint64_t maxUInt = 0xffffffffffffffff;
  word maxWord = (word) maxUInt;
  return w ^ maxWord;
}

void not(word w[2], word dest[2]) {
  dest[firstHalf] = notWord(w[firstHalf]);
  dest[secondHalf] = notWord(w[secondHalf]);
}

void and(word x[2], word y[2], word dest[2]) {
  dest[firstHalf]  = andWord(x[firstHalf], y[firstHalf]);
  dest[secondHalf] = andWord(x[secondHalf], y[secondHalf]);
}

void xor(word x[2], word y[2], word dest[2]) {
  dest[firstHalf]  = xorWord(x[firstHalf], y[firstHalf]);
  dest[secondHalf] = xorWord(x[secondHalf], y[secondHalf]);
}
