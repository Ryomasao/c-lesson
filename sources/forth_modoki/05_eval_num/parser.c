#include "clesson.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define NAME_SIZE 256

void error(char *message)
{
    printf("error: %s", message);
    exit(1);
}

int parse_one(int prev_ch, struct Token *out_token)
{
    int c = prev_ch;

    if (c == EOF)
    {
        c = cl_getc();
    }

    /**
     * SPACE
     */
    if (c == ' ')
    {
        out_token->ltype = SPACE;
        for (;;)
        {
            c = cl_getc();
            if (c != ' ')
            {
                return c;
            }
        }
    }

    /**
     * NUMBER
     */
    if (isdigit(c))
    {
        out_token->ltype = NUMBER;
        // clear
        out_token->u.number = 0;
        for (;;)
        {
            out_token->u.number *= 10;
            out_token->u.number += c - '0';
            c = cl_getc();
            if (!isdigit(c))
                return c;
        }
    }
    /**
     * EXECUTABLE NAME
     */
    if (isalpha(c))
    {
        char buff[NAME_SIZE] = {0};

        for (int i = 0; i < NAME_SIZE - 1; i++)
        {
            if (isalnum(c))
            {
                buff[i] = c;
                c = cl_getc();
            }
            else
            {
                out_token->ltype = EXECUTABLE_NAME;
                out_token->u.name = malloc(i + 1);
                strcpy(out_token->u.name, buff);
                return c;
            }
        }
    }

    /**
     * LITERAL NAME
     */
    if (c == '/')
    {
        // スラッシュは読み飛ばす
        c = cl_getc();
        if (!isalnum(c))
        {
            error("/の後に英数字がない");
        }

        char buff[NAME_SIZE] = {0};

        for (int i = 0; i < (NAME_SIZE - 1); i++)
        {
            if (isalnum(c))
            {
                buff[i] = c;
                c = cl_getc();
            }
            else
            {
                out_token->ltype = LITERAL_NAME;
                out_token->u.name = malloc(i + 1);
                // strcpyはbuffのNULL文字までをdistにcopyする
                // buffは宣言時にNULL文字で初期化しているので、確保したメモリ以上をコピーすることはなさそう？
                strcpy(out_token->u.name, buff);
                return c;
            }
        }
    }

    /**
     * OPEN CURLY
     */
    if (c == '{')
    {
        out_token->ltype = OPEN_CURLY;
        out_token->u.onechar = c;
        c = cl_getc();
        return c;
    }

    /**
     * CLOSE CURLY
     */
    if (c == '}')
    {
        out_token->ltype = CLOSE_CURLY;
        out_token->u.onechar = c;
        c = cl_getc();
        return c;
    }

    /**
     * EOL
     */
    out_token->ltype = END_OF_FILE;
    return EOF;
}

void parser_print_all()
{
    int ch = EOF;
    struct Token token = {
        UNKNOWN,
        {0}};

    do
    {
        ch = parse_one(ch, &token);
        if (token.ltype != UNKNOWN)
        {
            switch (token.ltype)
            {
            case NUMBER:
                printf("num: %d\n", token.u.number);
                break;
            case SPACE:
                printf("space!\n");
                break;
            case OPEN_CURLY:
                printf("Open curly brace '%c'\n", token.u.onechar);
                break;
            case CLOSE_CURLY:
                printf("Close curly brace '%c'\n", token.u.onechar);
                break;
            case EXECUTABLE_NAME:
                printf("EXECUTABLE_NAME: %s\n", token.u.name);
                break;
            case LITERAL_NAME:
                printf("LITERAL_NAME: %s\n", token.u.name);
                break;

            default:
                printf("Unknown type %d\n", token.ltype);
                break;
            }
        }
    } while (ch != EOF);
}

static void test_parse_one_number()
{
    char *input = "123";
    int expect = 123;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == NUMBER);
    assert(expect == token.u.number);
}

static void test_parse_one_empty_should_return_END_OF_FILE()
{
    char *input = "";
    int expect = END_OF_FILE;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect);
}

static void test_parse_one_executable_name()
{
    char *input = "add";
    char *expect_name = "add";
    int expect_type = EXECUTABLE_NAME;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(strcmp(token.u.name, "add") == 0);
}

static void test_parse_one_literal_name()
{
    char *input = "/add";
    char *expect_name = "add";
    int expect_type = LITERAL_NAME;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
    assert(strcmp(token.u.name, "add") == 0);
}

static void test_parse_one_open_curly()
{
    char *input = "{";
    int expect_type = OPEN_CURLY;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
}

static void test_parse_one_close_curly()
{
    char *input = "}";
    int expect_type = CLOSE_CURLY;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect_type);
}

static void unit_tests()
{
    test_parse_one_empty_should_return_END_OF_FILE();
    test_parse_one_number();
    test_parse_one_executable_name();
    test_parse_one_literal_name();
    test_parse_one_open_curly();
    test_parse_one_close_curly();
}

#if 0
int main()
{
    unit_tests();

    cl_getc_set_src("123 45 add /some { 2 3 add } def");
    parser_print_all();
    return 0;
}

#endif