/*
 * banker.cpp
 *
 * Student Name:
 * Student Number:
 *
 * Class: CPSC 457 Spring 2017
 * Instructor: Pavol Federl
 *
 * Copyright 2017 University of Calgary. All rights reserved.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

class Banker
{
private:
    int numProc;      // the number of processes
    int numResources; // the number of resources
    int * available;  // number of available instances of each resource
    int ** max;       // the max demand of each process, e.g., max[i][j] = k
                      // means process i needs at most k instances of resource j
    int ** allocation;// the number of resource instances already allocated
    int ** need;      // the number of resource isntances needed by each process

public:

    /* Initializing the vectors and matrixes for the Banker's Algorithm. Takes ownership of
     * all arrays.
     * @param avail  The available vector
     * @param m      The max demand matrix
     * @param alloc  The allocation matrix
     * @param p      The number of processes
     * @param r      The number of resources
     */
    Banker (int * avail, int ** m, int ** alloc, int p, int r) {
        numProc = p;
        numResources = r;

        // Setup the available vector, the max matrix, and the allocation matrix
        available = avail;
        max = m;
        allocation = alloc;

        // Initialize the need matrix
        need = new int*[numProc];
        for (int i = 0; i < numProc; i++)
            need[i] = new int[numResources];
    }

    /* Destroy the vectors and matrixes
     */
    ~Banker() {
        numProc = 0;
        numResources = 0;

        // Free all allocated memory space
        delete[] available;
        for (int i = 0; i < numProc; i++)
        {
            delete[] need[i];
            delete[] max[i];
            delete[] allocation[i];
        }
        delete[] need;
        delete[] max;
        delete[] allocation;
    }

    
    /* Check whether it is safe to grant the request
     * @param pid    The process that is making the request
     * @param req    The request
     * @param sequenceOrReason  The safe execution sequence returned by the algorithm
     * @return Whether granting the request will lead to a safe state.
     */
    bool isSafe (int pid, int * req, string & sequenceOrReason) {
        /* If there is a updates of *available, should start from the P0, if there is not, run in order*/
        int updates = 1;
        bool result = true;
        int uv = 0;
        sequenceOrReason = "";
        
        //sequenceOrReason = sequenceOrReason + 'P' + to_string(updates);
        cout << "Need matrix" << endl;
        for (int i = 0; i < numProc; i++)
        {
            cout << "<";
            for(int k = 0; k < numResources; k++){
                need[i][k] = max[i][k] - allocation[i][k];
                cout << " " << need[i][k];
            }
            cout << " >";
            cout << "" << endl;
        }
        cout << endl;
        
        cout << "Need matrix after request" << endl;
        for(int k = 0; k < numResources; k++){
            need[pid][k] -= req[k];
        }
        for (int i = 0; i < numProc; i++)
        {
            cout << "<";
            for(int k = 0; k < numResources; k++){
                cout << " " << need[i][k];
            }
            cout << " >";
            cout << "" << endl;
        }
        /*If the allocation after the request > max*/
        for(int o = 0; o < numResources; o++){
            if(allocation[pid][o] > max[pid][o]){
                result = false;
                sequenceOrReason = "The request is invalid!";
                return result;
            }
        }
        /*  If there is not enough resources available*/
        for(int q = 0; q < numResources; q++){
            if(available[q] < 0){
                result = false;
                sequenceOrReason = "There is not enough resources available!";
                return result;
            }
        }
        /* Compare the need with the available*/
        int m = 0;
        int n = 0;
        /* For the list, 1 indicates is finished, 0 indicates is waiting */
        int *list = (int *) malloc(sizeof(int) * numProc);
        /*  Initializing    */
        for(int g = 0; g < numProc; g++){
            list[g] = 0;
        }
        /*Need to find a way to determine whether a process has been finished*/
        while(n < numProc){
            if(available[m] < need[n][m]){
                m = 0;
                updates = 0;
                n++;
                //cout << "n = " << n << endl;
            }
            if(available[m] >= need[n][m]){
                /*Code for release from holding to available*/
                for(m = 1; m < numResources; m++){
                    if(available[m] < need[n][m]){
                        uv++;
                    }
                }
                if(list[n] == 0 && uv == 0){
                    sequenceOrReason = sequenceOrReason + " P" + to_string(n) + ",";
                for(int a = 0; a < numResources; a++){
                    available[a] += allocation[n][a];
                }
                list[n] = 1;
                m = 0;
                n = 0;
                updates = 1;
                    uv = 0;
                }
                else{
                    n++;
                    m = 0;
                    uv = 0;
                }
            }
        }
        sequenceOrReason.pop_back();
        int z = 0;
        /*Unsafe state*/
        while(z < numProc){
            if(list[z] == 0){
                result = false;
                sequenceOrReason = "The request would result in an unsafe state!";
                break;
            }
            z++;
        }
        return result;
    }
};

int main (int argc, char * const argv[])
{
    ifstream config;       // Configuration file
    string conffile;       // The configuration file name
    int numProc;           // The number of processes
    int numResources;      // The number of resources
    string sequenceOrReason;       // The execution sequence returned by the Banker's Algorithm
    int i, j, index;       // Indices for the vectors and matrixes
    int pid;               // The ID of the process that is making the request
    string reqStr;         // The request vector in string format

    // Read in the config file name from the commanda-line arguments
    if (argc < 2)
    {
        cout << "Usage: banker <config file>\n";
        return 0;
    }
    else
    {
        conffile = argv[1];
    }

    // Open the file
    config.open(conffile.c_str());

    // Get the number of process and the number of resources
    string line, var, equal;    // strings for parsing a line in the config file
    getline(config, line);
    istringstream iss(line);
    iss >> var >> equal >> numProc;     // Get the number of processes
    iss.clear();

    getline(config, line);
    iss.str(line);
    iss >> var >> equal >> numResources;    // Get the number of resources
    iss.clear();

    // Create the available vector, the max matrix, and the allocation matrix
    // according to the number of processes and the number of resources
    int * available = new int[numResources];
    int ** max = new int*[numProc];
    int ** allocation = new int*[numProc];
    for (int i = 0; i < numProc; i++)
    {
        max[i] = new int[numResources];
        allocation[i] = new int[numResources];
    }

    // Get the available vector
    getline(config, line);
    replace(line.begin(), line.end(), '<', ' ');  // Remove "<" and ">"
    replace(line.begin(), line.end(), '>', ' ');
    iss.str(line);
    iss >> var >> equal;
    for (j = 0; j < numResources; j++)        // Read in the "available" vector
        iss >> available[j];
    iss.clear();

    // Get the max matrix and the allocation matrix
    for (i = 0; i < numProc; i++)
    {
        getline(config, line);
        replace(line.begin(), line.end(), '<', ' ');
        replace(line.begin(), line.end(), '>', ' ');
        iss.str(line);
        iss >> var;
        index = atoi(&var.at(1));            // Get the process ID
        if (index < 0 || index >= numProc)
        {
            cerr << "Invalid process ID: " << var << endl;
            return 0;
        }

        // Get the number of resources allocated to process "index".
        for (j = 0; j < numResources; j++)
            iss >> allocation[index][j];

        // Get the max allocation to process "index".
        for (j = 0; j < numResources; j++)
            iss >> max[index][j];

        iss.clear();
    }

    // Get the request vector
    int * request = new int[numResources];
    getline(config, line);
    reqStr = line.substr(line.find('<'), line.find('>') - line.find('<') + 1);
    replace(line.begin(), line.end(), '<', ' ');
    replace(line.begin(), line.end(), '>', ' ');
    iss.str(line);
    iss >> var >> pid >> equal;
    for (j = 0; j < numResources; j++)          // Read in the "request" vector
        iss >> request[j];
    iss.clear();
    
    /*Debugging*/
    cout << "max matrix" << endl;
    for (int i = 0; i < numProc; i++)
    {
        cout << "<";
        for(int k = 0; k < numResources; k++){
            cout << " " << max[i][k];
        }
        cout << " >";
        cout << "" << endl;
    }
    cout << endl;
    cout << endl;
    cout << "request: " << endl;
    cout << "<";
    for (int i = 0; i < numResources; i++)
    {
        cout << " " << request[i];
    }
    cout << " >";
    cout << "" << endl;
    
    cout << "Available matrix: " << endl;
    cout << "<";
    for(int f = 0; f < numResources; f++){
        cout << " " <<available[f];
    }
    cout << " >";
    cout << endl;
    cout << endl;
    
    cout << "Available matrix after request: " << endl;
    cout << "<";
    for(int f = 0; f < numResources; f++){
        available[f] = available[f] - request[f];
        cout << " " <<available[f];
    }
    cout << " >";
    cout << endl;
    cout << endl;
    
    cout << "allocation: " << endl;
    for (int i = 0; i < numProc; i++)
    {
        cout << "<";
        for(int k = 0; k < numResources; k++){
            cout << " " << allocation[i][k];
        }
        cout << " >";
        cout << "" << endl;
    }
    cout << endl;
    
    cout << "allocation after request: " << endl;
    for(int k = 0; k < numResources; k++){
        allocation[pid][k] = allocation[pid][k] + request[k];
    }
    for (int i = 0; i < numProc; i++)
    {
        cout << "<";
        for(int k = 0; k < numResources; k++){
            cout << " " << allocation[i][k];
        }
        cout << " >";
        cout << "" << endl;
    }
    cout << endl;
    
    

    cout << endl;

    // Check the request using the Banker's algorithm.
    Banker * banker = new Banker(available, max, allocation, numProc, numResources);
    if (banker -> isSafe(pid, request, sequenceOrReason))
        cout << "Grant request " << reqStr << " from P" << pid << ".\n"
             << "Sequence: " << sequenceOrReason << ".\n";
    else
        cout << "Reject request " << reqStr << " from P" << pid << ".\n"
             << "Reason: " << sequenceOrReason << "\n";
}
