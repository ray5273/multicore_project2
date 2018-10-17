#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

double** construct_matrix(int size,int seed);
void print_matrix(int size,double** matrix);
double** initialize_matrix(int size,char type,double** A);
int* initialize_pi(int size);
void LUD(double **A,double **L, double **U,int size);
void LUD_PAR(double **A,double **L, double **U,int *PI,int size,int threads_num);

void multiply(double **L,double**U,int size){
		double A[size][size];
		for(int i = 0 ; i <size;i++){
			for(int j=0 ; j<size;j++){
				A[i][j]=0.0;
				for(int k =0;k<size;k++){
					A[i][j] +=L[i][k]*U[k][j];
				}
			}
		}				
	
		for(int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				printf("%.4e ",A[i][j]);
			}
			printf("\n");
		}
}


void LUD(double **A,double **L, double **U,int size){
		int t=0;
		for(int k=0;k<size;k++){
			int max = 0;
			
		
			U[k][k] = A[k][k];
			for(int i=k+1;i<size;i++){
				L[i][k] = A[i][k]/U[k][k];
				U[k][i] = A[k][i];	
			}
			for(int i=k+1;i<size;i++){
				for(int j=k+1;j<size;j++){
						A[i][j] = A[i][j] - L[i][k]*U[k][j];
					}
			}

		}	

}
void LUD_PAR(double **A,double **L, double **U,int *PI,int size,int threads_num){

		#pragma omp parallel shared(A,L,U) 
		{	int t=0;
			for(int k=0;k<size;k++){
				int max=0;
				for(int i=k;i<size;i++){
					if(max < abs(A[i][k])){
						max = abs(A[i][k]);
						t = i;
					}
				}	
				swap(PI[t],PI[k]);
				swap(A[k],A[t]);
				#pragma omp for
				for(int i=0;i<k;i++)
					swap(L[k][i],L[t][i]);			
			
				U[k][k] = A[k][k];
				#pragma omp for
				for(int i=k+1;i<size;i++){
					L[i][k] = A[i][k]/U[k][k];
					U[k][i] = A[k][i];	
				}
				#pragma omp for collapse(2)
				for(int i=k+1;i<size;i++){
					for(int j=k+1;j<size;j++){
							A[i][j] = A[i][j] - L[i][k]*U[k][j];
						}
				}
	
			}
		}	

}
int* initialize_pi(int size){
		int* pi = new int[size];

		for(int i=0;i<size;i++)
			pi[i]=0.0;
		return pi;
}
double** initialize_matrix(int size,char type,double **A){

		double** matrix = new double*[size];
		for(int i=0;i<size;i++)
			matrix[i] = new double[size];

		if(type == 'u'){					//initialize u as an n * n matrix with 0s below the diagonal
			for(int i=0;i<size;i++)
				for(int j=0;j<size;j++){
							matrix[i][j] = 0.0;
				}
		}else if(type =='l'){				//initialize l as an n * n matrix with 1s on the diagonal and 0s above the diagonal 
			for(int i=0;i<size;i++)
				for(int j=0;j<size;j++){
					if(i==j)
						matrix[i][j] = 1.0;
					else
						matrix[i][j]=0.0;
				}
		}
		return matrix;
}
void print_matrix(int size,double** matrix){

		for(int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				printf("%.4e ",(double)matrix[i][j]);
			}
			printf("\n");
		}	

}
double** construct_matrix(int size,int seed){
		double** matrix = new double*[size];
		for(int i=0;i<size;i++)
			matrix[i] = new double[size];	
		
		srand(seed);
		for(int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				matrix[i][j]=(double)rand();
			}
		}
			

		return matrix;
}

int main(int argc , char* argv[]){
		
		double** A,**L,**U,**B;		
		int* PI;
		if(argc!=5){
			printf("Usage: <executable> <mat_size> <rand_seed_num> <num_of_thread> <print flag>\n");
		}

		int size=atoi(argv[1]);
		int seed=atoi(argv[2]);
		int threads_num=atoi(argv[3]);
		int flag=atoi(argv[4]);
		omp_set_num_threads(threads_num);

		PI = initialize_pi(size);
		A = construct_matrix(size,seed);
		
	    B = new double*[size];
		for(int i=0;i<size;i++)
			B[i] = new double[size];
		for(int i=0;i<size;i++)
			for(int j=0;j<size;j++)
				B[i][j]=A[i][j];					
		U = initialize_matrix(size,'u',B);
		L = initialize_matrix(size,'l',B);	
	   	print_matrix(size,A);
		printf("\n"); 
	//	printf("threads num:%d\n",threads_num);
		double start = omp_get_wtime();	
		LUD_PAR(B,L,U,PI,size,threads_num);
		double end = omp_get_wtime();
		printf("%lf\n",end-start);
		if(flag==1){
			printf("L:\n");
			print_matrix(size,L);
			printf("U:\n");
			print_matrix(size,U);
			printf("A:\n");
			print_matrix(size,A);
			printf("estimate:\n");
			multiply(L,U,size);
		}
}
