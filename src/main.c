#include "black.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void help(void){
    puts("Black compiler 1.0.0");
    puts("Specification-oriented systems language -> C11");
    puts("");
    puts("Usage:");
    puts("  black input.black output.c");
    puts("  black -e \"source\" output.c");
    puts("");
    puts("Options:");
    puts("  -h, --help       Show this help");
    puts("  --version        Show version");
}
int main(int argc,char**argv){
    if(argc<2){help();return 1;}
    if(!strcmp(argv[1],"-h")||!strcmp(argv[1],"--help")){help();return 0;}
    if(!strcmp(argv[1],"--version")){puts("black 1.0.0");return 0;}
    if(!strcmp(argv[1],"-e")){if(argc<4){fprintf(stderr,"black: -e requires source and output path\n");return 2;}size_t n=strlen(argv[2])*8+8192;char*out=malloc(n);if(!out)return 2;BlackError e={0};int r=black_compile_string(argv[2],"<command-line>",out,n,&e);if(r<0){black_error_print(&e,stderr);free(out);return 1;}FILE*f=fopen(argv[3],"wb");if(!f){perror(argv[3]);free(out);return 1;}fputs(out,f);fclose(f);free(out);return 0;}
    if(argc<3){fprintf(stderr,"black: missing output path\n");return 2;}
    return black_compile_file_to_file(argv[1],argv[2])==0?0:1;
}
