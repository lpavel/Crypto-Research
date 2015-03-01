#ifndef SIMON_H
#define SIMON_H

#include<stdint.h>

typedef uint32_t word;

void readKeyBlock(word key[], word block[]);
void keyExpansion(word key[]);
word S(word w, int pos);
void printBlock(word block[], char* status);
void encrypt(word *xp, word *yp, word key[]);
void decrypt(word *xp, word *yp, word key[]);


#endif
