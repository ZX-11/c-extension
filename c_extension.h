#ifndef C_EXTENSION
#define C_EXTENSION
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// deducing types

#define typeof(T) __typeof__(T)
#define var(l,r) typeof(r) l=r
#define auto __auto_type

// errors

#ifdef DEBUG
#define panic(info) (fprintf(stderr, "panicked at '%s', %s:%zu\n\tinfo: %s\n",__FUNCTION__,__FILE__,__LINE__, info),exit(1))
#else
#define panic(info) (fprintf(stderr,"panicked: %s\n",info),exit(1))
#endif

#define expect(expr) ({errno=0;var(_expr,(expr));if(!(_expr))panic(errno?strerror(errno):"Assert failed: "#expr);_expr;})

// memory

#define new(T,...) ((T*)memcpy(expect(malloc(sizeof(T))), &(T){__VA_ARGS__}, sizeof(T)))
#define make(T,size) ((T*)expect(calloc(size,sizeof(T))))
#ifdef _WIN32
    #define malloc_usable_size(ptr) _msize(ptr)
#endif

// multiple returns

#define pair(T1,T2) struct {T1 _1;T2 _2;}
#define triple(T1,T2,T3) struct {T1 _1;T2 _2;T3 _3;}

#define tie(R1,R2,fn) {var(p,fn);R1=p._1;R2=p._2;}
#define tie3(R1,R2,R3,fn) {var(t,fn);R1=t._1;R2=t._2;R3=t._3;}

// macro functions

#define abs(x) ({var(_x,x);(_x)<0?(-_x):(_x);})
#undef max
#define max(x,y) ({var(_x,(x));var(_y,(y));_x>_y?_x:_y;})
#undef min
#define min(x,y) ({var(_x,(x));var(_y,(y));_x<_y?_x:_y;})
#define arrlen(x) (sizeof(x)/sizeof((x)[0]))
#define swap(x,y) {var(t,x);x=y;y=t;}
#define arglen(T,...) sizeof((T[]){__VA_ARGS__})/sizeof(T)

// extend for and if

#define for_array(i,arr) for (size_t i=0;i<arrlen(arr);i++)
#define for_string(p,str) for (char *p=str;*p;p++)
#define for_range(i,start,end) for (int64_t i=start;i<end;i++)
#define for_under(i,end) for (size_t i=0;i<end;i++)

// files

#define autoclose __attribute__((cleanup(__close_file)))

__attribute__((always_inline))
inline void __close_file(void* fpp) { if (*(FILE**)fpp) {fclose(*(FILE**)fpp); *(FILE**)fpp = NULL;} }

// string

static inline char *substring(char* dest, char *src, int64_t start, int64_t end) {
    size_t len = strlen(src);
    size_t newlen = end < start || end > len ? len - start : end - start;
    return strncpy(dest ? dest : make(char, newlen+1), src + start, newlen);
}

#define concat(dest,...) __concat__(dest,sizeof((char*[]){__VA_ARGS__})/sizeof(char*),(char*[]){__VA_ARGS__})
static inline char *__concat__(char* dest, size_t argc, char** argv) {
    size_t total_len = 1, len[argc];
    for (size_t i = 0; i < argc; i++) {
        total_len += (len[i] = strlen(argv[i]));
    }
    char* newstr = dest ? dest : make(char, total_len);
    for (size_t i = 0, offset = 0; i < argc; offset += len[i], i++) {
        strcpy(newstr + offset, argv[i]);
    }
    return newstr;
}

#endif