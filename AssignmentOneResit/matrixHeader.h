#pragma
#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
protected: // Protected variables
	int N;
	int M;
	double* data;
	Matrix(){ M = 0; N = 0; data = 0; }

public:

	Matrix(int sizeR, int sizeC, double* input_data); // Matrix constructor
	Matrix(int sizeR, int sizeC);
	~Matrix(); // Matrix destructor
	Matrix(const Matrix& existingMatrix); // Copy constructor
	
	double get(int i, int j)  const; // Matrix get function, allows the retreival of blocks in the current matrix.
	const void set(int i, int j, double& val); // Matrix set function, allows setting of specific blocks in the matrix.
	
	int getN() const; // Return N
	int getM() const; // Return M
	
	Matrix getBlock(int startRow, int endRow, int startColumn, int endColumn); // Matrix get function, returns a whole block from the matrix.

	// Operators.
	// Plus
	Matrix operator + (const Matrix& B);
	// Minus
	Matrix operator - (const Matrix& B);
	// Equals
	Matrix operator = (const Matrix& B);
	// Multiply
	Matrix operator * (const Matrix& B);
	// Divide
	Matrix operator / (const Matrix& B);
	// Increment
	Matrix operator ++ ();
	double operator () (int i, int j);
	void out();
	double sum();
};

// Matrix Class

//Constructor
Matrix::Matrix(int sizeR, int sizeC, double* inputData)
{
	M = sizeR;
	N = sizeC;
	data = new double[M*N];
	for (int i = 0; i < M*N; i++)
	{
		data[i] = inputData[i];
	}
}
// Populates the matrix with 0's
// By populating the matrix with 0's, it can be created without data to be input into it at the current time.
Matrix::Matrix(int sizeR, int sizeC)
{
	M = sizeR;
	N = sizeC;
	data = new double[M*N];

	for (int i = 0; i < M*N; i++) // Cycles through the matrix.
	{
		*(data + i) = 0; // Sets each data to 0
	}
}
//Matrix Destructor
Matrix::~Matrix()
{
	delete[] data;
}
//Copy Constructor
Matrix::Matrix(const Matrix& existingMatrix)
{
	M = existingMatrix.getM();
	N = existingMatrix.getN();
	data = new double[M*N];

	for (int i = 0; i < M; i++) // Perform deep copy of values in the matrix
	{
		for (int j = 0; j < N; j++)
		{
			data[i*N + j] = existingMatrix.get(i, j);
		}
	}
}
//Return Value of M
int Matrix::getM() const
{
	return M;
}

//Return Value of N
int Matrix::getN() const
{
	return N;
}
//Returns contents of the matrix at point (i, j)
double Matrix::get(int i, int j)  const
{
	return data[i*N + j];
}

//Sets contents of the matrix at point (i, j) with data 'val'
const void Matrix::set(int i, int j, double& val)
{
	data[i*N + j] = val;
}
//Return a block of the Matrix.
Matrix Matrix::getBlock(int startRow, int endRow, int startColumn, int endColumn)
{
	int Row = endRow - startRow; 
	int Column = endColumn - startColumn; 
	double* block = new double[(Row)*(Column)];
	int k = 0;

	for (int i = startRow; i < endRow; i++)
	{
		for (int j = startColumn; j < endColumn; j++)
		{
			block[k] = data[i*N + j];
			k++;
		}
	}

	Matrix t(Row, Column, block); 
	delete[] block; 
	return t;
}
// Operator Plus
Matrix Matrix::operator +(const Matrix& B)
{
	Matrix C = Matrix(M, N);
	double temp;

	for (int i = 0; i < M; i++)	
	{
		for (int j = 0; j < N; j++)
		{
			temp = data[i*N + j] + B.get(i, j);
			C.set(i, j, temp);
		}
	}

	return C;
}
// Operator Minus
Matrix Matrix::operator -(const Matrix& B)
{
	Matrix C = Matrix(M, N);
	double temp;
	double t;


	for (int i = 0; i < M; i++)		
	{
		for (int j = 0; j < N; j++)
		{
			temp = *(data + (i*N + j)) - B.get(i, j);
			C.set(i, j, temp);
		}
	}

	return C;
}
// Operator Equals
Matrix Matrix::operator =(const Matrix& B)
{
	if (this == &B) 
	{
		return *this;
	}

	else 
	{
		M = B.getM();
		N = B.getN();
		delete[] data;
		data = new double[M*N];


		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				data[i*N + j] = B.get(i, j);
			}
		}

		return *this;
	}
}
// Operator Multiply
Matrix Matrix::operator *(const Matrix& B)
{
	Matrix C = Matrix(M, B.getN());
	double temp;
	double t;


	for (int i = 0; i < M; i++)				
	{
		for (int j = 0; j < N; j++)
		{
			temp = data[i*N + j] * B.get(i, j);		
			C.set(i, j, temp);
		}
	}


	return C;
}
//Operator Divide
Matrix Matrix::operator /(const Matrix& B)
{
	Matrix C = Matrix(M, B.getN());
	double temp;


	for (int i = 0; i < M; i++)						//Divide the two matrices together and set new value
	{
		for (int j = 0; j < N; j++)
		{
			temp = data[i*N + j] / B.get(i, j);
			C.set(i, j, temp);
		}
	}

	return C;
}
// Operator Increment
Matrix Matrix::operator ++()
{
	for (int i = 0; i < M*N; i++) 
	{
		data[i] = data[i]++;
	}

	return *this;
}
//Indirectly call the get operator
double Matrix::operator() (int i, int j)
{
	return data[i*N + j];
}
// Operator Sum
double Matrix::sum()
{
	double total = 0.0;
	for (int i = 0; i < M*N; i++)
	{
		total = total + data[i];
	}


	return total;
}

#endif