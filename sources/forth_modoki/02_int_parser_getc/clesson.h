#include <stdio.h>

/*
return one character and move cursor.
return EOF if end of file.
*/
int cl_getc();

void cl_getc_set_src(char *str);

typedef enum
{
    ND_INT,   // INT型
    ND_SPACE, // SPACE型
    ND_EOF    // EOF
} NodeKind;

typedef struct Node
{
    NodeKind kind;
    int val;
} Node;