#include "clesson.h"
#include "parser.h"
#include "stack.h"

void eval()
{
    // NOTE
    // typedefを使う使わないは統一したほうがいい
    struct Token token = {UNKNOWN, {0}};
    StackData s_data;
    int c;

    while (c != EOF)
    {
        c = parse_one(EOF, &token);

        if (token.ltype == NUMBER)
        {
            stack_push_number(token.u.number);
            continue;
        }

        if (token.ltype == EXECUTABLE_NAME)
        {
            int l_v;
            int r_v;
            stack_pop(&s_data);
            l_v = s_data.u.number;
            stack_pop(&s_data);
            r_v = s_data.u.number;
            stack_push_number(l_v + r_v);
            continue;
        }
    }
}

static void test_eval_num_one()
{
    char *input = "123";
    int expect = 123;

    cl_getc_set_src(input);

    eval();

    StackData s_data;
    stack_pop(&s_data);
    int actual = s_data.u.number;
    assert(expect == actual);
}

static void test_eval_num_two()
{
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    cl_getc_set_src(input);

    eval();

    StackData s_data;
    stack_pop(&s_data);

    int actual1 = s_data.u.number;

    stack_pop(&s_data);
    int actual2 = s_data.u.number;

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}

static void test_eval_num_add()
{
    char *input = "1 2 add";
    int expect = 3;

    cl_getc_set_src(input);

    eval();

    StackData s_data;
    stack_pop(&s_data);
    int actual = s_data.u.number;
    assert(expect == actual);
}

int main()
{
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();

    return 0;
}
