//
// Created by qwqb233 on 2025/5/5.
//
#include <Arduino.h>
#include <Wire.h>
#include <iostream>
#include <stdexcept>
#include <cmath>


#include "../include/Matrix.h"


Matrix::Matrix(int * dims,int dim_size,std::vector<double> data_in) : dims(new int[dim_size])
{
    //TODO:多线程安全
    if(dims == nullptr || data_in.empty())
    {
        throw std::invalid_argument("Invalid argument");
    }

    this -> data = data_in;
    this -> data_size = data_in.size();
    this -> dim_size = dim_size;
    this -> dims = new int[dim_size];
    for(int i = 0;i < dim_size;i++)
    {
        this -> dims[i] = dims[i];
        this->data_size *= dims[i];
    }
}

Matrix::Matrix(int * dims,int dim_size,double * data) : dims(new int[dim_size])
{
    //TODO:多线程安全


    if(dims == nullptr || data == nullptr)
    {
        throw std::invalid_argument("Invalid argument");
    }

    this->dim_size = dim_size;
    this->data_size = 1;
    for(int i = 0;i < dim_size;i++)
    {
        this->data_size *= dims[i];
        this->dims[i] = dims[i];
    }

    try{

        this->data = std::vector<double>(this->data_size);
        for(int i = 0;i < this->data_size;i++) {
            this->data[i] = data[i];
        }
    }
    catch(std::exception& e)
    {
        throw std::invalid_argument("data is not valid");
    }


}

Matrix::~Matrix()
{
    delete[] this->dims;
}

void Matrix::print()
{
    for (int i = 0; i < this->dims[0]; i++) {
        for (int j = 0; j < this->dims[1]; j++) {
            Serial.printf("%lf ", this->data[i * this->dims[1] + j]);
            //std::cout << this->data[i * this->dims[1] + j] << " ";
        }
        Serial.printf("\r\n");
        //std::cout << std::endl;
    }
}

void Matrix::transpose()
{
    int rows = this->dims[0];
    int cols = this->dims[1];
    std::vector<double> new_data(rows * cols);

    new_data = this->data;
    for(int i = 0;i < rows;i++)
    {
        for(int j = 0;j < cols;j++)
        {
            this->data[i * cols + j] = new_data[j * rows + i];
        }
    }
}


//接口函数

std::vector<double> Matrix::get_data()
{
    return this->data;
}

int * Matrix::get_dims()
{
    int * result = new int[this->dim_size];
    for(int i = 0;i < this->dim_size;i++)
    {
        result[i] = this->dims[i];
    }
    return result;
}

int Matrix::get_dim_size()
{
    return this->dim_size;
}

long long Matrix::get_data_size()
{
    return this->data_size;
}


//运算符重载

Matrix Matrix::operator+(const Matrix& other)
{
    if(this->dim_size != other.dim_size || this -> dims[0] != other.dims[0] || this -> dims[1] != other.dims[1])
    {
        throw std::invalid_argument("Invalid argument");
    }

    Matrix result(this->dims,this->dim_size,this->data);
    for(int i = 0;i < this->data_size;i++)
    {
        result.data[i] += other.data[i];
    }

    return result;
}

Matrix Matrix::operator-(const Matrix& other)
{
    if(this->dim_size != other.dim_size || this -> dims[0] != other.dims[0] || this -> dims[1] != other.dims[1])
    {
        throw std::invalid_argument("Invalid argument");
    }

    Matrix result(this->dims,this->dim_size,this->data);
    for(int i = 0;i < this->data_size;i++)
    {
        result.data[i] -= other.data[i];
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other)
{
    if(this->dim_size != other.dim_size || this -> dims[1] != other.dims[0])
    {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    int rows1 = this->dims[0];
    int cols1 = this->dims[1];
    int cols2 = other.dims[1];
    Matrix result(new int[2]{rows1,cols2},2,new double[rows1 * cols2]{});

    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result.data[i * cols2 + j] += this->data[i * cols1 + k] * other.data[k * cols2 + j];
            }
        }
    }
    return result;
}


Matrix Matrix::operator+(double scalar)
{
    Matrix result(this->dims,this->dim_size,this->data);
    for(int i = 0;i < this->data_size;i++)
    {
       result.data[i] += scalar;
    }
    return result;
}

Matrix Matrix::operator-(double scalar)
{
    Matrix result(this->dims,this->dim_size,this->data);
    for(int i = 0;i < this->data_size;i++)
    {
        result.data[i] -= scalar;
    }
    return result;
}

Matrix Matrix::operator*(double scalar)
{
    Matrix result(this->dims,this->dim_size,this->data);
    for(int i = 0;i < this->data_size;i++)
    {
        result.data[i] *= scalar;
    }
    return result;
}

Matrix Matrix::operator/(double scalar)
{
    if(scalar == 0)
    {
        throw std::invalid_argument("Invalid argument");
    }
    Matrix result(this->dims,this->dim_size,this->data);
    for(int i = 0;i < this->data_size;i++)
    {
        result.data[i] /= scalar;
    }
    return result;
}

Matrix Matrix::operator^(double power)
{
    if(power < 0)
    {
        throw std::invalid_argument("Invalid argument");
    }
    Matrix result(this->dims,this->dim_size,this->data);
    for(int i = 0;i < this->data_size;i++)
    {
        result.data[i] = pow(result.data[i],power);
    }
    return result;
}

Matrix& Matrix::operator=(const Matrix& other)
{
    this->data.clear();
    delete[] this->dims;

    this->data = other.data;
    this->data_size = other.data_size;
    this->dim_size = other.dim_size;
    this->dims = new int[this->dim_size];
    for(int i = 0;i < this->dim_size;i++)
    {
        this->dims[i] = other.dims[i];
    }
    return *this;
}

bool Matrix::operator==(const Matrix& other)
{
    if(this->dim_size != other.dim_size || this -> dims[0] != other.dims[0] || this -> dims[1] != other.dims[1])
    {
        return false;
    }

    for(int i = 0;i < this->data_size;i++)
    {
        if(this->data[i] != other.data[i])
        {
            return false;
        }
    }
    return true;
}
