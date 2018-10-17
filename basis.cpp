#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <iostream>


using namespace std;

double** construct_matrix(int size,int seed);
void print_matrix(int size,double** matrix);
double** initialize_matrix(int size,char type,double** A);
double* initialize_pi(int size);
void LUD(double **A,double **L, double **U);





void LUD(double **A,double **L, double **U,double *PI,int size){

	

}

double* initialize_pi(int size){
		double* pi = new double[size];

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
						if(i>j)
							matrix[i][j] = 0.0;
						else
							matrix[i][j] = A[i][j];
				}
		}else if(type =='l'){				//initialize l as an n * n matrix with 1s on the diagonal and 0s above the diagonal 
			//그럼 이 나머지는? 원래 A에서 가져와야			
			for(int i=0;i<size;i++)
				for(int j=0;j<size;j++){
					if(i==j)
						matrix[i][j] = 1.0;
					else if(i<j)
						matrix[i][j] = 0.0;
					else
						matrix[i][j] = A[i][j];
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


		srand(seed);

		for(int i=0;i<size;i++)
			matrix[i] = new double[size];	

		for(int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				matrix[i][j]=(double)rand();
			}
		}
			

		return matrix;
}

int main(int argc , char* argv[]){
		
		double** A,**L,**U;		
		double* PI;
		if(argc!=5){
			printf("Usage: <executable> <mat_size> <rand_seed_num> <num_of_thread> <print flag>\n");
		}

		int size=atoi(argv[1]);
		int seed=atoi(argv[2]);
		int num_threads=atoi(argv[3]);
		int flag=atoi(argv[4]);


		PI = initialize_pi(size);
		A = construct_matrix(size,seed);
		U = initialize_matrix(size,'u',A);
		L = initialize_matrix(size,'l',A);



		if(flag==1){
			printf("L:\n");
			print_matrix(size,L);
			printf("U:\n");
			print_matrix(size,U);
			printf("A:\n");
			print_matrix(size,A);
		}
}
