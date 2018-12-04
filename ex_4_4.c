/*
Windows command:
gcc -g -Wall -o p_thread_A1 ex_4_4.c -lm -lpthread
p_thread_A1.exe <integer number of thread_count>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#include "timer.h"

const int MAX_THREADS = 20480;

int thread_count;

void Usage(char* prog_name);
void *Hello(void* rank);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
   long thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double start, finish, elapsed;


   /* Get number of threads from command line */
   if (argc != 2) Usage(argv[0]);

   thread_count = strtol(argv[1], NULL, 10);
   if (thread_count <= 0 || thread_count > MAX_THREADS)
   {
       Usage(argv[0]);
   }
   thread_handles = malloc (thread_count*sizeof(pthread_t));

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
   {
       pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
   }

   printf("Hello from the main thread\n");
   for (thread = 0; thread < thread_count; thread++)
   {
      pthread_join(thread_handles[thread], NULL);
   }

   GET_TIME(finish);
   elapsed = finish - start;
   //printf("thread %ld took %e seconds\n", thread,elapsed);

   //total_time += elapsed;
   free(thread_handles);

   printf("The code to be timed took %e seconds\n", elapsed);
   long avg = (long) elapsed/(long)thread_count;
   printf("average time for each thread is %e seconds\n", avg);


   return 0;
}  /* main */

/*-------------------------------------------------------------------*/
void *Hello(void* rank)
{
   long my_rank = (long) rank;  /* Use long in case of 64-bit system */

   printf("Hello from thread %ld of %d\n", my_rank, thread_count);

   return NULL;
}  /* Hello */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name)
{
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  /* Usage */
