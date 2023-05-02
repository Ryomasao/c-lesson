#include "stack.h"

#define STACK_SIZE 1024

typedef struct
{
    int pos;
    StackData data[STACK_SIZE];
} Stack;

Stack stack;

void stack_initialize()
{
    stack.pos = 0;
}

bool stack_push_number(int data)
{
    if (stack.pos == STACK_SIZE)
        return false;

    stack.data[stack.pos].type = STACK_NUMBER;
    stack.data[stack.pos]
        .u.number = data;
    stack.pos++;

    return true;
}

bool stack_push_name(char *data)
{
    if (stack.pos == STACK_SIZE)
        return false;

    stack.data[stack.pos].type = STACK_NAME;
    stack.data[stack.pos].u.name = data;
    stack.pos++;

    return true;
}

bool stack_pop(StackData *out_data)
{
    if (stack.pos == 0)
        return false;

    stack.pos--;

    StackData t = stack.data[stack.pos];

    out_data->type = t.type;

    switch (t.type)
    {
    case STACK_NUMBER:
        out_data->u.number = t.u.number;
        break;
    case STACK_NAME:
        out_data->u.name = t.u.name;
        break;
    }

    return true;
}

void test_stack()
{
    stack_initialize();

    stack_push_number(12);
    stack_push_number(34);

    StackData d;
    stack_pop(&d);
    assert(d.type == STACK_NUMBER);
    assert(d.u.number == 34);

    stack_push_name("foo");
    stack_pop(&d);
    assert(d.type == STACK_NAME);
    assert(strcmp(d.u.name, "foo") == 0);
}

#if 0
int main()
{
    test_stack();
    return 0;
}
#endif