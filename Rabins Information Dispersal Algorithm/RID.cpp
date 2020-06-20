
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>

using namespace std;
 
// Dimension of input square matrix
#define N 4
#define M 8
#define L 32


 
// Function to get cofactor of mat[p][q] in temp[][]. n is current
// dimension of mat[][]
void getCofactor(int mat[N][N], int temp[N][N], int p, int q, int n)
{
	int i = 0; 
	int j = 0;
 
	// Looping for each element of the matrix
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			//  Copying into temporary matrix only those element
			//  which are not in given row and column
			if (row != p && col != q)
			{
				temp[i][j++] = mat[row][col];
 				
				// Row is filled, so increase row index and
				// reset col index
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}
 

/* Recursive function for finding determinant of matrix.
   n is current dimension of mat[][]. */
int determinantOfMatrix(int mat[N][N], int n)
{
	int D = 0; // Initialize result
 
	//  Base case : if matrix contains single element
	if (n == 1)
		return mat[0][0];

	int temp[N][N]; // To store cofactors
 
	int sign = 1;  // To store sign multiplier
 
	// Iterate for each element of first row
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of mat[0][f]
		getCofactor(mat, temp, 0, f, n);
		D += sign * mat[0][f] * determinantOfMatrix(temp, n - 1);
 
		// terms are to be added with alternate sign
		sign = -sign;
	}
 
	return D;
}

template<class T>
void display(T mat[N][N])
{
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
			cout << mat[i][j] << "\t";
	printf("\n");
	}
}

// Function to get adjoint of mat[N][N] in adj[N][N].
void adjoint(int mat[N][N],int adj[N][N])
{
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }

    // temp is used to store cofactors of mat[][]
    int sign = 1, temp[N][N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Get cofactor of mat[i][j]
            getCofactor(mat, temp, i, j, N);

            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2 == 0)? 1: -1;

            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i] = (sign)*(determinantOfMatrix(temp, N-1));
        }
    }
}


// Function to calculate and store inverse
bool inverse(int mat[N][N], float inverse[N][N])
{
	// Find determinant of mat[][]
	int det = determinantOfMatrix(mat, N);

	// Find adjoint
	int adj[N][N];

	adjoint(mat, adj);

	// Find Inverse using formula "inverse(mat) = adj(mat)/det(mat)"
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			inverse[i][j] = adj[i][j] / float(det);

	return true;
}


int main()
{
	int s[N];
	int F[N];
    	int FM[N];
	int n, m;
	printf("\nEnter the m value for the Vandermonde Matrix: ");
    	std::cin >> m;
	printf("\nEnter the n value for the Vandermonde Matrix: ");
    	std::cin >> n;

    	double x[m];

    	for (int l = 0; l < m; l++) //fill in x-es
    	{
		printf("Enter X # %d: ", (l + 1));
       	 	double f;
        	std::cin >> f;
       	 	x[l] = f;
    	}

	printf("\nHere is your new Vandermonde Matrix: \n");

	std::vector<std::vector<double> > van ( m , std::vector<double>( n, 0));

   	for ( int i = 0; i < m; i++){ //create vandermonde matrix
        	for (int j = 0; j < n; j++)
            	{
                	van[i][j] = pow(x[i], j);
            	}
	}

    	for (int i = 0 ; i < m ; ++i) 
    	{
        	for (int j = 0 ; j < n ; ++j)
        	std::cout << van [ i ] [ j ] << " " ;
		std::cout << "\n" ;
    	}


	int mat[N][N] = {{1, 2, 4, 8}, // 16, 32, 64, 128},
			{1, 3, 9, 27}, // 81, 243, 729, 2187},
			{1, 4, 16, 64}, // 256, 1024, 4096, 16384},
			{1, 5, 25, 125} //, 625, 3125, 15625, 78125}
			};
	printf("\n");

        int temp = 0;
        for (int j = 0; j < N; j++){
                for (int x = 0; x < N; x++){
                        temp = temp + mat[j][x];
                }
                s[j] = temp;
		printf("S%d is : %d\n", j+1, s[j]);
        }



	printf("\nOriginal input matrix: \n");

	for(int x = 0; x < N; x++) 
	{
		for(int y = 0; y < N; y++)  // loop for the elements on the line
		{
			printf("%d\t", mat[x][y]);  // display the current element out of the array
		}
	printf("\n");  // when the inner loop is done, go to a new line
	}



	double determinant = determinantOfMatrix(mat, N);
	//printf("%1f", determinant);
	printf("\nDeterminant of matrix A is : %d \n\n", determinantOfMatrix(mat, N));

	int adj[N][N];  // To store adjoint of mat[][]
 
	float inv[N][N]; // To store inverse of mat[][]
        float invV[N][N]; // To store inverse of van[][]


	printf("\nThe Adjoint of matrix A is : \n");
	adjoint(mat, adj);
	display(adj);
 
	printf("\nThe Inverse of matrix A is : \n");
	
	if (inverse(mat, inv))
		display(inv);
	printf("\n");
/*
        printf("The Inverse of the vandermonde matrix is: \n");

        if (inverse(van, invV))
                display(invV);
        printf("\n");

*/

	printf("Multiplication of the input matrix and the Vandermonde Matrix:\n");	
	double Mult[N][N];
	for(int i = 0; i < N; i++){
    		for(int j = 0; j < N; j++){
      			Mult[i][j] = 0;
      			for(int k = 0; k < M; k++){
        			for(int l = 0; l < M; l++){
          				Mult[i][j] = van[i][k]*mat[l][j]+Mult[i][j];
        			}
      			}
    		}
  	}
	display(Mult);
	
        printf("\n");
	
	printf("F substrings generated from the multiplied matrices:\n");
        int temp2 = 0;
        for (int j = 0; j < N; j++){
                for (int x = 0; x < N; x++){
                        temp2 = temp2 + Mult[j][x];
                }
                F[j] = temp2;
                printf("F%d is : %d\n", j+1, F[j]);
        }
	printf("\n");


        printf("Multiplication of the inverse A matrix with F to get original Message Matrix:\n");
        double Mult2[N][N];
        for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                        Mult2[i][j] = 0;
                        for(int k = 0; k < M; k++){
                                for(int l = 0; l < M; l++){
                                        Mult2[i][j] = inv[i][j]*F[l]+Mult2[i][j];
                                }
                        }
                }
        }
        display(Mult2);

        printf("\n");





	return 0;
}

