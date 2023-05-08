#include "clesson.h"
#include "parser.h"
#include "stack.h"
#include "dict.h"
#include "stdbool.h"

typedef struct
{
    char *key;
    StackData value;
} KeyValue;

void add_op()
{
    StackData s_data;
    int l_v;
    int r_v;
    stack_pop(&s_data);
    l_v = s_data.u.number;
    stack_pop(&s_data);
    r_v = s_data.u.number;
    stack_push_number(l_v + r_v);
}

void eval()
{
    Token token = {UNKNOWN, {0}};
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
            if (strcmp(token.u.name, "def") == 0)
            {
                int v;
                char *literal_name;
                stack_pop(&s_data);
                v = s_data.u.number;
                stack_pop(&s_data);
                literal_name = s_data.u.name;
                dict_push_number(literal_name, v);
                continue;
            }

            NodeValue *v = dict_get(token.u.name);
            // 辞書に存在しない
            if (v == NULL)
            {
                assert(false);
            }

            if (v->type == NODE_FUNC)
            {
                v->u.cfunc();
                continue;
            }

            // UNKOWN EXECUTABLE WORD
            assert(false);
        }

        if (token.ltype == LITERAL_NAME)
        {
            stack_push_name(token.u.name);
            continue;
        }
    }
}

void test_eval_num_one()
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

void test_eval_num_two()
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

void test_eval_num_add()
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

void test_eval_num_def()
{
    char *input = "/abc 12 def";
    int expect = 12;

    cl_getc_set_src(input);
    eval();
}

void register_primitives()
{
    dict_push_cfunc("add", add_op);
}

int main()
{
    register_primitives();
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_num_def();

    return 0;
}
