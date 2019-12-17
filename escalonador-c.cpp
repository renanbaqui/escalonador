#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX 50 
#define MIN 10

int main(){

//  char *str = "10 cpu, 4 io, 5 cpu";
		
	char str[MAX];    	
	int n, i, j, k, l;
	int pro[MIN]; 			// soma do tempo de execucao total de cada processo
//	int pos[MIN]; 			// posicao do [processo]
	int est[MIN][MAX]; 		// estado do [processo] e [posicao]  0 - execucao, 1 - i/o, 2 - pronto, 3 - terminado
	long soma = 0, a = 0, b = 0, c, d, e;
	long cpu[MIN];  		// tempo de execucao de cada [processo]
	long p[MIN];			// posicao  do [processo] em est[MIN][MAX] = est[MIN][p[MIN]]
	long total = 0;			// soma total de todos os tempos de execucao dos processos
	long m[MIN][MIN]; 		// [processo][cpu ou i/o] par = cpu, impar = i/o
	printf("numero de processos:\n");
	scanf("%d", &n);	
	printf("insira os processos:\n");
	
	for (i=0; i<n; i++){
		cpu[i] = 0;
		p[i] = 2; 		// comeca no '2' pois todos os primeiros cpu e i/o bursts de cada processo ja sao preenchidos
	}
	
	for (i=0; i<n+1; i++){
	
		fgets(str, MAX, stdin); 
//	    printf("string is: %s\n", str);
		char *p = str;
	    j = 0;
	    
		while (*p) { // enquanto há mais caracteres para processar
	    	if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )) { // encontrou um numero
	        	long val = strtol(p, &p, 10); // leu numero
	        	m[i-1][j] = val;
				
				if (j == 0 || j%2 == 0){			
					cpu[i-1] = cpu[i-1] + m[i-1][j];
					total = total + m[i-1][j];
				}
				
				pro[i-1] = pro[i-1] + val;
				soma = soma + val;				
				j++;
//				printf("%ld\n", val); // imprimiu
	    	} 
			else { // do contrario, mover para o proximo caractere
	        p++;
	    	}
		}				
	}

	printf("soma dos tempos (sem escalonamento): %ld\n", soma);
	printf("soma de todos os tempos de execucao: %ld\n", total);
	
	
/*	
	for (i=0; i<n; i++){ 	// soma o tempo de execucao de cada processo
		for (j=0; j<MAX; j++){
			if (j == 0 || j%2 == 0){			
				cpu[i] = cpu[i] + m[i][j];
			}
		}		
	}
*/	
	
	
	printf("1:\ncpu[0]: %d\ncpu[1]: %d\ncpu[2]: %d\ntotal: %d\n", cpu[0], cpu[1], cpu[2], total);	

	for (i=0; i<n; i++){ 	// preenche todos os estados com valor '2' - pronto
		for (j=0; j<MAX; j++){
			est[i][j] = 2;			
		}		
	}	
	
	
	for (i=0; i<n; i++){	// insere todos os primeiros cpu e i/o bursts de cada processo
		
		b = a + m[i][0]; 	// b = 10
		
		for (j=a; j<b; j++){ 
			est[i][j] = 0;			// insere os cpu bursts	
		}
		
		cpu[i] = cpu[i] - m[i][0];	// diminui de cpu[i] o cpu burst
		total = total - m[i][0];	// diminui de total o cpu burst do processo
		
		a = b; 				// a = 10
		b = b + m[i][1];	// b = 10 + 4 = 14
		
		for (j=a; j<b; j++){
			est[i][j] = 1;			// insere os i/o bursts
		}	
	}
	
	printf("2:\ncpu[0]: %d\ncpu[1]: %d\ncpu[2]: %d\ntotal: %d\n", cpu[0], cpu[1], cpu[2], total);	
	
	/*	
	
	b = a + m[1][0];	// b = 10 + 7 = 17
	
	for (i=a; i<b; i++){
		est[1][i] = 0;
		est[2][i] = 2;
	}
	
	a = b;				// a = 17
	b = b + m[1][1];	// b = 17 + 10 = 27
	for (i=a; i<b; i++){
		est[1][i] = 1;		
	}
	
	
	b = a + m[2][0];	// b = 17 + 2 = 19
	for (i=a; i<b; i++){
		est[2][i] = 0;		
	}
	
	a = b;				// a = 19
	b = b + m[2][1];	// b = 19 + 9 = 28
	for (i=a; i<b; i++){
		est[2][i] = 1;		
	}	*/
	
	
	// a = 19
	// b = 19 + 9 = 28	
	
	// procura pelo estado de pronto mais proximo e insere o cpu burst
	
	b = MAX;
	
	int flag = 1;	
	
/*	
	if (cpu[i] == 0){			// se o processo ja foi todo executado, insere finalizado no restante dos estados
		for (j=a; j<MAX; j++){
			m[i][j] = 3;
		}
	}
*/	
	
	// a = 19
	// b = 19 + 9 = 28

//////////////
	
//while (total != 0){
	
	for (i=a; i<MAX; i++){					
		for (j=0; j<n; j++){					
			if(est[j][i] == 2){										// procura o proximo estado 'pronto'
				b = i + m[j][p[j]];									// define a posicao final b
				printf("b: %d\n", b);				// b = 24
				cpu[j] = cpu[j] - m[j][p[j]]; 						// retira de cpu[] a quantidade de cpu bursts a ser inserida
				printf("cpu[%d]: %d\n", j, cpu[j]); // cpu[0] = 0
				total = total - m[j][p[j]];							// retira do total a quantidade de cpu bursts a ser inserida
				printf("total: %d\n", total);
				for (k=i; k<b; k++){
					est[j][k] = 0;					
				}
				
				d = k;
				i = k ;							// 'i' recebe a ultima posicao inserida
				//a = b+3;
				printf("i: %d\n", i);
				p[j] = p[j] + 1; 				// adiciona posicao
				
				
				if (cpu[j] == 0){				// se o processo ja foi todo executado, insere 'finalizado' no restante dos estados
					for (k=b; k<MAX; k++){
						est[j][k] = 3;
					}
				}								
				
				
				if (cpu[j] != 0){				// 	se o processo não for finalizado, insere i/o
					// a = d; 					//	a = d = 30											//a + m[1][2];
					b = d + m[j][p[j]]; 		//	b = 30 + 4 = 34										//a + m[1][3];
					
					for (k=d; k<b; k++){		//insere i/o
						est[j][k] = 1;		
					}	
					p[j] = p[j] + 1;			// adiciona posicao
				}
			}				
		}
	}

	
/*
	
	a = a + m[0][2]; 	//a = 19 + 5 = 24
	b = MAX;
	
	flag = 1;
	
	while(flag){
	
		for (i=a; i<b; i++){
			for (j=1; j<3; j++){		
				if(est[j][i] == 2){
					c = i + m[j][2];					
					for (k=i; k<c; k++){
						est[j][k] = 0;						
					}
					d = k; 				// d recebe a ultima posicao preenchida
					flag = 0;
					goto end;				
				}				
			}			
		}
	} 
	
	end:
	a = d; 				//a = d = 30											//a + m[1][2];
	b = a + m[1][3]; 	//b = 30 + 4 = 34										//a + m[1][3];
	
	for (i=a; i<b; i++){
		est[1][i] = 1;		
	}	
	
	b = MAX;
	
	flag = 1;
	
	while(flag){
	
		for (i=a; i<b; i++){
			for (j=2; j<3; j++){		
				if(est[j][i] == 2){
					c = i + m[j][2];					
					for (k=i; k<c; k++){
						est[j][k] = 0;						
					}
					d = k;
					flag = 0;
					goto end2;				
				}				
			}			
		}
	} 
	
	end2:
	a = d; 				//a = d = 33 	
	b = d + m[2][3]; 	//b = 33 + 2 = 35		
	
	for (i=a; i<b; i++){
		est[2][i] = 1;		
	}
	
	flag = 1;
	
	b = MAX;
	
	while(flag){
	
		for (i=a; i<b; i++){
			for (j=1; j<3; j++){		
				if(est[j][i] == 2){
					c = i + m[j][4];					
					for (k=i; k<c; k++){
						est[j][k] = 0;						
					}
					d = k;
					flag = 0;
					goto end3;				
				}				
			}			
		}
	}
	
	end3:
	
	flag = 1;
	a = d; 				// a = d = 37;
	b = MAX;
	
	while(flag){
	
		for (i=a; i<b; i++){
			for (j=2; j<3; j++){		
				if(est[j][i] == 2){
					c = i + m[j][4];					
					for (k=i; k<c; k++){
						est[j][k] = 0;						
					}
					flag = 0;
					goto end4;				
				}				
			}			
		}
	} 
	
	end4:
*/		
/*	
	if (m[0][1]<m[1][0]){
		b = a + m[0][1];
	}
	else {
		b = a + m[1][0];
	}
	
	for (i=a; i<b; i++){
		est[0][i] = 1;
		est[1][i] = 0;
		est[2][i] = 2;
	}
	
	if ()
/*	
	for (i=total; i<total+m[0][1]; i++){
		est[0][i] = 1;
	}
	for (i=total+m[0][1]; i<total2; i++){
		est[0][i] = 2;
	}
	for (i=total; i<total2; i++){
		est[1][i] = 0;
		est[2][i] = 2;
	}
	total = total2;
	
	if(m[0][1] <= m[1][0]){
		total2 = total2 + m[0][2];
		for	(i=total; i<total2; i++){
			est[0][i] = 0;
			est[1][i] = 1;
			est[2][i] = 2;
		}
	}	*/
	
	// b = 38;
	
	
	for (i=0; i<n; i++){		// imprime a matriz de estados
		for (j=0; j<d; j++){
			printf("%d ", est[i][j]);
			if (j == d-1){
				printf("\n");
			}		
		}
	}	
	
	printf("total: %d unidades de tempo\n", b);
	printf("d = %d\n", d);
	
	
/*	total = total + m[0][0] + m[1][0];
	
	if (m[0][1] <= m[1][0]){
		total = total + m[0][2];
	}
	else {
		total = total + m[0][3];
	}
	
	printf("total: %ld\n", total); */
	
    return 0;
}
