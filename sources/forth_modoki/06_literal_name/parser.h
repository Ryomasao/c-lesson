enum LexicalType
{
    NUMBER,
    SPACE,
    EXECUTABLE_NAME,
    LITERAL_NAME,
    OPEN_CURLY,
    CLOSE_CURLY,
    DEF,
    END_OF_FILE,
    UNKNOWN
};

typedef struct
{
    enum LexicalType ltype;
    union
    {
        int number;
        char onechar;
        char *name;
    } u;
} Token;

int parse_one(int prev_ch, Token *out_token);