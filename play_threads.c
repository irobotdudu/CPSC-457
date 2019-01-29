#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
void *print_sum(void *unused);
void *print_xs(void *unused);

int arr[5] = {2,3,4,5,6};
void *print_xs(void *unused){
    while(1){
        fputc('X', stderr);
        printf("\n");
        sleep(5);
    }
    pthread_exit("Bye");
}

void *print_sum(void *unused){
    int sum = 0;
    for(int i = 0; i < 5; i++){
        sum += arr[i];
    }
    printf("sum = %d\n", sum);
    pthread_exit("Bye");
}
int main(void){
    
    pthread_t thread_id;
    pthread_t thread_id_1;
    pthread_create(&thread_id, NULL, &print_xs, NULL);
    //pthread_create(&thread_id_1, NULL, &print_sum, NULL);
    
    return 0;
}

