#include "compiler.h"

#include <stdlib.h>

#include <stdio.h>

VECTOR_TEMPLATE_IMPLEMENTATIONS(TokenVector, Token *);

static inline bool isAtEnd(Compiler *compiler) {
  return compiler->current && compiler->current->type == TOKEN_EOF;
}

static inline Token *next(Compiler *compiler) {
  if (isAtEnd(compiler)) {
    return compiler->current;
  }

  Token *token = malloc(sizeof(Token));
  *token = lexToken(&compiler->lexer);

  compiler->previous = compiler->current;
  compiler->current = token;

  VECTOR_ADD(TokenVector, &compiler->tokens, token);

  return token;
}

static inline bool checkTokenType(Compiler *compiler, TokenType type) {
  return compiler->current->type == type;
}

static inline bool matchTokenType(Compiler *compiler, TokenType type) {
  if (!checkTokenType(compiler, type)) {
    return false;
  }

  next(compiler);

  return true;
}

static inline void emitCode(Compiler *compiler, OpCode code) {
  writeChunk(&compiler->chunk, code);
}

static void emitLoadConstant(Compiler *compiler, Value value) {
  int val = addConstant(&compiler->chunk, value);

  writeChunk(&compiler->chunk, OP_LOAD_CONSTANT);
  writeChunk(&compiler->chunk, val);
}

static void compileNumber(Compiler *compiler) {
  Value value = strtod(compiler->previous->start, NULL);

  return emitLoadConstant(compiler, value);
}

static void maybeExpressionStatement(Compiler *compiler) {
  if (matchTokenType(compiler, TOKEN_NUMBER)) {
    return compileNumber(compiler);
  }

  printf("Unexpected Token\n");
  exit(1);
}

static void compilePrintStatement(Compiler *compiler) {
  maybeExpressionStatement(compiler);
  emitCode(compiler, OP_PRINT);
}

static void maybeStatement(Compiler *compiler) {
  if (matchTokenType(compiler, TOKEN_PRINT)) {
    return compilePrintStatement(compiler);
  }

  return maybeExpressionStatement(compiler);
}

void initCompiler(Compiler *compiler, const char *source) {
  MAKE_LEXER(lexer, source);
  MAKE_CHUNK(chunk);

  compiler->source = source;
  compiler->lexer = lexer;
  compiler->chunk = chunk;
  compiler->previous = NULL;
  compiler->current = NULL;
  VECTOR_INIT(TokenVector, &compiler->tokens);
}

void freeCompiler(Compiler *compiler) {
  VECTOR_FREE_FULL(TokenVector, &compiler->tokens);
  freeLexer(&compiler->lexer);
  freeChunk(&compiler->chunk);
  initCompiler(compiler, compiler->source);
}

Chunk compile(Compiler *compiler) {
  // Load the first token
  next(compiler);

  while (!matchTokenType(compiler, TOKEN_EOF)) {
    maybeStatement(compiler);
  }

  return compiler->chunk;
}
