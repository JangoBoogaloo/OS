#include <stdlib.h>
#include <stdio.h>

void *checked_malloc(const char* file, const int line, size_t size)
{
   void* ret = malloc(size);
    if(!ret){printf("Malloc returned NULL at %s:%d\n", file, line);}
     return ret;
}
