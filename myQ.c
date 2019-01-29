#include <stdio.h>
#include <stdlib.h>

#define FRAMESIZE 12

void refresh(int* src, int item, int index, int length){
    for(int i = index; i < length - 1; i++){
        src[i] = src[i + 1];
    }
    src[length - 1] = item;
}

int main(int argc, char** argv){
    /* read from the file and put elements into an array */
    FILE *myFile;
    char* filename = argv[1];
    int n_frames = atoi(argv[2]);
    printf("%d\n", n_frames);
    int* frames = (int*) malloc(sizeof(int) * n_frames);
    myFile = fopen(filename, "r");
    int numberArray[FRAMESIZE];
    int i;
    
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    
    for (i = 0; i < 12; i++){
        fscanf(myFile, "%d,", &numberArray[i] );
    }
    
    fclose(myFile);
    for (i = 0; i < 12; i++){
        printf("Number is: %d\n\n", numberArray[i]);
    }
    for(int j = 0; j<120; j++){
        printf("*");
    }
    printf("\n");
    refresh(numberArray, 99, 5, 12);
    
    for (i = 0; i < 12; i++){
        printf("Number is: %d\n\n", numberArray[i]);
    }
}
