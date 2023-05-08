#include "dict.h"

#define TABLE_SIZE 1024

typedef struct Node
{
    char *key;
    NodeValue value;
    struct Node *next;
} Node;

Node *node_array[TABLE_SIZE];

int hash(char *str)
{
    unsigned int val = 0;
    while (*str)
    {
        // ASCIIコードを全部足す
        val = val + *str;
        *str++;
    }

    return (int)(val % TABLE_SIZE);
}

void copy_value(NodeValue *from, NodeValue *to)
{
    to->type = from->type;
    switch (to->type)
    {
    case NODE_NUMBER:
        to->u.number = from->u.number;
        break;
    case NODE_FUNC:
        to->u.cfunc = from->u.cfunc;
        break;
    default:
        assert(false);
        break;
    }
}

Node *new_node(char *key, NodeValue *value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->next = NULL;
    node->key = key;
    copy_value(value, &node->value);
    return node;
}

int dict_push(char *key, NodeValue *value)
{
    int idx = hash(key);
    Node *head = node_array[idx];

    // node_arrayはNode型へのポインタを格納した配列
    if (head == NULL)
    {
        // ポインタが指しているアドレスがNULLの場合、Node型のサイズ分メモリを確保して、
        // そのメモリのアドレスを配列にセットしなおす
        node_array[idx] = new_node(key, value);
        return idx;
    }

    Node *node = head;

    while (true)
    {
        // update
        if (strcmp(node->key, key) == 0)
        {
            copy_value(value, &node->value);
            break;
        }
        // insert
        if (node->next == NULL)
        {
            node->next = new_node(key, value);
            break;
        }
        node = node->next;
    }

    return idx;
}

int dict_push_number(char *key, int value)
{
    NodeValue node_value;
    node_value.type = NODE_NUMBER;
    node_value.u.number = value;
    return dict_push(key, &node_value);
}

int dict_push_cfunc(char *key, void (*cfunc)())
{
    NodeValue node_value;
    node_value.type = NODE_FUNC;
    node_value.u.cfunc = cfunc;
    return dict_push(key, &node_value);
}

NodeValue *dict_get(char *key)
{
    int idx = hash(key);
    Node *head = node_array[idx];

    while (head)
    {
        if (strcmp(key, head->key) == 0)
        {
            return &head->value;
        }
        head = head->next;
    }

    return NULL;
}

#ifdef DEBUG

void print_all()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Node *n = node_array[i];
        Node *item = n;

        for (Node *item = n; item != NULL; item = item->next)
        {
            printf("hash:%d, key:%s, value:%d\n", i, item->key, item->value);
        }
    }
}

void clear_node_array()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        node_array[i] = NULL;
    }
}

void test_insert_key()
{
    clear_node_array();
    char *expect_key = "foo";
    int expect_value = 1;

    int idx = dict_push_number(expect_key, expect_value);
    Node *head = node_array[idx];
    assert(head->value.u.number = expect_value);
    assert(strcmp(head->key, expect_key) == 0);
}

void test_get()
{
    clear_node_array();
    char *expect_key = "foo";
    int expect_value = 1;
    dict_push_number(expect_key, expect_value);
    NodeValue *actual_value = dict_get(expect_key);
    assert(actual_value->u.number == expect_value);
}

void test_update_value()
{
    clear_node_array();
    char *expect_key = "foo";
    int expect_value = 10;
    dict_push_number(expect_key, 1);
    dict_push_number(expect_key, expect_value);
    NodeValue *actual_value = dict_get(expect_key);
    assert(actual_value->u.number == expect_value);
}

void test_get_null()
{
    clear_node_array();
    NodeValue *actual_value = dict_get("foo");
    assert(actual_value == NULL);
}

int main()
{
    test_insert_key();
    test_get();
    test_update_value();
    test_get_null();
    // print_all();
    return 0;
}

#endif
