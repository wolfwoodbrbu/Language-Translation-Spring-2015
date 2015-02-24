#define BUFFER_LEGNTH 80
typedef enum { false, true } bool;


typedef enum
{
    BEGIN,
    END,
    READ,
    WRITE,
    ID,
    INTLITERAL,
    LPAREN,
    RPAREN,
    SEMICOLON,
    COMMA,
    ASSIGNOP,
    PLUSOP,
    MINUSOP,
    SCANEOF,
    ERROR
} Token;
