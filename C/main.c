/*
 * Ray Weiming Luo
 * CSCI 322
 *
 * Program computes a 4*4 matrix with a 4*1000 matrix together 
 * with multiple threads.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrix.h"
#include "rdtsc.h"

bool verify(result_matrix_t result);

/* A struct to hold the pointer to the data that can be referenced
 * in other methods. Type will be args_t. */
typedef struct {
  result_matrix_t *result;
  unsigned num_threads;
  unsigned threadNum;
} args_t;

/* Method computes the math for matrix multiplication split between
 * the total number of threads given as a parameter. */
void* performMath (void *argv) {
  unsigned j;
  unsigned i;
  unsigned k;
  unsigned threadNum;
  unsigned num_threads;
  result_matrix_t *result;

  /* Get the values of the struct that was passed into argv. */
  args_t* args = (args_t*)(argv);
  result = args->result;
  num_threads = args->num_threads;
  threadNum = args->threadNum;

  /* Matrix multiplication split between number of threads. */
  for (i = 0; i < 4; ++i) {
    for (j = (threadNum*1000)/num_threads; j < ((threadNum+1)*1000)/num_threads; ++j) {
      (*result)[i][j] = 0;
      for (k = 0; k < 4; ++k) {
        (*result)[i][j] += LEFT[i][k]*RIGHT[k][j];
      }
    }
  }
  
  return NULL;
}

/*  This program times the multiplication of a 4x4 matrix, LEFT, by a 4x1000
 *  matrix, RIGHT.  The result is, of course, a 4x1000 matrix.  If the answer is
 *  correct, then the time is reported.
 */

int main(int argc, char* argv[]) {
  result_matrix_t result;
  int i, j, k;
  unsigned num_threads = 1;

  /* Recieve the number of threads if given, else default 1 thread. */
  if (argc >= 2) {
    num_threads = atoi(argv[1]);
  }

  pthread_t threads[num_threads];
  args_t initArgs[num_threads];

  /* Start the threads. */
  uint64_t start = rdtsc();

  /* Create the number of threads each with the arg_t struct.
   * Then each thread computes the matrix. */
  for (i = 0; i < num_threads; ++i) {
    args_t args = {&result, num_threads, i};
    initArgs[i] = args;
    pthread_create(&threads[i], NULL, performMath, &initArgs[i]);
  }

  /* Join all the threads after computation. */
  for(i = 0; i < num_threads; ++i) {
    pthread_join(threads[i], NULL);
  }
  
  uint64_t end  = rdtsc();

  if(verify(result)) {
    printf("time = %zu\n", end - start);
    printf("Good job!\n");
  }
  
  return EXIT_SUCCESS;
}

/* Verify that the result matrix is the correct answer.
 * 
 */
bool verify(result_matrix_t result) {
  bool error = false;
  int i, j;
  for(i = 0; !error && i < 4; ++i)
  {
    for(j = 0; !error && j < 1000; ++j)
    {
      if(ANSWER[i][j] != result[i][j]) {
        fprintf(stderr, "error at [%d,%d]\n", i, j);
        error = true;
      }
    }
  }
  return !error;
}
