#ifndef STACK_H

#define STACK_H

#include "errorHandle.h"
#define STK_DEBUG // FIX for debug only

#ifdef STK_DEBUG

#define ON_DEBUG(...) __VA_ARGS__

#else

#define ON_DEBUG(...)

#endif

#include <cstddef>
typedef int StackElem_t; // TODO remove it

struct Stack_t
{
    StackElem_t *data;
    size_t capacity;
    size_t size;

    ON_DEBUG(const char *dName);
    ON_DEBUG(const char *dCreationFile);
    ON_DEBUG(const size_t dCreationLine);
};

struct StackDebugData
{
    const char *const dName;
    const char *const dCreationFile;
    const size_t dCreationLine;
};

bool VerifyStack(const Stack_t *const stk);

Error StackInit(Stack_t *const stk, const size_t capacity ON_DEBUG(, StackDebugData *const debugData));

Error StackPush(Stack_t *const stl, StackElem_t element);

StackElem_t StackPop(Stack_t *const stk, Error *const errorPtr);

Error StackDestroy(Stack_t *const stk);

Error StackDump(const Stack_t *const stk);

Error ExtendStack(Stack_t *stk);

Error ShrinkStack(Stack_t *stk);

#endif
