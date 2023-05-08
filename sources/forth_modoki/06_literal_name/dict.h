#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

enum NodeValueType
{
    NODE_NUMBER,
    NODE_FUNC,
};

typedef struct
{
    enum NodeValueType type;
    union
    {
        int number;
        void (*cfunc)();
    } u;
} NodeValue;

int dict_push_number(char *key, int value);
int dict_push_cfunc(char *key, void (*cfunc)());
NodeValue *dict_get(char *key);