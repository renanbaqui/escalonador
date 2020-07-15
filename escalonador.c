#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX 50 
#define MIN 10

int main(){
		
	char str[MAX];    	
	int n, i, j, k, l;
	int pro[MIN]; 				// soma do tempo de execucao total de cada processo
	int est[MIN][MAX]; 			// estado do [processo] e [posicao]  0 - execucao, 1 - i/o, 2 - pronto, 3 - terminado
	long soma = 0, a = 0, b = 0, c, d, e;	// soma dos tempos sem escalonamento
	long cpu[MIN];  			// tempo de execucao de cada [processo]
	long p[MIN];				// posicao do [processo] em est[MIN][MAX] = est[MIN][p[MIN]]
	long total = 0;				// soma total de todos os tempos de execucao dos processos com escalonamento
	long m[MIN][MIN]; 			// [processo][cpu ou i/o] par = cpu, impar = i/o
	printf("numero de processos:\n");
	scanf("%d", &n);	
	printf("insira os processos:\n");
	
	for (i=0; i<n; i++){
		cpu[i] = 0;
		p[i] = 2; 			// comeca no '2' pois todos os primeiros 'cpu' e 'i/o bursts' de cada processo 
	}					// ja sao preenchidos 
	
	for (i=0; i<n+1; i++){
	
		fgets(str, MAX, stdin); 

		char *p = str;
	    	j = 0;
	    
		while (*p){ 									// enquanto ha mais caracteres para processar
	    		if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )){ 	// encontrou um numero
	        		long val = strtol(p, &p, 10); 					// leu numero
	        		m[i-1][j] = val;
				if (j == 0 || j%2 == 0){			
					cpu[i-1] = cpu[i-1] + m[i-1][j];
					total = total + m[i-1][j];
				}
				pro[i-1] = pro[i-1] + val;
				soma = soma + val;				
				j++;				
	    		} 
			else { 									// do contrario, mover para o proximo caractere
	        		p++;
	    		}
		}				
	}

	printf("soma dos tempos (sem escalonamento): %ld\n", soma);
	printf("soma de todos os tempos de execucao: %ld\n", total);
	
	for (i=0; i<n; i++){ 		// preenche todos os estados com valor '2' - pronto
		for (j=0; j<MAX; j++){
			est[i][j] = 2;			
		}		
	}	
		
	for (i=0; i<n; i++){		// insere todos os primeiros cpu e i/o bursts de cada processo
		
		b = a + m[i][0]; 	
		
		for (j=a; j<b; j++){ 
			est[i][j] = 0;			// insere os cpu bursts	
		}
		
		cpu[i] = cpu[i] - m[i][0];		// diminui de cpu[i] o cpu burst
		total = total - m[i][0];		// diminui de total o cpu burst do processo
		
		a = b; 				
		b = b + m[i][1];	
		
		for (j=a; j<b; j++){
			est[i][j] = 1;			// insere os i/o bursts
		}	
	}
	
	for (i=a; i<MAX; i++){					
		for (j=0; j<n; j++){					
			if(est[j][i] == 2){					// procura o proximo estado 'pronto'
				b = i + m[j][p[j]];				// define a posicao final 'b'
				printf("b: %d\n", b);				
				cpu[j] = cpu[j] - m[j][p[j]]; 			// retira de cpu[] a quantidade de cpu bursts a ser inserida
				printf("cpu[%d]: %d\n", j, cpu[j]); 
				total = total - m[j][p[j]];			// retira do total a quantidade de cpu bursts a ser inserida
				printf("total: %d\n", total);
				
				for (k=i; k<b; k++){
					est[j][k] = 0;					
				}
				
				d = k;
				i = k ;					// 'i' recebe a ultima posicao inserida
				
				printf("i: %d\n", i);
				p[j] = p[j] + 1; 			// adiciona posicao
								
				if (cpu[j] == 0){			// se o processo ja foi todo executado, insere 'finalizado' 
					for (k=b; k<MAX; k++){		
						est[j][k] = 3;
					}
				}								
				
				
				if (cpu[j] != 0){			// se o processo não for finalizado, insere 'i/o'
					
					b = d + m[j][p[j]]; 		
					
					for (k=d; k<b; k++){		// insere 'i/o'
						est[j][k] = 1;		
					}	
					p[j] = p[j] + 1;		// adiciona posicao
				}
			}				
		}
	}	
	
	for (i=0; i<n; i++){						// imprime a matriz de estados
		for (j=0; j<d; j++){
			printf("%d ", est[i][j]);
			if (j == d-1){
				printf("\n");
			}		
		}
	}	
	
	printf("saida: %d unidades de tempo\n", b);
	
    return 0;
}
