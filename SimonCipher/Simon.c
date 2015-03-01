#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>
#include"Simon.h"

const int BYTESIZE = 8;

const int blockSize = 2;
const int keySize   = 4;
const int T = 44;
const int m = 4;
const int j = 3; // in this particular setup

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

void keyExpansion(word key[]) {
  for(int i = m; i < T; ++i) {
    word tmp = S(key[i-1],-3);
    if(m==4) {
      tmp ^= key[i-3];
    }
    tmp ^= S(tmp, -1);
    key[i] = ~key[i-m] ^ tmp ^ z[j][(i-m) % 62] ^ 3;
  }
  /*
  for(int i = 0; i < T; ++i) {
    printf("%x \n", key[i]);
  }
  */
}

void encrypt(word *xp, word *yp, word key[]) {
  word x = *xp, y = *yp;
  for(int i = 0; i < T; ++i) {
    word tmp = x;
    x = y ^ (S(x,1) & S(x,8)) ^ S(x,2) ^ key[i];
    y = tmp;
  }
  *xp = x; *yp = y;
}

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

void readKeyBlock(word key[], word block[]) {

  key[3] = 0x1b1a1918;
  key[2] = 0x13121110;
  key[1] = 0x0b0a0908;
  key[0] = 0x03020100;

  block[0] = 0x656b696c;
  block[1] = 0x20646e75;
  }

void printBlock(word block[], char* status) {
  printf("%s: %x %x\n", status, block[0], block[1]);
}
