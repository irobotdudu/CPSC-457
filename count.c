/// counts number of primes from standard input
///
/// compile with:
///   $ gcc findPrimes.c -O3 -o count -lm
///
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>

int *threadResults;
int64_t *threadBounds;
int nThreads = 1;
int64_t testNum;


/// primality test, if n is prime, return 1, else return 0
int isPrime(int64_t n)
{
     if( n <= 1) return 0; // small numbers are not primes
     if( n <= 3) return 1; // 2 and 3 are prime
     if( n % 2 == 0 || n % 3 == 0) return 0; // multiples of 2 and 3 are not primes
     int64_t i = 5;
     int64_t max = sqrt(n);
     while( i <= max) {
         if (n % i == 0 || n % (i+2) == 0) return 0;
         i += 6;
     }
     return 1;
}

/*
         threadResults:
         0 = No divisors found
         1 = working
         9 = divisor found
 */

void* isPrimeT(int tid){
 int tr = 0;
     if( testNum % 2 == 0 || testNum % 3 == 0) {
         tr = 9;
         return 0;
     } // multiples of 2 and 3 are not primes

int64_t i = 5 + (6 * threadBounds[tid + tid + 1]);

    while( i <= (5 + (6 * threadBounds[tid + tid]))) {
         if (testNum % i == 0 || testNum % (i+2) == 0) {
             tr = 9;
             break;
         }
         i += 6;
    }
    //printf("Thread %d from %ld to %ld\n",tid,threadBounds[tid+tid], threadBounds[tid+tid+1]);
    //printf("Thread %d from %ld to %ld\n",tid,5 + (6 * threadBounds[tid+tid]), 5 + (6 * threadBounds[tid+tid+1]));
threadResults[tid] = tr;
pthread_exit(NULL);
}

int isPrimeThreaded(int64_t n){
    if( n <= 1) return 0; // small numbers are not primes
    if( n <= 3) return 1; // 2 and 3 are prime
int64_t max = sqrt(n);
testNum = n;
int64_t size = ((max - 5) / 6) + 1;
/* number of integers for each thread*/
int64_t Q = size / nThreads;
threadBounds[0] = 0;
int v = Q - 1;
if (v < 1)
    return isPrime(n);
/* Determine the boundary from %d to %d */
int w = 1;
    while(w < (nThreads<<1)){
        threadBounds[w++] = v;
        threadBounds[w++] = v + 1;
        v += Q;
}
threadBounds[(nThreads<<1) - 1] = size;

int j = 0;
/* 1.Assume that is a prime to start with
   2.Stop when divisors are found
   3.Or beyond the boundary
 */
int result = 1;
pthread_t threads[nThreads];
    for (int t = 0; t < nThreads; t++){
        pthread_create(&threads[t], NULL, isPrimeT, t);
    }
int go = 1;
    while (go){
        for (int i = 0; i < nThreads; i++){
            if (threadResults[i] == 9){
                result = 0;
                go = 0;
                break;
            }
            if (threadResults[i] == 0) /* No divisors found */
                j++;
            if (j == nThreads)
                go = 0;
            if (result == 0){
                for (int k=0; k < nThreads; k++)
                    pthread_cancel(threads[k]);
                go = 0;
            }
        }
    }
    return result;
}

int main( int argc, char ** argv)
{
    /// parse command line arguments
        if( argc != 1 && argc != 2) {
        printf("Uasge: countPrimes [nThreads]\n");
        exit(-1);
    }
    if( argc == 2) nThreads = atoi( argv[1]);

    /// handle invalid arguments
    if( nThreads < 1 || nThreads > 256) {
        printf("Bad arguments. 1 <= nThreads <= 256!\n");
    }
    if( nThreads == -1) {
        printf("I am not multithreaded yet :-(\n");
        exit(-1);
    }

    /// count the primes
    printf("Counting primes using %d thread%s.\n",
           nThreads, nThreads == 1 ? "s" : "");
    threadResults=malloc(nThreads*sizeof(int));
    for (int g=0; g<nThreads; g++)
    threadResults[g]=1;
    threadBounds = malloc((nThreads<<1)*sizeof(int64_t));
    int64_t count = 0;
    int64_t *numbers = malloc(640000*sizeof(int64_t));
    int i=0;
    while(1) {
        int64_t num;
        if( 1 != scanf("%ld", & num)) break;
        numbers[i++]=num;
    } if (nThreads==1)
    for (int j=0; j<i; j++)
       count+=isPrime(numbers[j]); 
      else
    for (int j=0; j<i; j++)
    count+=isPrimeThreaded(numbers[j]);
    /// report results
    printf("Found %ld primes.\n", count);

    return 0;
}

