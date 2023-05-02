#include <stdbool.h>
#include <string.h>
#include <assert.h>

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

bool stack_push_number(int data);
bool stack_push_name(char *data);
bool stack_pop(StackData *out_data);
