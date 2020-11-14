//=============================================================================
// escalonador_en SemVer v0.1.5
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX 50 
#define MIN 10

int main(){
		
	char str[MAX];    	
	int n, i, j, k, l;			// n = number of processes
	int pro[MIN]; 				// total sum of execution time of each [process]
	int est[MIN][MAX]; 			// [process] state and [position], processes:  0 - running, 1 - i/o, 2 - ready, 3 - finished
	long soma = 0, a = 0, b = 0, c, d, e;	// soma = sum of all times without scheduling
	long cpu[MIN];  			// execution (running) time of each [process]
	long p[MIN];				// [process] position at est[MIN][MAX] = est[MIN][p[MIN]]
	long total = 0;				// total sum of all execution (running) times with scheduling
	long m[MIN][MIN]; 			// m[process][cpu or i/o]; even = cpu, odd = i/o
	
	printf("enter the number of processes (maximum of 10):\n");
	scanf("%d", &n);	
	printf("enter the processes:\ne.g. 10 cpu, 1 io, 3 cpu\n");
	
	for (i=0; i<n; i++){			 
		cpu[i] = 0;
		p[i] = 2; 			// starts at '2' because all the first 'cpu' and 'i/o burst' of each process have already been filled in the matrix 
	}					 
	
	for (i=0; i<n+1; i++){
	
		fgets(str, MAX, stdin); 

		char *p = str;
	    	j = 0;
	    
		while (*p){ 									// while there are more characters to process
	    		if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )){ 	// 'if' finds a number
	        		long val = strtol(p, &p, 10); 					// reads the number
	        		m[i-1][j] = val;
				if (j == 0 || j%2 == 0){			
					cpu[i-1] = cpu[i-1] + m[i-1][j];
					total = total + m[i-1][j];
				}
				pro[i-1] = pro[i-1] + val;
				soma = soma + val;				
				j++;				
	    		} 
			else { 									// else, moves to next character 
	        		p++;
	    		}
		}				
	}

	printf("sum of times (no scheduling): %ld\n", soma);
	printf("sum of running times: %ld\n", total);
	
	for (i=0; i<n; i++){ 		// fills all states with value '2' - ready
		for (j=0; j<MAX; j++){
			est[i][j] = 2;			
		}		
	}	
		
	for (i=0; i<n; i++){		// inserts all the first 'cpu bursts' and 'i/o bursts' of each process
		
		b = a + m[i][0]; 	
		
		for (j=a; j<b; j++){ 
			est[i][j] = 0;			// inserts 'cpu burst' 	
		}
		
		cpu[i] = cpu[i] - m[i][0];		// subtracts 'cpu burst' from cpu[i] 
		total = total - m[i][0];		// subtracts from 'total' the process' cpu burst
		
		a = b; 				
		b = b + m[i][1];	
		
		for (j=a; j<b; j++){
			est[i][j] = 1;			// inserts 'i/o bursts'
		}	
	}
	
	for (i=a; i<MAX; i++){					
		for (j=0; j<n; j++){					
			if(est[j][i] == 2){					// looks for the next 'ready' state
				b = i + m[j][p[j]];				// defines the final 'b' position
//				printf("b: %ld\n", b);				
				cpu[j] = cpu[j] - m[j][p[j]]; 			// subtracts from cpu[] how many 'cpu bursts' are to be inserted
//				printf("cpu[%d]: %ld\n", j, cpu[j]); 
				total = total - m[j][p[j]];			// subtracts from total how many 'cpu bursts' are to be inserted
				printf("total: %ld\n", total);
				
				for (k=i; k<b; k++){
					est[j][k] = 0;					
				}
				
				d = k;
				if (n == 2){					// review this!
					i = k - 1;				// review this!					
				}
				else {
					i = k;					// 'i' receives the last inserted position (it works for 3+ processes)
				}
				
//				printf("i: %d\n", i);
				p[j] = p[j] + 1; 				// adds position
								
				if (cpu[j] == 0){				// if the process has already been executed, insert the status 'finished'
					for (k=b; k<MAX; k++){		
						est[j][k] = 3;
					}
				}								
				
				
				if (cpu[j] != 0){				// if the process hasn't been finished, inserts 'i/o'
					
					b = d + m[j][p[j]]; 		
					
					for (k=d; k<b; k++){			// inserts 'i/o'
						est[j][k] = 1;		
					}	
					p[j] = p[j] + 1;			// adds position
				}
			}				
		}
	}	
	
	printf("state matrix\n\n");						// prints matrix
	for (i=0; i<n; i++){								
		for (j=0; j<d; j++){
			printf("%d ", est[i][j]);
			if (j == d-1){
				printf("\n");
			}		
		}
	}	
	printf("\n0 - running\n1 - i/o\n2 - ready\n3 - finished\n");
	printf("output: %ld time units\n", b);
	
    return 0;
}
