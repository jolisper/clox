#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
  Chunk* chunk;
  /* We use an actual real C pointer right into the middle of the bytecode array
     instead of something like an integer index because it’s faster to dereference a
     pointer than look up an element in an array by index. */
  uint8_t* ip;
  Value stack[STACK_MAX];
  /* As with ip, we use a direct pointer instead of an integer index since it’s
   * faster to dereference the pointer than calculate the offset from the index
   * each time we need it.
   * stackTop points to where the next value to be pushed will go*/
  Value* stackTop;
} VM;

/* When we have a compiler that reports static errors and a VM that detects
   runtime errors, the interpreter will use this to know how to set the
   exit code of the process. */
typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();

#endif
