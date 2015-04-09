#ifndef SIMON_H
#define SIMON_H

#include<stdint.h>

// have only 2 variatns 32 and 64
// 32 => blocksize 64, keysize 128
// 64 => blocksize 128, keysize 128
#define WORDSIZE 64

#if (WORDSIZE == 32)
typedef uint32_t word;
#elif (WORDSIZE == 64)
typedef uint64_t word;
#endif

void createZ();
void readKeyBlock32(word key[][2], word block[][2]);
void readKeyBlock64(word key[][2], word block[][2]);

int keyExpansion(word key[][2]);
void S(word w[2], word dest[2], int pos);

void transformKeyBlock(word key[][2], word block[][2]);

void printBlockHex(word block[][2], char* status);
void printWordBits(word w, int isspac);
void printBlockInitialBits(word block[][2], char* status);
void printBlockDoubleBits (word block[][2], char* status);

word getBitAt(word x, word i);
void and(word x[2], word y[2], word dest[2]);
void xor(word x[2], word y[2], word dest[2]);
void not(word w[2], word dest[2]);
word andWord(word x, word y);
word xorWord(word x, word y);
word notWord(word w);

int encrypt(word x[2], word y[2], word key[][2]);
int decrypt(word x[2], word y[2], word key[][2]);

int verifyFault(word w[2]);

word expandEncoding(word w);
#endif
