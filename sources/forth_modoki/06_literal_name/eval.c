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

// #define MAX_DIST_SIZE 1024
// int dict_pos = 0;
// KeyValue dict_array[MAX_DIST_SIZE];
//
// typedef struct
//{
//     int pos;
//     int value;
// } DictResult;
//
// bool dict_get(char *key, DictResult *o_result)
//{
//     int i = 0;
//     for (i; i < dict_pos; i++)
//     {
//         if (strcmp(dict_array[i].key, key) == 0)
//         {
//             break;
//         }
//     }
//
//     if (dict_pos == i)
//     {
//         return false;
//     }
//
//     o_result->value = dict_array[i].value.u.number;
//     o_result->pos = i;
//
//     return true;
// }
//
// void dict_push(char *key, int value)
//{
//     if (dict_pos == MAX_DIST_SIZE)
//     {
//         // stack overflow
//         assert(false);
//     }
//
//     DictResult r;
//     if (dict_get(key, &r))
//     {
//         dict_array[r.pos].value.u.number = value;
//         return;
//     }
//
//     dict_array[dict_pos].key = key;
//     dict_array[dict_pos].value.u.number = value;
//     dict_pos++;
// }

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

            if (strcmp(token.u.name, "add") == 0)
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

            if (strcmp(token.u.name, "def") == 0)
            {
                int v;
                char *literal_name;
                stack_pop(&s_data);
                v = s_data.u.number;
                stack_pop(&s_data);
                literal_name = s_data.u.name;
                dict_push(literal_name, v);
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

int main()
{
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_num_def();

    return 0;
}
