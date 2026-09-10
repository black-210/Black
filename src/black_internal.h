#ifndef BLACK_INTERNAL_H
#define BLACK_INTERNAL_H
#include "black.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

void *black_xcalloc(size_t n, size_t s);
void *black_xrealloc(void *p, size_t n);
char *black_xstrndup(const char *s, size_t n);
void black_set_error(BlackError *e, BlackLocation loc, const char *fmt, ...);
int black_token_is_type(BlackTokenKind k);
BlackType *black_type_new(BlackTypeKind k);
BlackType *black_type_clone(const BlackType *t);
void black_type_free(BlackType *t);
int black_type_equal(const BlackType *a, const BlackType *b);
int black_type_is_integer(const BlackType *t);
int black_type_is_signed(const BlackType *t);
size_t black_type_size(const BlackType *t);
void black_expr_free(BlackExpr *e);
void black_stmt_free(BlackStmt *s);
void black_decl_free(BlackDecl *d);

#endif
