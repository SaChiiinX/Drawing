#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"

using namespace std;

Vector::Vector()
{
	// allocate memory
	data = new double[3];

	// intialize array to [0,0,1]
	data[0] = 0;
	data[1] = 0;
	data[2] = 1;
}

Vector::Vector(const Vector& oldVector)
{
	// allocate memory
	data = new double[3];

	// intialize array to [x,y,z] of another vector
	data[0] = oldVector.data[0];
	data[1] = oldVector.data[1];
	data[2] = oldVector.data[2];
}

Vector::Vector(const double x, const double y)
{
	// allocate memory
	data = new double[3];

	// intialize array to [x,y,1]
	data[0] = x;
	data[1] = y;
	data[2] = 1;
}


Vector::~Vector()
{
	// deallocate memory
	delete[] data;
}

double& Vector::operator[](int index) const
{
	// Allows indexing if dereference a vector pointer
	return data[index];
}

Matrix::Matrix() 
{
	// allocate space
	data = new double*[3];
	data[0] = new double[3];
	data[1] = new double[3];
	data[2] = new double[3];

	//intialize diagonal to be [1,1,1]
	data[0][0] = 1;
	data[1][1] = 1;
	data[2][2] = 1;
}

Matrix::Matrix(const Matrix& oldMatrix) 
{
	// allocate space
	data = new double*[3];
	data[0] = new double[3];
	data[1] = new double[3];
	data[2] = new double[3];

	// intialize diagonal to be [x,y,z] of another matrix
	data[0][0] = oldMatrix.data[0][0];
	data[1][1] = oldMatrix.data[1][1];
	data[2][2] = oldMatrix.data[2][2];
}

Matrix::~Matrix()
{
	// deallocate memory for the 2d array
	for (int i = 0; i < 3; i++) {
		delete[] data[i];
	}

	delete[] data;
}

Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{
	Matrix* ret = new Matrix();
	double sum = 0;
	for (int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++){
			// i,j loop for which index of 2d array to update

			for(int k = 0; k < 3; k++){
				// k loop to reloop over ith row(thisMatrix) and ith column(otherMatrix) to calculate product
				sum += data[i][k]*(*otherMatrix)[k][i];
			}
			(*ret)[i][j] = sum;
		}	  
	}
	return ret;
}

Vector* Matrix::multiply(const Vector* theVector) const
{
	// get values from 
	double x = (*theVector)[0];
	double y = (*theVector)[1];
	double z = (*theVector)[2];
	// gets new x and y for resulting vector from the multiplication
	double x1 = (data[0][0] * x) + (data[0][1] * y) + (data[0][2] * z);
	double y1 = (data[1][0]) * x + (data[1][1] * y) + (data[1][2] * z);

	return new Vector(x1, y1);
}

double* Matrix::operator[](int index) const
{
	// allows indexing if you dereference matrix pointer
	return data[index];
}

Matrix* Matrix::translation(double deltaX, double deltaY)
{
	Matrix* ret = new Matrix();
	(*ret)[0][2] = deltaX;
	(*ret)[1][2] = deltaY;

	return ret;
}

Matrix* Matrix::rotation(double theta)
{
	Matrix* ret = new Matrix();
	(*ret)[0][0] = cos(theta);
	(*ret)[0][1] = -sin(theta);
	(*ret)[1][0] = sin(theta);
	(*ret)[1][1] = cos(theta);

	return ret;
}
Matrix* Matrix::shearing(double shearXY, double shearYX)
{
	Matrix* ret = new Matrix();
	(*ret)[0][1] = shearXY;
	(*ret)[1][0] = shearYX;

	return ret;
}

Matrix* Matrix::scaling(double scaleX, double scaleY)
{
	Matrix* ret = new Matrix();
	(*ret)[0][0] = scaleX;
	(*ret)[1][1] = scaleY;

	return ret;
}

Matrix* Matrix::getInverse() const
{
	Matrix* answer = new Matrix();
	double det = getDeterminant();

	answer->data[0][0] = -data[1][2] * data[2][1] + data[1][1] * data[2][2];
	answer->data[0][1] = data[0][2] * data[2][1] - data[0][1] * data[2][2];
	answer->data[0][2] = -data[0][2] * data[1][1] + data[0][1] * data[1][2];
	answer->data[1][0] = data[1][2] * data[2][0] - data[1][0] * data[2][2];
	answer->data[1][1] = -data[0][2] * data[2][0] + data[0][0] * data[2][2];
	answer->data[1][2] = data[0][2] * data[1][0] - data[0][0] * data[1][2];
	answer->data[2][0] = -data[1][1] * data[2][0] + data[1][0] * data[2][1];
	answer->data[2][1] = data[0][1] * data[2][0] - data[0][0] * data[2][1];
	answer->data[2][2] = -data[0][1] * data[1][0] + data[0][0] * data[1][1];

	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			answer->data[i][j] /= det;

	return answer;
}

double Matrix::getDeterminant() const
{
	return  -data[0][2] * data[1][1] * data[2][0] + data[0][1] * data[1][2] * data[2][0] +
		data[0][2] * data[1][0] * data[2][1] - data[0][0] * data[1][2] * data[2][1] -
		data[0][1] * data[1][0] * data[2][2] + data[0][0] * data[1][1] * data[2][2];
}
