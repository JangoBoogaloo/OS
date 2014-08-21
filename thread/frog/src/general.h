#ifndef GENERAL_H__
#define GENERAL_H__

#include <stdlib.h>

/* pthread function */
#include <pthread.h>
#include <syslog.h>

extern pthread_mutex_t console_mutex;
/**
 * App Exit 
 */
#define APP_EXIT(status) do{printf("Exit at %s:%d, status=%d\n", __FILE__, __LINE__,  status);\
   exit(status);}while(0)

void *checked_malloc(const char* file, const int line, size_t size);
#define MALLOC(size) checked_malloc(__FILE__, __LINE__, size)

#endif
