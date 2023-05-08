#include "clesson.h"
#include "parser.h"
#include "stack.h"
#include "dict.h"
#include "stdbool.h"

void add_op()
{
    int r_v = stack_pop_number();
    int l_v = stack_pop_number();
    stack_push_number(l_v + r_v);
}

void sub_op()
{
    int r_v = stack_pop_number();
    int l_v = stack_pop_number();
    stack_push_number(l_v - r_v);
}

void mul_op()
{
    int r_v = stack_pop_number();
    int l_v = stack_pop_number();
    stack_push_number(l_v * r_v);
}

void div_op()
{
    int r_v = stack_pop_number();
    int l_v = stack_pop_number();
    stack_push_number(l_v / r_v);
}

void def_op()
{
    int v = stack_pop_number();
    char *literal_name = stack_pop_name();
    dict_push_number(literal_name, v);
}

void eval()
{
    Token token = {UNKNOWN, {0}};
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
            else
            {
                stack_push_number(v->u.number);
                continue;
            }

            // UNKOWN EXECUTABLE WORD
            assert(false);
        }

        /**
         * 変数宣言 eg) /abc
         */
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

    int actual = stack_pop_number();
    assert(expect == actual);
}

void test_eval_num_two()
{
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    cl_getc_set_src(input);

    eval();

    int actual1 = stack_pop_number();
    int actual2 = stack_pop_number();

    assert(expect1 == actual1);
    assert(expect2 == actual2);
}

void test_eval_num_add()
{
    char *input = "1 2 add";
    int expect = 3;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();
    assert(expect == actual);
}

void test_eval_num_sub()
{
    char *input = "10 2 sub";
    int expect = 8;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();
    assert(expect == actual);
}

void test_eval_num_mul()
{
    char *input = "10 2 mul";
    int expect = 20;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();
    assert(expect == actual);
}

void test_eval_num_div()
{
    char *input = "10 2 div";
    int expect = 5;

    cl_getc_set_src(input);

    eval();

    int actual = stack_pop_number();
    assert(expect == actual);
}

void test_eval_num_def()
{
    char *input = "/abc 123 def /efg 456 def abc efg add";
    int expect = 579;

    cl_getc_set_src(input);
    eval();

    int actual = stack_pop_number();
    assert(expect == actual);
}

void register_primitives()
{
    dict_push_cfunc("add", add_op);
    dict_push_cfunc("sub", sub_op);
    dict_push_cfunc("mul", mul_op);
    dict_push_cfunc("div", div_op);
    dict_push_cfunc("def", def_op);
}

int main()
{
    register_primitives();
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_num_sub();
    test_eval_num_mul();
    test_eval_num_div();
    test_eval_num_def();

    return 0;
}
