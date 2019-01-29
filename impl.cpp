// CPSC457 Fall 2017, University of Calgary
// Skeleton C++11 program for Q7 of Assignment 5.
//
// The program reads in the input, then calls the (wrongly implemented) checkConsistency()
// function, and finally formats the output.
//
// You only need to reimplement the checkConsistency() function.
//
// Author: Pavol Federl (pfederl@ucalgary.ca or federl@gmail.com)
// Date: November 29, 2017
// Version: 1
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
typedef std::string SS;
typedef std::vector<SS> VS;
struct DEntry {
    SS fname = SS( 4096, 0);
    int size = 0;
    int ind = 0;
    bool tooManyBlocks = false;
    bool tooFewBlocks = false;
    bool hasCycle = false;
    bool sharesBlocks = false;
};
static SS join( const VS & toks, const SS & sep) {
    SS res;
    bool first = true;
    for( auto & t : toks) { res += (first ? "" : sep) + t; first = false;}
    return res;
}
// re-implement this function
//
// Parameters:
// blockSize - contains block size as read in from input
// files - array containing the entries as read in from input
// fat - array representing the FAT as read in from input
// Return value:
// the function should return the number of free blocks
// also, for ever entry in the files[] array, you need to set the appropriate flags:
// i.e. tooManyBlocks, tooFewBlocks, hasCycle and sharesBlocks
int checkConsistency( int blockSize, std::vector<DEntry> & files, std::vector<int> & fat, int nfiles, int fatSize)
{
    // make the first entry contain no errors
    /*
    if( files.size() > 0) {
        files[0].hasCycle = false;
        files[0].tooFewBlocks = false;
        files[0].tooManyBlocks = false;
        files[0].sharesBlocks = false;
    }
    // make the 2nd entry contain one error
    if( files.size() > 1) {
        files[1].hasCycle = true;
        files[1].tooFewBlocks = false;
        files[1].tooManyBlocks = false;
        files[1].sharesBlocks = false;
    }
    // make the 3rd entry contain two errors
    if( files.size() > 2) {
        files[2].hasCycle = false;
        files[2].tooFewBlocks = false;
        files[2].tooManyBlocks = true;
        files[2].sharesBlocks = true;
    }
    // finally, return the number of free blocks
     */
    int blocks = 0;
    int free_blk = 0;
    int star_ptr;
    int ini_ptr;
    int pre_ptr;
    int next_ptr;
    int sum_blocks = 0;
    int* used = (int*)malloc(sizeof(int) * fatSize);
    int* looped = (int*)malloc(sizeof(int) * fatSize);
    int* temp = (int*)malloc(sizeof(int)*fatSize);
    int shared[nfiles][fatSize];
    
    for(int w = 0; w < nfiles; w++){
        for(int v = 0; v < fatSize; v++){
            shared[w][v] = 0;
            //cout << " " << shared[w][v];
        }
        //cout << endl;
    }
    
    for(int g = 0; g < fatSize; g++){
        looped[g] = 0;
        used[g] = 0;
    }
    /*
    for(int r = 0; r < fatSize; r++){
        
        cout << " " << fat[r];
    }
    cout << " " << endl;
     */
    for(int b = 0; b < nfiles; b++){
        //cout << "name: " << files[b].fname << endl;
        for(int a = 0; a < fatSize; a++){
            looped[a] = 0;
        }
        
        blocks = 0;
        while(1){
            if(files[b].ind == -1){
                if(files[b].size > 0)
                    files[b].tooFewBlocks = true;
                break;
            }
            if(files[b].size == 0){
                files[b].tooManyBlocks = true;
            }
            //cout << "Starting at: " << files[b].ind << endl;
            if(fat[files[b].ind] == files[b].ind){
                files[b].hasCycle = true;
                used[files[b].ind]++;
                //
                shared[b][files[b].ind]++;
                break;
            }
            blocks++;
            /*Fisrt pointer*/
            ini_ptr = files[b].ind;
            used[ini_ptr]++;
            //
            shared[b][ini_ptr]++;
            looped[ini_ptr]++;
           // cout << "Looped for the 1st: " << looped[ini_ptr] << endl;
            /*Set the next pointer with the value of the first pointer*/
            next_ptr = fat[ini_ptr];
            //looped[next_ptr] = looped[next_ptr] + 1;
            //used[next_ptr] = used[next_ptr] + 1;
            //cout << "temp pointer: " << next_ptr << endl;
            /*Check if it reaches the end*/
            while(next_ptr != -1){
                looped[next_ptr]++;
                used[next_ptr]++;
                //
                shared[b][next_ptr]++;
                //cout << "next pointer: " << next_ptr << endl;
                //cout << "Looped: " << looped[next_ptr] << endl;
                if(looped[next_ptr] > 1  ){
                    //cout << "pre_ptr: " << looped[pre_ptr] << endl;
                    //cout << "next_ptr: " << looped[next_ptr] << endl;
                    //used[star_ptr] = -1;
                    files[b].hasCycle = true;
                    break;
                }
                blocks++;
                
                pre_ptr = next_ptr;
                next_ptr = fat[next_ptr];
               // cout << "next pointer: " << next_ptr << endl;
                
            }
            sum_blocks += blocks;
            //cout << "Reach the end!" << endl;
            //cout << "next_ptr: " << next_ptr << endl;
            //cout << "Contain blocks: " << blocks << endl;
            //cout << "Total size: " << blocks * blockSize << endl;
            //cout << "File size: " << files[b].size << endl;
            //cout << "Sum of blocks: " << sum_blocks << endl;
            if(((blockSize * blocks) - files[b].size) >= blockSize){
                files[b].tooManyBlocks = true;
                break;
            }
            if((blockSize * blocks) < files[b].size){
                files[b].tooFewBlocks = true;
                break;
            }
            
            //cout << "Too many blocks? " << files[b].tooManyBlocks << endl;
            if(next_ptr == -1){
                break;
            }
            if(files[b].hasCycle == true){
                break;
            }
            
        }
        //cout << "Has a Cycle? " << files[b].hasCycle << endl;
        //cout << "Too many blocks? " << files[b].tooManyBlocks << endl;
        //cout << "Too few blocks? " << files[b].tooFewBlocks << endl;
    }
    //cout << "used table " << endl;
    /*
    for(int a = 0; a < fatSize; a++){
        if(used[a] > 1){
            files[a].sharesBlocks = true;
        }
        cout << " " << used[a];
    }
    cout << endl;
     
    
        cout << "looped table " << endl;
        for(int c = 0; c < fatSize; c++){
            cout << " " << looped[c];
        }
        cout << endl;
    */
    
    /*Detecting if blocks are shared*/
    /*
    for(int w = 0; w < nfiles; w++){
        for(int v = 0; v < fatSize; v++){
            //shared[w][v] = 0;
            cout << " " << shared[w][v];
        }
        cout << endl;
    }
    */
    for(int a = 0; a < nfiles; a++){
        /*Copy the reference array*/
        //cout << "reference: " << endl;
        for(int b = 0; b < fatSize; b++){
            temp[b] = shared[a][b];
            //cout << " " << temp[b];
            
        }
        //cout << endl;
        int d = a+1;
        while(d < nfiles){
            int c = 0;
            //cout << "d = " << d << endl;
            while(c < fatSize){
                if(temp[c]!= 0 && shared[d][c]!= 0){
                    //cout << "Shared!" << endl;
                    files[a].sharesBlocks = true;
                    files[d].sharesBlocks = true;
                    break;
                }
                c++;
            }
            d++;
        }
    }
    
    
    
    
    
    /*Calculating the free blocks*/
    for(int h = 0; h < fatSize; h++){
        //cout << " " << used[h];
        //cout << " " << free_blk;
        if(used[h] == 0)
            free_blk++;
    }
    return free_blk;
}
int main()
{
    try {
        // read in blockSize, nFiles, fatSize
        int blockSize, nFiles, fatSize;
        if( 3 != scanf( "%d %d %d", & blockSize, & nFiles, & fatSize))
            throw "cannot read blockSize, nFiles and fatSize";
        if( blockSize < 1 || blockSize > 1024) throw "bad block size";
        if( nFiles < 0 || nFiles > 50) throw "bad number of files";
        if( fatSize < 1 || fatSize > 200000) throw "bad FAT size";
        // read in the entries
        std::vector<DEntry> entries;
        for( int i = 0 ; i < nFiles ; i ++ ) {
            DEntry e;
            if( 3 != scanf( "%s %d %d", (char *) e.fname.c_str(), & e.ind, & e.size))
                throw "bad file entry";
            e.fname = e.fname.c_str();
            if( e.fname.size() < 1 || e.fname.size() > 16)
                throw "bad filename in file entry";
            if( e.ind < -1 || e.ind >= fatSize) throw "bad first block in fille entry";
            if( e.size < 0 || e.size > 1073741824) throw "bad file size in file entry";
            entries.push_back( e);
        }
        // read in the FAT
        std::vector<int> fat( fatSize);
        for( int i = 0 ; i < fatSize ; i ++ ) {
            if( 1 != scanf( "%d", & fat[i])) throw "could not read FAT entry";
            if( fat[i] < -1 || fat[i] >= fatSize) throw "bad FAT entry";
        }
        
        // run the consistency check
        int nFreeBlocks = checkConsistency( blockSize, entries, fat, nFiles,fatSize);
        //
        /*
        printf( "Block size: %d\n", blockSize);
        printf( "Number of Files: %d\n", nFiles);
        printf( "FAT size: %d\n", fatSize);
        */
        
        // format the output
        size_t maxflen = 0;
        for( auto & e : entries ) maxflen = std::max( maxflen, e.fname.size());
        SS fmt = " %" + std::to_string( maxflen) + "s: %s\n";
        printf( "Issues with files:\n");
        for( auto & e : entries ) {
            VS issues;
            if( e.tooFewBlocks) issues.push_back( "not enough blocks");
            if( e.tooManyBlocks) issues.push_back( "too many blocks");
            if( e.hasCycle) issues.push_back( "contains cycle");
            if( e.sharesBlocks) issues.push_back( "shares blocks");
            printf( fmt.c_str(), e.fname.c_str(), join( issues, ", ").c_str());
        }
        printf( "Number of free blocks: %d\n", nFreeBlocks);
    }
    catch( const char * err) {
        fprintf( stderr, "Error: %s\n", err);
    }
    catch( ... ) {
        fprintf( stderr, "Errro: unknown.\n");
    }
    return 0;
}
