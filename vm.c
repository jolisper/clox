#include <stdio.h>

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include "vm.h"

/* The choice to have a static VM instance is a concession for the book, but not
necessarily a sound engineering choice for a real language implementation. If
you’re building a VM that’s designed to be embedded in other host applications,
it gives the host more flexibility if you do explicitly take a VM pointer and
pass it around.

That way, the host app can control when and where memory for the VM is
allocated, run multiple VMs in parallel, etc.

What I’m doing here is a global variable, and everything bad you’ve heard about
global variables is still true when programming in the large. But when keeping
things small for a book… */
VM vm;

static void resetStack(){
  /* The only initialization we need is to set stackTop to point to the beginning
     of the array to indicate that the stack is empty. */
    vm.stackTop = vm.stack;
}

void initVM() {
  resetStack();
}

void freeVM() {
}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {

/* The READ_BYTE macro reads the byte currently pointed at by ip and then
   advances the instruction pointer. */
#define READ_BYTE() (*vm.ip++)

/* The READ_CONSTANT macro reads the next byte from the bytecode, treats
   the resulting number as an index, and looks up the corresponding
   location in the chunk’s constant table. */
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    /* We loop, printing each value in the array, starting at the first (bottom
     * of the stack) and ending when we reach the top. */
    printf("          ");
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");

    /* Since disassembleInstruction() takes an integer byte offset and we store
       the current instruction reference as a direct pointer, we first do a
       little pointer math to convert ip back to a relative offset from the
       beginning of the bytecode.

       In the first iteration vm.ip and vm.chunk->code points to the same
       place in memory, so the aritmetic operation goes to zero, the first
       valid index for chunk code array (chunk->code[0]).
    */
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OP_RETURN: {
      printValue(pop());
      printf("\n");
      return INTERPRET_OK;
    }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Chunk *chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}
