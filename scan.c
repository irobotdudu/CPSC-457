#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    /* read from the file and put elements into an array */
    int buffer[5000];
    FILE *myFile;
    char* filename = argv[1];
    myFile = fopen(filename, "r");
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    int i=0;
    int num;
    while(fscanf(myFile, "%d", &num) > 0) {
        buffer[i] = num;
        i++;
    }
    for(int j = 0; j < i; j++){
        printf("%d ", buffer[j]);
    }
    return 0;
}
