#include "h/errorHandle.h"
#include "h/hash.h"
#include "h/stack.h"
#include <cassert>
#include <cstdint>
#include <cstdio>

int main()
{
    // TODO Store error context at heap
    // TODO Catch multiple stack errors
    // TODO think about RETURN_EXITCODE_IF_FAIL replace

    Stack_t stk = {};
    StackDebugData stkDD = {};
    Error error = CreateSuccess();

    RETURN_EXITCODE_IF_FAIL(error = StackDebugDataInit(&stkDD, "stk", __FILE__, __FUNCTION__, __LINE__ + 1));
    RETURN_EXITCODE_IF_FAIL(error = StackInit(&stk, 4, &stkDD));

    // Hash test
    // Stack_t stk1 = stk;
    // const uint64_t hash = CalcHash(&stk, sizeof(stk));
    // const uint64_t hash1 = CalcHash(&stk1, sizeof(stk));
    // fprintf(stderr, "DEBUG: StackHash: <%lu>\n", hash);
    // fprintf(stderr, "DEBUG: StackHash 1: <%lu>\n", hash1);

    // stk.data[1] = 5;
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, -3));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, -2));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, -1));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, 0));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, 1));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, 2));
    // stk->data = NULL;
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, 3));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, 4));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(&stk, 5));

    STACK_DUMP(&stk);
    StackFreeData(&stk);
    return 0;
}
