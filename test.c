#include <pthread.h>
#include <iostream>
#include <cstdlib>

using namespace std;

// The adder thread that will add the number from 1 to n
void *adder(void * number);

// The sum variable that will be updated by all threads
unsigned long sum;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: sum <integer value>" << endl;
        exit(1);
    }
    
    unsigned long number = atol(argv[1]);
    if (number < 0) {
        cerr << "Argument " << number << " must be non-negative." << endl;
        exit(1);
    }
    
    pthread_t tid; // the thread identifierier
    pthread_attr_t attr; // set of attributes for the thread
    
    // get the default attributes
    pthread_attr_init(&attr);
    
    // create the thread
    pthread_create(&tid,&attr,adder,(void *) number);
    
    // Wait for the thread to exit
    pthread_join(tid,NULL);
    
    cout << "Sum(" << 1 << ", " << number << ") = " << sum << endl;
    
    return 0;
}

// Add the numbers from 1 to n
void *adder(void * number)
{
    cout << "Thread ID = " << pthread_self() << endl;
    
    unsigned long num = (unsigned long) number;
    for (int i = 1; i <= num; i++)
    {
        sum += i;
    }
    
    pthread_exit(0);
}
