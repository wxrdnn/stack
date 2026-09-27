#ifndef STACK_H

#define STACK_H

#include "debug.h"
#include "errorHandle.h"
#include <stddef.h>

const size_t cStackCapMultiplierOnExtend = 2;

struct StackDebugData
{
    const char *const dName;
    const char *const dCreationFile;
    const size_t dCreationLine;
};

typedef int StackElem_t; // TODO remove it

struct Stack_t
{
    StackElem_t *data;
    size_t capacity;
    size_t size;

    ONDEBUG(StackDebugData *debugData);
};

bool StackVerify(const Stack_t *const stk);

Error StackCreate(Stack_t **stk, const size_t capacity ONDEBUG(, StackDebugData *const debugData));

Error StackPush(Stack_t *const stk, StackElem_t element);

StackElem_t StackPop(Stack_t *const stk, Error *const error);

void StackDestroy(Stack_t *const stk);

Error StackDump(const Stack_t *const stk);

Error StackExtend(Stack_t *stk);

Error StackResize(Stack_t *stk, size_t newCapacity);

#endif
