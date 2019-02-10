#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define Buffer_Limit 10000

int buffer_index = 0;
char *buffer_queue;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Buffer_Queue_Not_Full = PTHREAD_COND_INITIALIZER;
pthread_cond_t Buffer_Queue_Not_Empty = PTHREAD_COND_INITIALIZER;

void *Consumer() 
{
    while(1){
        pthread_mutex_lock(&mutex);
        if(buffer_index == -1)
        {
            pthread_cond_wait(&Buffer_Queue_Not_Empty, &mutex);
        }
        printf("Consumer:%d\n", buffer_index--); // decrement
        pthread_mutex_unlock(&mutex); // once done, unlock for next pthread_mutex_lock
        pthread_cond_signal(&Buffer_Queue_Not_Full);   
    }
   
}

void *Producer() 
{
    while(1){
    pthread_mutex_lock(&mutex);
    if(buffer_index == Buffer_Limit)
    {
        pthread_cond_wait(&Buffer_Queue_Not_Full, &mutex);
    }
    printf("Producer:%d\n", buffer_index++); // decrement
    pthread_mutex_unlock(&mutex); // once done, unlock for next pthread_mutex_lock
    pthread_cond_signal(&Buffer_Queue_Not_Empty); 
    }
}

int main()
{    
      pthread_t producer_thread_id, consumer_thread_id;
      buffer_queue = (char *) malloc(sizeof(char) * Buffer_Limit);            
      pthread_create(&producer_thread_id, NULL, Producer, NULL); // int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
      pthread_create(&consumer_thread_id, NULL, Consumer, NULL);
      pthread_join(consumer_thread_id, NULL);
      pthread_join(producer_thread_id, NULL);
      return 0;
}