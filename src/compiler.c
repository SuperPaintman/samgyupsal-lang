#include "compiler.h"

#include <stdlib.h>

// TODO
#include "debug.h"
#include <stdio.h>

// Internal macros
#define RAISE_ERROR(compiler) raiseError(compiler, __FUNCTION__, __LINE__)
#define DEBUG_COMPILER(compiler) debugCompiler(compiler, __FUNCTION__, __LINE__)

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
 *   = expr_binary_additive
 *   ;
 * ```
 */
static void compileExpr(Compiler *compiler);

/**
 * ```ebnf
 * expr_binary_additive
 *   = expr_binary_multiplicative
 *   | term '+' term
 *   | term '-' term
 *   ;
 * ```
 */
static void compileExprBinaryAdditive(Compiler *compiler);

/**
 * ```ebnf
 * expr_binary_multiplicative
 *   = expr_unary
 *   | term '*' term
 *   | term '/' term
 *   ;
 * ```
 */
static void compileExprBinaryMultiplicative(Compiler *compiler);

/**
 * ```ebnf
 * expr_unary
 *   = term
 *   ;
 * ```
 */
static void compileExprUnary(Compiler *compiler);

/**
 * ```ebnf
 * term
 *   = IDENTIFIER
 *   | NONE
 *   | BOOLEAN
 *   | NUMBER
 *   | STRING
 *   ;
 * ```
 */
static void compileTerm(Compiler *compiler);

/**
 * ```ebnf
 * NONE
 * ```
 */
static void compileNone(Compiler *compiler);

/**
 * ```ebnf
 * BOOLEAN
 * ```
 */
static void compileBoolean(Compiler *compiler);

/**
 * ```ebnf
 * NUMBER
 * ```
 */
static void compileNumber(Compiler *compiler);

/**
 * ```ebnf
 * STRING
 * ```
 */
static void compileString(Compiler *compiler);

// Vectors
VECTOR_TEMPLATE_IMPLEMENTATIONS(TokenVector, Token *);

// Helpers
// TOOD
static void debugCompiler(Compiler *compiler, const char *funname, int line) {
  printf("Debug compiler at %d in %s:\n", line, funname);
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
static void raiseError(Compiler *compiler, const char *funname, int line) {
  // TODO
  printf("---------\n");
  printf("Unexpected Token at %d in %s:\n", line, funname);
  debugCompiler(compiler, funname, line);
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
    RAISE_ERROR(compiler);
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

static void emitLoadObject(Compiler *compiler, Value value) {
  Object *next = compiler->chunk.objects;

  compiler->chunk.objects = AS_OBJECT(value);
  compiler->chunk.objects->next = next;

  emitLoadConstant(compiler, value);
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
  compileExprBinaryAdditive(compiler);
}

static void compileExprBinaryAdditive(Compiler *compiler) {
  compileExprBinaryMultiplicative(compiler);

  if (!(matchTokenType(compiler, TOKEN_PLUS) ||
        matchTokenType(compiler, TOKEN_MINUS))) {
    return;
  }

  TokenType opType = compiler->previous->type;

  compileExprBinaryMultiplicative(compiler);

  switch (opType) {
  case TOKEN_PLUS:
    emitCode(compiler, OP_ADD);
    break;
  case TOKEN_MINUS:
    emitCode(compiler, OP_SUB);
    break;
  default:
    // TODO: add error handler
    RAISE_ERROR(compiler);
  }
}

static void compileExprBinaryMultiplicative(Compiler *compiler) {
  compileExprUnary(compiler);

  if (!(matchTokenType(compiler, TOKEN_STAR) ||
        matchTokenType(compiler, TOKEN_SLASH))) {
    return;
  }

  TokenType opType = compiler->previous->type;
  compileExprUnary(compiler);

  switch (opType) {
  case TOKEN_STAR:
    emitCode(compiler, OP_MUL);
    break;
  case TOKEN_SLASH:
    emitCode(compiler, OP_DIV);
    break;
  default:
    // TODO: add error handler
    RAISE_ERROR(compiler);
  }
}

static void compileExprUnary(Compiler *compiler) {
  // TODO

  return compileTerm(compiler);
}

static void compileTerm(Compiler *compiler) {
  if (matchTokenType(compiler, TOKEN_NONE)) {
    return compileNone(compiler);
  }

  if (matchTokenType(compiler, TOKEN_TRUE) ||
      matchTokenType(compiler, TOKEN_FALSE)) {
    return compileBoolean(compiler);
  }

  if (matchTokenType(compiler, TOKEN_NUMBER)) {
    return compileNumber(compiler);
  }

  if (matchTokenType(compiler, TOKEN_STRING)) {
    return compileString(compiler);
  }

  RAISE_ERROR(compiler);
}

static void compileNone(Compiler *compiler) { emitCode(compiler, OP_NONE); }

static void compileBoolean(Compiler *compiler) {
  emitCode(compiler,
           compiler->previous->type == TOKEN_TRUE ? OP_TRUE : OP_FALSE);
}

static void compileNumber(Compiler *compiler) {
  Value value = MAKE_NUMBER(strtod(compiler->previous->start, NULL));

  return emitLoadConstant(compiler, value);
}

static void compileString(Compiler *compiler) {
  Value value = MAKE_OBJECT(copyString(compiler->previous->start + 1,
                                       compiler->previous->length - 2));

  return emitLoadObject(compiler, value);
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

Chunk *compile(Compiler *compiler) {
  compileProgram(compiler);

  return &compiler->chunk;
}
