#include <unistd.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<fstream>
using namespace std;
void print_arrays(int* arr, int volumn);
void transfer_arrays(int* src, int* dest, int start, int amount, int src_length);
void *sum_arrays(void *arg)
{
    int *args_array;
    args_array = arg;
    int sum;
    for(int i = 0; i < 6; i++){
        sum+= args_array[i];
    }
    
    printf("The sum is = %d\n",sum);
    
    return NULL;
}

void transfer_arrays(int* src, int* dest, int start, int amount, int src_length){
    int p = 0;
    int temp = start;
    while(p < amount){
        if((src_length - temp) <= p){
        dest[p] = 0;
            p++;
            break;
        }
        dest[p] = src[start];
        start++;
        p++;
    }
}

void print_arrays(int* arr, int volumn){
    for(int j = 0; j < volumn; j++){
        cout << j <<" : " << arr[j] << endl;
    }
}

int main(int argc, char * const argv[])
{
    int count = 0;
    int i = 0;
    int temp;
    //int times;
    string line;
    string temp_line;
    int chunk = atoi(argv[2]);
    
    string filename;
    if (argc != 3) {
        cerr << "Usage: readFile <input file> and the number of threads T" << endl;
        return -1;
    }
    else {
        filename = argv[1];
    }
    cout << "The first argument is : " << filename << endl;
    cout << "The second argument is : " << chunk << endl;
    //cout << "Numbers of lines in the file : " << count << endl;
    int fd = open( filename.c_str(), O_RDONLY);
    if( fd < 0) {
        cerr << "Could not open file " << filename << "\n";
        exit(-1);
    }
    
    /* Computing the amount of integers */
    ifstream file(filename);
    while (getline(file, line)){
        count++;
    }
    cout << "Numbers of lines in the file : " << count << endl;
    file.clear();
    int *array = new int[count];
    close( fd);
    /* put integers into array */
    FILE * pFile;
    int number[count];
    pFile = fopen (argv[1],"r");
    for (int k = 0; k < count; k++) {
        fscanf(pFile, "%d", &number[k]);
        cout<<"Number: " << number[k]<<endl;
        array[k] = number[k];
        
    }
   fclose (pFile);
 
    print_arrays(array, count);
    int amt = (int)(count / chunk) + 1;
    static int vol = amt;
    cout << "There are " << amt << " numbers in an array." << endl;
    int *sub_arr = new int[amt];
    transfer_arrays(array, sub_arr, 0, amt, count);
    int *sub_arr_1 = new int[amt];
    transfer_arrays(array, sub_arr_1, 6, amt, count);
    int *sub_arr_2 = new int[amt];
    transfer_arrays(array, sub_arr_2, 12, amt, count);
    print_arrays(sub_arr, amt);
    printf("\n");
    print_arrays(sub_arr_1, amt);
    printf("\n");
    print_arrays(sub_arr_2, amt);
    //int **ary = new int[chunk][amt];
    //sum_arrays(sub_arr);
    pthread_t tid_sum;
    pthread_create(&tid_sum,NULL,sum_arrays, sub_arr);
    pthread_join(tid_sum,NULL);
    return 0;
}

