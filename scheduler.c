#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *arrivals;
int *burst;
char *Pline;
int ls;
int t;
int P;
int *rq;
int *wait;

void print_readyQ(){
  int i = 0;
  printf("Ready Queue:\n");
  while(rq[i] != -1)
    printf("Process = %d Burst = %d\n", rq[i], burst[rq[i++]]);
  printf("END\n");
}

void waitprint(){
  float a = 0;
  for (int p = 0; p < P; p++){
    printf("P%d waited %d.000000 sec.\n", p, wait[p]);
    a += (float) wait[p];
}
  a = a / (float) P;
  printf("Average wait time = %f seconds.\n", a);
}


int search_readyQ(int pid){
  for (int i = 0; i < P; i++){ 
    if (rq[i] == pid)
      return 1;
  }
  return 0;
}

void push_readyQ(int pid){
  if (rq[0] == -1){
      rq[0] = pid;
      return;
    }
  int i = -1;
  while (rq[++i] != -1);
  rq[i] = pid;
}


int pop_readyQ(){
  if (rq[0] == -1)
    return -1;
  int out = rq[0];
  for (int i = 1; i < P + 1; i++)
    rq[i - 1] = rq[i];
  return out;
}

void sort_readyQ(){
  /*sort the queue by the burst time. */
  if (rq[0] == -1 || rq[1] == -1)
    return;
  /*temp burst time queue*/
  int* tbt = malloc(P * sizeof(int));
  /*temp ready queue*/
  int* trq = malloc(P * sizeof(int));
  for (int h = 0; h < P; h++){
    tbt[h] = burst[h];
    trq[h] = rq[h];
  }
  int min;
  int a = 0;
  int B = 0;
  int brk = 1;
  while (brk){
    min = 9999;
    for (int m = 0; m < P; m++){
      if (trq[m] == -1)
	break;
      if (tbt[trq[m]] <= min){
	min = tbt[trq[m]];
	B = m;
      }
    }
    if (tbt[trq[B]] < min){
      rq[a++] = trq[B];
      tbt[trq[B]] = min;
    }
    else
      brk = 0;
  }
  free(tbt);
  free(trq);
}



void println(int active){ 
  for (int p = 0; p < P; p++)
    if (search_readyQ(p))
      wait[p]++;
  if (t < 10)
    Pline[1] = (char) t + 48;
  else{
    char *s = malloc(4);
    sprintf(s, "%d", t);
    Pline[1] = s[0];
    Pline[2] = s[1];
    if (t >= 100)
      Pline[3] = s[2];
    free(s);
  }
  if (active == -1){
    char gh[5];
    strncpy(gh, Pline, 4);
    printf("%s\n", gh);
  }
  else{
    int position;
    for (int p = 0; p < P; p++){
      if (p < 10)
	position = (p<<2) + 6;
      else position = 46 + (5 * (p - 11));
      if (p == active)
	Pline[position] = '.';
      else{
	//printf("p = %d srq = %d\n", p, search_readyQ(p));
	if (search_readyQ(p) == 0)
	  Pline[position] = ' ';
	else Pline[position] = '+';
      } 
    }

    printf("%s\n", Pline);
  }
}




int main(int args, char **argv){
  if (args == 1){ //static debug code here
    P = 60;
    rq = malloc((P + 1) * sizeof(int));
    for (int u = 0; u < P + 1; u++)
      rq[u] = -1;
    push_readyQ(8);
    push_readyQ(9);
    for (int gh = 0; gh < 92; gh++)
      printf("%d\n", pop_readyQ());
    return 58;
}
  if (args < 3 || args > 4){
    printf("INVALID INPUT\n");
    return -1;
  }
  char mode = 'E';
  if (strcmp(argv[2], "RR")==0 || strcmp(argv[2], "rr")==0)
    mode='R';
  if (strcmp(argv[2], "SJF")==0 || strcmp(argv[2], "sjf")==0)
    mode='S';
  if (mode=='E'){
    printf("INVALID MODE\n");
    return -1;
  }
  int Quantum;
  if (mode == 'R')
    Quantum = atoi(argv[3]);
  if (mode == 'S' && args == 4)
    printf("time slice not used, input ignored\n");

  /*load from file*/
  char *line = malloc(16);
  arrivals = malloc(30 * sizeof(int));
  burst = malloc(30 * sizeof(int));
  FILE *fp;
  fp = fopen(argv[1], "r");
  P = 0;
  while (fgets(line, 16, fp) != NULL){
    if (strcmp(line,"\n") == 0 || strcmp(line, "\r\n") == 0);
    else{
      arrivals[P] = atoi(strtok(line," "));
      burst[P++] = atoi(strtok(NULL," "));
    }
  }
  free(line);
  fclose(fp);
  /*init readyQ and waitQ*/
  rq = malloc((P + 1) * sizeof(int));
  for (int p = 0; p < P + 1; p++)
    rq[p] = -1;
  wait = malloc(P * sizeof (int));
  for(int i = 0; i < P; i++)
    wait[i] = 0;
// init
  if (0){ //Debug prints processes and burst times
    for (int p = 0; p < P; p++)
      printf("A=%d B=%d\n", arrivals[p], burst[p]);
  }
// header printing
  char *spacer = "  P";
  char *num = malloc(8);
  if (P < 10)
    ls = (4 + (P<<2));
  else 
    ls = (44 + (5 * (P - 10)));
  char *header = malloc(ls + 1);
  strcat(header,"TIME");
  line = malloc(ls + 1);
  Pline = malloc(ls + 1);
  for (int h = 0; h < ls; h++)
    Pline[h] = ' ';
  Pline[ls] = (int) 0;
  for (int b = 0; b < ls; b++)
    line[b] = '-';
  line[ls] = (char) 0;
  for (int i = 0; i < P; i++){
    strcat(header,spacer);
    sprintf(num, "%d", i);
    strcat(header, num);
  }
  printf("%s\n%s\n", header, line);


  t = 0;
  int ets;
  int active = -1;
  int reload;

   /*Round-Robin*/
  if (mode == 'R')
    while (1){
      //print_readyQ();
      if (active == -1)  
	while(t <= arrivals[P - 1]){
	  /*push the process which has arrived*/
	  for (int u = 0; u < P; u++){
	    if (arrivals[u] == t)
	      push_readyQ(u);
	  }
	  /*If the queue is empty, advanced the clock by 1*/
	  if (rq[0] == -1){
	    println(active);
	    t++;
	  }
	  else
	    break;
	}
      active = pop_readyQ();
      if (burst[active] <= Quantum){
	ets = t + burst[active];
	reload = 0;
      }
      /*Done*/
      else{
	ets = t + Quantum;
	reload = 1; //printf("Reload %d\n", burst[active] - Q);
      }
      /*Need a reload*/
      while (t < ets){
	println(active);
	t++;
	if (t <= arrivals[P - 1]) 
	  for (int u = 0; u < P; u++)
	    if (arrivals[u] == t)
	      push_readyQ(u);
      }
      if (reload){
	push_readyQ(active);
	burst[active] -= Quantum;
      }
      if (t > arrivals[P-1] && rq[0] == -1 && active == -1)
	break;  // stop running when the time exceeds the latest arrival, the queue is empty and nothing is running.
    }

  /*SJF*/
  if (mode == 'S')
    while(1){
      if (active == -1)  
	while(t <= arrivals[P - 1]){
	  // while the queue is empty and nothing is running, advance the clock until the last arrival or the queue is no longer empty;
	  for (int u = 0; u < P; u++)
	    if (arrivals[u] == t)
	      push_readyQ(u);
	  if (rq[0] == -1) {
	    println(active);
	    t++;
	  }
	  else
	    break;
	}
      sort_readyQ(); 
      active = pop_readyQ();
      ets = t + burst[active];
      while (t < ets){
	println(active);
	t++; 
	if (t <= arrivals[P - 1])
	  for (int u = 0; u < P; u++)
	    if (arrivals[u] == t)
	      push_readyQ(u);
      }
      /* Stop running when the time exceeds the latest arrival, the queue is empty and nothing is running.*/
      if (t > arrivals[P - 1] && rq[0] == -1 && active == -1)
	break; 


    }

  printf("%s\n", line);
  waitprint();
}
