#include"Simon.h"
#include<stdio.h>
extern int BYTESIZE;

extern int blockSize;
extern int keySize;
extern int T;
extern int m;
extern int j; // in this particular setup

int main() {
  word key[keySize * T][2];
  word block[blockSize][2];

#if (WORDSIZE == 32)
  readKeyBlock32(key, block);
#elif (WORDSIZE == 64)
  readKeyBlock64(key, block);
#endif

  printBlock(block, "initial");
  transformKeyBlock(key,Block);
  printBlock(block, "initial Transformed");
  
  
  keyExpansion(key);
  encrypt(&block[0], &block[1], key);
  printBlock(block, "encrypted");

  decrypt(&block[0], &block[1], key);
  printBlock(block, "decrypted");
  
  return 0;
}
