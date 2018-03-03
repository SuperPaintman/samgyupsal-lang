#ifndef SAMGYUPSAL_DEBUG_H_
#define SAMGYUPSAL_DEBUG_H_

// Includes
#include "chunk.h"
#include "token.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
void disassembleChunk(Chunk *chunk);
int disassembleInstruction(Chunk *chunk, int offset);
void printToken(Token *token);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_DEBUG_H_
