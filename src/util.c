#include "black_internal.h"

void *black_xcalloc(size_t n, size_t s) { void *p=calloc(n,s); return p; }
void *black_xrealloc(void *p, size_t n) { return realloc(p,n); }
char *black_xstrndup(const char *s, size_t n) { char *p=(char*)malloc(n+1); if(!p)return NULL; memcpy(p,s,n); p[n]=0; return p; }
void black_set_error(BlackError *e, BlackLocation loc, const char *fmt, ...) {
    if(!e) return;
    e->location=loc;
    va_list ap; va_start(ap,fmt); vsnprintf(e->message,sizeof(e->message),fmt,ap); va_end(ap);
}
const char *black_token_name(BlackTokenKind k) {
#define N(x) case x:return #x
switch(k){N(BLACK_T_EOF);N(BLACK_T_ID);N(BLACK_T_NUMBER);N(BLACK_T_STRING);N(BLACK_T_CHAR);N(BLACK_T_FN);N(BLACK_T_LET);N(BLACK_T_CONST);N(BLACK_T_STATIC);N(BLACK_T_EXTERN);N(BLACK_T_INLINE);N(BLACK_T_UNSAFE);N(BLACK_T_VOLATILE);N(BLACK_T_STRUCT);N(BLACK_T_ENUM);N(BLACK_T_RETURN);N(BLACK_T_KERNEL);N(BLACK_T_SPEC);N(BLACK_T_TARGET);N(BLACK_T_ABI);N(BLACK_T_SECTION);N(BLACK_T_ASM);N(BLACK_T_TRUE);N(BLACK_T_FALSE);N(BLACK_T_VOID);N(BLACK_T_BOOL);N(BLACK_T_U8);N(BLACK_T_U16);N(BLACK_T_U32);N(BLACK_T_U64);N(BLACK_T_I8);N(BLACK_T_I16);N(BLACK_T_I32);N(BLACK_T_I64);N(BLACK_T_USIZE);N(BLACK_T_ISIZE);default:return "UNKNOWN";}
#undef N
}
