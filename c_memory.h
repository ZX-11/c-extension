#ifndef C_MEMORY
#define C_MEMORY
#include "c_extension.h"

#define free_s(ptr) {if(ptr)free(ptr);ptr=NULL;}
#define deref(ptr) (*expect(ptr))

// unique_ptr

#define unique __attribute__((cleanup(__unique_free)))

__attribute__((always_inline))
inline void __unique_free(void *ptr) { free_s(*(void**)ptr); }

#define umove(up) ({var(t,up);up=NULL;t;})
#define utake(up) unique void* __##up##__ = up;
#define ureset(up,ptr) (free(up),up=ptr,ptr=NULL,up)

// shared_ptr

#define shared __attribute__((cleanup(__shared_free)))

__attribute__((always_inline))
inline void __shared_free(void *ptr) {
    if (--((size_t*)(*(void**)ptr))[-1] == 0)
        free(&((size_t*)(*(void**)ptr))[-1]);
}

#define smalloc(size) ({size_t* p=malloc(sizeof(size_t)+size);p[0]=1;(void*)(p+1);})
#define srealloc(block,size) ({size_t* p=realloc(block,sizeof(size_t)+size);(void*)(p+1);})
#define sref(sp) (((size_t*)sp)[-1]++,sp)

#define snew(T,...) ((T*)memcpy(expect(smalloc(sizeof(T))), &(T){__VA_ARGS__}, sizeof(T)))
#define smake(T,size) ((T*)expect(smalloc(size*sizeof(T))))

#endif
