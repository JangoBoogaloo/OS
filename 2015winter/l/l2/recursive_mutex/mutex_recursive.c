#define _GNU_SOURCE

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>

pthread_mutexattr_t attributes;
pthread_mutex_t mutex;

void test_recursive_mutex(){
  int i, j;

  pthread_mutexattr_init(&attributes);

  pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mutex, &attributes);

  printf("Testing recursive mutex.\n");

  for(i = 0; i < 10; i++){
    for(j = 0; j < i; j++)
      printf("%c", ' ');
    printf("locking\n");
    pthread_mutex_lock(&mutex);
  }// for

  for(i = 9; i >= 0; i--){
    for(j = 0; j < i; j++)
      printf("%c", ' ');
    printf("unlocking\n");
    pthread_mutex_unlock(&mutex);
  }

}

int main(){
  test_recursive_mutex();
  printf("\nEnd of processing\n");
  return 0;
}// main
