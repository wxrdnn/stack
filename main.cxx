#include "h/errorHandle.h"
#include "h/stack.h"
#include <cassert>

int main()
{
    // TODO Catch multiple stack errors
    // TODO think about RETURN_EXITCODE_IF_FAIL replace

    Stack_t stk = {};
    StackDebugData stkDD = {};
    Error error = CreateSuccess();

    RETURN_EXITCODE_IF_FAIL(error = StackDebugDataInit(&stkDD, "stk", __FILE__, __FUNCTION__, __LINE__ + 1));
    RETURN_EXITCODE_IF_FAIL(error = StackInit(&stk, 4, &stkDD));

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

    StackDump(&stk);
    StackFreeData(&stk);
    return 0;
}
