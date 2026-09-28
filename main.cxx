#include "h/debug.h"
#include "h/errorHandle.h"
#include "h/stack.h"

int main()
{
    Stack_t *stk = 0;
    StackDebugData *stkDD = 0;
    Error error = CreateSuccess();

    RETURN_EXITCODE_IF_FAIL(error = StackDebugDataCreate(&stkDD, "stk", __FILE__, __FUNCTION__, __LINE__ + 1));
    RETURN_EXITCODE_IF_FAIL(error = StackCreate(&stk, 4, stkDD));

    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, -3));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, -2));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, -1));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, 0));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, 1));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, 2));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, 3));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, 4));
    RETURN_EXITCODE_IF_FAIL(error = StackPush(stk, 5));

    StackDump(stk);

    StackDestroy(stk);
    return 0;
}
