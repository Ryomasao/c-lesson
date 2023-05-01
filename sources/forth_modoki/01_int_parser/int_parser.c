#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

static const char *const input = "123 456  1203";
int position = 0;

int main()
{
    // write something here.
    int unit = 0;

    int a[3] = {};
    int index = 0;
    int row = 0;

    while (input[position])
    {
        if (isdigit(input[position]))
        {
            a[row] = a[row] * 10;
            a[row] += input[position] - '0';
        }
        else
        {
            if (a[row] != 0)
            {
                row++;
            }
        }
        position++;
    }

    int answer1 = 0;
    int answer2 = 0;
    int answer3 = 0;

    answer1 = a[0];
    answer2 = a[1];
    answer3 = a[2];

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);
    assert(answer3 == 1203);
}
