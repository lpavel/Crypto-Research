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

void readKeyBlock32(word key[], word block[]);
void readKeyBlock64(word key[], word block[]);
void keyExpansion(word key[]);
word S(word w, int pos);
void printBlock(word block[], char* status);
void encrypt(word *xp, word *yp, word key[]);
void decrypt(word *xp, word *yp, word key[]);
word expandEncoding(word w);
#endif
