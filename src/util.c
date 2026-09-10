#include "black_internal.h"

void *black_xcalloc(size_t n, size_t s)
{
    void *p = calloc(n, s);
    if (!p && n && s) {
        fprintf(stderr, "black: memory allocation failed\n");
        exit(1);
    }
    return p;
}

void *black_xrealloc(void *p, size_t n)
{
    void *new_p = realloc(p, n);
    if (!new_p && n) {
        fprintf(stderr, "black: memory reallocation failed\n");
        exit(1);
    }
    return new_p;
}

char *black_xstrndup(const char *s, size_t n)
{
    char *p = (char *)malloc(n + 1);
    if (!p)
        return NULL;
    memcpy(p, s, n);
    p[n] = 0;
    return p;
}

void black_set_error(BlackError *e, BlackLocation loc, const char *fmt, ...)
{
    if (!e)
        return;
    
    e->location = loc;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(e->message, sizeof(e->message), fmt, ap);
    va_end(ap);
}

const char *black_token_name(BlackTokenKind k)
{
#define N(x) case x: return #x
    switch (k) {
    N(BLACK_T_EOF);
    N(BLACK_T_ID);
    N(BLACK_T_NUMBER);
    N(BLACK_T_STRING);
    N(BLACK_T_CHAR);
    N(BLACK_T_FN);
    N(BLACK_T_LET);
    N(BLACK_T_CONST);
    N(BLACK_T_STATIC);
    N(BLACK_T_EXTERN);
    N(BLACK_T_INLINE);
    N(BLACK_T_UNSAFE);
    N(BLACK_T_VOLATILE);
    N(BLACK_T_STRUCT);
    N(BLACK_T_ENUM);
    N(BLACK_T_RETURN);
    N(BLACK_T_KERNEL);
    N(BLACK_T_SPEC);
    N(BLACK_T_TARGET);
    N(BLACK_T_ABI);
    N(BLACK_T_SECTION);
    N(BLACK_T_ASM);
    N(BLACK_T_IF);
    N(BLACK_T_WHILE);
    N(BLACK_T_TRUE);
    N(BLACK_T_FALSE);
    N(BLACK_T_LPAREN);
    N(BLACK_T_RPAREN);
    N(BLACK_T_LBRACE);
    N(BLACK_T_RBRACE);
    N(BLACK_T_LBRACKET);
    N(BLACK_T_RBRACKET);
    N(BLACK_T_COLON);
    N(BLACK_T_COMMA);
    N(BLACK_T_SEMI);
    N(BLACK_T_DOT);
    N(BLACK_T_ARROW);
    N(BLACK_T_ASSIGN);
    N(BLACK_T_PLUS);
    N(BLACK_T_MINUS);
    N(BLACK_T_STAR);
    N(BLACK_T_SLASH);
    N(BLACK_T_PERCENT);
    N(BLACK_T_AMP);
    N(BLACK_T_PIPE);
    N(BLACK_T_CARET);
    N(BLACK_T_TILDE);
    N(BLACK_T_BANG);
    N(BLACK_T_EQ);
    N(BLACK_T_NE);
    N(BLACK_T_LT);
    N(BLACK_T_LE);
    N(BLACK_T_GT);
    N(BLACK_T_GE);
    N(BLACK_T_AND_AND);
    N(BLACK_T_OR_OR);
    N(BLACK_T_PLUS_EQ);
    N(BLACK_T_MINUS_EQ);
    N(BLACK_T_COLON_COLON);
    N(BLACK_T_AT);
    default:
        return "UNKNOWN";
    }
#undef N
}
