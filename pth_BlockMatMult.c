/* File:
*        pth_BlockMatMult.c
*
* Purpose:
*                               C=A*B
*        Computes a parallel matrix product. Matrix C
*        is partitioned by block matrices. 
*
*  Input:
*        m, n, k: matrix dimensions
*        A: m by n matrix
*        B: n by k matrix
*
*  Output:
*        C: m by k matrix
*
*  Note:
*        - First we assume m=n=k so all matrices
*          are squared ( n by n ).
*        - We also assume that n is divisible by
*          the number of threads (p).
*        - We also assume that p is a perfect square
*          number.
*        - TODO: Check the above assumptions. If p is
*          not a perfect square, we have a segmentation
*          error.
*
*  Compile:
*        gcc -g -Wall -o pth_BlockMatMult.exe pth_BlockMatMult.c lab1_IO.c -lpthread -lm
*  Usage:
*        pth_BlockMatMult.exe <thread_count>
*
*  Disclaimer:
*        This code was built using pth_mat_vec.c as a reference.
*
*  Student:
*        Breno Bahia (ID: 1449808)        
*/

/* preprocessing directives */
/* include headers */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "lab1_IO.h"

/* Declare variables
   Global varibles (All threads have access to them);
   The matrices A and B are loaded from data_output file; */
int thread_count;
int n, m;
int** A;
int** B;
int** C;
   
/* Function prototypes */
/* Serial functions */
void Usage(char *prog_name);
void Print_matrix(char* title, int **A, int m, int n);
void Print_vector(char* title, int *y, int m);

/* Parallel function - the one used by the threads
   It doesn't return anything and it takes only one
   void* input - this can be a pointer to a structure
   though.
 */
void *pth_BlockMatMult(void* rank);

/*----------------------------------------------------------------------------*/
// begin main
int main(int argc, char* argv[]){

   // define variables
   int i;
   long thread;
   pthread_t* thread_handles;
   
   /* Verify input from command line
      The only input accepted is the number of threads */
   if (argc !=2){
      Usage(argv[0]);
   } // end if
   
   // Get thread_count from command line
   thread_count = atoi(argv[1]);
   
   // Allocate memory for pthread_t
   thread_handles = malloc(thread_count*sizeof(pthread_t));
   
   // Load matrices and dimension
   Lab1_loadinput(&A, &B, &n);
   
   // Set m=n
   m=n;
   
   // Allocate memory for result matrix C
   C = malloc(n*sizeof(int*));
   for (i = 0; i < n; i++) {
       C[i] = malloc(n * sizeof(int));
   } // end for
   
   // Print matrix
   /*
    TODO: Print_matrix prints a big column.
          Set it to print rows and columns.
    
     Print_matrix("We read", A, n, n);
     Print_matrix("We read", B, n, n);
    */
   
   // create each thread and run function in it
   for (thread=0; thread < thread_count; thread++){
      // create thread
      pthread_create(&thread_handles[thread], NULL, pth_BlockMatMult, (void*) thread);
   } // end for
   
   // join threads after execution
   for (thread=0; thread < thread_count; thread++){
      pthread_join(thread_handles[thread],NULL);
   } // end for
   
   // Print output
   // Print_matrix("The result is", C, n, n);
   
   // Save output
   Lab1_saveoutput(C, &n, 0.0);
   
   // Free memory
   free(A);
   free(B);
   free(C);
  
   /* terminate */
   return 0;
   
} /* end main */
   

                          /* Define functions */

/*------------------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  // end Usage

/*------------------------------------------------------------------------------
 * Function: Print_matrix( char* prompt, int x[], int m, int n )
 * Purpose:     Print a matrix
 */
void Print_matrix( char* prompt, int **A, int m, int n ) {
   // define variables
   int i,j;
   
   // prompt
   printf("%s\n", prompt);
   for (i=0; i<m; i++)
   {
      for (j=0; j<n; j++)
      {
         printf("%d\t", A[i][j]);
         printf("\n");
      } // end for
   } // end for
   
} // end Print_matrix

/*------------------------------------------------------------------------------
 * Function: Print_vector( char* prompt, int x[], int m, int n )
 * Purpose:     Print a vector
 */
void Print_vector( char* prompt, int *y, int m ) {
   // define variables
   int i;
   
   // prompt
   printf("%s\n", prompt);
   for (i=0; i<m; i++){
      printf( "%d\t", y[i] );
      printf("\n");
   } // end for
   
} // end Print_matrix

/*------------------------------------------------------------------------------
 * Function pth_BlockMatMult( void* rank )
 * Purpose: Parallel Matrix multiplication
 *
 * Local input: rank
 * Global input: A, B, n, thread_count
 * Output (Global): C
 *
 * Student:
 *          Breno Bahia (ID: 1449808)
 */ 
  
  void *pth_BlockMatMult( void* rank ) {
   // define variabels
   long my_rank = (long) rank;
   int i, j, k;
   int p = sqrt(thread_count);
   
   // Block indexes
   int blockRow = floor( my_rank/p );
   int blockCol = my_rank % p ;
   
   // Row treatment
   int local_m = m/p; // number of rows in thread
   int myFirstRow = blockRow*local_m;
   int myLastRow  = ( blockRow + 1 )*local_m - 1;
   
   // Col treatment
   int local_n = n/p; /* number of columns in thread
                         notice that local_m and local_n
                         should be equal
                        */
   int myFirstCol = blockCol*local_n;
   int myLastCol  = ( blockCol + 1 )*local_n - 1;
   
   
   for ( i = myFirstRow ; i <= myLastRow; i++ ) 
   {
      for ( j = myFirstCol ; j <= myLastCol; j++ ) 
      {
         C[i][j]=0;
         for ( k = 0; k < n; k++ )
         {
            C[i][j] += A[i][k]*B[k][j];
         } // end for
      } // end for
   } // end for

   return NULL;
} // end pth_BlockMatMult
