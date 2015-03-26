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

//stick to the definition:
// 1 -> 01
// 0 -> 10
word expandEncoding(word w) {
  int i;
  word newWord = 0;
  word two = 2;
  for(i = 0; i < HALFWORD; ++i) {
    if(!!(w & (ONE << i))) {
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


void createZ() {
  word i,k;
  for(i = 0; i < 5; ++i) {
    for(k = 0; k < 62; ++k) {
      if(z[i][k] == 0) {
	balancedZ[i][k][firstHalf] = expandEncoding(0);
	balancedZ[i][k][secondHalf] = expandEncoding(0);
      }
      else {
	balancedZ[i][k][firstHalf] = expandEncoding(1);
	balancedZ[i][k][secondHalf] = expandEncoding(0);
      }
    }
  }
}

word getBitAt(word x, word i) {
  return !!(x & (ONE << i));
}


word andWord(word x, word y) {
  int i;
  word newWord = 0;
  word two = 2;
  for(i = 0; i < HALFWORD; ++i) {
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
  int i;
  word newWord = 0;
  word two = 2;
  for(i = 0; i < HALFWORD; ++i) {
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
  dest[firstHalf] = andWord(x[firstHalf], y[firstHalf]);
  dest[secondHalf] = andWord(x[secondHalf], y[secondHalf]);
}

void xor(word x[2], word y[2], word dest[2]) {
  dest[firstHalf] = xorWord(x[firstHalf], y[firstHalf]);
  dest[secondHalf] = xorWord(x[secondHalf], y[secondHalf]);
}

void S(word w[2], word dest[2], int pos) {
  word numbits = sizeof(word) * BYTESIZE;
  pos *= 2;
  if(pos >= 0) {
    word carry1 = w[firstHalf] >> (numbits - pos);
    word carry2 = w[secondHalf] >> (numbits - pos);
    dest[firstHalf] = (w[firstHalf] << pos) | carry2;
    dest[secondHalf] = (w[secondHalf] << pos) | carry1;
  }
  else {
    word carry1 = w[firstHalf] << (numbits - (-pos));
    word carry2 = w[secondHalf] << (numbits - (-pos));
    dest[firstHalf] = (w[firstHalf] >> (-pos)) | carry2;
    dest[secondHalf] = (w[secondHalf] >> (-pos)) | carry1;
  }
}

void keyExpansion(word key[keySize * T][2]) {
  int i, p;
  word three[2];
  three[firstHalf] = expandEncoding(3);
  three[secondHalf] = expandEncoding(0);
  for(i = m; i < T; ++i) {
    word tmp[2];
    S(key[i-1], tmp,-3); // tmp = S(key[i], -3)
    if(m==4) {
      xor(tmp, key[i-3], tmp); // tmp ^= key[i-3]
    }
    word retS[2],var1[2], var2[2], notKey[2];
    S(tmp, retS, -1);        // retS = S(tmp, -1)
    xor(tmp, retS , tmp);    // tmp = tmp ^ S(tmp, -1)
    not(key[i-m], notKey);   // notKey = ~key[i-m]
    xor(notKey, tmp, var1);  // var1 = tmp ^ ~key[i-m]
    xor(balancedZ[j][(i-m) % 62], three, var2); // var2 = z[j][(i-m)%62] ^ 3
    xor(var1, var2, key[i]); // key[i] = ~key[i-m] ^ tmp ^ z[j][(i-m)%62] ^ 3
    
    int count1 = 0;
    for(p = 0; p < 64; ++p) {
      if(getBitAt(key[i][firstHalf],p)) count1++;
      if(getBitAt(key[i][secondHalf],p)) count1++;
    }
    if(count1 != 64) {
      count1 = -5;
    }   
  }
}

void encrypt(word x[2], word y[2], word key[keySize * T][2]) {
  for(int i = 0; i < T; ++i) {
    word tmp[2] = {x[firstHalf], x[secondHalf]};
    word oneS[2]; S(x,oneS,1);
    word twoS[2]; S(x,twoS,2);
    word eightS[2]; S(x, eightS, 8);
    
    word term2[2];
    word term3[2];
    xor(twoS, key[i], term3); // term3 = S(x,2) ^ key[i]
    and(oneS, eightS, term2); // term2 = S(x,1) & S(x,8)
    xor(term2, term3, term2); // term2 = (S(x,1) & S(x,8)) ^ S(x,2) ^ key[i]
    xor(y, term2, x);         // x = y ^ (S(x,1) & S(x,8)) ^ S(x,2) ^ key[i]
    y[firstHalf] = tmp[firstHalf];
    y[secondHalf] = tmp[secondHalf];
  }
}


void decrypt(word x[2], word y[2], word key[keySize * T][2]){
  for(int i = T - 1; i >= 0 ; --i) {
    word tmp[2] = {x[firstHalf], x[secondHalf]};
    word oneS[2]; S(y,oneS,1);
    word twoS[2]; S(y,twoS,2);
    word eightS[2]; S(y, eightS, 8);

    word term2[2];
    word term3[2];
    
    x[firstHalf]  = y[firstHalf];
    x[secondHalf] = y[secondHalf];
    xor(tmp, key[i], term2);  // term2 = tmp ^ key[i]
    xor(term2, twoS, term2);  // temp2 = tmp ^ key[i] ^ S(y,2)
    and(oneS, eightS, term3); // temp3 = S(y,1) & S(y,8)
    xor(term2, term3, y);     // y = tmp ^ key[i] ^ S(y,2) ^ (S(y,1) & S(y,8))
  }
}


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
	block[0][firstHalf] = 0x6373656420737265;
	block[1][firstHalf] = 0x6c6c657661727420;
}

void printBlockHex(word block[blockSize][2], char* status) {
  printf("%s: ", status);
  for(int i = 0; i < blockSize; ++i) {
#if (WORDSIZE == 32)
    printf("%x ", block[i][firstHalf]);
#elif (WORDSIZE == 64)
    printf("%llx ", block[i][firstHalf]);
#endif
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
  printf("%s\n", status);
  for(int j = 0; j < blockSize; ++j) {
    printf("block[%d]:",j);
    printWordBits(block[j][firstHalf], TRUE);
    printf("\n");
  }
}

void printBlockDoubleBits(word block[blockSize][2], char* status) {
  printf("%s\n", status);
  for(int j = 0; j < blockSize; ++j) {
    printf("block[%d]:",j);
    printWordBits(block[j][firstHalf], FALSE);
    printf(" ");
    printWordBits(block[j][secondHalf], FALSE);
    printf("\n");
  }
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
