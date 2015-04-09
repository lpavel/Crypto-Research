#include"Simon.h"
#include<stdio.h>
extern int BYTESIZE;

extern int blockSize;
extern int keySize;
extern int T;
extern int m;
extern int j; // in this particular setup

#define TRUE  1
#define FALSE 0

int main() {
  word key[keySize * T][2];
  word block[blockSize][2];

  createZ();
  
#if (WORDSIZE == 32)
  readKeyBlock32(key, block);
#elif (WORDSIZE == 64)
  readKeyBlock64(key, block);
#endif
  /*
  printBlockInitialBits(block, "initial block");
  printBlockInitialBits(key, "initial key");
  */  transformKeyBlock(key, block);
  /*printBlockDoubleBits(block, "initial Transformed block");
  printBlockDoubleBits(key, "initial Transformed key");
  */
  if(keyExpansion(key) == FALSE) {
    printf("Fault Detected\n");
    return -1; // detect a fault and abandon everything
  }
  
  if(encrypt(block[0], block[1], key) == FALSE) {
    printf("Fault Detected\n");
    return -1; // detect a fault and abandon everything
  }
  //  printBlockDoubleBits(block, "encrypted");
  printf("%llx %llx %llx %llx \n",
	 block[0][1], block[0][0],
	 block[1][1], block[1][0]);
  if(decrypt(block[0], block[1], key) == FALSE) {
    printf("Fault Detected\n");
    return -1; // detect a fault and abandon everything    
  }
  //  printBlockDoubleBits(block, "decrypted");
  
  return 0;
}
