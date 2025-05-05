/*
 * @Author: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @Date: 2025-05-02 21:49:11
 * @LastEditors: qwqb233 57853293+qwqb233@users.noreply.github.com
 * @FilePath: \C3_TFT\src\linera.cpp
 * @Description: 
 */
#include <string.h>
#include "../include/linera.h"


Matrix::Matrix(int * dims, int dims_num, std::vector<double> data)
{
    matrix = new matrix_t;
    if(matrix == nullptr)
    {
        return;
    }
    int data_size = 1;
    matrix->dims_num = dims_num;
    matrix->dims = new int[matrix->dims_num];
    if(matrix->dims == nullptr)
    {
        delete matrix;
        return;
    }
    for(int i = 0; i < matrix->dims_num; i++)
    {
        matrix->dims[i] = dims[i];
        data_size *= dims[i];
    }
    for(int i = 0;i < data_size;i++)
    {
        matrix->data.push_back(data[i]);
    }
    matrix->data_size = data_size;
}

Matrix::Matrix(int * dims, int dims_num, double * data)
{   
    matrix = new matrix_t;
    if(matrix == nullptr)
    {
        return;
    }
    int data_size = 1;
    matrix->dims_num = dims_num;
    matrix->dims = new int[matrix->dims_num];
    if(matrix->dims == nullptr)
    {
        delete matrix;
        return;
    }
    for(int i = 0; i < matrix->dims_num; i++)
    {
        matrix->dims[i] = dims[i];
        data_size *= dims[i];
    }
    for(int i = 0;i < data_size;i++)
    {
        matrix->data.push_back(data[i]);
    }
    matrix->data_size = data_size;
}

Matrix::~Matrix()
{
    delete[] matrix->dims;
    matrix->data.clear();
    delete matrix;
}

matrix_t Matrix::operator+(const Matrix &other)
{
    matrix_t matrix_result;
    if(!this->matrix_equal(this->matrix, other.matrix))
        return matrix_t{}; // Return empty matrix if dimensions are not equal
    
    matrix_result.dims_num = this->matrix->dims_num;
    matrix_result.dims = new int[matrix_result.dims_num];
    memcpy(matrix_result.dims, this->matrix->dims, matrix_result.dims_num * sizeof(int));
    matrix_result.data_size = this->matrix->data_size;

    for(int i = 0;i < this->matrix->data_size; i++)
    {
        matrix_result.data[i] = this->matrix->data[i] + other.matrix->data[i];
    }
    return matrix_result;
}

matrix_t Matrix::operator+(const double &other)
{
    matrix_t matrix_result;
    matrix_result.dims_num = this->matrix->dims_num;
    matrix_result.dims = new int[matrix_result.dims_num];
    memcpy(matrix_result.dims, this->matrix->dims, matrix_result.dims_num * sizeof(int));
    matrix_result.data_size = this->matrix->data_size;

    for(int i = 0;i < this->matrix->data_size; i++)
    {
        matrix_result.data[i] = this->matrix->data[i] + other;
    }
    return matrix_result;
}

matrix_t Matrix::operator-(const Matrix &other)
{
    matrix_t matrix_result;
    if(!this->matrix_equal(this->matrix, other.matrix))
        return matrix_t{}; // Return empty matrix if dimensions are not equal
    
    matrix_result.dims_num = this->matrix->dims_num;
    matrix_result.dims = new int[matrix_result.dims_num];
    memcpy(matrix_result.dims, this->matrix->dims, matrix_result.dims_num * sizeof(int));
    matrix_result.data_size = this->matrix->data_size;

    for(int i = 0;i < this->matrix->data_size; i++)
    {
        matrix_result.data[i] = this->matrix->data[i] - other.matrix->data[i];
    }
    return matrix_result;
}

matrix_t Matrix::operator-(const double &other)
{
    matrix_t matrix_result;
    matrix_result.dims_num = this->matrix->dims_num;
    matrix_result.dims = new int[matrix_result.dims_num];
    memcpy(matrix_result.dims, this->matrix->dims, matrix_result.dims_num * sizeof(int));
    matrix_result.data_size = this->matrix->data_size;

    for(int i = 0;i < this->matrix->data_size; i++)
    {
        matrix_result.data[i] = this->matrix->data[i] - other;
    }
    return matrix_result;
}

matrix_t Matrix::operator*(const Matrix &other)
{
    matrix_t matrix_result;
    if (!this->matrix_can_multiply(this->matrix, other.matrix))
        return matrix_t{}; // Return empty matrix if dimensions are not compatible

    matrix_result.dims_num = 2; // Matrix multiplication is only valid for 2D matrices
    matrix_result.dims = new int[2];
    matrix_result.dims[0] = this->matrix->dims[0];
    matrix_result.dims[1] = other.matrix->dims[1];
    matrix_result.data_size = matrix_result.dims[0] * matrix_result.dims[1];

    matrix_result.data.resize(matrix_result.data_size, 0); // Initialize with zeros

    for (int i = 0; i < this->matrix->dims[0]; i++) {
        for (int j = 0; j < other.matrix->dims[1]; j++) {
            for (int k = 0; k < this->matrix->dims[1]; k++) {
                matrix_result.data[i * matrix_result.dims[1] + j] +=
                    this->matrix->data[i * this->matrix->dims[1] + k] *
                    other.matrix->data[k * other.matrix->dims[1] + j];
            }
        }
    }

    return matrix_result;
}

matrix_t Matrix::operator*(const double &other)
{
    matrix_t matrix_result;
    matrix_result.dims_num = this->matrix->dims_num;
    matrix_result.dims = new int[matrix_result.dims_num];
    memcpy(matrix_result.dims, this->matrix->dims, matrix_result.dims_num * sizeof(int));
    matrix_result.data_size = this->matrix->data_size;

    for(int i = 0;i < this->matrix->data_size; i++)
    {
        matrix_result.data[i] = this->matrix->data[i] * other;
    }
    return matrix_result;
}

matrix_t Matrix::operator/(const double &other)
{
    matrix_t matrix_result;
    matrix_result.dims_num = this->matrix->dims_num;
    matrix_result.dims = new int[matrix_result.dims_num];
    memcpy(matrix_result.dims, this->matrix->dims, matrix_result.dims_num * sizeof(int));
    matrix_result.data_size = this->matrix->data_size;

    for(int i = 0;i < this->matrix->data_size; i++)
    {
        matrix_result.data[i] = this->matrix->data[i] / other;
    }
    return matrix_result;
}


bool Matrix::matrix_can_multiply(matrix_t * matrix1, matrix_t * matrix2)
{
    if(matrix1->dims_num != 2 || matrix2->dims_num != 2)
    {
        return false;
    }
    else if(matrix1->dims[1] != matrix2->dims[0])
    {
        return false;
    }
    else if(matrix1->dims[0] != matrix2->dims[1])
    {
        return false;
    }
    return true;
}

bool Matrix::matrix_equal(matrix_t * matrix1, matrix_t * matrix2)
{
    if(matrix1->dims_num != matrix2->dims_num)
    {
        return false;
    }
    else
    {
        for(int i = 0; i < matrix1->dims_num; i++)
        {
            if(matrix1->dims[i] != matrix2->dims[i])
            {
                return false;
            }
        }
    }

    return true;
}
