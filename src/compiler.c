#include "black_internal.h"

void black_error_print(const BlackError*e,FILE*f){if(!e)return;fprintf(f,"%s:%zu:%zu: error: %s\n",e->location.filename?e->location.filename:"<input>",e->location.line,e->location.column,e->message);}
int black_compile_string(const char*s,const char*f,char*out,size_t n,BlackError*e){BlackTokenList ts={0};if(black_lex(s,f,&ts,e)<0)return -1;BlackProgram*p=black_parse(&ts,e);black_free_tokens(&ts);if(!p)return -1;if(black_semantic_check(p,e)<0){black_free_program(p);return -1;}int r=black_codegen_c(p,out,n,e);black_free_program(p);return r;}
int black_compile_file_to_file(const char*in,const char*out){FILE*fp=fopen(in,"rb");if(!fp){fprintf(stderr,"black: cannot open '%s'\n",in);return -1;}fseek(fp,0,SEEK_END);long sz=ftell(fp);fseek(fp,0,SEEK_SET);if(sz<0){fclose(fp);return -1;}char*s=malloc((size_t)sz+1);if(!s){fclose(fp);return -1;}if(fread(s,1,(size_t)sz,fp)!=(size_t)sz){free(s);fclose(fp);return -1;}s[sz]=0;fclose(fp);size_t cap=(size_t)sz*8+8192;char*c=malloc(cap);if(!c){free(s);return -1;}BlackError e={0};int r=black_compile_string(s,in,c,cap,&e);if(r<0){black_error_print(&e,stderr);free(c);free(s);return -1;}FILE*fo=fopen(out,"wb");if(!fo){fprintf(stderr,"black: cannot write '%s'\n",out);free(c);free(s);return -1;}fwrite(c,1,strlen(c),fo);fclose(fo);free(c);free(s);return 0;}

/* Legacy C12 API compatibility. The implementation is Black; these names remain so existing callers keep building. */
void c12_lexer_init(C12Lexer*l,const char*s,size_t n){black_lexer_init(l,s,n,"<string>");}
C12Token c12_next_token(C12Lexer*l){return black_next_token(l);}
int c12_lex(const char*s,C12TokenList*l){BlackError e={0};return black_lex(s,"<string>",l,&e);}
void c12_free_tokens(C12TokenList*l){black_free_tokens(l);}
int c12_compile(const char*s,char*out,size_t n){BlackError e={0};return black_compile_string(s,"<string>",out,n,&e);}
int c12_compile_string(const char*s,char*out,size_t n){return c12_compile(s,out,n);}
void c12_compile_string_to_file(const char*s,const char*p){FILE*f=fopen(p,"wb");if(!f)return;size_t cap=strlen(s)*8+8192;char*b=malloc(cap);if(b){if(c12_compile(s,b,cap)==0)fwrite(b,1,strlen(b),f);free(b);}fclose(f);}
int c12_compile_file_to_file(const char*i,const char*o){return black_compile_file_to_file(i,o);}

int black_semantic_check(BlackProgram*p,BlackError*e){if(!p)return 0;for(size_t i=0;i<p->count;i++){BlackDecl*d=p->decls[i];if(d->kind==BLACK_DECL_FUNCTION){for(size_t j=0;j<d->as.function.param_count;j++){BlackType*pt=d->as.function.params[j].type;if(pt->kind==BLACK_TYPE_POINTER&&pt->as.pointer.base->kind==BLACK_TYPE_FUNCTION){black_set_error(e,d->location,"function pointer parameter '%s' is not supported",d->as.function.params[j].name);return -1;}}}}return 0;}