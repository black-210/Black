#ifndef BLACK_H
#define BLACK_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BLACK_VERSION_MAJOR 1
#define BLACK_VERSION_MINOR 0
#define BLACK_VERSION_PATCH 0

/* ---------- source locations ---------- */
typedef struct {
    const char *filename;
    size_t line;
    size_t column;
} BlackLocation;

typedef struct {
    BlackLocation location;
    char message[512];
} BlackError;

/* ---------- lexer ---------- */
typedef enum {
    BLACK_T_EOF = 0,
    BLACK_T_INVALID,
    BLACK_T_ID,
    BLACK_T_NUMBER,
    BLACK_T_STRING,
    BLACK_T_CHAR,
    BLACK_T_FN,
    BLACK_T_LET,
    BLACK_T_CONST,
    BLACK_T_STATIC,
    BLACK_T_EXTERN,
    BLACK_T_INLINE,
    BLACK_T_UNSAFE,
    BLACK_T_VOLATILE,
    BLACK_T_STRUCT,
    BLACK_T_ENUM,
    BLACK_T_RETURN,
    BLACK_T_KERNEL,
    BLACK_T_SPEC,
    BLACK_T_TARGET,
    BLACK_T_ABI,
    BLACK_T_SECTION,
    BLACK_T_ASM,
    BLACK_T_IF,
    BLACK_T_WHILE,
    BLACK_T_TRUE,
    BLACK_T_FALSE,
    BLACK_T_VOID,
    BLACK_T_BOOL,
    BLACK_T_U8, BLACK_T_U16, BLACK_T_U32, BLACK_T_U64,
    BLACK_T_I8, BLACK_T_I16, BLACK_T_I32, BLACK_T_I64,
    BLACK_T_USIZE, BLACK_T_ISIZE,
    BLACK_T_LPAREN, BLACK_T_RPAREN,
    BLACK_T_LBRACE, BLACK_T_RBRACE,
    BLACK_T_LBRACKET, BLACK_T_RBRACKET,
    BLACK_T_COLON, BLACK_T_COMMA, BLACK_T_SEMI,
    BLACK_T_DOT,
    BLACK_T_ARROW,
    BLACK_T_ASSIGN,
    BLACK_T_PLUS, BLACK_T_MINUS, BLACK_T_STAR, BLACK_T_SLASH, BLACK_T_PERCENT,
    BLACK_T_AMP, BLACK_T_PIPE, BLACK_T_CARET, BLACK_T_TILDE,
    BLACK_T_BANG,
    BLACK_T_EQ, BLACK_T_NE, BLACK_T_LT, BLACK_T_LE, BLACK_T_GT, BLACK_T_GE,
    BLACK_T_AND_AND, BLACK_T_OR_OR,
    BLACK_T_PLUS_EQ, BLACK_T_MINUS_EQ,
    BLACK_T_COLON_COLON,
    BLACK_T_AT
} BlackTokenKind;

typedef struct {
    BlackTokenKind kind;
    const char *text;
    size_t length;
    uint64_t number;
    BlackLocation location;
} BlackToken;

typedef struct {
    const char *source;
    size_t length;
    size_t position;
    size_t line;
    size_t column;
    const char *filename;
} BlackLexer;

typedef struct {
    BlackToken *items;
    size_t count;
    size_t capacity;
} BlackTokenList;

/* ---------- types ---------- */
typedef enum {
    BLACK_TYPE_VOID,
    BLACK_TYPE_BOOL,
    BLACK_TYPE_U8, BLACK_TYPE_U16, BLACK_TYPE_U32, BLACK_TYPE_U64,
    BLACK_TYPE_I8, BLACK_TYPE_I16, BLACK_TYPE_I32, BLACK_TYPE_I64,
    BLACK_TYPE_USIZE, BLACK_TYPE_ISIZE,
    BLACK_TYPE_POINTER,
    BLACK_TYPE_ARRAY,
    BLACK_TYPE_STRUCT,
    BLACK_TYPE_ENUM,
    BLACK_TYPE_FUNCTION
} BlackTypeKind;

typedef struct BlackType BlackType;
typedef struct BlackExpr BlackExpr;
typedef struct BlackStmt BlackStmt;
typedef struct BlackDecl BlackDecl;

typedef struct {
    char *name;
    BlackType *type;
} BlackField;

struct BlackType {
    BlackTypeKind kind;
    int is_const;
    int is_volatile;
    union {
        struct { BlackType *base; } pointer;
        struct { BlackType *element; size_t count; } array;
        struct { char *name; BlackField *fields; size_t field_count; } record;
        struct { char *name; } named;
        struct { BlackType **params; size_t param_count; BlackType *return_type; } function;
    } as;
};

/* ---------- AST ---------- */
typedef enum {
    BLACK_EXPR_INT,
    BLACK_EXPR_STRING,
    BLACK_EXPR_CHAR,
    BLACK_EXPR_BOOL,
    BLACK_EXPR_NAME,
    BLACK_EXPR_UNARY,
    BLACK_EXPR_BINARY,
    BLACK_EXPR_ASSIGN,
    BLACK_EXPR_CALL,
    BLACK_EXPR_INDEX,
    BLACK_EXPR_MEMBER,
    BLACK_EXPR_CAST
} BlackExprKind;

typedef enum {
    BLACK_UN_NEG,
    BLACK_UN_NOT,
    BLACK_UN_BITNOT,
    BLACK_UN_DEREF,
    BLACK_UN_ADDR
} BlackUnaryOp;

typedef enum {
    BLACK_BIN_ADD, BLACK_BIN_SUB, BLACK_BIN_MUL, BLACK_BIN_DIV, BLACK_BIN_MOD,
    BLACK_BIN_EQ, BLACK_BIN_NE, BLACK_BIN_LT, BLACK_BIN_LE, BLACK_BIN_GT, BLACK_BIN_GE,
    BLACK_BIN_AND, BLACK_BIN_OR, BLACK_BIN_XOR,
    BLACK_BIN_LOGICAL_AND, BLACK_BIN_LOGICAL_OR
} BlackBinaryOp;

struct BlackExpr {
    BlackExprKind kind;
    BlackLocation location;
    BlackType *resolved_type;
    union {
        uint64_t integer;
        char *string;
        int boolean;
        char character;
        char *name;
        struct { BlackUnaryOp op; BlackExpr *value; } unary;
        struct { BlackBinaryOp op; BlackExpr *left; BlackExpr *right; } binary;
        struct { BlackExpr *left; BlackExpr *right; } assign;
        struct { BlackExpr *callee; BlackExpr **args; size_t arg_count; } call;
        struct { BlackExpr *base; BlackExpr *index; } index;
        struct { BlackExpr *base; char *member; } member;
        struct { BlackType *type; BlackExpr *value; } cast;
    } as;
};

typedef enum {
    BLACK_ST_BLOCK,
    BLACK_ST_LET,
    BLACK_ST_EXPR,
    BLACK_ST_RETURN,
    BLACK_ST_IF,
    BLACK_ST_WHILE,
    BLACK_ST_ASM
} BlackStmtKind;

struct BlackStmt {
    BlackStmtKind kind;
    BlackLocation location;
    union {
        struct { BlackStmt **items; size_t count; } block;
        struct { char *name; BlackType *type; BlackExpr *initializer; int is_const; int is_static; int is_volatile; } let;
        struct { BlackExpr *expr; } expr;
        struct { BlackExpr *expr; } ret;
        struct { BlackExpr *condition; BlackStmt *then_branch; BlackStmt *else_branch; } if_stmt;
        struct { BlackExpr *condition; BlackStmt *body; } while_stmt;
        struct { char *text; } asm_stmt;
    } as;
};

typedef struct {
    char *name;
    BlackType *type;
    int is_const;
    int is_volatile;
} BlackParam;

typedef struct {
    char *name;
    BlackType *underlying;
} BlackEnumValue;

typedef enum {
    BLACK_DECL_FUNCTION,
    BLACK_DECL_STRUCT,
    BLACK_DECL_ENUM,
    BLACK_DECL_EXTERN_VAR,
    BLACK_DECL_SPEC
} BlackDeclKind;

struct BlackDecl {
    BlackDeclKind kind;
    BlackLocation location;
    char *name;
    union {
        struct {
            BlackParam *params; size_t param_count;
            BlackType *return_type; BlackStmt *body;
            int is_static, is_extern, is_inline, is_unsafe;
            char *section; char *abi; char *interrupt_arch;
        } function;
        struct { BlackField *fields; size_t field_count; } structure;
        struct { BlackEnumValue *values; size_t value_count; } enumeration;
        struct { BlackType *type; int is_const; int is_volatile; char *section; } extern_var;
        struct { char *text; } spec;
    } as;
};

typedef struct {
    BlackDecl **decls;
    size_t count;
    size_t capacity;
    int is_kernel;
    char *target;
    char *abi;
} BlackProgram;

/* ---------- compatibility structs from the original C12 API ---------- */
typedef BlackToken C12Token;
typedef BlackLexer C12Lexer;
typedef BlackTokenList C12TokenList;
typedef struct { const char *name; const char *type; } C12Field;
typedef struct { const char *name; C12Field *fields; size_t field_count; } C12Struct;
typedef BlackTokenKind C12TokenKind;

/* Original API preserved, now implemented by Black. */
void c12_lexer_init(C12Lexer *lexer, const char *source, size_t length);
C12Token c12_next_token(C12Lexer *lexer);
int c12_lex(const char *source, C12TokenList *list);
void c12_free_tokens(C12TokenList *list);
int c12_compile(const char *source, char *output, size_t output_size);
int c12_compile_string(const char *source, char *output, size_t output_size);
void c12_compile_string_to_file(const char *source, const char *output_path);
int c12_compile_file_to_file(const char *input_path, const char *output_path);

/* ---------- Black public compiler API ---------- */
void black_lexer_init(BlackLexer *lexer, const char *source, size_t length, const char *filename);
BlackToken black_next_token(BlackLexer *lexer);
int black_lex(const char *source, const char *filename, BlackTokenList *list, BlackError *error);
void black_free_tokens(BlackTokenList *list);

BlackProgram *black_parse(const BlackTokenList *tokens, BlackError *error);
void black_free_program(BlackProgram *program);
int black_semantic_check(BlackProgram *program, BlackError *error);
int black_codegen_c(const BlackProgram *program, char *output, size_t output_size, BlackError *error);
int black_compile_string(const char *source, const char *filename, char *output, size_t output_size, BlackError *error);
int black_compile_file_to_file(const char *input_path, const char *output_path);

const char *black_token_name(BlackTokenKind kind);
const char *black_type_name(const BlackType *type);
void black_error_print(const BlackError *error, FILE *stream);
void
#ifdef __cplusplus
}
#endif
#endif
typedef struct {
    BlackDecl **decls;
    size_t count;
    size_t capacity;
    int is_kernel;
    char *target;
    char *abi;
} BlackProgram;
