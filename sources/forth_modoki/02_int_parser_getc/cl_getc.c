#include "clesson.h"
#include <string.h>

#define MAX_SIZE_INPUT 63

static char input[MAX_SIZE_INPUT + 1] = "123 456";
static int pos = 0;

int cl_getc()
{
    if (strlen(input) == pos)
        return -1;
    return input[pos++];
}

void cl_getc_set_src(char *str)
{
    pos = 0;
    strncpy(input, str, MAX_SIZE_INPUT);
}