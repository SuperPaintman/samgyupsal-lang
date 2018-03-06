#include "compiler.h"

#include <stdlib.h>

// TODO
#include "debug.h"
#include <stdio.h>

// Declarations
/**
 * ```ebnf
 * program
 *   = { stmt }
 *   ;
 * ```
 */
static void compileProgram(Compiler *compiler);

/**
 * ```ebnf
 * stmt
 *   = stmt_expr
 *   | stmt_print
 *   ;
 * ```
 */
static void compileStmt(Compiler *compiler);

/**
 * ```ebnf
 * stmt_print
 *   = 'print' expr
 *   ;
 * ```
 */
static void compileStmtPrint(Compiler *compiler);

/**
 * ```ebnf
 * stmt_expr
 *   = expr
 *   ;
 * ```
 */
static void compileStmtExpr(Compiler *compiler);

/**
 * ```ebnf
 * expr
 *   = term
 *   | expr_binary
 *   ;
 * ```
 */
static void compileExpr(Compiler *compiler);

/**
 * ```ebnf
 * expr_binary
 *   = term '+' term
 *   | term '-' term
 *   ;
 * ```
 */
static void compileExprBinary(Compiler *compiler);

/**
 * ```ebnf
 * term
 *   = IDENTIFIER
 *   | NUMBER
 *   ;
 * ```
 */
static void compileTerm(Compiler *compiler);

/**
 * ```ebnf
 * NUMBER
 * ```
 */
static void compileNumber(Compiler *compiler);

// Vectors
VECTOR_TEMPLATE_IMPLEMENTATIONS(TokenVector, Token *);

// Helpers
// TOOD
static void debugCompiler(Compiler *compiler) {
  if (compiler->previous) {
    printf("  previous:\n    ");
    printToken(compiler->previous);
  }
  if (compiler->current) {
    printf("  current:\n    ");
    printToken(compiler->current);
  }
}

// TOOD
static void raiseError(Compiler *compiler) {
  // TODO
  printf("---------\n");
  printf("Unexpected Token:\n");
  debugCompiler(compiler);
  printf("---------\n");
  exit(1);
}

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

static inline void expectTokenType(Compiler *compiler, TokenType type) {
  if (!matchTokenType(compiler, type)) {
    raiseError(compiler);
  }
}

// Code Emitters
static inline void emitCode(Compiler *compiler, OpCode code) {
  writeChunk(&compiler->chunk, code);
}

static void emitLoadConstant(Compiler *compiler, Value value) {
  int val = addConstant(&compiler->chunk, value);

  writeChunk(&compiler->chunk, OP_LOAD_CONSTANT);
  writeChunk(&compiler->chunk, val);
}

// Compile
static void compileProgram(Compiler *compiler) {
  // Load the first token
  next(compiler);

  if (isAtEnd(compiler)) {
    return;
  }

  do {
    if (matchTokenType(compiler, TOKEN_NEWLINE)) {
      continue;
    }

    compileStmt(compiler);

    if (isAtEnd(compiler)) {
      break;
    }

    expectTokenType(compiler, TOKEN_NEWLINE);
  } while (!isAtEnd(compiler));
}

static void compileStmt(Compiler *compiler) {
  // 'print'
  if (matchTokenType(compiler, TOKEN_PRINT)) {
    return compileStmtPrint(compiler);
  }

  return compileStmtExpr(compiler);
}

static void compileStmtPrint(Compiler *compiler) {
  compileExpr(compiler);
  emitCode(compiler, OP_PRINT);
}

static void compileStmtExpr(Compiler *compiler) {
  return compileExpr(compiler);
}

static void compileExpr(Compiler *compiler) {
  compileTerm(compiler);

  compileExprBinary(compiler);
}

static void compileExprBinary(Compiler *compiler) {
  if (isAtEnd(compiler)) {
    return;
  }

  if (checkTokenType(compiler, TOKEN_NEWLINE)) {
    return;
  }

  if (!(matchTokenType(compiler, TOKEN_PLUS) ||
        matchTokenType(compiler, TOKEN_MINUS))) {
    raiseError(compiler);
  }

  TokenType opType = compiler->previous->type;

  compileTerm(compiler);

  switch (opType) {
  case TOKEN_PLUS:
    emitCode(compiler, OP_ADD);
    break;
  case TOKEN_MINUS:
    emitCode(compiler, OP_SUB);
    break;
  default:
    // TODO: add error handler
    exit(1);
  }

  return compileExprBinary(compiler);
}

static void compileTerm(Compiler *compiler) {
  if (matchTokenType(compiler, TOKEN_NUMBER)) {
    return compileNumber(compiler);
  }

  raiseError(compiler);
}

static void compileNumber(Compiler *compiler) {
  Value value = strtod(compiler->previous->start, NULL);

  return emitLoadConstant(compiler, value);
}

// Exposed
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
  compileProgram(compiler);

  return compiler->chunk;
}
