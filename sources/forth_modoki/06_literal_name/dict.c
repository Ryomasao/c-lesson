#include "dict.h"

#define TABLE_SIZE 1024

typedef struct Node
{
    char *key;
    int value;
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

int dict_push(char *key, int value)
{
    int idx = hash(key);
    Node *head = node_array[idx];

    // node_arrayはNode型へのポインタを格納した配列
    if (head == NULL)
    {
        // ポインタが指しているアドレスがNULLの場合、Node型のサイズ分メモリを確保して、
        // そのメモリのアドレスを配列にセットしなおす
        head = (Node *)malloc(sizeof(Node));
        head->next = NULL;
        head->key = key;
        head->value = value;
        node_array[idx] = head;
        return idx;
    }

    Node *node = head;

    for (;;)
    {
        // update
        if (strcmp(node->key, key) == 0)
        {
            node->value = value;
            break;
        }

        // insert
        if (node->next == NULL)
        {
            Node *new_node = (Node *)malloc(sizeof(Node));
            new_node->key = key;
            new_node->value = value;
            node->next = new_node;
            break;
        }
        node = node->next;
    }
    return idx;
}

int *dict_get(char *key)
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

#ifdef INVESTIGATE

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

    int idx = dict_push(expect_key, expect_value);
    Node *head = node_array[idx];
    assert(head->value = expect_value);
    assert(strcmp(head->key, expect_key) == 0);
}

void test_get()
{
    clear_node_array();
    char *expect_key = "foo";
    int expect_value = 1;
    dict_push(expect_key, expect_value);
    int *actual_value = dict_get(expect_key);
    assert(*actual_value = expect_value);
}

void test_update_value()
{
    clear_node_array();
    char *expect_key = "foo";
    int expect_value = 10;
    dict_push(expect_key, 1);
    dict_push(expect_key, expect_value);
    int *actual_value = dict_get(expect_key);
    assert(*actual_value = expect_value);
}

void test_get_null()
{
    clear_node_array();
    int *actual_value = dict_get("foo");
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
