#include"Simon.h"

extern int BYTESIZE;

extern int blockSize;
extern int keySize;
extern int T;
extern int m;
extern int j; // in this particular setup

extern uint32_t z;

int main() {
  word key[keySize * T];
  word block[blockSize];
  readKeyBlock(key, block);

  keyExpansion(key);
  encrypt(&block[0], &block[1], key);
  printBlock(block, "encrypted");

  decrypt(&block[0], &block[1], key);
  printBlock(block, "decrypted");
  
  return 0;
}
