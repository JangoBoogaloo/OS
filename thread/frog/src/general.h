#ifndef GENERAL_H__
#define GENERAL_H__

#include <stdlib.h>

/**
 * App Exit 
 */
#define APP_EXIT(status) do{printf("Exit at %s:%d, status=%d\n", __FILE__, __LINE__,  status);\
   exit(status);}while(0)

#define MALLOC(size) checked_malloc(__FILE__, __LINE__, size)

#endif
