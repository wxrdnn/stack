#ifndef STACK_H

#define STACK_H

#include "debug.h"
#include "errorHandle.h"
#include <cstdint>
#include <stddef.h>

const size_t cStackCapMultiplierOnExtend = 2;
const int cIntPoisonValue = (int)0xB1BAB0BA;
const uint64_t cCanaryValue = 0xB00B1E5;

enum StackErrorCode
{
    secSuccess = 0,
    secNullStackPointer,
    secNullDataPointer,
    secNullDebugDataPointer,
    secIncorrectDebugData,
    secSizeLargerThanCapacity,
    secPoisonValueDetected,
    secNullNameInDebugData,
    secNullCreationFileInDebugData,
    secNullCreationFunctionInDebugData,
    secTopCanaryChanged,
    secBottomCanaryChanged
};

struct StackDebugData
{
    const char *name;
    const char *creationFile;
    const char *creationFunction;
    size_t creationLine;
};

typedef int StackElem_t; // TODO remove it

struct Stack_t
{
    ONDEBUG(uint64_t canaryTop;)
    StackElem_t *data;
    size_t capacity;
    size_t size;

    ONDEBUG(StackDebugData *debugData);
    ONDEBUG(uint64_t canaryBottom;)
};

#define VERIFY_STACK(__stk)                                                                                            \
    {                                                                                                                  \
        StackErrorCode __stkErrCode = StackVerify(__stk);                                                              \
        if (__stkErrCode != secSuccess)                                                                                \
        {                                                                                                              \
            fprintf(                                                                                                   \
                stderr, __RED "ERROR: StackVerify(%s) failed, StackErrorCode: %d.\n" __RESET, #__stk, __stkErrCode);   \
        }                                                                                                              \
        ASSERT(__stkErrCode == secSuccess);                                                                            \
    }

StackErrorCode StackVerify(const Stack_t *const stk);

StackErrorCode StackDebugDataVerify(const StackDebugData *const data);

Error StackInit(Stack_t *const stk, const size_t capacity ONDEBUG(, StackDebugData *const debugData));

Error StackPush(Stack_t *const stk, StackElem_t element);

StackElem_t StackPop(Stack_t *const stk, Error *const error);

void StackFreeData(Stack_t *const stk);

void StackDump(const Stack_t *const stk);

Error StackExtend(Stack_t *stk);

Error StackResize(Stack_t *stk, size_t newCapacity);

Error StackDebugDataInit(StackDebugData *const stkDebugData, const char *const name, const char *const creationFile,
                         const char *const creationFunction, const size_t creationLine);

#endif
