#ifndef C_VECTOR
#define C_VECTOR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "c_extension.h"

typedef struct { size_t len, cap; void* data[]; } vector;

// func vec::head(&self) -> vector
#define vec_head(vec)        (*(vector*)((size_t*)(vec)-2))

// func vec::len(&self) -> size_t
#define vec_len(vec)         (vec_head(vec).len)

// func vec::cap(&self) -> size_t
#define vec_cap(vec)         (vec_head(vec).cap)

// func vec::default<T>() -> *T
#define vec_default(T)       (vector *v=expect(malloc(sizeof(size_t)*2+sizeof(T)*16));v->len=0;v->cap=16;(T*)v->data;})

// func vec::make<T>(length : size_t) -> *T
#define vec_make(T,_len)     ({expect(_len >= 0);vector *v=expect(malloc(sizeof(size_t)*2+sizeof(T)*_len));v->len=v->cap=_len;(T*)v->data;})

// func vec::new<T>(length : size_t, capacity : size_t) -> *T
#define vec_new(T,_len,_cap) ({expect(_len >= 0);var(size,max(_cap,_len));vector *v=expect(malloc(sizeof(size_t)*2+sizeof(T)*size));v->len=_len;v->cap=size;(T*)v->data;})

// func vec::of<T>({...} : T) -> *T
#define vec_of(T,...)        ({size_t size=(sizeof((T[]){__VA_ARGS__})/sizeof(T));(T*)memcpy(vec_make(T,size),&(T[]){__VA_ARGS__},sizeof(T)*size);})

// func vec::resize(&self, new_size : size_t) -> *T
#define vec_resize(vec,size) ({expect(size >= 0);vector* header=&vec_head(vec);var(_size,size);header=expect(realloc(header,sizeof(size_t)*2+sizeof(vec[0])*_size));header->cap=_size;header->len=min(_size,header->len);vec=(void*)header->data;})

// func vec::grow(&self) -> *T
#define vec_grow(vec)        (vec_resize(vec,max(8,(vec_cap(vec)*2))))

// func vec::trim(&self) -> *T
#define vec_trim(vec)        (vec_resize(vec,vec_len(vec)))

// func vec::at(&self, index : size_t) -> &T
#define vec_at(vec,i)        (expect(i < vec_len(vec)),vec[i])

// func vec::push(&self, value : T) -> T
#define vec_push(vec,value)  ({if(vec_len(vec)>=vec_cap(vec))vec_grow(vec);vec[vec_len(vec)++]=value;})

// func vec::pop(&self, value : T) -> T
#define vec_pop(vec,value)   (expect(vec_len(vec) > 0), vec[--vec_len(vec)])

// func vec::begin(&self) -> *T
#define vec_begin(vec)       (vec)

// func vec::end(&self) -> *T
#define vec_end(vec)         (vec+vec_len(vec))

// func vec::front(&self) -> &T
#define vec_front(vec)       (vec[0])

// func vec::back(&self) -> &T
#define vec_back(vec)        (vec[vec_len(vec)-1])

// func vec::delete(&self) -> ()
#define vec_delete(vec)      {free(&vec_head(vec));vec=NULL;}

#endif