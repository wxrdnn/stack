#include "h/stack.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include <cstddef>
#include <cstdlib>

bool StackVerify(const Stack_t *const stk)
{
    bool isCorrect = true;

    isCorrect = stk && stk->data ONDEBUG(&&stk->debugData);
    isCorrect = isCorrect && (stk->capacity >= stk->size);

    if (!isCorrect)
    {
        StackDump(stk);
    }

    return isCorrect;
}

Error StackCreate(Stack_t **stk, const size_t capacity ONDEBUG(, StackDebugData *const debugData))
{
    ASSERT(stk);
    ONDEBUG(ASSERT(debugData));

    Error error = CreateSuccess();
    *stk = (Stack_t *)calloc(1, sizeof(Stack_t));
    if (!(*stk))
    {
        return error = CreateError(ecCantAllocateMemory, "Stack_t structure");
    }

    (*stk)->data = (StackElem_t *)calloc(capacity, sizeof(StackElem_t));
    if (!(*stk)->data)
    {
        return error = CreateError(ecCantAllocateMemory, "Stack_t element");
    }

    (*stk)->capacity = capacity;
    (*stk)->size = 0;
    ONDEBUG((*stk)->debugData = debugData);

    ASSERT(StackVerify(*stk));
    return error;
}

Error StackPush(Stack_t *const stk, StackElem_t element)
{
    ASSERT(StackVerify(stk));
    Error error = CreateSuccess();

    if (stk->size == stk->capacity)
    {
        error = StackExtend(stk);

        if (IsFail(&error))
        {
            return error;
        }
    }

    ASSERT(stk->size < stk->capacity);

    stk->data[stk->size++] = element;

    ASSERT(StackVerify(stk));
    return error;
}

StackElem_t StackPop(Stack_t *const stk, Error *const error)
{
    ASSERT(StackVerify(stk));

    *error = CreateSuccess();

    StackElem_t poppedElement = stk->data[stk->size--]; // TODO Error handling

    // if (log(stk->capacity) / log(stk->size))
    // TODO Shrink if stack is smaller than its 2 extensions

    ASSERT(StackVerify(stk));
    return poppedElement;
}

void StackDestroy(Stack_t *const stk)
{
    ASSERT(StackVerify(stk));

    free(stk->data);
    ONDEBUG(free(stk->debugData));

    free(stk);
    return;
}

Error StackDump(const Stack_t *const stk)
{
}

Error StackExtend(Stack_t *stk)
{
    ASSERT(StackVerify(stk));

    size_t newCapacity = stk->capacity * cStackCapMultiplierOnExtend;
    Error error = StackResize(stk, newCapacity);

    ASSERT(StackVerify(stk));
    return error;
}

Error StackResize(Stack_t *stk, size_t newCapacity)
{
    ASSERT(StackVerify(stk));

    Error error = CreateSuccess();

    stk->data = (StackElem_t *)realloc(stk->data, newCapacity);
    if (!stk->data)
    {
        return error = CreateError(ecCantAllocateMemory, "Stack_t data extention");
    }
    stk->capacity = newCapacity;

    return error;
}
