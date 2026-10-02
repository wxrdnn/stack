#include "h/stack.h"
#include "h/colors.h"
#include "h/debug.h"
#include "h/errorHandle.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

StackErrorCode StackVerify(const Stack_t *const stk)
{
    if (!stk)
    {
        StackDump(stk);
        return secNullStackPointer;
    }

    if (!stk->data)
    {
        StackDump(stk);
        return secNullDataPointer;
    }

    ONDEBUG(

        if (!stk->debugData) {
            StackDump(stk);
            return secNullDebugDataPointer;
        }

        StackErrorCode debugDataCode = StackDebugDataVerify(stk->debugData);

        if (debugDataCode != secSuccess) {
            StackDump(stk);
            return debugDataCode;
        }

    )

    if (stk->size > stk->capacity)
    {
        StackDump(stk);
        return secSizeLargerThanCapacity;
    }

    // TODO Poison value detection

    return secSuccess;
}

StackErrorCode StackDebugDataVerify(const StackDebugData *const data)
{
    if (!data)
    {
        return secNullDebugDataPointer;
    }

    if (!data->name)
    {
        return secNullNameInDebugData;
    }

    if (!data->creationFile)
    {
        return secNullCreationFileInDebugData;
    }

    if (!data->creationFunction)
    {
        return secNullCreationFunctionInDebugData;
    }

    return secSuccess;
}

Error StackInit(Stack_t *const stk, const size_t capacity ONDEBUG(, StackDebugData *const debugData))
{
    ASSERT(stk);
    ONDEBUG(ASSERT(debugData));

    Error error = CreateSuccess();
    if (!(stk))
    {
        return error = CreateError(ecCantAllocateMemory, "Stack_t structure");
    }

    (stk)->data = (StackElem_t *)calloc(capacity, sizeof(StackElem_t));
    if (!(stk)->data)
    {
        return error = CreateError(ecCantAllocateMemory, "Stack_t element");
    }

    (stk)->capacity = capacity;
    (stk)->size = 0;
    ONDEBUG((stk)->debugData = debugData);

    VERIFY_STACK(stk);
    return error;
}

Error StackPush(Stack_t *const stk, StackElem_t element)
{
    ASSERT(StackVerify(stk) == secSuccess);
    Error error = CreateSuccess();

    // error.exitCode = ecFileIsBusy;
    // return error;

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

    ASSERT(StackVerify(stk) == secSuccess);
    return error;
}

StackElem_t StackPop(Stack_t *const stk, Error *const error)
{
    ASSERT(StackVerify(stk) == secSuccess);

    *error = CreateSuccess();

    StackElem_t poppedElement = stk->data[stk->size--]; // TODO Error handling

    // if (stl->capacity / stk->size > cStackCapMultipluer)
    // TODO Shrink if stack is smaller than its 2 extensions

    ASSERT(StackVerify(stk) == secSuccess);
    return poppedElement;
}

void StackFreeData(Stack_t *const stk)
{
    ASSERT(StackVerify(stk) == secSuccess);

    free(stk->data);
    // ONDEBUG(free(stk->debugData));

    return;
}

// lang-format off
void StackDump(const Stack_t *const stk) // Disables by NDEBUG
{
    ONDEBUG(

        if (stk == NULL) {
            fprintf(stderr, __RED "ERROR: StackDump() got NULL instead of Stack_t pointer!\n" __RESET);
            return;
        }

        if (StackDebugDataVerify(stk->debugData) != secSuccess) { // TODO expand it
            fprintf(stderr, __RED "ERROR: StackDump() got corrupted StackDebugData in Stack_t!\n" __RESET);
        }

        fprintf(stderr,
                __YELLOW "DEBUG: Begin dump of Stack_t named \"%s\" at [%p] created in %s() at %s:%lu.\n" __RESET,
                stk->debugData->name,
                stk,
                stk->debugData->creationFunction,
                stk->debugData->creationFile,
                stk->debugData->creationLine);

        if (stk->data == NULL) {
            fprintf(
                stderr,
                __RED
                "ERROR: StackDump() got NULL data pointer in Stack_t!\n" __RESET); // TODO still print size and capacity
            return;
        }

        fprintf(stderr, __YELLOW);
        fprintf(stderr, "DEBUG: Stack_t %s\n", stk->debugData->name);
        fprintf(stderr, "DEBUG: {\n");
        fprintf(stderr, "DEBUG:     capacity = %lu\n", stk->capacity);
        fprintf(stderr, "DEBUG:     size = %lu\n", stk->size);
        fprintf(stderr, "DEBUG:     data at [%p]\n", stk->data);
        fprintf(stderr, "DEBUG:     {\n");

        for (size_t i = 0; i < stk->capacity; ++i) {
            fprintf(stderr,
                    "DEBUG:         data[%lu] at [%p] = %d",
                    i,
                    stk->data + i,
                    stk->data[i]); // TODO fix hardcoded StackElem_t format

            if (stk->data[i] == cIntPoisonValue)
            {
                fprintf(stderr, "\t\t <- Poison value");
            }

            fprintf(stderr, "\n");
        }

        fprintf(stderr, "DEBUG:     }\n");
        fprintf(stderr, "DEBUG: }\n");
        fprintf(stderr, __RESET);

        fprintf(stderr,
                __YELLOW "DEBUG: End dump of Stack_t named \"%s\" at [%p] created in %s() at %s:%lu.\n" __RESET,
                stk->debugData->name,
                stk,
                stk->debugData->creationFunction,
                stk->debugData->creationFile,
                stk->debugData->creationLine);)
}
// lang-format on

Error StackExtend(Stack_t *stk)
{
    ASSERT(StackVerify(stk) == secSuccess);

    size_t newCapacity = stk->capacity * cStackCapMultiplierOnExtend;
    size_t oldCapacity = stk->capacity;
    Error error = StackResize(stk, newCapacity);
    fprintf(stderr,
            "DEBUG: Values before memset: oldCapacity = %lu, newCapacity = %lu, cIntPoisonValue = %d\n",
            oldCapacity,
            newCapacity,
            cIntPoisonValue);
    // Instead of memset, that sets values by 1 byte: 0xB1BAB0BA -> 0xBABABABA
    for (size_t i = oldCapacity; i < newCapacity; ++i)
    {
        stk->data[i] = cIntPoisonValue;
    }

    ASSERT(StackVerify(stk) == secSuccess);
    return error;
}

Error StackResize(Stack_t *stk, size_t newCapacity)
{
    ASSERT(StackVerify(stk) == secSuccess);

    Error error = CreateSuccess();

    stk->data = (StackElem_t *)realloc(stk->data, newCapacity * sizeof(StackElem_t));
    if (!stk->data)
    {
        return error = CreateError(ecCantAllocateMemory, "Stack_t data extention");
    }
    ONDEBUG(fprintf(stderr,
                    __BLUE "DEBUG: Resized Stack_t %s. Old size: %lu -> new size: %lu.\n" __RESET,
                    stk->debugData->name,
                    stk->capacity,
                    newCapacity));
    stk->capacity = newCapacity;

    ASSERT(StackVerify(stk) == secSuccess);
    return error;
}

Error StackDebugDataInit(StackDebugData *const stkDebugData, const char *const name, const char *const creationFile,
                         const char *const creationFunction, const size_t creationLine)
{
    ASSERT(stkDebugData);

    Error error = CreateSuccess();
    if (!(stkDebugData))
    {
        error = CreateError(ecCantAllocateMemory, "Stack_t debug data");
        return error;
    }

    (stkDebugData)->name = name;
    (stkDebugData)->creationFile = creationFile;
    (stkDebugData)->creationFunction = creationFunction;
    (stkDebugData)->creationLine = creationLine;

    return error;
}
