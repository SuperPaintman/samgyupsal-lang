#ifndef SAMGYUPSAL_VALUE_H_
#define SAMGYUPSAL_VALUE_H_

// Includes
#include <stdbool.h>
#include <stdint.h>

// Macros
#define IS_NONE(value) ((value).type == VALUE_NONE)
#define IS_BOOLEAN(value) ((value).type == VALUE_BOOLEAN)
#define IS_NUMBER(value) ((value).type == VALUE_NUMBER)
#define IS_OBJECT(value) ((value).type == VALUE_OBJECT)
#define IS_STRING(value)                                                       \
  (IS_OBJECT(value) && OBJECT_TYPE(value) == OBJECT_STRING)

#define AS_NONE(value) ((value).as.none)
#define AS_BOOLEAN(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJECT(value) ((value).as.object)
#define AS_STRING(value) ((ObjectString *)AS_OBJECT(value))
#define AS_CSTRING(value) AS_STRING(value)->chars

#define MAKE_NONE() ((Value){.type = VALUE_NONE, .as = {.none = 0}})
#define MAKE_BOOLEAN(value)                                                    \
  ((Value){.type = VALUE_BOOLEAN, .as = {.boolean = value}})
#define MAKE_NUMBER(value)                                                     \
  ((Value){.type = VALUE_NUMBER, .as = {.number = value}})
#define MAKE_OBJECT(value)                                                     \
  ((Value){.type = VALUE_OBJECT, .as = {.object = (Object *)value}})

#define OBJECT_TYPE(value) AS_OBJECT(value)->type

// Types
typedef enum {
  VALUE_NONE,
  VALUE_BOOLEAN,
  VALUE_NUMBER,
  VALUE_OBJECT
} ValueType;

typedef enum { OBJECT_STRING } ObjectType;

typedef struct Object {
  ObjectType type;
  struct Object *next;
} Object;

typedef struct {
  Object object;
  uint32_t length;
  char *chars;
} ObjectString;

typedef struct {
  ValueType type;
  union {
    unsigned none : 1;
    bool boolean;
    double number;
    Object *object;
  } as;
} Value;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
void initObject(Object *object, ObjectType type);
void initObjectString(ObjectString *object, char *chars, uint32_t length);
Object *makeObject(ObjectType type);
ObjectString *makeString(const char *chars, uint32_t length);
void freeObject(Object *object);
void freeObjects(Object *objects);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_VALUE_H_
