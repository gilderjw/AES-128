/* 
 * AES.c
 * Author: Jim Gildersleeve
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rcon.h"
#include "sbox.h"

//constants for generating the key schedule
#define KEY_SIZE 16
#define B 176


void parseFile(char* filename);
uint8_t* generateKeySchedule();
void print4xN(uint8_t* schedule, uint8_t N);

static uint32_t iterations;
static uint32_t rounds;
static uint8_t key[KEY_SIZE];
static uint8_t plaintext[KEY_SIZE];


int main(int argc, char const *argv[]) {
  parseFile((char *)argv[1]);

  printf("%d iterations\n", iterations);
  printf("%d rounds\n", rounds);

  for (int i = 0; i < 16; i++){
    printf("%02x ", key[i]);
  }
  printf(" key\n");
  for (int i = 0; i < 16; i++){
    printf("%02x ", plaintext[i]);
  }
  printf(" plaintext\n");

  generateKeySchedule();
  return 0;
}

void parseFile(char* fileName) {
  FILE* input = fopen(fileName, "r");
  fscanf(input, "%d%d", &iterations, &rounds);

  for (uint8_t i = 0; i < 16; i++){
    fscanf(input, "%2hhx", &key[i]);
  }

  for (int i = 0; i < 16; i++) {
    fscanf(input, "%2hhx", &plaintext[i]);
  }
}

/**
 * This function rotates a 32-bit integer 
 */
uint32_t getNextKeyColumn(uint32_t input, uint8_t rconValue) {
  uint32_t shiftedInput = (input << 8) | (input >> 24); //perform rotate on input

  uint8_t* shiftedInputbuffer = (uint8_t*) &shiftedInput; //lets call it an array of bytes

  for (int i = 0; i < 4; i++) {
    shiftedInputbuffer[i] = rsbox[shiftedInputbuffer[i]]; //replace bytes with sbox contents
  }

  shiftedInputbuffer[0] ^=  rcon[rconValue];

  return shiftedInput;
}

uint8_t* generateKeySchedule(){
  uint8_t rconIteration = 1;
  uint32_t* keySchedule = (uint32_t*) malloc(B/4*sizeof(uint32_t));

  for (int i = 0; i < 4; i++) {
    memcpy(&keySchedule[i], &key[i*4], sizeof(uint8_t) * 4);
  }

  // uint32_t newColumn = getNextKeyColumn(keySchedule[4], rconIteration);
  // memcpy(keySchedule, &newColumn, sizeof(uint32_t));

  print4xN((uint8_t*) keySchedule, 6);

}

void print4xN(uint8_t* buf, uint8_t N) {

  for (int i = 0; i < 4; i++){
    for (int j = 0; j < N; j++) {
      printf("%02x ", buf[4*j+i]);
    }
    printf("\n");
  }
}