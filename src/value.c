#include "value.h"

#include <stdlib.h>
#include <string.h>

// Internal macros
#define MAKE_OBJ(name, type, objectType)                                       \
  type *name = malloc(sizeof(type));                                           \
  initObject(&name->object, objectType);

static inline void freeString(ObjectString *object) {
  free(object->chars);
  free(object);
}

void initObject(Object *object, ObjectType type) {
  object->type = type;
  object->next = NULL;
}
void initObjectString(ObjectString *object, char *chars, uint32_t length) {
  object->length = length;
  object->chars = chars;
}

ObjectString *makeString(const char *chars, uint32_t length) {
  char *buf = malloc(sizeof(char) * (length + 1));

  memcpy(buf, chars, length);
  buf[length] = '\0';

  MAKE_OBJ(object, ObjectString, OBJECT_STRING);
  initObjectString(object, buf, length);

  return object;
}

void freeObject(Object *object) {
  switch (object->type) {
  case OBJECT_STRING:
    return freeString((ObjectString *)object);
  default:
    // Unknown object type
    // return free(object); // Why not? :)
    return;
  }
}

void freeObjects(Object *objects) {
  while (objects) {
    Object *next = objects->next;

    freeObject(objects);

    objects = next;
  }
}
