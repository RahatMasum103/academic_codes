/*
Windows command:
gcc -g -Wall -o p_thread_A2 ex_4_2.c -lm -lpthread
p_thread_A2.exe <integer number of thread_count>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#include "timer.h"
#include <math.h>

const int MAX_THREADS = 20480;
const int total_iteration = 1020;

int thread_count;

void Usage(char* prog_name);
float randNumGen();
void *calculate_pi(void *thread_id);

int main(int argc, char *argv[])
{

   double start, finish, elapsed;
   pthread_t* thread_handles;
   long t;
   long pi_estimate;
   double count=0;

   /* Get number of threads from command line */
   if (argc != 2) Usage(argv[0]);

   thread_count = strtol(argv[1], NULL, 10);
   if (thread_count <= 0 || thread_count > MAX_THREADS)
   {
       Usage(argv[0]);
   }
   thread_handles = malloc (thread_count*sizeof(pthread_t));

   GET_TIME(start);
   for(t=0;t<thread_count;t++)
    {
        pthread_create(&thread_handles[t], NULL, calculate_pi, (void *)t);
    }

   for(t=0;t<thread_count;t++)
    {
      pthread_join(thread_handles[t], NULL);
    }

     GET_TIME(finish);
     elapsed = finish - start;
     free(thread_handles);

     pi_estimate = 4*(thread_count/(double)total_iteration);
     printf("Value for Pi is %e \n",pi_estimate);
     printf("The code to be timed took %e seconds\n", elapsed);
}

/*---------------------------------------------------------------------------------------*/

float randNumGen()
{
   int random_value = rand(); //Generate a random number
   float unit_random = random_value / (float) RAND_MAX; //make it between 0 and 1
   return unit_random;
}

/*-------------------------------------------------------------------*/
void Usage(char* prog_name)
{
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  /* Usage */


/*---------------------------------------------------------------------*/
void *calculate_pi(void *thread_id)
{
   long longTid;
   longTid = (long)thread_id;

   int tid = (int)longTid;
   float *number_in_circle = (float *)malloc(sizeof(float));
   *number_in_circle=0;

   long number_of_tosses= (long) total_iteration/(long) thread_count;
   int toss=0;

   //calculation
   for(toss=0;toss<number_of_tosses;toss++){
      float x = randNumGen();
      float y = randNumGen();

      float distance_squared  = sqrt((x*x) + (y*y));

      if(distance_squared <1){
         *number_in_circle+=1;
      }
   }
}

