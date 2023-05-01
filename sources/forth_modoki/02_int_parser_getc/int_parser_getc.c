#include "clesson.h"
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int parse_one(Node *node, int forwarded)
{
    /**
     * cl_getcは、EOFを考慮してくれている。
     * EOFの実態は、-1とint。
     * なので、cl_getcの返り値はcharではなくってintであることに注意。
     */
    int c = forwarded;

    if (c == '\0')
    {
        c = cl_getc();
    }

    if (c == ' ')
    {

        node->kind = ND_SPACE;

        for (;;)
        {
            c = cl_getc();
            if (c != ' ')
            {
                return c;
            }
        }
    }

    if (isdigit(c))
    {
        node->kind = ND_INT;
        for (;;)
        {
            node->val = c - '0' + (node->val * 10);
            c = cl_getc();
            if (!isdigit(c))
            {
                return c;
            }
        }
    }

    node->kind = ND_EOF;
    return '\0';
}

void test_parse_one_123()
{
    int c;
    int val;
    int v2 = EOF;

    cl_getc_set_src("123");
    Node *node = calloc(1, sizeof(Node));
    c = parse_one(node, '\0');

    assert(node->val == 123);
    assert(c == EOF);
}

int main()
{
    int answer1 = 0;
    int answer2 = 0;

    int forwarded;

    // write something here.
    test_parse_one_123();

    cl_getc_set_src("123 456");

    Node *node1 = calloc(1, sizeof(Node));
    Node *node2 = calloc(1, sizeof(Node));
    Node *node3 = calloc(1, sizeof(Node));
    forwarded = parse_one(node1, '\0');
    forwarded = parse_one(node2, forwarded);
    forwarded = parse_one(node3, forwarded);

    answer1 = node1->val;
    answer2 = node3->val;
    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);

    return 0;
}