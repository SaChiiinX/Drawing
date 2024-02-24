#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"

using namespace std;

Vector::Vector()
{
		data = new double[3];
		data[0] = 0;
		data[1] = 0;
		data[2] = 1;
}

Vector::Vector(const Vector& oldVector)
{
	data = new double[3];
	data[0] = oldVector.data[0];
	data[1] = oldVector.data[1];
	data[2] = oldVector.data[2];
}

Vector::Vector(const double x, const double y)
{
	data = new double[3];
	data[0] = x;
	data[1] = y;
	data[2] = 1;
}


Vector::~Vector()
{
	delete[] data;
}

double& Vector::operator[](int index) const
{
	return data[index];
}

Matrix::Matrix() 
{
	data = new double*[3];
	data[0] = new double[3];
	data[1] = new double[3];
	data[2] = new double[3];
	data[0][0] = 1;
	data[1][1] = 1;
	data[2][2] = 1;
}

Matrix::Matrix(const Matrix& oldMatrix) 
{
	data = new double*[3];
	data[0] = new double[3];
	data[1] = new double[3];
	data[2] = new double[3];
	data[0][0] = oldMatrix.data[0][0];
	data[1][1] = oldMatrix.data[1][1];
	data[2][2] = oldMatrix.data[2][2];
}

Matrix::~Matrix()
{
	for (int i = 0; i < 3; i++) {
		delete[] data[i];
	}
}

Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{
	Matrix* ret = new Matrix();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			ret->data[i][j] = data[i][j] * otherMatrix->data[i][j];
			ret->data[i][j + 1] = data[i][j] * otherMatrix->data[i + 1][j];
			ret->data[i][j + 1] = data[i][j] * otherMatrix->data[i + 2][j];
		}
	}
	return ret;
}

Vector* Matrix::multiply(const Vector* theVector) const
{
	double x = 0;
	double y = 0;
	for (int i = 0; i < 3; i++) {
		x += data[i][0] * (*theVector)[0];
		y += data[i][1] * (*theVector)[1];
	}

	return new Vector(x, y);
}

double* Matrix::operator[](int index) const
{
	return data[index];
}

Matrix* Matrix::translation(double deltaX, double deltaY)
{
	Matrix* ret = new Matrix();
	ret->data[0][2] = deltaX;
	ret->data[1][2] = deltaY;

	return ret;
}

Matrix* Matrix::rotation(double theta)
{
	Matrix* ret = new Matrix();
	ret->data[0][0] = cos(theta);
	ret->data[0][1] = -sin(theta);
	ret->data[1][0] = sin(theta);
	ret->data[1][1] = cos(theta);

	return ret;
}
Matrix* Matrix::shearing(double shearXY, double shearYX)
{
	Matrix* ret = new Matrix();
	ret->data[0][1] = shearXY;
	ret->data[1][0] = shearYX;

	return ret;
}

Matrix* Matrix::scaling(double scaleX, double scaleY)
{
	Matrix* ret = new Matrix();
	ret->data[0][0] = scaleX;
	ret->data[1][1] = scaleY;

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
