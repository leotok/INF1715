#include <limits.h>

typedef union seminfo {
    char *s;
    int i;
    float f;
} SemInfo;

extern SemInfo seminfo;
extern int yy_lines;
extern int yylex(void);
