#include <stdio.h>
#include <stdlib.h>

#define FRAMESIZE 12

void refresh(int* src, int push_item, int pop_index, int length){
    if(pop_index == length - 1)
        src[length - 1] = push_item;
    else{
        for(int i = pop_index; i < length - 1; i++){
            src[i] = src[i + 1];
        }
        src[length - 1] = push_item;
    }
}

int opitimal(int* src, int*frames, int n_frames, int n_items){
    int target;
    int target_1;
    int max = 0;
    int temp;
    int k = 0;
    int index;
    int found = 0;
    int count = 0;
    int page_fault = 0;
    int m = 0;
    for(int l = 0; l < n_frames; l++){
        frames[l] = src[l];
        page_fault++;
    }
    
    for(int z = n_frames; z < n_items; z++){
        target = src[z];
        //printf("target = %d\n", target);
        while(k < n_frames){
            if(frames[k] == target){
                found = 1;
                break;
            }
            k++;
        }
        if(found == 0){
            page_fault++;
            
            while(m < n_frames){
                target_1 = frames[m];
                temp = z;
                while(temp < n_items){
                    if(target_1 == src[temp]){
                        if(temp > max){
                            max = temp;
                            index = m;
                        }
                        break;
                    }
                    temp++;
                }
                if(temp >= n_items){
                    refresh(frames, target, m, n_frames);
                    count = 1;
                    break;
                }
                m++;
            }
            m = 0;
            if(count == 0)
                refresh(frames, target, index, n_frames);
            count = 0;
        }
        for(int p = 0; p < n_frames; p++){
            //printf("%d ", frames[p]);
        }
        //printf("\n");
        max = 0;
        found = 0;
        k = 0;
    }
    return page_fault;
}

int leastRT(int* src, int*frames, int n_frames, int n_items){
    int target;
    int target_1;
    int max = 0;
    int temp;
    int k = 0;
    int index;
    int found = 0;
    int count = 0;
    int page_fault = 0;
    int m = 0;
    for(int l = 0; l < n_frames; l++){
        frames[l] = src[l];
        page_fault++;
    }
    
    for(int z = n_frames; z < n_items; z++){
        target = src[z];
        //printf("target = %d\n", target);
        while(k < n_frames){
            if(frames[k] == target){
                found = 1;
                refresh(frames, target, k, n_frames);
                break;
            }
            k++;
        }
        if(found == 0){
            page_fault++;
            refresh(frames, target, 0, n_frames);
            
        }
        for(int p = 0; p < n_frames; p++){
            //printf("%d ", frames[p]);
        }
        //printf("\n");
        max = 0;
        found = 0;
        k = 0;
    }
    
    
    
    return page_fault;
}
int clock(int* src, int*frames, int n_frames, int n_items){
    int target;
    int* clock_bit = (int*)malloc(sizeof(int) * n_frames);
    int target_1;
    int max = 0;
    int temp;
    int k = 0;
    int index;
    int found = 0;
    int count = 0;
    int page_fault = 0;
    int m = 0;
    int ptr = 0;
    int bit_index = 0;
    for(int l = 0; l < n_frames; l++){
        frames[l] = src[l];
        clock_bit[l] = 1;
        page_fault++;
        ptr++;
    }
    ptr = ptr % n_frames;
    for(int z = n_frames; z < n_items; z++){
        target = src[z];
        //printf("target = %d\n", target);
        while(k < n_frames){
            if(frames[k] == target){
                found = 1;
                break;
            }
            k++;
        }
        if(found == 0){
            page_fault++;
            while(1){
                ptr = ptr % n_frames;
            if(clock_bit[ptr] == 0){
                frames[ptr] = target;
                clock_bit[ptr] = 1;
                ptr++;
                break;
            }
            
            if(clock_bit[ptr] == 1){
                clock_bit[ptr] = 0;
                ptr++;
            }
            }
            
        }
        for(int p = 0; p < n_frames; p++){
            //printf("%d ", frames[p]);
            //printf("clock_bits = %d ", clock_bit[p]);
        }
        //printf("\n");
        found = 0;
        k = 0;
        ptr = ptr % n_frames;
    }
    return page_fault;
}

int main(int argc, char** argv){
   /* read from the file and put elements into an array */
    FILE *myFile;
    int buffer[5000];
    char* filename = argv[2];
    int num_frames = atoi(argv[1]);
    int* opt = (int*) malloc(sizeof(int) * num_frames);
    int* lru = (int*) malloc(sizeof(int) * num_frames);
    int* clk = (int*) malloc(sizeof(int) * num_frames);
    myFile = fopen(filename, "r");
    
    int i;
    
    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }
    int d = 0;
    int num;
    while(fscanf(myFile, "%d", &num) > 0) {
        buffer[d] = num;
        d++;
    }
    
    int numberArray[d];
    for(int w = 0; w < d; w++){
        numberArray[w] = buffer[w];
    }
    for(int j = 0; j < d; j++){
        //printf("%d ", numberArray[j]);
    }
     //printf("\n");
    fclose(myFile);
    printf("Optimal : \n");
    
    int page_F;
    
    page_F = opitimal(numberArray, opt, num_frames, d);
    printf("  - frames: ");
    for(int g = 0; g < num_frames; g++){
        printf("%d ", opt[g]);
    }
    printf("\n");
    printf("  - page faults: %d\n", page_F);
    printf("\n");
     
    printf("LRU : \n");
    
    page_F = leastRT(numberArray, lru, num_frames, d);
    printf("  - frames: ");
    for(int g = 0; g < num_frames; g++){
        printf("%d ", lru[g]);
    }
    printf("\n");
    printf("  - page faults: %d\n", page_F);
    printf("\n");
    
    printf("Clock : \n");
    page_F = clock(numberArray, clk, num_frames, d);
    printf("  - frames: ");
    for(int g = 0; g < num_frames; g++){
        printf("%d ", clk[g]);
    }
    printf("\n");
    printf("  - page faults: %d\n", page_F);
    printf("\n");
    return 0;
}
