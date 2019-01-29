
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
int main(void){
    int incre = 0;
    int nfiles = 5;
    int fatSize = 8;
    int* temp = (int*)malloc(sizeof(int)*fatSize);
    int shared[nfiles][fatSize];
    for(int w = 0; w < nfiles; w++){
        for(int v = 0; v < fatSize; v++){
            shared[w][v] = incre;
            cout << " " << shared[w][v];
        }
        incre++;
        cout << endl;
    }
    for(int a = 0; a < nfiles; a++){
        /*Copy the reference array*/
        cout << "reference: " << endl;
        for(int b = 0; b < fatSize; b++){
            temp[b] = shared[a][b];
            cout << " " << temp[b];
            
        }
        cout << endl;
        int d = a+1;
        while(d < nfiles){
        int c = 0;
            cout << "d = " << d << endl;
        while(c < fatSize){
            if(temp[c] < shared[d][c]){
                    cout << "Smaller!" << endl;
                    break;
            }
            c++;
        }
            d++;
        }
    }
    return 0;
}
