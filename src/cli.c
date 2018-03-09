#include "cli.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "debug.h"
#include "util.h"
#include "vector.h"
#include "vm.h"

// Internal macros
#define FLAG(sForm, fForm, prop, desk)                                         \
  {                                                                            \
    .shortForm = sForm, .fullForm = fForm, .property = prop,                   \
    .description = desk, .value = NULL, .on = false                            \
  }

// Internal types
typedef struct {
  char *shortForm;
  char *fullForm;
  char *property;
  char *description;
  char *value;
  bool on;
} Flag;

typedef enum { OK, UnexpectedFlag, MissingValue } ArgumentParsingErrno;

static char *binname;
static char *filename;
static Flag flagHelp = FLAG("-h", "--help", NULL, NULL);
static Flag flagPrintBytecode = FLAG(NULL, "--print-bytecode", NULL, NULL);
static Flag flagPrintSource = FLAG(NULL, "--print-source", NULL, NULL);
static Flag *flags[] = {&flagHelp, &flagPrintBytecode, &flagPrintSource};
static bool argumentsMode;
static Vector arguments;

void printHelp() {
  printf("Usage:\n"
         "  %s <filename>\n"
         "Options and arguments:\n",
         binname);

  for (int i = 0, ii = sizeof(flags) / sizeof(Flag *); i < ii; i++) {
    Flag *flag = flags[i];

    printf("  %-4s %-20s %-16s %s "
           "\n",
           flag->shortForm ? flag->shortForm : "",
           flag->fullForm ? flag->fullForm : "",
           flag->property ? flag->property : "",
           flag->description ? flag->description : "");
  }
}

void printArgumentParsingErrnoDescription(ArgumentParsingErrno err) {
  switch (err) {
  case UnexpectedFlag:
    printf("Unexpected flag\n");
    break;

  case MissingValue:
    printf("Flag value is missing\n");
    break;

  case OK:
  default:
    // Unreachable
    break;
  }
}

ArgumentParsingErrno parseArgument(char *arg, char *next, int *offset) {
  if (argumentsMode) {
    VECTOR_ADD(Vector, &arguments, arg);

    return OK;
  }

  char c = arg[0];

  if (c != '-') {
    if (!filename) {
      filename = arg;
    }

    return OK;
  }

  if (strcmp(arg, "--") == 0) {
    argumentsMode = true;

    return OK;
  }

  for (int i = 0, ii = sizeof(flags) / sizeof(Flag *); i < ii; i++) {
    Flag *flag = flags[i];

    if (!((flag->shortForm && strcmp(arg, flag->shortForm) == 0) ||
          (flag->fullForm && strcmp(arg, flag->fullForm) == 0))) {
      continue;
    }

    if (flag->property) {
      if (!next) {
        return MissingValue;
      }

      flag->value = arg;
      (*offset)++;
    } else {
      flag->on = true;
    }

    return OK;
  }

  return UnexpectedFlag;
}

ArgumentParsingErrno parseArguments(int argc, char **argv) {
  binname = argv[0];
  filename = NULL;
  argumentsMode = false;
  VECTOR_INIT(Vector, &arguments);

  for (int i = 1; i < argc; i++) {
    char *arg1 = argv[i];
    char *arg2 = (i + 1) < argc ? argv[i + 1] : NULL;

    int err = parseArgument(arg1, arg2, &i);
    if (err) {
      return err;
    }
  }

  return OK;
}

int runCLI(int argc, char **argv) {
  int err = parseArguments(argc, argv);
  if (err) {
    printArgumentParsingErrnoDescription(err);
    return 1;
  }

  if (flagHelp.on) {
    printHelp();
    return 0;
  }

  if (!filename) {
    printHelp();
    return 1;
  }

  char *source = readFile(filename);
  if (source == NULL) {
    printf("File not found\n");
    return 1;
  }

  if (flagPrintSource.on) {
    printf("%s\n", source);
  }

  MAKE_VM(vm);
  MAKE_COMPILER(compiler, source);

  Chunk *chunk = compile(&compiler);

  if (flagPrintBytecode.on) {
    disassembleChunk(chunk);
  }

  interpret(&vm, chunk);

  freeCompiler(&compiler);
  freeVM(&vm);

  freeFile(source);

  return 0;
}
