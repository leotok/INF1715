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
    TK_ID
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
    "TK_ID"
};

typedef union seminfo {
    char *s;
    int i;
    float f;
} SemInfo;

extern SemInfo seminfo;