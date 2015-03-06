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

void readKeyBlock32(word key[][2], word block[][2]);
void readKeyBlock64(word key[][2], word block[][2]);

void keyExpansion(word key[][2]);
word S(word w, int pos);

void transformKeyBlock(word key[][2], word block[][2]);

void printBlockHex(word block[][2], char* status);
void printWordBits(word w, int isspac);
void printBlockInitialBits(word block[][2], char* status);
void printBlockDoubleBits (word block[][2], char* status);

void encrypt(word *xp, word *yp, word key[][2]);
void decrypt(word *xp, word *yp, word key[][2]);

word expandEncoding(word w);
#endif
