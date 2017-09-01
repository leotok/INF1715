typedef enum
{
    TK_INT = 1,
    TK_FLOAT,
    TK_CHAR,
    TK_VOID,
    TK_NEW,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_AS,
    TK_RETURN,
    TK_DEC,
    TK_ID,
    TK_EQUAL,
    TK_LESSEQUAL,
    TK_GREATEREQUAL,
    TK_AND,
    TK_OR,
    TK_STRING
} token_enum;

extern char *enum_to_text[] = {
    "TK_INT",
    "TK_FLOAT",
    "TK_CHAR",
    "TK_VOID",
    "TK_NEW",
    "TK_IF",
    "TK_ELSE",
    "TK_WHILE",
    "TK_AS",
    "TK_RETURN",
    "TK_DEC",
    "TK_ID",
    "TK_EQUAL",
    "TK_LESSEQUAL",
    "TK_GREATEREQUAL",
    "TK_AND",
    "TK_OR",
    "TK_STRING"
};

typedef union seminfo {
    char *s;
    int i;
    float f;
} SemInfo;

extern SemInfo seminfo;
