# MPI_Matrix_Multiplication
Write one MPI program that multiply two integer matrices A X B = C. 

There would be p
processes, where n mod p == 0. The sequential code for the multiplication is like this:

int *A, *B, *C;
for (i = 0; i < n; i++) {
for (j = 0; j < n; j++) {
C[i][j] = 0;
for (k = 0; k < n; k++)
	C[i][j] += A[i][k]*B[k][j];
}
}
Write an MPI program to implement the multiplication using the striping data decomposition method.
In your program, one process (e.g. process 0) opens a file named “data.txt” and read two
matrices, A and B, respectively.
Then it evenly partitions A in strips and send them to respective processes, and send the
entire B to all the processes. After each process finishes their work, process 0 collects the
results from the other processes and prints matrix C. In addition, please calculate the
number of bytes that have been transmitted between processes. (e.g. if process 0 sends
and process 1 receives one integer, then the size of MPI_INT in bytes are transmitted.

You can use MPI_Type_size(MPI_Datatype datatype, int *size) to obtain the size of an
MPI datatype.). Please print out this number at the end of an execution. For better
performance and more concise program, don’t send/receive the data in a one-integer-by-one- integer manner.
The format of the data.txt is like this (its first line gives the value of n):

4
1 2 3 4
5 6 7 8
9 10 11 12
13 14 15 16
101 102 103 104
105 106 107 108
109 110 111 112
113 114 115 116