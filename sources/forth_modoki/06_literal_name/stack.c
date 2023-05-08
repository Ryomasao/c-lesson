#include "stack.h"

#define STACK_SIZE 1024

enum StackDataType
{
    STACK_NUMBER,
    STACK_NAME
};

union StackBody
{
    int number;
    char *name;
};

typedef struct
{
    enum StackDataType type;
    union StackBody u;
} StackData;

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

void stack_push_number(int data)
{
    if (stack.pos == STACK_SIZE)
    {
        assert(false);
    }

    stack.data[stack.pos].type = STACK_NUMBER;
    stack.data[stack.pos]
        .u.number = data;
    stack.pos++;
}

void stack_push_name(char *data)
{
    if (stack.pos == STACK_SIZE)
    {
        assert(false);
    }

    stack.data[stack.pos].type = STACK_NAME;
    stack.data[stack.pos].u.name = data;
    stack.pos++;
}

void stack_pop(StackData *out_data)
{
    if (stack.pos == 0)
        assert(false);

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
}

int stack_pop_number()
{
    StackData data;
    stack_pop(&data);
    if (data.type != STACK_NUMBER)
        assert(false);
    return data.u.number;
}

char *stack_pop_name()
{
    StackData data;
    stack_pop(&data);
    if (data.type != STACK_NAME)
        assert(false);

    return data.u.name;
}

void test_stack()
{
    stack_initialize();
    stack_push_number(12);
    stack_push_number(34);
    assert(34 == stack_pop_number());
    stack_push_name("foo");
    assert(strcmp(stack_pop_name(), "foo") == 0);
}

#if 0
int main()
{
    test_stack();
    return 0;
}
#endif