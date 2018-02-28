#ifndef SAMGYUPSAL_DEBUG_H_
#define SAMGYUPSAL_DEBUG_H_

// Includes
#include "chunk.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
void disassembleChunk(Chunk *chunk);
int disassembleInstruction(Chunk *chunk, int offset);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_DEBUG_H_
