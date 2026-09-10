#include "black_internal.h"

typedef struct { const char *s; BlackTokenKind k; } KW;
static const KW keywords[] = {
 {"fn",BLACK_T_FN},{"let",BLACK_T_LET},{"const",BLACK_T_CONST},{"static",BLACK_T_STATIC},{"extern",BLACK_T_EXTERN},{"inline",BLACK_T_INLINE},{"unsafe",BLACK_T_UNSAFE},{"volatile",BLACK_T_VOLATILE},{"struct",BLACK_T_STRUCT},{"enum",BLACK_T_ENUM},{"return",BLACK_T_RETURN},{"kernel",BLACK_T_KERNEL},{"spec",BLACK_T_SPEC},{"target",BLACK_T_TARGET},{"abi",BLACK_T_ABI},{"section",BLACK_T_SECTION},{"asm",BLACK_T_ASM},{"if",BLACK_T_IF},{"while",BLACK_T_WHILE},{"true",BLACK_T_TRUE},{"false",BLACK_T_FALSE},{"void",BLACK_T_VOID},{"bool",BLACK_T_BOOL},{"u8",BLACK_T_U8},{"u16",BLACK_T_U16},{"u32",BLACK_T_U32},{"u64",BLACK_T_U64},{"i8", BLACK_T_I8}, {"i16", BLACK_T_I16}, {"i32", BLACK_T_I32}, {"i64", BLACK_T_I64}, {"usize", BLACK_T_USIZE}, {"isize", BLACK_T_ISIZE}
};
static BlackToken tok(BlackLexer *l, BlackTokenKind k, size_t start, size_t len, uint64_t n){BlackToken t={0};t.kind=k;t.text=l->source+start;t.length=len;t.number=n;t.location.filename=l->filename;t.location.line=l->line;t.location.column=l->column;return t;}
static int peek(BlackLexer*l,size_t n){return l->position+n<l->length?(unsigned char)l->source[l->position+n]:-1;}
static int take(BlackLexer*l){int c=peek(l,0);if(c<0)return c;l->position++;if(c=='\n'){l->line++;l->column=1;}else l->column++;return c;}
static int kw(const char*s,size_t n){for(size_t i=0;i<sizeof(keywords)/sizeof(keywords[0]);i++)if(strlen(keywords[i].s)==n&&!memcmp(s,keywords[i].s,n))return keywords[i].k;return BLACK_T_ID;}
void black_lexer_init(BlackLexer*l,const char*s,size_t n,const char*f){memset(l,0,sizeof(*l));l->source=s;l->length=n;l->line=1;l->column=1;l->filename=f?f:"<string>";}
BlackToken black_next_token(BlackLexer*l){
 for(;;){int c=peek(l,0); if(c<0)return tok(l,BLACK_T_EOF,l->position,0,0); if(isspace(c)){take(l);continue;} if(c=='/'&&peek(l,1)=='/'){while(peek(l,0)>=0&&take(l)!='\n');continue;} if(c=='/'&&peek(l,1)=='*'){take(l);take(l);while(peek(l,0)>=0&&!(peek(l,0)=='*'&&peek(l,1)=='/'))take(l);if(peek(l,0)>=0){take(l);take(l);}continue;} break;}
 size_t st=l->position; size_t line=l->line,col=l->column; int c=take(l);
 if(isalpha(c)||c=='_'){while(isalnum(peek(l,0))||peek(l,0)=='_')take(l);BlackToken t=tok(l,kw(l->source+st,l->position-st),st,l->position-st,0);t.location.line=line;t.location.column=col;return t;}
 if(isdigit(c)){int base=10;uint64_t v=(uint64_t)(c-'0');if(c=='0'&&(peek(l,0)=='x'||peek(l,0)=='X')){base=16;v=0;take(l);}else if(c=='0'&&(peek(l,0)=='b'||peek(l,0)=='B')){base=2;v=0;take(l);}while(1){int d=peek(l,0);int x=-1;if(isdigit(d))x=d-'0';else if(base==16&&d>='a'&&d<='f')x=d-'a'+10;else if(base==16&&d>='A'&&d<='F')x=d-'A'+10;if(x<0||x>=base)break;v=v*(uint64_t)base+(uint64_t)x;take(l);}BlackToken t=tok(l,BLACK_T_NUMBER,st,l->position-st,v);t.location.line=line;t.location.column=col;return t;}
 if(c=='"'||c=='\''){int quote=c;take(l);while(peek(l,0)>=0&&peek(l,0)!=quote){if(peek(l,0)=='\\'){take(l);if(peek(l,0)>=0)take(l);}else take(l);}if(peek(l,0)==quote)take(l);BlackToken t=tok(l,quote=='"'?BLACK_T_STRING:BLACK_T_CHAR,st,l->position-st,0);t.location.line=line;t.location.column=col;return t;}
 BlackTokenKind k=BLACK_T_EOF; size_t len=1;
 switch(c){case '(':k=BLACK_T_LPAREN;break;case ')':k=BLACK_T_RPAREN;break;case '{':k=BLACK_T_LBRACE;break;case '}':k=BLACK_T_RBRACE;break;case '[':k=BLACK_T_LBRACKET;break;case ']':k=BLACK_T_RBRACKET;break;case ':':if(peek(l,0)==':'){take(l);k=BLACK_T_COLON_COLON;len=2;}else k=BLACK_T_COLON;break;case ',':k=BLACK_T_COMMA;break;case ';':k=BLACK_T_SEMI;break;case '.':k=BLACK_T_DOT;break;case '@':k=BLACK_T_AT;break;case '+':if(peek(l,0)=='='){take(l);k=BLACK_T_PLUS_EQ;len=2;}else k=BLACK_T_PLUS;break;case '-':if(peek(l,0)=='>'){take(l);k=BLACK_T_ARROW;len=2;}else if(peek(l,0)=='='){take(l);k=BLACK_T_MINUS_EQ;len=2;}else k=BLACK_T_MINUS;break;case '*':k=BLACK_T_STAR;break;case '/':k=BLACK_T_SLASH;break;case '%':k=BLACK_T_PERCENT;break;case '&':if(peek(l,0)=='&'){take(l);k=BLACK_T_AND_AND;len=2;}else k=BLACK_T_AMP;break;case '|':if(peek(l,0)=='|'){take(l);k=BLACK_T_OR_OR;len=2;}else k=BLACK_T_PIPE;break;case '^':k=BLACK_T_CARET;break;case '~':k=BLACK_T_TILDE;break;case '!':if(peek(l,0)=='='){take(l);k=BLACK_T_NE;len=2;}else k=BLACK_T_BANG;break;case '=':if(peek(l,0)=='='){take(l);k=BLACK_T_EQ;len=2;}else k=BLACK_T_ASSIGN;break;case '<':if(peek(l,0)=='='){take(l);k=BLACK_T_LE;len=2;}else k=BLACK_T_LT;break;case '>':if(peek(l,0)=='='){take(l);k=BLACK_T_GE;len=2;}else k=BLACK_T_GT;break;default:k=BLACK_T_INVALID;break;}
BlackToken t = tok(l, k, st, len, 0);
t.location.line = line;
t.location.column = col;
return t;
}

int black_lex(const char *s, const char *f, BlackTokenList *l, BlackError *e)
{
	memset(l, 0, sizeof(*l));
	BlackLexer x;
	black_lexer_init(&x, s, strlen(s), f);

	for (;;) {
		BlackToken t = black_next_token(&x);

		if (t.kind == BLACK_T_INVALID) {
			if (e)
				black_set_error(e, t.location, "invalid character '%.*s'", (int)t.length, t.text);
			black_free_tokens(l);
			return -1;
		}

		if (l->count == l->capacity) {
			l->capacity = l->capacity ? l->capacity * 2 : 64;
			l->items = black_xrealloc(l->items, l->capacity * sizeof(*l->items));
		}

		l->items[l->count++] = t;

		if (t.kind == BLACK_T_EOF)
			return 0;
	}
}

void black_free_tokens(BlackTokenList *l)
{
    free(l->items);
    memset(l, 0, sizeof(*l));
}

int black_set_error(BlackError *e, BlackLocation loc, const char *fmt, ...)
{
    if (!e)
        return -1;

    va_list args;
    va_start(args, fmt);
    vsnprintf(e->message, sizeof(e->message), fmt, args);
    va_end(args);

    e->location = loc;
    return -1;
}
int black_error_print(const BlackError *e, FILE *f)
{
    if (!e)
        return 0;

    fprintf(f, "%s:%zu:%zu: error: %s\n",
            e->location.filename ? e->location.filename : "<input>",
            e->location.line,
            e->location.column,
            e->message);
    return -1;
}
void black_free_program(BlackProgram *program)
{
    if (!program)
        return;

    for (size_t i = 0; i < program->count; i++) {
        BlackDecl *decl = program->decls[i];
        if (decl) {
            // Free the declaration and its associated resources
            free(decl);
        }
    }

    free(program->decls);
    free(program->target);
    free(program->abi);
    free(program);
}








