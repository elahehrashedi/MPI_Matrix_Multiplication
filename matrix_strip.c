// Elaheh Rashedi



//CAUTION: I send and recieve the matrices by using int *
// it is one dimentional, but I changed it to two dimention with some calculations

// the printf is used for debugging

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

	int n , pid, p ;

	int *A, *B, *C;

	// the name of the tags for sending and receiving data
	
	int tagn = 0 ;
	int tagA = 1 ;
	int tagB = 2 ;
	int tagC = 3 ;

	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &p); // obtaining the number of processors
	MPI_Comm_rank(MPI_COMM_WORLD, &pid); // obtaining PID
        
	MPI_Status status;

	if (pid ==0 ) {

	

		FILE *fp = fopen("data.txt","r"); 
		
		fscanf ( fp , "%d", & n );
		//printf ("rows is %d \n", n );
		
		// first , I allocate a memory to matrices
		A = (int*) malloc( n *n * sizeof( int* ));
		B = (int*) malloc( n *n *sizeof( int* ));
		C = (int*) malloc( n *n* sizeof( int* ));	

		// then, read them from input file
		for ( int r = 0 ; r < n ; r++)
			for ( int c = 0 ; c < n ; c++ )
				fscanf ( fp , "%d", & A[r*n + c] );	


		for ( int r = 0 ; r < n ; r++)
			for ( int c = 0 ; c < n ; c++ )
				fscanf ( fp , "%d", & B[r*n + c] );	
	

		fclose (fp);

		// this offset shows the portion of the matrix A that belong to each process
		int offset = n / p * n ; // number of rows that belong to each process

		for ( int dest = 1 ; dest < p ; dest ++ ) // p0 keeps first part
		{
			// here, p0 send a portion of matrix A to all processors

			MPI_Send(& n , 1 , MPI_INT, dest , tagn, MPI_COMM_WORLD);
			MPI_Send(& A[offset], n / p * n , MPI_INT, dest , tagA, MPI_COMM_WORLD);
			MPI_Send(& B[0] , n * n , MPI_INT, dest , tagB, MPI_COMM_WORLD);
			printf("Sent %d elements to task %d offset= %d\n",n*n/p,dest,offset);
			offset += n / p * n ;		
		}


		// p0 should make his own calculations, too
		for ( int i=0 ; i< (n/p) ; i++ )
			for ( int j = 0 ; j<n ; j++ )
			{			
				C[i*n+j] = 0 ;	
				for ( int k = 0 ; k<n ; k++ )
					C[i*n+j] += A[i*n+k]*B[k*n+j];
				
			}


		offset = n / p * n ;
		// then it should receive the calculated matrix from all other processors
		for ( int dest = 1 ; dest < p ; dest ++ ) // get data from others
		{

			MPI_Recv(&C[offset] , n / p * n, MPI_INT, dest, tagC, MPI_COMM_WORLD, &status);
			printf("Receive %d elements from task %d offset= %d\n",n*n/p,dest,offset);
			offset += n / p * n ;		
		}

		printf ("The output is \n") ;

		for ( int i=0 ; i< n ; i++ ){
			for ( int j = 0 ; j<n ; j++ )	
				printf ( "%d  " , C[i*n+j] ) ;	
			printf ("\n");
		}

		// writing to the output file

		FILE *out = fopen("matrix_result.txt","w");
		if (out == NULL)
		{
    			printf("Error opening file!\n");
    			exit(1);
		}

		for ( int i=0 ; i< n ; i++ ){
			for ( int j = 0 ; j<n ; j++ )	
				fprintf ( out , "%d  " , C[i*n+j] ) ;	
			fprintf (out , "\n");
		}

		
		fprintf ( out , "Thank you ") ;
		fclose ( out) ;

	}

		
	// the other processors other than p0, do this part
	if ( pid != 0 )
	{

		// all processors receive the n from p0
		MPI_Recv(&n, 1, MPI_INT, 0, tagn , MPI_COMM_WORLD, &status);
		//printf ("n is %d \n", n );

		// they have their temporal matrixes with different size from the original one
		A = (int*) malloc( n/p * n * sizeof( int* ));
		B = (int*) malloc( n * n* sizeof( int* ));
		C = (int*) malloc( n /p *n * sizeof( int* ));

		// receiving A and B from p0

		MPI_Recv(&A[0] , n / p * n, MPI_INT, 0, tagA, MPI_COMM_WORLD, &status);
		MPI_Recv(&B[0] , n * n, MPI_INT, 0, tagB, MPI_COMM_WORLD, &status);

		// calculating the matrix C
		for ( int i=0 ; i< (n/p) ; i++ )
			for ( int j = 0 ; j<n ; j++ )
			{			
				C[i*n+j] = 0 ;	
				for ( int k = 0 ; k<n ; k++ )
					C[i*n+j] += A[i*n+k]*B[k*n+j];
				

			}

		// send matrix C to p0
		MPI_Send(& C[0] , n * n /p , MPI_INT, 0 , tagC, MPI_COMM_WORLD);


	}
	

	//printf("hello world, I am Process %d of %d \n", pid , p );

	MPI_Finalize();

	return 0;
}
